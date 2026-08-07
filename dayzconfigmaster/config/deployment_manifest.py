# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Deployment manifest for per-instance server file preparation.

The manifest captures all inputs that affect what ends up on disk when an
instance is started. If the manifest for the current configuration matches the
last-deployed manifest, the slow copy/symlink/integration steps can be skipped.
"""

from __future__ import annotations

import hashlib
import json
import os
from dataclasses import asdict, dataclass, field
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple


def _file_fingerprint(path: Path) -> str:
    """Return a fast fingerprint for a file (mtime + size + sha256 head)."""
    try:
        stat = path.stat()
        with path.open("rb") as f:
            head = f.read(65536)
        digest = hashlib.sha256(head).hexdigest()[:16]
        return f"{stat.st_mtime}:{stat.st_size}:{digest}"
    except OSError:
        return ""


def _dir_fingerprint(path: Path) -> str:
    """Return a fingerprint summarising a directory tree.

    For mod folders this avoids a full sha256 walk; we record the existence of
    key marker files (meta.cpp) and aggregate file sizes/mtimes.
    """
    if not path.exists():
        return ""

    meta_fingerprints: List[str] = []
    total_size = 0
    total_mtime = 0.0
    file_count = 0

    # Walk with a bounded depth. Real mod folders can be huge.
    for root, _dirs, files in os.walk(path):
        for name in files:
            if name.lower() == "meta.cpp":
                meta_fingerprints.append(_file_fingerprint(Path(root) / name))
            try:
                st = (Path(root) / name).stat()
                total_size += st.st_size
                total_mtime += st.st_mtime
                file_count += 1
            except OSError:
                continue

    meta_fingerprints.sort()
    meta_part = "|".join(meta_fingerprints)
    return f"files={file_count}:size={total_size}:mtime_sum={total_mtime:.6f}:meta=({meta_part})"


@dataclass
class DeploymentManifest:
    """Inputs that determine whether an instance deployment is up to date."""

    instance_id: int
    map_name: str
    dayz_server_path: str
    instance_root: str
    mods_str: str
    mission_source_path: Optional[str] = None
    spawn_loadout_json: str = "{}"
    mod_settings_overrides_json: str = "{}"
    mod_integration_state_json: str = "{}"
    cfg_content_hash: str = ""
    mod_fingerprints: Dict[str, str] = field(default_factory=dict)
    mission_source_fingerprint: str = ""
    mission_deployed_fingerprint: str = ""
    version: int = 2

    def to_dict(self) -> Dict[str, Any]:
        return asdict(self)

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "DeploymentManifest":
        return cls(
            instance_id=data.get("instance_id", 0),
            map_name=data.get("map_name", ""),
            dayz_server_path=data.get("dayz_server_path", ""),
            instance_root=data.get("instance_root", ""),
            mods_str=data.get("mods_str", ""),
            mission_source_path=data.get("mission_source_path"),
            spawn_loadout_json=data.get("spawn_loadout_json", "{}"),
            mod_settings_overrides_json=data.get("mod_settings_overrides_json", "{}"),
            mod_integration_state_json=data.get("mod_integration_state_json", "{}"),
            cfg_content_hash=data.get("cfg_content_hash", ""),
            mod_fingerprints=data.get("mod_fingerprints", {}),
            mission_source_fingerprint=data.get("mission_source_fingerprint", ""),
            mission_deployed_fingerprint=data.get("mission_deployed_fingerprint", ""),
            version=data.get("version", 2),
        )

    def equals(self, other: "DeploymentManifest") -> bool:
        """Return True if the two manifests describe an identical deployment."""
        return (
            self.instance_id == other.instance_id
            and self.map_name == other.map_name
            and self.dayz_server_path == other.dayz_server_path
            and self.instance_root == other.instance_root
            and self.mods_str == other.mods_str
            and self.mission_source_path == other.mission_source_path
            and self.spawn_loadout_json == other.spawn_loadout_json
            and self.mod_settings_overrides_json == other.mod_settings_overrides_json
            and self.mod_integration_state_json == other.mod_integration_state_json
            and self.cfg_content_hash == other.cfg_content_hash
            and self.mod_fingerprints == other.mod_fingerprints
            and self.mission_source_fingerprint == other.mission_source_fingerprint
            and self.mission_deployed_fingerprint == other.mission_deployed_fingerprint
            and self.version == other.version
        )


class DeploymentManifestManager:
    """Compute, save and load deployment manifests for an instance."""

    MANIFEST_FILENAME = "dcm_deployed_manifest.json"

    def __init__(self, instance_root: Path):
        self.instance_root = Path(instance_root)
        self.manifest_path = self.instance_root / self.MANIFEST_FILENAME

    def load(self) -> Optional[DeploymentManifest]:
        if not self.manifest_path.exists():
            return None
        try:
            data = json.loads(self.manifest_path.read_text(encoding="utf-8"))
            return DeploymentManifest.from_dict(data)
        except (json.JSONDecodeError, OSError):
            return None

    def save(self, manifest: DeploymentManifest) -> None:
        self.instance_root.mkdir(parents=True, exist_ok=True)
        self.manifest_path.write_text(
            json.dumps(manifest.to_dict(), indent=2, ensure_ascii=False),
            encoding="utf-8",
        )

    def compute(
        self,
        instance: Dict[str, Any],
        dayz_server_path: Path,
        instance_root: Path,
        mods_str: str,
        mission_source_path: Optional[Path],
        spawn_loadout: Optional[object],
        mod_settings_overrides: Optional[Dict[str, Any]],
        mod_integration_state: Optional[Dict[str, Any]],
        cfg_content: str,
        resolve_mod_source: Optional[callable] = None,
    ) -> DeploymentManifest:
        """Build a manifest from the current configuration state.

        ``resolve_mod_source`` should accept a mod token and return the real
        source folder (e.g. the workshop item). If omitted, instance-local
        wrappers are fingerprinted, which is less stable.
        """
        instance_id = int(instance.get("id", {}).get() or 1)
        map_name = instance.get("map", {}).get() or ""

        mod_fingerprints: Dict[str, str] = {}
        for raw_mod in [m.strip() for m in mods_str.split(";") if m.strip()]:
            mod_path: Optional[Path] = None
            if resolve_mod_source is not None:
                try:
                    mod_path = resolve_mod_source(raw_mod)
                except Exception:
                    mod_path = None
            if mod_path is None:
                mod_path = Path(raw_mod)
                if not mod_path.is_absolute():
                    mod_path = dayz_server_path / raw_mod
            if mod_path is not None and mod_path.exists():
                mod_fingerprints[raw_mod] = _dir_fingerprint(mod_path)
            else:
                mod_fingerprints[raw_mod] = ""

        mission_source_fingerprint = ""
        if mission_source_path is not None and mission_source_path.exists():
            mission_source_fingerprint = _dir_fingerprint(mission_source_path)

        mission_deployed_fingerprint = ""
        deployed_mission_dir = self._find_deployed_mission_dir(instance_root)
        if deployed_mission_dir is not None:
            mission_deployed_fingerprint = _dir_fingerprint(deployed_mission_dir)

        def _jsonify(obj: Any) -> str:
            try:
                return json.dumps(obj, sort_keys=True, ensure_ascii=False)
            except TypeError:
                return ""

        return DeploymentManifest(
            instance_id=instance_id,
            map_name=map_name,
            dayz_server_path=str(dayz_server_path),
            instance_root=str(instance_root),
            mods_str=mods_str,
            mission_source_path=str(mission_source_path) if mission_source_path else None,
            spawn_loadout_json=_jsonify(spawn_loadout),
            mod_settings_overrides_json=_jsonify(mod_settings_overrides),
            mod_integration_state_json=_jsonify(mod_integration_state),
            cfg_content_hash=hashlib.sha256(cfg_content.encode("utf-8")).hexdigest()[:32],
            mod_fingerprints=mod_fingerprints,
            mission_source_fingerprint=mission_source_fingerprint,
            mission_deployed_fingerprint=mission_deployed_fingerprint,
        )

    @staticmethod
    def _find_deployed_mission_dir(instance_root: Path) -> Optional[Path]:
        """Locate the active mission directory inside the instance."""
        mpmissions = instance_root / "mpmissions"
        if not mpmissions.exists():
            return None
        for item in mpmissions.iterdir():
            if item.is_dir() and "dayzoffline" in item.name.lower():
                return item
        for item in mpmissions.iterdir():
            if item.is_dir() and (item / "db" / "types.xml").exists():
                return item
        return None

    def is_up_to_date(self, current: DeploymentManifest) -> bool:
        """Return True if the saved manifest matches the current one."""
        saved = self.load()
        if saved is None:
            return False
        return saved.equals(current)

    def mark_forced(self) -> None:
        """Invalidate the saved manifest so the next start does a full deploy."""
        if self.manifest_path.exists():
            try:
                self.manifest_path.unlink()
            except OSError:
                pass


def compute_quick_skip_status(
    instance_root: Path,
    current: DeploymentManifest,
) -> Tuple[bool, str]:
    """Return (can_skip, reason).

    ``can_skip`` is True only when the saved manifest matches *and* the
    expected output directories still exist.
    """
    manager = DeploymentManifestManager(instance_root)
    saved = manager.load()
    if saved is None:
        return False, "no previous deployment manifest"
    if not saved.equals(current):
        return False, "configuration changed since last deployment"

    # Verify a few key outputs exist.
    root = Path(current.instance_root)
    mission_dir = manager._find_deployed_mission_dir(root)
    if mission_dir is None or not mission_dir.exists():
        return False, "mission folder missing"

    return True, "manifest matches and outputs present"
