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

from .types_xml import TypesXml


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
    "mi8", "mi17", "mi24", "hind", "ka60", "kamov",
    "blackhawk", "uh60", "apache", "ah64", "ah1", "ah1z", "cobra",
    "as350", "ec135", "r22", "bell407", "bell429",
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
    "rffs", "expansionhelicopter", "expansionplane",
)

_AIR_RE = re.compile(
    "|".join(re.escape(k) for k in _AIR_KEYWORDS), re.IGNORECASE
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
    return bool(_AIR_RE.search(name))


def ensure_aircraft_lifetime(
    types_path: Path,
    max_lifetime: int = MAX_VEHICLE_LIFETIME,
) -> AircraftLifetimeResult:
    """Set ``<lifetime>`` to *max_lifetime* for all aircraft types.

    Args:
        types_path: Path to the mission ``db/types.xml`` file.
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
