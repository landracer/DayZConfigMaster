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
from urllib.error import URLError
from urllib.request import urlopen

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

# Substrings that identify parts, wrecks, static objects, containers, clothing,
# or other mod assets that are not placeable aircraft/vehicles. These must be
# rejected before any aircraft prefix/token matching so we never promote a
# wreck or spraycan to a dynamically-spawning vehicle.
_NON_AIRCRAFT_SUBSTRINGS = (
    "wreck", "wrecked", "crashed", "crashsite",
    "wheel", "tire", "spraycan", "carcover",
    "crate", "static", "uniform", "clothing",
)

# Suffixes used by mod module/base classes that are not placeable vehicles.
_NON_AIRCRAFT_SUFFIXES = ("_mod", "_core", "_base", "_define")


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

    # Reject obvious non-aircraft substrings first.
    for bad in _NON_AIRCRAFT_SUBSTRINGS:
        if bad in lower:
            return False

    # Reject mod module/base classes.
    for bad in _NON_AIRCRAFT_SUFFIXES:
        if lower.endswith(bad):
            return False

    # Mod prefixes that are too generic on their own must be paired with an
    # aircraft token (e.g. ext_mi8, lm_a10). This prevents lm_patty_wagon,
    # ext_mi24_wheel_*, ext_spraycan_*, etc. from being treated as aircraft.
    if lower.startswith(("ext_", "lm_")):
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

    # Known aircraft mod prefixes.
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

