# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Mod integration for DayZ server mission configuration.

DayZ mods that add new items, vehicles, or spawn points must register their
classnames in the server's mission XML files (``types.xml``,
``cfgspawnabletypes.xml``, ``cfgeventspawns.xml``, ``events.xml``, etc.).
Loading the mod via ``-mod=`` is not enough.

This module scans workshop/local mod folders for XML configuration fragments,
maps them to the correct server mission file, and merges them into the active
instance's mission directory. It backs up the original files first and avoids
duplicate entries when re-applied.
"""

from __future__ import annotations

import json
import re
import shutil
from dataclasses import dataclass, field
from datetime import datetime, timezone
from pathlib import Path
from typing import Dict, List, Optional, Set, Tuple
from xml.etree import ElementTree as ET

from .xml_repair import (
    TARGET_TO_ROOT_TAG,
    XmlValidationResult,
    repair_mod_xml,
    validate_cfgeconomycore,
    validate_mission_xml,
)


# Mapping from XML root element tag to the relative path inside the mission
# directory where the fragment should be merged.
# NOTE: these match the *actual root tags* of the DayZ root mission files,
# not the CE-routed type tokens used by cfgeconomycore.xml.
ROOT_TO_MISSION_PATH: Dict[str, str] = {
    "types": "db/types.xml",
    "spawnabletypes": "cfgspawnabletypes.xml",
    "eventposdef": "cfgeventspawns.xml",
    "events": "db/events.xml",
    "economy": "db/economy.xml",
    "lists": "cfglimitsdefinition.xml",
    "user_lists": "cfglimitsdefinitionuser.xml",
    "randompresets": "cfgrandompresets.xml",
    "env": "cfgenvironment.xml",
    "prototype": "mapgroupproto.xml",
    "playerspawnpoints": "cfgplayerspawnpoints.xml",
}

# Directories inside a mod folder that commonly hold XML fragments.
CONFIG_SEARCH_DIRS = {"types", "xml", "extras", "config", "configs", "mission"}

# Filename patterns that hint at a target mission file.
FILENAME_HINTS: Dict[str, str] = {
    "types.xml": "db/types.xml",
    "cfgspawnabletypes.xml": "cfgspawnabletypes.xml",
    "cfgeventspawns.xml": "cfgeventspawns.xml",
    "events.xml": "db/events.xml",
    "economy.xml": "db/economy.xml",
    "cfglimitsdefinition.xml": "cfglimitsdefinition.xml",
    "cfglimitsdefinitionuser.xml": "cfglimitsdefinitionuser.xml",
    "cfgrandompresets.xml": "cfgrandompresets.xml",
    "cfgenvironment.xml": "cfgenvironment.xml",
    "mapgroupproto.xml": "mapgroupproto.xml",
    "mapgrouppos.xml": "mapgrouppos.xml",
    "mapgroupcluster.xml": "mapgroupcluster.xml",
    "mapgroupdirt.xml": "mapgroupdirt.xml",
    "mapclusterproto.xml": "mapclusterproto.xml",
    "cfgplayerspawnpoints.xml": "cfgplayerspawnpoints.xml",
}


@dataclass
class ModConfigFragment:
    """A single XML configuration fragment discovered inside a mod folder."""
    source_path: Path
    target_mission_path: str
    root_tag: str
    entry_count: int = 0
    validation_message: str = ""
    repaired: bool = False
    ok: bool = True
    source_text: Optional[str] = None


@dataclass
class ModIntegrationResult:
    """Result of applying mod integration to an instance."""
    success: bool
    messages: List[str] = field(default_factory=list)
    warnings: List[str] = field(default_factory=list)
    errors: List[str] = field(default_factory=list)
    backups: List[Path] = field(default_factory=list)
    modified_files: List[Path] = field(default_factory=list)

    @property
    def ok(self) -> bool:
        return self.success and not self.errors


class ModIntegrationManager:
    """Discover and merge mod-provided XML fragments into mission files."""

    def __init__(self, instance_root: Path):
        self.instance_root = Path(instance_root)
        self.state_path = self.instance_root / "mod_integration.json"

    # ------------------------------------------------------------------
    # State persistence
    # ------------------------------------------------------------------
    def load_state(self) -> Dict[str, any]:
        """Load persisted integration state for this instance."""
        if self.state_path.exists():
            try:
                return json.loads(self.state_path.read_text(encoding="utf-8"))
            except (json.JSONDecodeError, OSError):
                pass
        return {"active_mods": [], "last_applied": None}

    def save_state(self, active_mods: List[str]) -> None:
        """Persist which mods are currently integrated."""
        state = {
            "active_mods": sorted(set(active_mods)),
            "last_applied": datetime.now(timezone.utc).isoformat(),
        }
        self.state_path.write_text(json.dumps(state, indent=2), encoding="utf-8")

    # ------------------------------------------------------------------
    # Discovery
    # ------------------------------------------------------------------
    def scan_mod(self, mod_folder: Path) -> List[ModConfigFragment]:
        """Return all XML configuration fragments found inside a mod folder."""
        fragments: List[ModConfigFragment] = []
        if not mod_folder.exists():
            return fragments

        # Mod wrappers contain symlinks to the real workshop folders. Python's
        # Path.rglob does not follow directory symlinks, so we walk manually.
        for root, dirs, files in mod_folder.walk(follow_symlinks=True):
            for filename in files:
                if filename.lower().endswith(".xml"):
                    item = Path(root) / filename
                    fragment = self._classify_fragment(item)
                    if fragment:
                        fragments.append(fragment)

        return fragments

    def _classify_fragment(self, xml_path: Path) -> Optional[ModConfigFragment]:
        """Determine which mission file an XML fragment should merge into.

        The fragment is validated and repaired on the spot.  Files that cannot
        be made well-formed are returned as non-ok fragments so the caller can
        log a warning and skip them instead of letting them corrupt the target.
        """
        try:
            text = xml_path.read_text(encoding="utf-8", errors="ignore")
        except OSError:
            return None

        # Skip files that are clearly not DayZ mission fragments.
        if not text.strip().startswith("<"):
            return None

        # Resolve the target mission path first so we know which root element
        # the fragment is expected to have.
        lower_name = xml_path.name.lower()
        target_mission_path: Optional[str] = None
        if lower_name in FILENAME_HINTS:
            target_mission_path = FILENAME_HINTS[lower_name]
        else:
            root_tag = self._peek_root_tag(text)
            if root_tag and root_tag.lower() in ROOT_TO_MISSION_PATH:
                target_mission_path = ROOT_TO_MISSION_PATH[root_tag.lower()]

        if target_mission_path is None:
            return None

        expected_root = TARGET_TO_ROOT_TAG.get(
            target_mission_path, self._peek_root_tag(text) or "root"
        )
        repair = repair_mod_xml(text, expected_root)
        if not repair.ok:
            return ModConfigFragment(
                source_path=xml_path,
                target_mission_path=target_mission_path,
                root_tag=expected_root,
                ok=False,
                validation_message=f"REJECTED: {repair.error}",
            )

        root_tag = self._peek_root_tag(repair.text) or expected_root
        return ModConfigFragment(
            source_path=xml_path,
            target_mission_path=target_mission_path,
            root_tag=root_tag,
            source_text=repair.text,
            validation_message=repair.message,
            repaired=repair.was_repaired,
        )

    @staticmethod
    def _peek_root_tag(text: str) -> Optional[str]:
        """Return the root element tag without fully parsing the document."""
        # Strip XML declaration and comments, then grab the first tag.
        cleaned = re.sub(r"<\?xml.*?\?>", "", text, flags=re.DOTALL)
        cleaned = re.sub(r"<!--.*?-->", "", cleaned, flags=re.DOTALL)
        match = re.search(r"<([a-zA-Z_][\w.-]*)", cleaned)
        return match.group(1) if match else None

    # ------------------------------------------------------------------
    # Mission file resolution
    # ------------------------------------------------------------------
    def _find_mission_dir(
        self, target_name: Optional[str] = None
    ) -> Optional[Path]:
        """Locate the active mission directory inside the instance.

        The function never guesses between multiple mission folders.  It
        requires an explicit *target_name* or a ``serverDZ_*.cfg`` template
        that matches a folder under ``mpmissions/``.  This prevents mod XML
        from being silently written into the wrong map's mission folder.
        """
        mpmissions = self.instance_root / "mpmissions"
        if not mpmissions.exists():
            return None

        # Explicit target wins.
        if target_name:
            exact = mpmissions / target_name
            if exact.is_dir():
                return exact
            return None

        # Parse serverDZ_*.cfg for the template= line.
        template = self._read_server_template()
        if template:
            exact = mpmissions / template
            if exact.is_dir():
                return exact
            return None

        return None

    def _read_server_template(self) -> Optional[str]:
        """Return the mission template name from serverDZ_*.cfg, if present."""
        for cfg_path in sorted(self.instance_root.glob("serverDZ_*.cfg")):
            try:
                text = cfg_path.read_text(encoding="utf-8", errors="ignore")
            except OSError:
                continue
            match = re.search(r'template\s*=\s*"([^"]+)"', text)
            if match:
                return match.group(1)
        return None

    def _resolve_target_path(self, mission_dir: Path, relative_path: str) -> Path:
        """Return the absolute server mission file path for a relative target."""
        return mission_dir / relative_path

    def _expected_root_for_target(self, target_path: Path) -> Optional[str]:
        """Return the expected root element tag for a mission XML file."""
        name = target_path.name.lower()
        name_hint = {
            "types.xml": "types",
            "cfgspawnabletypes.xml": "spawnabletypes",
            "cfgeventspawns.xml": "eventposdef",
            "events.xml": "events",
            "economy.xml": "economy",
            "cfglimitsdefinition.xml": "limits",
            "cfglimitsdefinitionuser.xml": "limitss",
            "cfgrandompresets.xml": "randompresets",
            "cfgenvironment.xml": "environment",
            "mapgroupproto.xml": "prototype",
            "mapgrouppos.xml": "map",
            "mapgroupcluster.xml": "map",
            "mapgroupdirt.xml": "map",
            "mapclusterproto.xml": "prototype",
            "cfgplayerspawnpoints.xml": "playerspawnpoints",
        }.get(name)
        if name_hint:
            return name_hint
        # For files we do not have a hint for, skip structural validation.
        return None

    @staticmethod
    def _serialize_et(root: ET.Element) -> str:
        """Serialize an ElementTree element to a DayZ-style XML string."""
        ET.indent(root, space="    ")
        body = ET.tostring(root, encoding="unicode")
        return '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>\n' + body

    def _ce_type_for_file(self, filename: str) -> str:
        """Return the DayZ ``<file type="...">`` value for a mission file."""
        return self._CE_TYPE_FOR_FILE.get(filename.lower(), "")

    def _ensure_ce_references(
        self,
        mission_dir: Path,
        modified_files: List[Path],
    ) -> ModIntegrationResult:
        """Ensure ``cfgeconomycore.xml`` contains ``<ce>`` entries for modified files.

        DayZ ignores custom mission XML unless it is referenced by
        ``cfgeconomycore.xml``.  This method adds missing references after
        mod integration and validates the resulting core file.
        """
        result = ModIntegrationResult(success=True)
        core_path = mission_dir / "cfgeconomycore.xml"
        if not core_path.exists():
            result.warnings.append(
                "cfgeconomycore.xml not found; cannot ensure CE references"
            )
            return result

        try:
            text = core_path.read_text(encoding="utf-8", errors="ignore")
        except OSError as exc:
            result.errors.append(f"Cannot read cfgeconomycore.xml: {exc}")
            result.success = False
            return result

        try:
            root = ET.fromstring(text)
        except ET.ParseError as exc:
            result.errors.append(f"cfgeconomycore.xml parse error: {exc}")
            result.success = False
            return result

        existing_ce = root.findall("ce")
        if not existing_ce:
            # DayZ loads the standard mission CE files by default when no <ce>
            # sections exist (matching stock cfgeconomycore.xml).  Adding a
            # partial <ce> section here can switch the server into explicit-routing
            # mode and break loading of unreferenced files.  Leave the core file
            # untouched in this case.
            result.messages.append(
                "cfgeconomycore.xml has no <ce> sections; relying on DayZ default "
                "file loading"
            )
            return result

        changed = False
        for target_path in modified_files:
            if target_path == core_path:
                continue
            try:
                rel_path = target_path.relative_to(mission_dir).as_posix()
            except ValueError:
                continue
            if "/" in rel_path:
                folder, name = rel_path.rsplit("/", 1)
            else:
                folder, name = "", rel_path
            type_attr = self._ce_type_for_file(name)
            if not type_attr:
                continue

            already_referenced = False
            for ce in root.findall("ce"):
                if ce.get("folder", "") != folder:
                    continue
                for file_elem in ce.findall("file"):
                    if file_elem.get("name") == name:
                        already_referenced = True
                        break
                if already_referenced:
                    break

            if already_referenced:
                continue

            ce_elem = None
            for ce in root.findall("ce"):
                if ce.get("folder", "") == folder:
                    ce_elem = ce
                    break
            if ce_elem is None:
                ce_elem = ET.SubElement(root, "ce", {"folder": folder})
            ET.SubElement(ce_elem, "file", {"name": name, "type": type_attr})
            changed = True
            result.messages.append(
                f"Added <ce> reference for {rel_path} (type={type_attr})"
            )

        if changed:
            backup_path = self._backup_file(core_path)
            if backup_path:
                result.backups.append(backup_path)
            try:
                core_path.write_text(self._serialize_et(root), encoding="utf-8")
                result.modified_files.append(core_path)
            except OSError as exc:
                result.errors.append(f"Failed to update cfgeconomycore.xml: {exc}")
                result.success = False
                return result

        final_text = core_path.read_text(encoding="utf-8", errors="ignore")
        validation = validate_cfgeconomycore(final_text)
        for warning in validation.warnings:
            result.warnings.append(f"cfgeconomycore.xml: {warning}")
        if not validation.ok:
            for error in validation.errors:
                result.errors.append(f"cfgeconomycore.xml: {error}")
            result.success = False

        return result

    # ------------------------------------------------------------------
    # DayZ <ce> sections use a specific set of type tokens.  Note that
    # cfglimitsdefinition*.xml use "limits" / "limitsuser", even though the
    # root element is <lists> / <user_lists>.
    _CE_TYPE_FOR_FILE: Dict[str, str] = {
        "types.xml": "types",
        "cfgspawnabletypes.xml": "spawnabletypes",
        "events.xml": "events",
        "cfgeventspawns.xml": "eventposdef",
        "economy.xml": "economy",
        "cfglimitsdefinition.xml": "limits",
        "cfglimitsdefinitionuser.xml": "limitsuser",
        "cfgrandompresets.xml": "randompresets",
        "cfgenvironment.xml": "environment",
    }

    # Standard CE files that DayZ loads by default when cfgeconomycore.xml has
    # no <ce> sections.  If a mission already uses <ce>, these are the typical
    # references that should exist.
    _STANDARD_CE_FILES: Dict[str, str] = {
        "db/types.xml": "types",
        "db/events.xml": "events",
        "cfgspawnabletypes.xml": "spawnabletypes",
        "cfgeventspawns.xml": "eventposdef",
    }

    # ------------------------------------------------------------------
    # Merge logic
    # ------------------------------------------------------------------

    def apply_integration(
        self,
        mod_folders: List[Path],
        active_mods: Optional[Set[str]] = None,
        target_name: Optional[str] = None,
    ) -> ModIntegrationResult:
        """Merge XML fragments from active mods into the instance mission files.

        Each fragment is validated and repaired before it is merged.  After
        merging, the resulting mission XML is validated again and the
        mission's ``cfgeconomycore.xml`` is updated to reference any modified
        files so DayZ actually loads them.

        Args:
            mod_folders: List of resolved mod folder paths.
            active_mods: Optional set of mod link names (e.g. ``@3369325490``)
                to integrate. If omitted, all provided mods are integrated.

        Returns:
            ModIntegrationResult describing what was changed.
        """
        result = ModIntegrationResult(success=True)
        mission_dir = self._find_mission_dir(target_name=target_name)
        if mission_dir is None:
            result.success = False
            result.errors.append(
                f"No mission directory found under {self.instance_root / 'mpmissions'}"
            )
            return result

        # Group fragments by target mission file.  Fragments that failed
        # validation/repair are logged as warnings and skipped.
        targets: Dict[Path, List[Tuple[Path, str, Optional[str]]]] = {}
        for mod_folder in mod_folders:
            link_name = "@" + mod_folder.name.lstrip("@")
            if active_mods is not None and link_name not in active_mods:
                continue

            fragments = self.scan_mod(mod_folder)
            usable_fragments = [f for f in fragments if f.ok]
            for fragment in fragments:
                if not fragment.ok:
                    result.warnings.append(
                        f"{link_name}: {fragment.source_path.name} "
                        f"{fragment.validation_message}"
                    )

            if not usable_fragments:
                if not fragments:
                    result.messages.append(f"{link_name}: no XML fragments found")
                continue

            for fragment in usable_fragments:
                target_path = self._resolve_target_path(
                    mission_dir, fragment.target_mission_path
                )
                if target_path not in targets:
                    targets[target_path] = []
                targets[target_path].append(
                    (fragment.source_path, link_name, fragment.source_text)
                )
                status = " (repaired)" if fragment.repaired else ""
                result.messages.append(
                    f"{link_name}: will merge {fragment.source_path.name} -> "
                    f"{fragment.target_mission_path}{status}"
                )

        if not targets:
            result.messages.append("No mod XML fragments to merge.")
            return result

        # Backup and merge each target file, then validate the merged result.
        for target_path, sources in targets.items():
            if not target_path.exists():
                result.warnings.append(
                    f"Target mission file does not exist: {target_path}"
                )
                continue

            backup_path = self._backup_file(target_path, mission_dir=mission_dir)
            if backup_path:
                result.backups.append(backup_path)

            try:
                modified = self._merge_fragments(target_path, sources)
                if modified:
                    result.modified_files.append(target_path)
                    result.messages.append(f"Updated {target_path.name}")
                else:
                    result.messages.append(f"No changes needed for {target_path.name}")
            except Exception as exc:
                result.success = False
                result.errors.append(f"Failed to merge into {target_path}: {exc}")
                continue

            # Final validation of the merged mission file.
            expected_root = self._expected_root_for_target(target_path)
            if expected_root:
                final_text = target_path.read_text(encoding="utf-8", errors="ignore")
                validation = validate_mission_xml(final_text, expected_root)
                for warning in validation.warnings:
                    result.warnings.append(f"{target_path.name}: {warning}")
                if not validation.ok:
                    for error in validation.errors:
                        result.errors.append(f"{target_path.name}: {error}")
                    result.success = False
                    if backup_path:
                        try:
                            shutil.copy2(backup_path, target_path)
                            result.messages.append(
                                f"Restored {target_path.name} due to validation failure"
                            )
                        except OSError as exc:
                            result.errors.append(
                                f"Could not restore {target_path.name}: {exc}"
                            )

        # Ensure cfgeconomycore.xml references every file we touched so DayZ
        # actually loads the merged content.
        if result.modified_files:
            ce_result = self._ensure_ce_references(mission_dir, result.modified_files)
            result.messages.extend(ce_result.messages)
            result.warnings.extend(ce_result.warnings)
            result.errors.extend(ce_result.errors)
            if not ce_result.ok:
                result.success = False

        if result.ok:
            self.save_state(
                ["@" + f.name.lstrip("@") for f in mod_folders]
                if active_mods is None
                else list(active_mods)
            )

        return result

    def _backup_file(
        self, target_path: Path, mission_dir: Optional[Path] = None
    ) -> Optional[Path]:
        """Create a pristine backup of a mission file if one doesn't exist."""
        backup_dir = self.instance_root / "backups" / "mission"
        backup_dir.mkdir(parents=True, exist_ok=True)

        # Preserve the relative mission path in the backup filename so we can
        # restore to the correct location later.
        if mission_dir is None:
            mission_dir = self._find_mission_dir()
        if mission_dir is None:
            return None
        try:
            rel_path = target_path.relative_to(mission_dir).as_posix()
        except ValueError:
            rel_path = target_path.name

        safe_name = rel_path.replace("/", "__")
        pristine = backup_dir / f"{safe_name}.original"
        if not pristine.exists():
            try:
                shutil.copy2(target_path, pristine)
            except OSError:
                pass

        latest = backup_dir / f"{safe_name}.latest"
        try:
            shutil.copy2(target_path, latest)
        except OSError:
            return None
        return latest

    def _merge_fragments(
        self,
        target_path: Path,
        sources: List[Tuple[Path, str, Optional[str]]],
    ) -> bool:
        """Merge XML fragments into a target file, avoiding duplicates.

        Args:
            sources: List of ``(source_path, link_name, source_text)`` tuples.
                *source_text* may be ``None`` to read the file on demand.

        Returns:
            True if the target file was modified, False otherwise.
        """
        target_text = target_path.read_text(encoding="utf-8", errors="ignore")
        original_text = target_text

        for source_path, link_name, provided_text in sources:
            source_text = (
                provided_text
                if provided_text is not None
                else source_path.read_text(encoding="utf-8", errors="ignore")
            )
            target_tag = self._resolve_target_root_tag(target_path, target_text)
            root_tag = self._peek_root_tag(source_text)
            if not root_tag:
                continue

            # Some mod files are raw fragments without a root element (e.g.
            # cfgspawnabletypes.xml containing only <type> blocks). In that
            # case, use the target file's root tag and treat the whole file as
            # the inner content.
            if root_tag == target_tag:
                inner = self._extract_inner_xml(source_text, root_tag)
            else:
                inner = source_text.strip()
                root_tag = target_tag

            if not inner:
                continue

            target_text = self._append_to_root(target_text, root_tag, inner, link_name)

        if target_text != original_text:
            target_path.write_text(target_text, encoding="utf-8")
            return True
        return False

    @staticmethod
    def _resolve_target_root_tag(target_path: Path, target_text: str) -> str:
        """Return the root element tag of the target mission file."""
        # Use the filename as a hint first, then fall back to parsing.
        name_hint = {
            "types.xml": "types",
            "cfgspawnabletypes.xml": "spawnabletypes",
            "cfgeventspawns.xml": "eventposdef",
            "events.xml": "events",
            "economy.xml": "economy",
            "cfglimitsdefinition.xml": "lists",
            "cfglimitsdefinitionuser.xml": "user_lists",
            "cfgrandompresets.xml": "randompresets",
            "cfgenvironment.xml": "env",
            "mapgroupproto.xml": "prototype",
            "mapgrouppos.xml": "map",
            "mapgroupcluster.xml": "map",
            "mapgroupdirt.xml": "map",
            "mapclusterproto.xml": "prototype",
            "cfgplayerspawnpoints.xml": "playerspawnpoints",
        }.get(target_path.name.lower())
        if name_hint:
            return name_hint
        return ModIntegrationManager._peek_root_tag(target_text) or "root"

    @staticmethod
    def _extract_inner_xml(text: str, root_tag: str) -> str:
        """Return the content between the root element's start and end tags."""
        # Use a non-greedy match for the first root element only.
        pattern = re.compile(
            rf"<{re.escape(root_tag)}[^>]*>(.*?)</{re.escape(root_tag)}>",
            re.DOTALL | re.IGNORECASE,
        )
        match = pattern.search(text)
        return match.group(1).strip() if match else ""

    @staticmethod
    def _sanitize_xml_comments(text: str) -> str:
        """Remove invalid double-dashes inside XML comments.

        DayZ mod fragments sometimes contain decorative comments like
        ``<!-- ------MUMMY------ -->`` which are not valid XML and can cause
        the whole file to be rejected by the server parser.
        """

        def _clean(match: "re.Match[str]") -> str:
            body = match.group(1)
            if "--" in body:
                clean = re.sub(r"-{2,}", "-", body)
                return "<!-- " + clean + " -->"
            return match.group(0)

        return re.sub(r"<!--(.*?)-->(?=\s*<|\s*$)", _clean, text, flags=re.DOTALL)

    def _append_to_root(
        self,
        target_text: str,
        root_tag: str,
        inner_xml: str,
        link_name: str,
    ) -> str:
        """Append inner XML to the target document's root element.

        Avoids appending entries whose ``name`` attribute already exists in the
        target document.
        """
        inner_xml = self._sanitize_xml_comments(inner_xml)
        existing_names = self._collect_existing_names(target_text, root_tag)
        new_entries = self._filter_new_entries(inner_xml, existing_names)
        if not new_entries:
            return target_text

        comment = f"\n<!-- Begin integration: {link_name} -->\n"
        footer = f"\n<!-- End integration: {link_name} -->\n"
        block = comment + new_entries + footer

        # Insert before the closing root tag.
        close_pattern = re.compile(
            rf"(</{re.escape(root_tag)}>\s*)$",
            re.IGNORECASE,
        )
        return close_pattern.sub(block + r"\1", target_text)

    @staticmethod
    def _collect_existing_names(text: str, root_tag: str) -> Set[str]:
        """Collect existing ``name`` attribute values under the root element.

        Only considers names that appear inside existing integration blocks or
        in the original vanilla portion of the file. Names inside integration
        blocks are tracked so re-applying integration stays idempotent.
        """
        names: Set[str] = set()

        # First, collect names from any existing integration blocks so that
        # re-appending the same mod does not create duplicates.
        for block in re.finditer(
            r"<!-- Begin integration: @\S+ -->(.*?)<!-- End integration: @\S+ -->",
            text,
            re.DOTALL,
        ):
            for elem in re.finditer(
                r'<([a-zA-Z_][\w.-]*)\s+[^>]*name\s*=\s*["\']([^"\']+)["\']',
                block.group(1),
            ):
                names.add(elem.group(2).lower())

        inner = ModIntegrationManager._extract_inner_xml(text, root_tag)
        if not inner:
            return names

        try:
            # Wrap in a dummy root so ElementTree can parse the fragment.
            wrapped = f"<root>{inner}</root>"
            root = ET.fromstring(wrapped)
            for elem in root.iter():
                name = elem.get("name")
                if name:
                    names.add(name.lower())
        except ET.ParseError:
            pass

        return names

    @staticmethod
    def _filter_new_entries(inner_xml: str, existing_names: Set[str]) -> str:
        """Return only top-level entries whose ``name`` is not already present."""
        # Split the fragment into top-level elements by matching start/end tags.
        # This is intentionally permissive because DayZ XML is not always strict.
        entries: List[str] = []
        pos = 0
        while pos < len(inner_xml):
            # Skip whitespace and comments between top-level entries.
            remainder = inner_xml[pos:].lstrip()
            skipped = len(inner_xml[pos:]) - len(remainder)
            pos += skipped
            if pos >= len(inner_xml):
                break

            match = re.search(r"<([a-zA-Z_][\w.-]*)", inner_xml[pos:])
            if not match:
                break

            tag = match.group(1)
            start = pos + match.start()

            # Try a matching closing tag first, then fall back to self-closing.
            end_match = re.search(
                rf"</{re.escape(tag)}>\s*",
                inner_xml[start:],
                re.IGNORECASE,
            )
            if end_match:
                end = start + end_match.end()
            else:
                self_close = re.search(
                    rf"<{re.escape(tag)}[^>]*/>\s*",
                    inner_xml[start:],
                    re.IGNORECASE,
                )
                if not self_close:
                    break
                end = start + self_close.end()

            entry = inner_xml[start:end].strip()

            name_match = re.search(r'name\s*=\s*["\']([^"\']+)["\']', entry)
            if name_match:
                name = name_match.group(1).lower()
                if name not in existing_names:
                    entries.append(entry)
                    existing_names.add(name)
            else:
                # Keep entries without a name attribute (e.g. some event blocks).
                entries.append(entry)

            pos = end

        return "\n".join(entries)

    # ------------------------------------------------------------------
    # Restore
    # ------------------------------------------------------------------
    def restore_backups(self) -> ModIntegrationResult:
        """Restore the pristine mission file backups for this instance."""
        result = ModIntegrationResult(success=True)
        backup_dir = self.instance_root / "backups" / "mission"
        if not backup_dir.exists():
            result.messages.append("No backups found.")
            return result

        mission_dir = self._find_mission_dir()
        if mission_dir is None:
            result.errors.append("No mission directory found to restore into.")
            result.success = False
            return result

        for backup in backup_dir.iterdir():
            if not backup.name.endswith(".original"):
                continue
            safe_name = backup.name[:-len(".original")]
            rel_path = safe_name.replace("__", "/")
            target_path = mission_dir / rel_path
            try:
                shutil.copy2(backup, target_path)
                result.messages.append(f"Restored {rel_path}")
                result.modified_files.append(target_path)
            except OSError as exc:
                result.errors.append(f"Failed to restore {rel_path}: {exc}")

        if result.ok:
            self.save_state([])

        return result

    # ------------------------------------------------------------------
    # Summary
    # ------------------------------------------------------------------
    def summarize_mod(self, mod_folder: Path) -> Dict[str, any]:
        """Return a human-readable summary of a mod's integration fragments."""
        fragments = self.scan_mod(mod_folder)
        by_target: Dict[str, List[str]] = {}
        for fragment in fragments:
            by_target.setdefault(fragment.target_mission_path, []).append(
                fragment.source_path.name
            )
        return {
            "link_name": "@" + mod_folder.name.lstrip("@"),
            "fragments": [
                {"target": target, "files": files}
                for target, files in by_target.items()
            ],
        }
