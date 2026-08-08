# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Ensure aircraft and helicopter types have the maximum lifetime.

DayZ deletes vehicles whose ``<lifetime>`` has elapsed once players leave
proximity. Mod aircraft and helicopters are frequently shipped with a very
short default lifetime (often 3 seconds), which removes admin-placed spawns.
This module normalizes their ``<lifetime>`` to the 45-day maximum so placed
aircraft stay where they are.
"""

import re
from dataclasses import dataclass, field
from pathlib import Path
from typing import List, Optional

from .types_xml import TypesXml, TypeEntry


# 45 days in seconds. DayZ uses this value as the practical maximum lifetime
# for persistent vehicles; higher values still cap at server cleanup logic.
MAX_VEHICLE_LIFETIME = 3_888_000

# Class-name tokens that identify an aircraft or helicopter. Keep this list
# conservative: short/generic terms such as "chopper" match knives/axes, and
# "fighter" matches clothing. Model names and well-known mod prefixes are
# preferred.
_AIR_KEYWORDS = (
    "heli", "helicopter", "helo", "plane", "aircraft", "jet", "gyro",
    "airplane", "glider", "uav", "drone",
    "vtol", "paramotor", "paraglider", "autogyro",
    "ultralight", "microlight", "airship", "zeppelin", "blimp",
    # Common helicopter/aircraft model identifiers.
    "uh1", "huey", "mh6", "littlebird", "ch47", "chinook",
    "mi8", "mi17", "mi24", "hind", "ka60", "kamov", "ka26",
    "blackhawk", "uh60", "apache", "ah64", "ah1", "ah1z", "cobra",
    "as350", "ec135", "r22", "bell407", "bell429", "bell412",
    "c130", "hercules", "an2", "mv22", "v22", "osprey",
    "draken", "l39", "cessna",
    "ch53", "superstallion", "globemaster", "c17", "c5",
    "a10", "warthog", "thunderbolt",
    "f16", "f18", "f22", "f35",
    "su25", "su27", "mig29",
    "b1b", "b2", "tornado", "harrier",
    "p38", "p51", "bf109", "spitfire", "biplane",
    "h125", "h145", "ec145", "aw139", "aw101",
    "r44", "r66", "s76", "ch146", "nh90", "merlin", "seaking", "puma",
    # Common mod prefixes.
    "expansionhelicopter", "expansionplane",
    "lm_", "ext_",
)

# Class-name prefixes used by popular aircraft/helicopter mods.
_AIR_PREFIXES = (
    "rffsheli_", "lm_", "ext_", "expansionhelicopter", "expansionplane",
    "expansionmh6", "expansionuh1h",
)

# Tokens that should only match whole underscore-delimited words. Short tokens
# such as "an2" or "plane" easily appear inside unrelated class names (e.g.
# "land_wreck_sed01_aban2_police_de" or "shirt_planeblack"), so we match them
# as standalone tokens rather than substrings.
_AIR_TOKENS = frozenset(
    k for k in _AIR_KEYWORDS if k not in _AIR_PREFIXES
)


@dataclass
class AircraftLifetimeResult:
    """Result of normalizing aircraft lifetimes in a types.xml file."""

    success: bool
    types_path: Path
    changed: List[str] = field(default_factory=list)
    skipped: List[str] = field(default_factory=list)
    backup_path: Optional[Path] = None
    error: str = ""

    @property
    def changed_count(self) -> int:
        return len(self.changed)

    @property
    def skipped_count(self) -> int:
        return len(self.skipped)


def _is_aircraft(name: str) -> bool:
    """Return True if *name* looks like an aircraft or helicopter class."""
    lower = name.lower()

    # Known mod prefixes first.
    for prefix in _AIR_PREFIXES:
        if lower.startswith(prefix):
            return True

    # Token-based matching on underscore/non-alphanumeric boundaries.
    # We also allow a keyword to appear at the start of a token when the
    # remainder is a model variant: digits (Cessna180) or a single letter
    # (C130J). This avoids matching "plane" inside "planeblack".
    tokens = re.split(r"[^a-z0-9]+", lower)
    for tok in tokens:
        if tok in _AIR_TOKENS:
            return True
        for kw in _AIR_TOKENS:
            if tok.startswith(kw):
                rest = tok[len(kw):]
                if not rest or rest.isdigit() or (len(rest) == 1 and rest.isalpha()):
                    return True
    return False


def ensure_aircraft_lifetime(
    types_path: Path,
    max_lifetime: int = MAX_VEHICLE_LIFETIME,
) -> AircraftLifetimeResult:
    """Set ``<lifetime>`` to *max_lifetime* for all aircraft types.

    Args:
        types_path: Path to a ``types.xml`` file.
        max_lifetime: Desired lifetime in seconds. Defaults to 45 days.

    Returns:
        An :class:`AircraftLifetimeResult` describing which classes were
        updated, which already had the maximum lifetime, and where the backup
        was saved.
    """
    if not types_path.exists():
        return AircraftLifetimeResult(
            success=False,
            types_path=types_path,
            error=f"types.xml not found at {types_path}",
        )

    types_xml = TypesXml.from_file(str(types_path))
    if types_xml is None:
        return AircraftLifetimeResult(
            success=False,
            types_path=types_path,
            error=f"Could not parse {types_path}",
        )

    # Make the built-in backup helper aware of the source path.
    types_xml._last_loaded_path = str(types_path)

    changed: List[str] = []
    skipped: List[str] = []

    for _, entry in types_xml.get_all_types().items():
        name = entry.name
        if not _is_aircraft(name):
            continue
        if entry.lifetime >= max_lifetime:
            skipped.append(name)
            continue
        changed.append(name)

    if not changed:
        return AircraftLifetimeResult(
            success=True,
            types_path=types_path,
            changed=[],
            skipped=skipped,
        )

    # Snapshot the original state before mutating anything.
    backup_path = types_xml.backup_types()

    for name in changed:
        entry = types_xml.get_type(name)
        if entry is None:
            continue
        entry.lifetime = max_lifetime
        types_xml.set_type(entry)

    if types_xml.save(str(types_path)):
        return AircraftLifetimeResult(
            success=True,
            types_path=types_path,
            changed=changed,
            skipped=skipped,
            backup_path=backup_path,
        )

    return AircraftLifetimeResult(
        success=False,
        types_path=types_path,
        changed=changed,
        error=f"Could not save {types_path}",
    )


@dataclass
class AircraftMergeResult:
    """Result of merging aircraft types from root into db/types.xml."""

    success: bool
    db_types_path: Path
    added: List[str] = field(default_factory=list)
    updated: List[str] = field(default_factory=list)
    backup_path: Optional[Path] = None
    error: str = ""

    @property
    def added_count(self) -> int:
        return len(self.added)

    @property
    def updated_count(self) -> int:
        return len(self.updated)


def ensure_aircraft_types_in_db(
    mission_dir: Path,
    max_lifetime: int = MAX_VEHICLE_LIFETIME,
) -> AircraftMergeResult:
    """Merge aircraft entries from root ``types.xml`` into ``db/types.xml``.

    DayZ loads ``db/types.xml`` by default. Modded servers often keep the full
    mod type list in the mission root ``types.xml``, but those classes are
    invisible to Central Economy unless they are also present (or referenced)
    in ``db/types.xml``. This copies any aircraft classes from root that are
    missing from ``db/types.xml`` and ensures their lifetime is maxed.

    Args:
        mission_dir: The mission folder (e.g. ``mpmissions/dayzOffline.enoch``).
        max_lifetime: Desired lifetime in seconds. Defaults to 45 days.

    Returns:
        An :class:`AircraftMergeResult` describing what was copied/updated.
    """
    root_types = mission_dir / "types.xml"
    db_types = mission_dir / "db" / "types.xml"

    if not root_types.exists():
        return AircraftMergeResult(
            success=False,
            db_types_path=db_types,
            error=f"Root types.xml not found at {root_types}",
        )
    if not db_types.exists():
        return AircraftMergeResult(
            success=False,
            db_types_path=db_types,
            error=f"db/types.xml not found at {db_types}",
        )

    root_xml = TypesXml.from_file(str(root_types))
    db_xml = TypesXml.from_file(str(db_types))
    if root_xml is None:
        return AircraftMergeResult(
            success=False,
            db_types_path=db_types,
            error=f"Could not parse {root_types}",
        )
    if db_xml is None:
        return AircraftMergeResult(
            success=False,
            db_types_path=db_types,
            error=f"Could not parse {db_types}",
        )

    db_xml._last_loaded_path = str(db_types)

    added: List[str] = []
    updated: List[str] = []

    for _, entry in root_xml.get_all_types().items():
        if not _is_aircraft(entry.name):
            continue
        existing = db_xml.get_type(entry.name)
        if existing is None:
            # Copy the entry and force max lifetime.
            new_entry = TypeEntry(
                name=entry.name,
                categories=list(entry.categories),
                usages=list(entry.usages),
                values=list(entry.values),
                nominal=entry.nominal,
                min=entry.min,
                lifetime=max_lifetime,
                restock=entry.restock,
                quantmin=entry.quantmin,
                quantmax=entry.quantmax,
                cost=entry.cost,
                tags=list(entry.tags),
                flags=dict(entry.flags),
            )
            db_xml.set_type(new_entry)
            added.append(entry.name)
        elif existing.lifetime < max_lifetime:
            existing.lifetime = max_lifetime
            db_xml.set_type(existing)
            updated.append(entry.name)

    if not added and not updated:
        return AircraftMergeResult(
            success=True,
            db_types_path=db_types,
            added=[],
            updated=[],
        )

    backup_path = db_xml.backup_types()

    if db_xml.save(str(db_types)):
        return AircraftMergeResult(
            success=True,
            db_types_path=db_types,
            added=added,
            updated=updated,
            backup_path=backup_path,
        )

    return AircraftMergeResult(
        success=False,
        db_types_path=db_types,
        added=added,
        updated=updated,
        error=f"Could not save {db_types}",
    )


# Regex for the "SCRIPT : ClassName" lines that DayZ mods print on startup.
# Example: " SCRIPT       : RFFSHeli_UH1H_Heli"
_SCRIPT_LOG_CLASS_RE = re.compile(
    r"^\s*SCRIPT\s+\(?(?:W|E)?\)?\s*:?\s*(?P<class>[A-Za-z_][A-Za-z0-9_]*)",
    re.MULTILINE | re.IGNORECASE,
)


@dataclass
class AircraftImportResult:
    """Result of importing aircraft classes discovered from script logs."""

    success: bool
    db_types_path: Path
    imported: List[str] = field(default_factory=list)
    backup_path: Optional[Path] = None
    error: str = ""

    @property
    def imported_count(self) -> int:
        return len(self.imported)


def _base_rffs_vehicle_name(script_name: str) -> Optional[str]:
    """Return the base CfgVehicles name for an RFFSHeli script class.

    RFFSHeli prints script module classes such as ``RFFSHeli_UH1H_Heli`` to
    the script log, but the actual placeable vehicle class in CfgVehicles is
    ``RFFSHeli_UH1H`` (without the ``_Heli`` suffix). DayZ's Central Economy
    only knows about the CfgVehicles class, so we must ensure the base name
    is present in types.xml.
    """
    lower = script_name.lower()
    if not lower.startswith("rffsheli_") or not lower.endswith("_heli"):
        return None
    base = script_name[:-5]  # strip trailing "_Heli"
    if not base or base == script_name:
        return None
    return base


def discover_aircraft_classes_from_script_logs(
    profiles_dir: Path,
) -> List[str]:
    """Return aircraft-looking class names printed in script logs.

    Mods such as RFFSHeli and LM print their vehicle class names to the
    script log on server startup (``SCRIPT : ClassName``). We harvest those
    lines and keep names that look like aircraft or helicopters.

    For RFFSHeli we also emit the base CfgVehicles class (e.g.
    ``RFFSHeli_UH1H``) alongside the script class (``RFFSHeli_UH1H_Heli``)
    because DayZ uses the base class for lifetime/persistence.
    """
    classes: List[str] = []
    if not profiles_dir.exists():
        return classes

    # Look at the most recent script logs.
    logs = sorted(profiles_dir.glob("script_*.log"), reverse=True)[:5]
    seen = set()
    for log_path in logs:
        try:
            text = log_path.read_text(encoding="utf-8", errors="ignore")
        except OSError:
            continue
        for match in _SCRIPT_LOG_CLASS_RE.finditer(text):
            name = match.group("class")
            if not name or name.lower() in seen:
                continue
            if not _is_aircraft(name):
                continue
            # Avoid generic script/base/container classes that are not
            # placeable vehicles.
            lower = name.lower()
            if lower in ("rffsheli_base", "lm_aircraft", "lm_helicopters",
                         "lm_planes", "lm_jets", "lm_plane_assets"):
                continue
            if lower.endswith("_mod") or lower.endswith("_core"):
                continue
            if lower in ("rffsheli_define", "rffsheli_misc", "rffsheli_uniform",
                         "lm_gunner", "lm_target_systems"):
                continue
            seen.add(lower)
            classes.append(name)
            # RFFSHeli script classes use a "_Heli" suffix; make sure the
            # actual CfgVehicles class (without the suffix) is also emitted.
            base = _base_rffs_vehicle_name(name)
            if base and base.lower() not in seen:
                seen.add(base.lower())
                classes.append(base)

    return classes


def import_missing_aircraft_classes_to_db(
    mission_dir: Path,
    profiles_dir: Path,
    max_lifetime: int = MAX_VEHICLE_LIFETIME,
) -> AircraftImportResult:
    """Import aircraft classes from script logs into ``db/types.xml``.

    This catches script-defined mod aircraft (e.g. ``RFFSHeli_UH1H_Heli``,
    ``LM_MH6``) that do not appear in any ``types.xml`` but are printed to
    the script log at startup. Without a ``types.xml`` entry, they fall back
    to DayZ's short default lifetime.

    Args:
        mission_dir: The mission folder.
        profiles_dir: The server profiles directory containing ``script_*.log``.
        max_lifetime: Desired lifetime in seconds.

    Returns:
        An :class:`AircraftImportResult` describing imported classes.
    """
    db_types = mission_dir / "db" / "types.xml"
    if not db_types.exists():
        return AircraftImportResult(
            success=False,
            db_types_path=db_types,
            error=f"db/types.xml not found at {db_types}",
        )

    db_xml = TypesXml.from_file(str(db_types))
    if db_xml is None:
        return AircraftImportResult(
            success=False,
            db_types_path=db_types,
            error=f"Could not parse {db_types}",
        )

    db_xml._last_loaded_path = str(db_types)

    discovered = discover_aircraft_classes_from_script_logs(profiles_dir)
    imported: List[str] = []

    for name in discovered:
        existing = db_xml.get_type(name)
        from .types_xml import Category
        if existing is not None:
            # Repair existing entries that are missing the vehicle category.
            if not any(cat.name.lower() == "vehicle" for cat in existing.categories):
                existing.categories.append(Category(name="vehicle"))
                if existing.lifetime < max_lifetime:
                    existing.lifetime = max_lifetime
                db_xml.set_type(existing)
                imported.append(name)
            continue
        entry = TypeEntry(
            name=name,
            nominal=0,
            min=0,
            lifetime=max_lifetime,
            restock=0,
        )
        # Add the standard vehicle category so CE treats these the same as
        # persisted airplanes/vehicles.
        entry.categories.append(Category(name="vehicle"))
        db_xml.set_type(entry)
        imported.append(name)

    if not imported:
        return AircraftImportResult(
            success=True,
            db_types_path=db_types,
            imported=[],
        )

    backup_path = db_xml.backup_types()

    if db_xml.save(str(db_types)):
        return AircraftImportResult(
            success=True,
            db_types_path=db_types,
            imported=imported,
            backup_path=backup_path,
        )

    return AircraftImportResult(
        success=False,
        db_types_path=db_types,
        imported=imported,
        error=f"Could not save {db_types}",
    )