# Official RFFSHeli types.xml from the mod author's GitHub. It contains the
# authoritative entries for all helicopters, wrecks, parts and gear. Merging
# this into db/types.xml is the most reliable way to prevent RFFSHeli
# helicopters from despawning and to make RFFSHeli loot spawn correctly.
_RFFSHELI_TYPES_URL = (
    "https://raw.githubusercontent.com/RedFalconKen/RedFalconFlightSystem-Heliz/"
    "main/Config%20Files/Types.XML/RFFSHelis_Types.xml"
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


def _is_official_rffsheli_type(name: str) -> bool:
    """Return True if *name* is defined in the official RFFSHeli types.xml."""
    lower = name.lower()
    if not lower.startswith("rffsheli_"):
        return False
    # Cached copy of the official file, populated on first call.
    if not hasattr(_is_official_rffsheli_type, "_names"):
        try:
            with urlopen(_RFFSHELI_TYPES_URL, timeout=15) as response:
                text = response.read().decode("utf-8", errors="ignore")
            xml = TypesXml.from_text(text)
            _is_official_rffsheli_type._names = {
                n.lower() for n in xml.get_all_types().keys()
            } if xml else set()
        except (URLError, OSError, Exception):
            _is_official_rffsheli_type._names = set()
    return lower in _is_official_rffsheli_type._names


def ensure_rffsheli_types_in_db(
    mission_dir: Path,
    max_lifetime: int = MAX_VEHICLE_LIFETIME,
) -> AircraftMergeResult:
    """Merge the official RFFSHeli types.xml entries into ``db/types.xml``.

    The RFFSHeli mod ships many helicopter variants, wrecks, parts and
    clothing items. The mod author's GitHub provides a reference
    ``RFFSHelis_Types.xml`` with the correct ``<lifetime>``, ``<category>``,
    ``<usage>`` and ``<tag>`` values. Copying missing entries into
    ``db/types.xml`` prevents placed helicopters from despawning and lets
    RFFSHeli loot spawn naturally.

    Existing entries are left untouched unless their lifetime is shorter than
    *max_lifetime*.

    Args:
        mission_dir: The mission folder.
        max_lifetime: Desired lifetime in seconds.

    Returns:
        An :class:`AircraftMergeResult` describing added/updated entries.
    """
    db_types = mission_dir / "db" / "types.xml"
    if not db_types.exists():
        return AircraftMergeResult(
            success=False,
            db_types_path=db_types,
            error=f"db/types.xml not found at {db_types}",
        )

    try:
        with urlopen(_RFFSHELI_TYPES_URL, timeout=15) as response:
            text = response.read().decode("utf-8", errors="ignore")
    except (URLError, OSError) as exc:
        return AircraftMergeResult(
            success=False,
            db_types_path=db_types,
            error=f"Could not download official RFFSHeli types.xml: {exc}",
        )

    official_xml = TypesXml.from_text(text)
    db_xml = TypesXml.from_file(str(db_types))
    if official_xml is None:
        return AircraftMergeResult(
            success=False,
            db_types_path=db_types,
            error="Could not parse official RFFSHeli types.xml",
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

    for _key, entry in official_xml.get_all_types().items():
        name = entry.name
        existing = db_xml.get_type(name)
        if existing is None:
            # Copy the official entry verbatim, capping lifetime at max.
            new_entry = TypeEntry(
                name=entry.name,
                categories=list(entry.categories),
                usages=list(entry.usages),
                values=list(entry.values),
                nominal=entry.nominal,
                min=entry.min,
                lifetime=max(entry.lifetime, max_lifetime)
                if entry.lifetime < max_lifetime
                else entry.lifetime,
                restock=entry.restock,
                quantmin=entry.quantmin,
                quantmax=entry.quantmax,
                cost=entry.cost,
                tags=list(entry.tags),
                flags=dict(entry.flags),
            )
            db_xml.set_type(new_entry)
            added.append(name)
        elif existing.lifetime < max_lifetime and entry.lifetime >= max_lifetime:
            existing.lifetime = max_lifetime
            db_xml.set_type(existing)
            updated.append(name)

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
        lower = name.lower()
        # RFFSHeli script module classes use a "_Heli" suffix. The actual
        # CfgVehicles classes are handled by ensure_rffsheli_types_in_db(),
        # so we skip the script-only suffix here to avoid cluttering the DB.
        if lower.startswith("rffsheli_") and lower.endswith("_heli"):
            continue
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


@dataclass
class AircraftCleanupResult:
    """Result of cleaning bogus aircraft entries from db/types.xml."""

    success: bool
    db_types_path: Path
    removed: List[str] = field(default_factory=list)
    backup_path: Optional[Path] = None
    error: str = ""

    @property
    def removed_count(self) -> int:
        return len(self.removed)


# Substrings used to identify entries that DCM mistakenly promoted to dynamic
# vehicle spawns. These are wrecks, parts, static objects, etc. that were
# copied into db/types.xml with category=vehicle and usage=Town.
_BOGUS_VEHICLE_SPAWN_SUBSTRINGS = (
    "wreck", "wheel", "spraycan", "carcover", "crate", "static",
)


def _is_bogus_vehicle_spawn(entry: TypeEntry) -> bool:
    """Return True if *entry* is a wreck/part/static object masquerading as a vehicle."""
    lower = entry.name.lower()

    # Must look like a non-aircraft asset.
    if not any(bad in lower for bad in _BOGUS_VEHICLE_SPAWN_SUBSTRINGS):
        return False

    # Must be flagged as a vehicle so CE tries to spawn it dynamically.
    if not any(cat.name.lower() == "vehicle" for cat in entry.categories):
        return False

    # Must have a town usage to spawn in built-up areas.
    usages = {use.name.lower() for use in entry.usages}
    if "town" not in usages:
        return False

    # Must not be a real aircraft class after tightening the detector.
    if _is_aircraft(entry.name):
        return False

    return True


def remove_bogus_vehicle_spawns(
    db_types_path: Path,
    max_lifetime: int = MAX_VEHICLE_LIFETIME,
) -> AircraftCleanupResult:
    """Remove DCM-added wreck/part/static entries that spawn as vehicles in towns.

    Earlier versions of the aircraft detector copied entries such as
    ``land_wreck_hb01_aban2``, ``ext_mi24_wheel_*``, ``ext_spraycan_*``,
    ``statichelicrash`` and ``staticairplanecrate`` from root ``types.xml``
    into ``db/types.xml`` with ``category="vehicle"`` and ``usage="Town"``.
    DayZ's Central Economy then spawned those wrecks/parts inside houses.
    This function strips those bogus entries back out.

    Args:
        db_types_path: Path to ``db/types.xml``.
        max_lifetime: The DCM max-lifetime value used to identify touched
            entries (defaults to 45 days).

    Returns:
        An :class:`AircraftCleanupResult` describing removed entries.
    """
    if not db_types_path.exists():
        return AircraftCleanupResult(
            success=False,
            db_types_path=db_types_path,
            error=f"db/types.xml not found at {db_types_path}",
        )

    db_xml = TypesXml.from_file(str(db_types_path))
    if db_xml is None:
        return AircraftCleanupResult(
            success=False,
            db_types_path=db_types_path,
            error=f"Could not parse {db_types_path}",
        )

    db_xml._last_loaded_path = str(db_types_path)

    removed: List[str] = []
    for name in list(db_xml.get_all_types().keys()):
        entry = db_xml.get_type(name)
        if entry is None:
            continue
        if not _is_bogus_vehicle_spawn(entry):
            continue
        # Only remove entries that DCM touched (max lifetime).
        if entry.lifetime != max_lifetime:
            continue
        db_xml.remove_type(name)
        removed.append(entry.name)

    if not removed:
        return AircraftCleanupResult(
            success=True,
            db_types_path=db_types_path,
            removed=[],
        )

    backup_path = db_xml.backup_types()

    if db_xml.save(str(db_types_path)):
        return AircraftCleanupResult(
            success=True,
            db_types_path=db_types_path,
            removed=removed,
            backup_path=backup_path,
        )

    return AircraftCleanupResult(
        success=False,
        db_types_path=db_types_path,
        removed=removed,
        error=f"Could not save {db_types_path}",
    )


@dataclass
class VehicleSpawnRepairResult:
    """Result of repairing vehicle spawn usage tags in a types.xml file."""

    success: bool
    types_path: Path
    removed: List[str] = field(default_factory=list)
    repaired: List[str] = field(default_factory=list)
    backup_path: Optional[Path] = None
    error: str = ""

    @property
    def removed_count(self) -> int:
        return len(self.removed)

    @property
    def repaired_count(self) -> int:
        return len(self.repaired)


def repair_vehicle_spawn_usages(
    types_path: Path,
    max_lifetime: int = MAX_VEHICLE_LIFETIME,
) -> VehicleSpawnRepairResult:
    """Repair DCM spawn-generator damage in a ``types.xml`` file.

    The spawn generator used to assign ``usage="Town"`` to every integrated
    class, including vehicles, aircraft and boats.  Vehicles/air/water are
    spawned through events.xml, so a ``Town`` usage tells Central Economy to
    place them as dynamic loot inside houses.  This function:

    * Strips ``Town`` usage from aircraft and from ``air``/``water`` entries.
    * Removes bogus ``vehicle`` entries that have ``Town`` usage and are not
      aircraft (cars, boats, parts, wrecks that were promoted to dynamic
      spawns).

    Args:
        types_path: Path to the ``types.xml`` file to repair.
        max_lifetime: The DCM max-lifetime value used to identify spawn-
            generator entries (defaults to 45 days).

    Returns:
        A :class:`VehicleSpawnRepairResult` describing removed and repaired
        entries.
    """
    if not types_path.exists():
        return VehicleSpawnRepairResult(
            success=False,
            types_path=types_path,
            error=f"types.xml not found at {types_path}",
        )

    xml = TypesXml.from_file(str(types_path))
    if xml is None:
        return VehicleSpawnRepairResult(
            success=False,
            types_path=types_path,
            error=f"Could not parse {types_path}",
        )

    xml._last_loaded_path = str(types_path)

    removed: List[str] = []
    repaired: List[str] = []

    for name in list(xml.get_all_types().keys()):
        entry = xml.get_type(name)
        if entry is None:
            continue

        categories = {cat.name.lower() for cat in entry.categories}
        usages = {use.name.lower() for use in entry.usages}
        if "town" not in usages:
            continue

        def _strip_town() -> None:
            entry.usages = [
                use for use in entry.usages
                if use.name.lower() != "town"
            ]
            xml.set_type(entry)

        if "air" in categories or "water" in categories:
            _strip_town()
            repaired.append(entry.name)
        elif "vehicle" in categories:
            if _is_bogus_vehicle_spawn(entry) and entry.lifetime == max_lifetime:
                # Spawn-generator promoted a wreck/part/static object to a
                # dynamic town spawn.  Remove it entirely.
                xml.remove_type(name)
                removed.append(entry.name)
            else:
                # Real vehicles and aircraft use event spawning, not Town.
                _strip_town()
                repaired.append(entry.name)

    if not removed and not repaired:
        return VehicleSpawnRepairResult(
            success=True,
            types_path=types_path,
            removed=[],
            repaired=[],
        )

    backup_path = xml.backup_types()

    if xml.save(str(types_path)):
        return VehicleSpawnRepairResult(
            success=True,
            types_path=types_path,
            removed=removed,
            repaired=repaired,
            backup_path=backup_path,
        )

    return VehicleSpawnRepairResult(
        success=False,
        types_path=types_path,
        removed=removed,
        repaired=repaired,
        error=f"Could not save {types_path}",
    )
