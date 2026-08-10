# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Mod integration workflow logic for DayZ server configuration files.

This module provides headless business logic for enabling mod features by
editing the core server XML files (events.xml, cfgspawnabletypes.xml,
types.xml, cfgeventspawns.xml).  It is intentionally kept free of Tkinter so
it can be unit-tested and reused outside the GUI.
"""

from __future__ import annotations

import random
import re
import shutil
import time
from dataclasses import dataclass, field
from datetime import datetime
from pathlib import Path
from typing import Dict, List, Optional, Tuple
from xml.etree import ElementTree as ET


@dataclass
class IntegrationAction:
    """A single proposed/executed configuration change."""
    file_name: str
    description: str
    applied: bool = False
    error: str = ""


@dataclass
class IntegrationResult:
    """Result of a mod integration workflow."""
    success: bool
    actions: List[IntegrationAction] = field(default_factory=list)
    backup_dir: Optional[Path] = None

    @property
    def ok(self) -> bool:
        # An action may be applied but carry a warning (e.g. "no wheel
        # template").  Treat those as success; only real failures count.
        return self.success and not any(a.error for a in self.actions if not a.applied)


@dataclass
class SpawnableClass:
    """A discovered class that can be added to the server's spawn tables."""
    name: str
    source: str
    category: str  # vehicle, air, water, weapon, gear, generic
    wheel: Optional[Tuple[str, int]] = None
    declared_categories: Tuple[str, ...] = ()
    declared_usages: Tuple[str, ...] = ()
    invalid_categories: Tuple[str, ...] = ()
    invalid_usages: Tuple[str, ...] = ()
    confidence: str = "heuristic"  # "ce" or "heuristic"


@dataclass
class LimitsDefinition:
    """Valid <category> and <usage> tokens declared by a mission."""
    categories: set = field(default_factory=set)
    usages: set = field(default_factory=set)

    def merge(self, other: "LimitsDefinition") -> "LimitsDefinition":
        return LimitsDefinition(
            categories=self.categories | other.categories,
            usages=self.usages | other.usages,
        )


@dataclass
class EconomyCore:
    """CE file routing declared by cfgeconomycore.xml."""
    types_files: List[Path] = field(default_factory=list)
    spawnabletypes_files: List[Path] = field(default_factory=list)
    events_files: List[Path] = field(default_factory=list)
    other_files: List[Path] = field(default_factory=list)


SUPPORTED_FILES = (
    "events.xml",
    "cfgspawnabletypes.xml",
    "types.xml",
    "cfgeventspawns.xml",
)


def parse_limits_definition(path: Path) -> LimitsDefinition:
    """Parse a cfglimitsdefinition.xml file into allowed categories/usages."""
    limits = LimitsDefinition()
    if not path.exists():
        return limits
    try:
        root = ET.parse(path).getroot()
    except ET.ParseError:
        return limits

    for cat in root.findall(".//categories/category"):
        name = cat.get("name")
        if name:
            limits.categories.add(name)

    for usage in root.findall(".//usageflags/usage"):
        name = usage.get("name")
        if name:
            limits.usages.add(name)

    # Some missions use <tag> elements inside <tags> as well.
    for tag in root.findall(".//tags/tag"):
        name = tag.get("name")
        if name:
            limits.categories.add(name)

    return limits


def load_mission_limits(mission_root: Path) -> LimitsDefinition:
    """Load and merge mission limits definitions.

    Merges the base ``cfglimitsdefinition.xml`` with
    ``cfglimitsdefinitionuser.xml`` when the latter exists.
    """
    limits = parse_limits_definition(mission_root / "cfglimitsdefinition.xml")
    user_path = mission_root / "cfglimitsdefinitionuser.xml"
    if user_path.exists():
        limits = limits.merge(parse_limits_definition(user_path))
    return limits


def parse_economy_core(mission_root: Path) -> EconomyCore:
    """Parse ``cfgeconomycore.xml`` and resolve referenced CE file paths."""
    eco = EconomyCore()
    core_path = mission_root / "cfgeconomycore.xml"
    if not core_path.exists():
        return eco
    try:
        root = ET.parse(core_path).getroot()
    except ET.ParseError:
        return eco

    for ce in root.findall("ce"):
        folder = ce.get("folder", "").strip()
        folder_path = mission_root / folder if folder else mission_root
        for file_elem in ce.findall("file"):
            name = file_elem.get("name")
            ftype = (file_elem.get("type", "") or "").lower()
            if not name:
                continue
            full_path = folder_path / name
            if ftype == "types":
                eco.types_files.append(full_path)
            elif ftype == "spawnabletypes":
                eco.spawnabletypes_files.append(full_path)
            elif ftype == "events":
                eco.events_files.append(full_path)
            else:
                eco.other_files.append(full_path)

    return eco


def _map_limit_category_to_internal(cat: str) -> Optional[str]:
    """Map a cfglimitsdefinition category to our internal spawnable category."""
    mapping = {
        "weapons": "weapon",
        "clothes": "gear",
        "containers": "gear",
        "food": "generic",
        "tools": "gear",
        "vehicles": "vehicle",
    }
    return mapping.get(cat.lower())


def _classify_from_declared_tags(
    declared_categories: set,
    limits: LimitsDefinition,
) -> Tuple[Optional[str], List[str], List[str]]:
    """Return (internal_category, invalid_categories, invalid_usages).

    Category is ``None`` when no mappable declared category is present.
    """
    invalid_cats = sorted(declared_categories - limits.categories)
    # usages are validated later when usages are known.
    mapped: Optional[str] = None
    for cat in declared_categories:
        internal = _map_limit_category_to_internal(cat)
        if internal is not None:
            # Prefer explicit weapon mapping; fall back to first match.
            if internal == "weapon" or mapped is None:
                mapped = internal
    return mapped, invalid_cats, []


VEHICLE_TEMPLATES: Dict[str, Tuple[str, int]] = {
    "OffroadHatchback": ("HatchbackWheel", 4),
    "OffroadHatchback_Blue": ("HatchbackWheel", 4),
    "OffroadHatchback_White": ("HatchbackWheel", 4),
    "Hatchback_02": ("Hatchback_02_Wheel", 4),
    "Hatchback_02_Blue": ("Hatchback_02_Wheel", 4),
    "Hatchback_02_Black": ("Hatchback_02_Wheel", 4),
    "Sedan_02": ("Sedan_02_Wheel", 4),
    "Sedan_02_Red": ("Sedan_02_Wheel", 4),
    "CivilianVan": ("Van_01_Wheel", 4),
    "CivilianVan_Black": ("Van_01_Wheel", 4),
    "CivilianVan_Wine": ("Van_01_Wheel", 4),
    "Truck_01_Covered": ("Truck_01_Wheel", 6),
    "Truck_01_Covered_Blue": ("Truck_01_Wheel", 6),
    "Truck_01_Covered_Orange": ("Truck_01_Wheel", 6),
    "Truck_01_Transport": ("Truck_01_Wheel", 6),
    "Offroad_02": ("Offroad_02_Wheel", 4),
    "Offroad_02_Blue": ("Offroad_02_Wheel", 4),
    "Offroad_02_Green": ("Offroad_02_Wheel", 4),
    "Offroad_02_Red": ("Offroad_02_Wheel", 4),
    "Offroad_02_White": ("Offroad_02_Wheel", 4),
}


# Primary vehicle keywords.  These are generic terms that unambiguously
# identify a class as a vehicle.  Brand/model tokens are deliberately NOT
# here because they also match weapons/clothing (e.g. "AK", "GTR").
_VEHICLE_KEYWORDS = (
    "car", "truck", "van", "offroad", "hatchback", "sedan", "suv", "bus",
    "bike", "motorcycle", "moto", "quad", "atv", "uaz", "v3s", "gunter",
    "sarka", "olga", "ada", "m1025", "humvee", "tank", "apc", "heli",
    "helicopter", "plane", "boat", "ship", "vehicle", "motorbike",
    "scooter", "trailer", "motorhome", "rv",
)
_VEHICLE_RE = re.compile(
    "|".join(re.escape(k) for k in _VEHICLE_KEYWORDS),
    re.IGNORECASE,
)

# Brand/model tokens that, when combined with a wheel match, confirm a
# vehicle base.  These are NOT used as standalone admission criteria.
_VEHICLE_BRAND_TOKENS = (
    "audi", "bmw", "ford", "dodge", "chevrolet", "nissan", "porsche",
    "toyota", "jeep", "gmc", "kamaz", "mitsubishi", "honda", "civic",
    "mustang", "raptor", "bronco", "challenger", "charger", "ram",
    "skyline", "supra", "tahoe", "runner", "lancer", "evo",
    "rs6", "gt3", "gtr", "nismo", "typhoon",
)
_VEHICLE_BRAND_RE = re.compile(
    "|".join(re.escape(k) for k in _VEHICLE_BRAND_TOKENS),
    re.IGNORECASE,
)

# ------------------------------------------------------------------
# Broader spawnable classification
# ------------------------------------------------------------------

# Tokens that strongly indicate a weapon class.
_WEAPON_KEYWORDS = (
    "rifle", "pistol", "shotgun", "smg", "submachine", "machinegun", "lmg",
    "hmg", "sniper", "dmr", "carbine", "launcher", "rocket", "missile",
    "grenade", "mine", "trap", "melee", "knife", "sword", "axe", "mace",
    "spear", "akm", "ak74", "ak101", "ak102", "m4a1", "m16", "ar15",
    "scar", "fal", "mosin", "svd", "vss", "mp5", "ump", "glock", "colt",
    "1911", "deagle", "revolver", "magnum", "cz", "fnx", "mak", "mp133",
    "izh", "winchester", "repeater", "blazer", "longhorn", "flaregun",
    "crossbow", "bow", "rpg", "law", "m72", "m249", "m60", "pkp", "rpk",
    "m14", "m1a", "sks", "aug", "famas", "g3", "m110", "mk17", "mk18",
    "mk14", "m24", "m40", "awp", "awm", "l96", "tundra", "cr527", "cz527",
    "ssg82", "scout",
)
_WEAPON_RE = re.compile(
    "|".join(re.escape(k) for k in _WEAPON_KEYWORDS),
    re.IGNORECASE,
)

# Tokens that strongly indicate clothing / gear / loadout classes.
_GEAR_KEYWORDS = (
    "shirt", "pants", "jeans", "jacket", "coat", "hoodie", "sweater",
    "tshirt", "vest", "platecarrier", "holster", "sheath", "pouch", "bag",
    "backpack", "duffel", "drybag", "assaultbag", "huntingbag", "tortilla",
    "canteen", "bottle", "canister", "mask", "gasmask", "bandana",
    "balaclava", "hat", "cap", "beanie", "helmet", "boonie", "ushanka",
    "goggles", "glasses", "gloves", "shoes", "boots", "sneakers",
    "jungleboots", "combatboots", "wellies", "loadout", "uniform",
    "outfit", "suit", "camo", "tactical", "military", "police", "hunter",
    "civilian", "gorka", "usmc", "bdf", "nato", "un", "medic",
    "firefighter", "ems", "press", "construction", "hiking", "moto",
    "rider", " NBC", "gorka", "bdu", "acs", "ghillie",
)
_GEAR_RE = re.compile(
    "|".join(re.escape(k) for k in _GEAR_KEYWORDS),
    re.IGNORECASE,
)

# Tokens that identify air / water vehicles.
# Keep this list conservative: short/generic substrings like "trike", "stol",
# "fighter", "bomber", or "chopper" create false positives (land trikes,
# Firefighter clothing, Bomber jackets, Chopper knives/axes) and confuse the
# category filters. Model names are safer than generic aviation terms.
_AIR_KEYWORDS = (
    "heli", "helicopter", "helo", "plane", "aircraft", "jet", "gyro",
    "airplane", "glider", "uav", "drone",
    "vtol", "paramotor", "paraglider", "autogyro",
    "ultralight", "microlight", "airship", "zeppelin", "blimp",
    # Common helicopter/aircraft model identifiers that rarely appear in
    # non-air class names.
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
)
_AIR_RE = re.compile("|".join(re.escape(k) for k in _AIR_KEYWORDS), re.IGNORECASE)
_WATER_KEYWORDS = ("boat", "ship", "yacht", "raft", "submarine", "water")
_WATER_RE = re.compile("|".join(re.escape(k) for k in _WATER_KEYWORDS), re.IGNORECASE)

# Sub-component keywords that indicate an aircraft base even when the base
# name itself does not contain an air keyword (e.g. UH1H + UH1H_Rotor).
# Avoid short substrings (e.g. "fin") that match unrelated land-vehicle words.
_AIR_PART_KEYWORDS = (
    "rotor", "blade", "skid", "propeller", "tailrotor", "mainrotor",
    "rotorblade", "rotorhead", "swashplate",
    "heli", "helicopter", "wing", "aileron", "flap", "spoiler",
    "elevator", "rudder", "stabilizer", "verticalstabilizer", "horizontalstabilizer",
    "landinggear", "nosegear", "maingear", "undercarriage",
    "cockpit", "fuselage", "tailboom", "empennage", "canopy",
    "float", "pontoon", "turbine", "pylon",
)
_WATER_PART_KEYWORDS = (
    "hull", "rudder", "sail", "anchor", "keel", "deck", "bow", "stern",
    "outboard", "inboard",
)

# Tokens that identify sub-components / consumables that should never be
# treated as standalone spawnables.
_SPAWNABLE_REJECT_KEYWORDS = (
    "_mag", "_magazine", "ammo", "round", "bullet", "shell", "cartridge",
    "suppressor", "silencer", "optic", "scope", "sight", "bipod", "bayonet",
    "buttstock", "handguard", "rail", "grip", "clip", "fuse",
    "trigger", "detonator", "repairkit", "repair_kit", "weaponcleaning",
    "weaponparts", "gunparts", "canopener", "knife_blade",
    "grenadesmoke", "grenadechem", "grenadeflash", "smokegrenade",
    "flashbang", "teargas", "gasgrenade", "roadflare", "matchbox",
    "sparkplug", "spark_plug",
)
_SPAWNABLE_REJECT_RE = re.compile(
    "|".join(re.escape(k) for k in _SPAWNABLE_REJECT_KEYWORDS),
    re.IGNORECASE,
)


def _is_workshop_mod_id(name: str) -> bool:
    """Return True if *name* looks like a numeric Steam Workshop item id."""
    try:
        int(name)
        return True
    except ValueError:
        return False


def _read_workshop_display_name(folder: Path) -> Optional[str]:
    """Read the display name from a workshop item's meta.cpp or mod.cpp."""
    name_re = re.compile(
        r'^\s*name\s*=\s*["\'](.+?)["\']\s*;',
        re.IGNORECASE | re.MULTILINE,
    )
    for filename in ("meta.cpp", "mod.cpp"):
        filepath = folder / filename
        if not filepath.exists():
            continue
        try:
            content = filepath.read_text(encoding="utf-8", errors="ignore")
            match = name_re.search(content)
            if match:
                value = match.group(1).strip()
                if value and not value.startswith(("$", "#")):
                    return value
        except (OSError, PermissionError):
            pass
    return None


def _looks_like_vehicle(name: str) -> bool:
    """Return True if *name* matches the vehicle keyword heuristic."""
    return bool(_VEHICLE_RE.search(name))


def _extract_vehicle_names_from_xml(path: Path) -> List[str]:
    """Parse *path* and return relevant class names.

    For types.xml and cfgspawnabletypes.xml we return *all* <type> names so
    that base inference (color variants + wheel matching) can identify
    mod-added vehicles even when their class name is just a brand/model token.
    For events.xml and classnames.txt-style XML we keep the keyword filter
    because those files are less structured.
    """
    try:
        text = path.read_text(encoding="utf-8", errors="ignore")
    except (OSError, PermissionError):
        return []
    try:
        root = ET.fromstring(text)
    except ET.ParseError:
        return []

    names: List[str] = []
    tag = root.tag.lower() if root.tag else ""

    # <type name="..."> appears in types.xml and cfgspawnabletypes.xml.
    if tag in ("types", "spawnabletypes"):
        for elem in root.iter("type"):
            name = elem.get("name")
            if name:
                names.append(name)

    # <event name="..."> appears in events.xml.
    if tag == "events":
        for elem in root.iter("event"):
            name = elem.get("name")
            if name and _looks_like_vehicle(name):
                names.append(name)

    # <class name="..."> appears in some mod classnames.txt files.
    if tag in ("classnames", ""):
        for elem in root.iter("class"):
            name = elem.get("name")
            if name:
                names.append(name)

    return names


def _extract_vehicle_names_from_text(path: Path) -> List[str]:
    """Best-effort extraction from non-XML text files like classnames.txt."""
    try:
        text = path.read_text(encoding="utf-8", errors="ignore")
    except (OSError, PermissionError):
        return []
    # Match either <class name="..." /> or bare quoted names used by mod docs.
    names: List[str] = []
    for match in re.finditer(r'class\s+name\s*=\s*"([^"]+)"', text, re.IGNORECASE):
        name = match.group(1).strip()
        if name:
            names.append(name)
    return names


def _is_likely_vehicle_part(name: str) -> bool:
    """Return True if *name* looks like a vehicle part rather than a vehicle.

    Mods often list every door, hood, wheel, and seat alongside the actual
    vehicle class.  These clutter the picker and will break the quick-setup
    workflow because they have no wheel template.

    The "_wheel" keyword is intentionally a word boundary check at the end
    of the name only, so "Wheel" by itself (which some mods use as a generic
    catch-all term) is not rejected.
    """
    lower = name.lower()
    part_keywords = (
        "cargo", "codriver", "driver", "hood", "trunk", "door",
        "rollbar", "engine", "light", "plate", "interior", "seat", "glass",
        "battery", "sparkplug", "spark_plug", "oil", "brake", "radiator",
        "fuel", "mirror", "bumper", "fender", "grill", "frame", "chassis",
        "tail", "windshield", "carbatt", "headlight", "taillight",
        # Common typos / abbreviated part names seen in mod class lists.
        "codrvier", "codrv", "drvier", "drver", "drivr",
    )
    if any(kw in lower for kw in part_keywords):
        return True
    # Only treat a trailing _wheel as a part, not standalone "wheel".
    return lower.endswith("_wheel")


def _is_likely_static_wreck(name: str) -> bool:
    """Return True if *name* is a static wreck or scenery object.

    These classes exist in types.xml with ``nominal=0`` and ``lifetime=0``
    because they are map-placed objects, not spawnable loot or drivable
    vehicles.  Treating them as spawnables causes them to appear inside
    buildings when tagged with ``usage="Town"`` / ``value="Tier12"``.
    """
    lower = name.lower()
    wreck_prefixes = (
        "land_wreck_",
        "staticobj_wreck_",
        "static_",
        "wreck_",
    )
    if any(lower.startswith(p) for p in wreck_prefixes):
        return True
    # Abandoned / destroyed vehicle suffixes commonly used for static objects.
    if "_aban" in lower or "_destroyed" in lower or "_debris" in lower:
        return True
    # Animal and zombie classes are not spawnable loot.
    if lower.startswith("animal_") or lower.startswith("zmb"):
        return True
    return False


def _filter_vehicle_candidates(names: List[str]) -> List[str]:
    """Remove obvious vehicle parts from a list of candidate class names."""
    return [n for n in names if not _is_likely_vehicle_part(n)]


def _normalize_class_name(name: str) -> str:
    """Return an alphanumeric lowercase representation of a class name."""
    return re.sub(r"[^a-z0-9]", "", name.lower())


def _longest_common_substring(a: str, b: str) -> str:
    """Return the longest common substring of *a* and *b*."""
    if not a or not b:
        return ""
    m, n = len(a), len(b)
    longest = ""
    lengths = [[0] * (n + 1) for _ in range(m + 1)]
    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if a[i - 1] == b[j - 1]:
                lengths[i][j] = lengths[i - 1][j - 1] + 1
                if lengths[i][j] > len(longest):
                    longest = a[i - lengths[i][j]:i]
            else:
                lengths[i][j] = 0
    return longest


def _find_wheel_for_base(base: str, all_names: set) -> Optional[str]:
    """Return a wheel class name associated with *base*, if any."""
    # Exact-preference candidates first.
    candidates = [
        f"{base}_Wheel",
        f"{base}_wheel",
        f"{base.lower()}_wheel",
    ]
    for wheel in candidates:
        if wheel in all_names:
            return wheel

    base_norm = _normalize_class_name(base)
    if not base_norm:
        return None

    wheels = [n for n in all_names if n.lower().endswith("_wheel")]
    best: Optional[str] = None
    best_score = 0
    MIN_SUBSTRING_LEN = 5

    for wheel in wheels:
        wheel_norm = _normalize_class_name(wheel.replace("wheel", "").replace("Wheel", ""))
        if not wheel_norm:
            continue

        # Direct substring containment (catches FordBronco -> bronco_wheel).
        if base_norm in wheel_norm or wheel_norm in base_norm:
            return wheel

        # Longest common substring (catches NissanGTRCustom -> NissanGTRNismo_Wheel).
        lcs = _longest_common_substring(base_norm, wheel_norm)
        if len(lcs) >= MIN_SUBSTRING_LEN and len(lcs) > best_score:
            best_score = len(lcs)
            best = wheel

    return best


def _candidate_bases(name: str) -> List[str]:
    """Return base-name candidates for *name*, longest first.

    For ``Audi_RS6_ABT_Black`` the candidates are ``Audi_RS6_ABT_Black``
    (the name itself) and ``Audi_RS6_ABT`` (everything before the last
    underscore).  This lets us find the longest prefix that other names
    extend with a suffix.
    """
    candidates = [name]
    if "_" in name:
        candidates.append(name.rsplit("_", 1)[0])
    return candidates


def _group_by_base_prefix(names: List[str], all_names: Optional[set] = None) -> Dict[str, List[str]]:
    """Group class names by their longest shared base prefix.

    The base prefix is inferred even when the plain base class is not listed
    in the source data.  For example, `Audi_RS6_ABT_Black`,
    `Audi_RS6_ABT_Blue`, and `Audi_RS6_ABT_Wheel` all share the synthetic
    base `Audi_RS6_ABT`.  Multi-segment names like `Dodge_ChallengerSRT`
    and `Dodge_ChallengerSRT_Wheel` group under `Dodge_ChallengerSRT`.
    """
    names_set = set(names)
    lookup = all_names if all_names is not None else names_set

    def _is_part_for_grouping(name: str) -> bool:
        """Parts are skipped as group members, but wheels are kept as evidence."""
        return _is_likely_vehicle_part(name) and not name.lower().endswith("_wheel")

    # Determine the best base for each name.  Prefer the shortest candidate
    # when it is itself a real class (e.g. `SpeedBoat_Hull` -> `SpeedBoat`).
    # Otherwise fall back to the longest candidate that has extensions, so
    # multi-token names like `Dodge_ChallengerSRT` are not split at the
    # first underscore.
    name_to_base: Dict[str, Optional[str]] = {name: None for name in names_set}
    for name in names_set:
        if _is_part_for_grouping(name):
            continue
        candidates = _candidate_bases(name)
        shorter = candidates[-1] if len(candidates) > 1 else name
        best_base: Optional[str] = name
        if shorter != name and shorter in names_set and not _is_likely_vehicle_part(shorter):
            best_base = shorter
        else:
            for candidate in candidates:
                if _is_likely_vehicle_part(candidate):
                    continue
                prefix = candidate + "_"
                has_extension = any(
                    other != name
                    and other.startswith(prefix)
                    and not _is_part_for_grouping(other)
                    for other in lookup
                )
                if has_extension:
                    best_base = candidate
                    break
        name_to_base[name] = best_base

    groups: Dict[str, List[str]] = {}
    for name, base in name_to_base.items():
        if _is_part_for_grouping(name):
            continue
        if base is not None and not _is_likely_vehicle_part(base):
            groups.setdefault(base, []).append(name)
        else:
            groups.setdefault(name, [])
    return groups


def _looks_like_brand_vehicle(name: str) -> bool:
    """Return True if *name* contains a known brand/model token.

    This is used only as a secondary confidence signal in combination with
    a wheel match; it is never sufficient on its own because many weapons
    and items share brand/model names.
    """
    return bool(_VEHICLE_BRAND_RE.search(name))


def _infer_vehicle_bases(names: List[str], all_names: Optional[set] = None) -> List[str]:
    """Infer whole-vehicle base names from a list of class names.

    Uses prefix grouping so custom skin suffixes do not need to be listed
    explicitly.  Returns only base names; color variants and wheels are used
    as evidence but not returned.  A candidate is only admitted as a vehicle
    if at least one of these holds:

      * it has an associated wheel class (the strongest vehicle signal)
      * its name contains a generic vehicle term (car, truck, bike, etc.)
      * it is a standalone name with both a brand/model token AND a matching
        wheel class

    Brand/model keywords alone are NOT enough to avoid weapons and items.
    """
    names_set = set(names)
    lookup = all_names if all_names is not None else names_set
    groups = _group_by_base_prefix(names, lookup)

    vehicles: set = set()
    for base, variants in groups.items():
        if _is_likely_vehicle_part(base):
            continue
        has_wheel = _find_wheel_for_base(base, lookup) is not None
        looks_generic = _looks_like_vehicle(base)
        looks_brand = _looks_like_brand_vehicle(base)

        # Multiple variants with a wheel class is a confident vehicle match.
        if len(variants) >= 2 and has_wheel:
            vehicles.add(base)
        # A brand/model base without a wheel is too risky (weapons/items).
        elif has_wheel and (looks_generic or looks_brand):
            vehicles.add(base)
        # Standalone names must contain a generic vehicle term.
        elif not variants and looks_generic:
            vehicles.add(base)

    return sorted(vehicles)


# Special-case patterns that are frequently misclassified.
_JETSKI_RE = re.compile(r"jet[_\s]?ski", re.IGNORECASE)
_FIRE_APPARATUS_RE = re.compile(
    r"fire[_\s]?(truck|engine|ladder|rescue|apparatus|tender|pumper)",
    re.IGNORECASE,
)


def _classify_spawnable(name: str, all_names: set) -> Optional[SpawnableClass]:
    """Classify a candidate class name into a spawnable category.

    Returns ``None`` if the name looks like a part, consumable, or other
    non-spawnable sub-component.
    """
    # Watercraft whose names contain air substrings (e.g. JetSki) must be
    # caught before the air keyword check.
    if _JETSKI_RE.search(name):
        return SpawnableClass(name, "", "water")

    # Air and water vehicles take precedence over land-vehicle keywords such
    # as "heli" so ``LittleBird_Heli`` is classified as air, not a land
    # vehicle. They also take precedence over the generic part filter so that
    # names like ``Ultralight_Trike`` or ``CargoPlane`` are not discarded as
    # parts.
    if _AIR_RE.search(name):
        return SpawnableClass(name, "", "air")
    if _WATER_RE.search(name):
        return SpawnableClass(name, "", "water")

    # Fire apparatus base names contain part-like words (Engine, Ladder) that
    # the generic part filter would otherwise discard.
    if _FIRE_APPARATUS_RE.search(name):
        return SpawnableClass(name, "", "vehicle")

    if _is_likely_vehicle_part(name):
        return None
    if _SPAWNABLE_REJECT_RE.search(name):
        return None

    # Vehicles (land) get the most evidence checks.
    wheel = _find_wheel_for_base(name, all_names)
    looks_generic_vehicle = _looks_like_vehicle(name)
    looks_brand = _looks_like_brand_vehicle(name)
    if wheel is not None and (looks_generic_vehicle or looks_brand):
        return SpawnableClass(name, "", "vehicle", (wheel, 4))
    if looks_generic_vehicle:
        return SpawnableClass(name, "", "vehicle")

    # Weapons and gear.
    if _WEAPON_RE.search(name):
        return SpawnableClass(name, "", "weapon")
    if _GEAR_RE.search(name):
        return SpawnableClass(name, "", "gear")

    # Fallback: keep standalone names that look like they could be a mod class
    # but are too generic to classify.  We require at least one underscore or a
    # mixed case pattern so bare short tokens are not admitted.
    if len(name) >= 6 and ("_" in name or not name.islower()):
        return SpawnableClass(name, "", "generic")

    return None


def _has_variant_keyword(variants: List[str], keywords: Tuple[str, ...]) -> bool:
    """Return True if any variant contains one of the given keywords."""
    return any(kw in v.lower() for v in variants for kw in keywords)


def _infer_spawnable_bases(
    names: List[str],
    all_names: Optional[set] = None,
) -> List[SpawnableClass]:
    """Infer whole spawnable base names from a list of class names.

    Similar to `_infer_vehicle_bases` but returns classified spawnables,
    including weapons, gear, helicopters, boats, and generic mod classes.
    """
    names_set = set(names)
    lookup = all_names if all_names is not None else names_set
    groups = _group_by_base_prefix(names, lookup)

    spawnables: List[SpawnableClass] = []
    seen: set = set()

    for base, variants in groups.items():
        if base in seen:
            continue

        # Try the base itself first (handles skin variant groups like
        # Audi_RS6_ABT_Black / Audi_RS6_ABT_Wheel -> Audi_RS6_ABT).
        classification = _classify_spawnable(base, lookup)

        # If the base didn't classify as air/water, look at variant part
        # names (e.g. UH1H_Rotor, SpeedBoat_Hull) to identify the base type.
        if classification is None or classification.category not in ("air", "water"):
            if _has_variant_keyword(variants, _AIR_PART_KEYWORDS):
                classification = SpawnableClass(base, "", "air")
            elif _has_variant_keyword(variants, _WATER_PART_KEYWORDS):
                classification = SpawnableClass(base, "", "water")

        if classification is not None:
            seen.add(base)
            # Mark all variants as seen so skin/part names are not returned
            # as separate spawnables (e.g. UH1H_Camo / UH1H_Rotor).
            seen.update(variants)
            spawnables.append(classification)
            continue

        # If the base did not classify, try each variant in case one of the
        # skin names is itself a spawnable class (e.g. AKM_Black).
        for variant in variants:
            if variant in seen:
                continue
            classification = _classify_spawnable(variant, lookup)
            if classification is not None:
                seen.add(variant)
                spawnables.append(classification)
                break

    # Standalone names that were not grouped.
    for name in names_set:
        if name in seen:
            continue
        classification = _classify_spawnable(name, lookup)
        if classification is not None:
            seen.add(name)
            spawnables.append(classification)

    # Stable sort by name.
    return sorted(spawnables, key=lambda s: s.name)


def _extract_type_tags(type_elem: ET.Element) -> Tuple[set, set]:
    """Return (categories, usages) declared on a <type> element."""
    categories = set()
    usages = set()
    for cat in type_elem.findall("category"):
        name = cat.get("name")
        if name:
            categories.add(name)
    for usage in type_elem.findall("usage"):
        name = usage.get("name")
        if name:
            usages.add(name)
    for tag in type_elem.findall("tag"):
        name = tag.get("name")
        if name:
            categories.add(name)
    return categories, usages


def _collect_class_names_from_folder(folder: Path) -> set:
    """Return every class name found in *folder* (mod or mission)."""
    names: set = set()
    for path in folder.rglob("*"):
        if not path.is_file():
            continue
        suffix = path.suffix.lower()
        if suffix == ".xml":
            pass
        elif suffix == ".txt" and path.name.lower() == "classnames.txt":
            pass
        else:
            continue
        rel_parts = path.relative_to(folder).parts
        if len(rel_parts) > 4:
            continue
        try:
            if path.stat().st_size > 5 * 1024 * 1024:
                continue
        except OSError:
            continue
        if suffix == ".xml":
            names.update(_extract_vehicle_names_from_xml(path))
        else:
            names.update(_extract_vehicle_names_from_text(path))
    return names


# Cache for workshop class-name scans. The workshop folder can be very
# large, and several integration workflows call this repeatedly. We cache
# per process and invalidate when the workshop directory's top-level mtime
# changes or after a timeout.
_WORKSHOP_CLASS_CACHE: Dict[Path, Tuple[float, float, frozenset]] = {}
_WORKSHOP_CLASS_CACHE_TTL_SECONDS = 300


def _workshop_scan_fingerprint(workshop_dir: Path) -> float:
    """Return a cheap fingerprint for cache invalidation.

    Uses the latest mtime among the top-level workshop subdirectories. This
    catches mod installs/removals without rescanning every file each call.
    """
    try:
        latest = workshop_dir.stat().st_mtime
        for entry in workshop_dir.iterdir():
            if not entry.is_dir():
                continue
            mtime = entry.stat().st_mtime
            if mtime > latest:
                latest = mtime
        return latest
    except OSError:
        return 0.0


def _collect_class_names_from_workshop(
    workshop_dir: Path,
    timeout_seconds: Optional[float] = None,
) -> set:
    """Return every class name found across all workshop mod folders.

    Args:
        workshop_dir: Workshop content directory to scan.
        timeout_seconds: If given, abort the scan after this many seconds and
            return whatever was collected so far. This prevents the UI from
            hard-locking when the workshop folder is enormous.
    """
    now = time.time()
    fingerprint = _workshop_scan_fingerprint(workshop_dir)
    cached = _WORKSHOP_CLASS_CACHE.get(workshop_dir)
    if cached is not None:
        cached_time, cached_fingerprint, cached_names = cached
        if (
            now - cached_time < _WORKSHOP_CLASS_CACHE_TTL_SECONDS
            and fingerprint == cached_fingerprint
        ):
            return set(cached_names)

    deadline = None
    if timeout_seconds is not None:
        deadline = now + timeout_seconds

    all_names: set = set()
    for folder in sorted(workshop_dir.iterdir()):
        if deadline is not None and time.time() > deadline:
            break
        if not folder.is_dir() or not _is_workshop_mod_id(folder.name):
            continue
        all_names.update(_collect_class_names_from_folder(folder))

    _WORKSHOP_CLASS_CACHE[workshop_dir] = (now, fingerprint, frozenset(all_names))
    return all_names


def _scan_folder_for_vehicles(folder: Path) -> List[str]:
    """Recursively scan a mod/mission folder for vehicle class names."""
    names = list(_collect_class_names_from_folder(folder))
    # Pass unfiltered names so wheel classes can be used to identify bases.
    return _infer_vehicle_bases(names)


def _classify_with_ce_tags(
    name: str,
    declared_categories: set,
    declared_usages: set,
    limits: LimitsDefinition,
    lookup: set,
) -> Optional[SpawnableClass]:
    """Classify a class using its declared CE tags, falling back to heuristics.

    Returns ``None`` only if the name should be excluded (e.g. a part).
    """
    invalid_cats = sorted(declared_categories - limits.categories)
    invalid_usages = sorted(declared_usages - limits.usages)

    ce_category, _, _ = _classify_from_declared_tags(declared_categories, limits)

    # Even when CE says "vehicle", refine air/water via name heuristics so the
    # UI can show a dedicated Aircraft/Helicopter filter.
    if ce_category in ("vehicle", None):
        heuristic = _classify_spawnable(name, lookup)
        if heuristic is None:
            return None
        category = heuristic.category
        wheel = heuristic.wheel
    else:
        category = ce_category
        wheel = None

    return SpawnableClass(
        name=name,
        source="",
        category=category,
        wheel=wheel,
        declared_categories=tuple(sorted(declared_categories)),
        declared_usages=tuple(sorted(declared_usages)),
        invalid_categories=tuple(invalid_cats),
        invalid_usages=tuple(invalid_usages),
        confidence="ce" if declared_categories or declared_usages else "heuristic",
    )


def _scan_ce_files_for_spawnables(
    paths: List[Path],
    limits: LimitsDefinition,
    source: str,
    lookup: set,
) -> Dict[str, SpawnableClass]:
    """Scan types/spawnabletypes XML files referenced by cfgeconomycore.xml."""
    results: Dict[str, SpawnableClass] = {}
    for path in paths:
        if not path.exists():
            continue
        try:
            root = ET.parse(path).getroot()
        except ET.ParseError:
            continue
        for type_elem in root.findall(".//type"):
            name = type_elem.get("name")
            if not name:
                continue
            declared_cats, declared_usages = _extract_type_tags(type_elem)
            spawnable = _classify_with_ce_tags(
                name, declared_cats, declared_usages, limits, lookup
            )
            if spawnable is None:
                continue
            spawnable.source = source
            # Prefer the first confident source; do not overwrite existing.
            if name not in results:
                results[name] = spawnable
    return results


def discover_spawnable_classes(
    mission_root: Optional[Path] = None,
    workshop_dir: Optional[Path] = None,
) -> Dict[str, SpawnableClass]:
    """Return a mapping of spawnable class name to spawnable metadata.

    Discovers candidate spawnable class names from:
      * built-in vanilla vehicle templates
      * the active mission's ``cfgeconomycore.xml`` referenced CE files
      * the active mission's XML files and config subfolders (fallback)
      * workshop mod folders that ship XML fragments or classnames.txt

    Classes are classified into categories such as ``vehicle``, ``air``,
    ``water``, ``weapon``, ``gear``, and ``generic``.

    The returned dict is stable: vanilla templates are included first, then
    mission-derived names, then workshop-derived names.  Workshop entries are
    keyed by the mod's display name (read from meta.cpp/mod.cpp) when possible.
    """
    results: Dict[str, SpawnableClass] = {}

    # 1) Vanilla vehicle templates are always available as a baseline.
    for name in VEHICLE_TEMPLATES:
        results[name] = SpawnableClass(name, "Vanilla template", "vehicle", confidence="ce")

    limits = LimitsDefinition()
    eco = EconomyCore()
    if mission_root is not None and mission_root.exists():
        limits = load_mission_limits(mission_root)
        eco = parse_economy_core(mission_root)

    # 2) Active mission CE files declared in cfgeconomycore.xml.
    mission_names: set = set()
    if mission_root is not None and mission_root.exists():
        if eco.types_files or eco.spawnabletypes_files or eco.events_files:
            ce_files = eco.types_files + eco.spawnabletypes_files + eco.events_files
            for spawnable in _scan_ce_files_for_spawnables(
                ce_files, limits, "Mission CE", mission_names
            ).values():
                if spawnable.name not in results:
                    results[spawnable.name] = spawnable
                    mission_names.add(spawnable.name)
        else:
            # Fallback: scan root XML files when cfgeconomycore.xml is missing.
            root_names: List[str] = []
            root_sources: Dict[str, str] = {}
            for xml_name in ("types.xml", "cfgspawnabletypes.xml", "events.xml"):
                path = mission_root / xml_name
                if path.exists():
                    for name in _extract_vehicle_names_from_xml(path):
                        root_names.append(name)
                        root_sources.setdefault(name, f"Mission: {xml_name}")
            mission_names = set(root_names)
            for spawnable in _infer_spawnable_bases(root_names, mission_names):
                if spawnable.name not in results:
                    spawnable.source = root_sources.get(spawnable.name, "Mission XML")
                    results[spawnable.name] = spawnable
                    mission_names.add(spawnable.name)

        # Also scan mission config subfolders for anything not in CE files.
        for spawnable in _infer_spawnable_bases(
            list(_collect_class_names_from_folder(mission_root)), mission_names
        ):
            if spawnable.name not in results:
                spawnable.source = "Mission config"
                results[spawnable.name] = spawnable
                mission_names.add(spawnable.name)

    # 3) Workshop mod folders.
    if workshop_dir is not None and workshop_dir.exists():
        all_workshop_names = _collect_class_names_from_workshop(workshop_dir)
        for folder in sorted(workshop_dir.iterdir()):
            if not folder.is_dir() or not _is_workshop_mod_id(folder.name):
                continue
            display_name = _read_workshop_display_name(folder) or folder.name
            folder_names = _collect_class_names_from_folder(folder)
            root_names = []
            root_sources: Dict[str, str] = {}
            for xml_name in ("types.xml", "cfgspawnabletypes.xml", "events.xml"):
                path = folder / xml_name
                if path.exists():
                    for name in _extract_vehicle_names_from_xml(path):
                        root_names.append(name)
                        root_sources.setdefault(name, display_name)
            # Use the per-mod names for source attribution, but the full
            # workshop set for wheel matching so cross-file wheel references
            # inside a mod are resolved.
            for spawnable in _infer_spawnable_bases(root_names + list(folder_names), all_workshop_names):
                if spawnable.name not in results:
                    spawnable.source = root_sources.get(spawnable.name, display_name)
                    results[spawnable.name] = spawnable
            for spawnable in _infer_spawnable_bases(list(folder_names), all_workshop_names):
                if spawnable.name not in results:
                    spawnable.source = display_name
                    results[spawnable.name] = spawnable

    return results


def discover_vehicle_classes(
    mission_root: Optional[Path] = None,
    workshop_dir: Optional[Path] = None,
) -> Dict[str, str]:
    """Backward-compatible mapping of vehicle class name to source string.

    Only returns spawnables classified as vehicles, air, or water vehicles.
    """
    found = discover_spawnable_classes(mission_root, workshop_dir)
    return {
        name: meta.source
        for name, meta in found.items()
        if meta.category in ("vehicle", "air", "water")
    }


def _backup_path(target: Path) -> Path:
    """Return a timestamped backup path inside target's backups directory."""
    backup_dir = target.parent / "backups" / "integration"
    backup_dir.mkdir(parents=True, exist_ok=True)
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    return backup_dir / f"{target.stem}_{timestamp}{target.suffix}"


def _indent_xml(elem: ET.Element, level: int = 0) -> None:
    """Pretty-indent an ElementTree element in-place."""
    i = "\n" + level * "\t"
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = i + "\t"
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
        for child in elem:
            _indent_xml(child, level + 1)
        if not child.tail or not child.tail.strip():
            child.tail = i
    else:
        if level and (not elem.tail or not elem.tail.strip()):
            elem.tail = i


def _serialize_et(root: ET.Element) -> str:
    """Serialize an ElementTree element to a DayZ-style XML string.

    Uses 4-space indentation and a double-quoted UTF-8 declaration with
    ``standalone="yes"`` to match stock DayZ mission files.
    """
    # Indent in-place; callers already pass a tree that is about to be saved.
    ET.indent(root, space="    ")
    body = ET.tostring(root, encoding="unicode")
    # ET.tostring does not include the XML declaration.
    return '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>\n' + body


class XmlConfigEditor:
    """Edit core DayZ server XML configuration files."""

    supported_files = list(SUPPORTED_FILES)

    # DayZ missions normally keep types.xml and events.xml under db/.
    # Older/simplified mission layouts keep them at the mission root.
    # cfgspawnabletypes.xml and cfgeventspawns.xml are always at root.
    _DB_FILES = {"types.xml", "events.xml"}

    def __init__(self, mission_root: Path):
        self.mission_root = Path(mission_root)
        self._paths: Dict[str, Path] = {
            name: self._resolve_path(self.mission_root, name) for name in SUPPORTED_FILES
        }

    @staticmethod
    def _resolve_path(mission_root: Path, name: str) -> Path:
        """Return the concrete path to use for *name*.

        For types.xml and events.xml, prefer ``db/<file>`` when it exists or
        when the root file does not exist. This matches DayZ's default CE
        layout and fixes maps that ship without root economy files.
        """
        root_path = mission_root / name
        db_files = XmlConfigEditor._DB_FILES
        if name not in db_files:
            return root_path
        db_path = mission_root / "db" / name
        # Prefer db/ if it exists or if root does not exist.
        if db_path.exists() or not root_path.exists():
            return db_path
        return root_path

    def path_for(self, name: str) -> Optional[Path]:
        """Return the on-disk path for a supported XML file."""
        return self._paths.get(name)

    def file_exists(self, name: str) -> bool:
        """Return True if the named supported XML file exists on disk."""
        path = self.path_for(name)
        return bool(path and path.exists())

    def read_text(self, name: str) -> Optional[str]:
        """Read the named supported XML file and return its text, or None."""
        path = self.path_for(name)
        if not path or not path.exists():
            return None
        try:
            return path.read_text(encoding="utf-8")
        except OSError:
            return None

    # Default root tags for the supported XML files.  Used to create or
    # repair empty / malformed files during integration.
    _DEFAULT_ROOTS = {
        "events.xml": "events",
        "cfgspawnabletypes.xml": "spawnabletypes",
        "types.xml": "types",
        "cfgeventspawns.xml": "eventposdef",
    }

    def _load(self, name: str) -> Optional[ET.Element]:
        text = self.read_text(name)
        if text is None:
            return None
        try:
            return ET.fromstring(text)
        except ET.ParseError:
            return None

    def _load_or_create(self, name: str) -> Optional[ET.Element]:
        """Load *name* or initialise it with its default root element.

        This is used by integration writes so that an empty or malformed XML
        file is repaired instead of causing a silent failure.
        """
        root = self._load(name)
        if root is not None:
            return root
        default_tag = self._DEFAULT_ROOTS.get(name)
        if default_tag is None:
            return None
        path = self.path_for(name)
        if path is None:
            return None
        try:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(f"<{default_tag}></{default_tag}>\n", encoding="utf-8")
            return ET.fromstring(path.read_text(encoding="utf-8"))
        except (OSError, ET.ParseError):
            return None

    def _save(self, name: str, root: ET.Element) -> bool:
        path = self.path_for(name)
        if path is None:
            return False
        try:
            backup = _backup_path(path)
            if path.exists():
                shutil.copy2(path, backup)
            path.write_text(_serialize_et(root), encoding="utf-8")
            return True
        except OSError:
            return False
        except Exception:
            # Defensive: any unexpected serialization/write failure should
            # not crash the UI thread; return False so the action is reported
            # as failed.
            return False

    # ------------------------------------------------------------------
    # events.xml helpers
    # ------------------------------------------------------------------
    def _find_event(self, root: ET.Element, vehicle_class_name: str) -> Optional[ET.Element]:
        """Find an <event> element by name (case-insensitive)."""
        target = vehicle_class_name.lower()
        for event in root.findall("event"):
            if event.get("name", "").lower() == target:
                return event
        return None

    def enable_vehicle_spawning(
        self,
        vehicle_class_name: str,
        active: bool = True,
        limit_nominal: int = 10,
        event_nominal: int = 1,
        event_min: int = 1,
        event_max: int = 1,
    ) -> bool:
        """Enable or disable a vehicle event in events.xml.

        Generates a DayZ-compliant ``<event>`` block with text content for
        numeric fields instead of the malformed ``min/max`` attributes the
        previous implementation produced.

        *limit_nominal* controls how many of this vehicle can be present on
        the map at once through the event system.  It maps to the
        ``<limit nominal="...">`` attribute inside the event definition.

        *event_nominal*, *event_min* and *event_max* control how many event
        spawn points are active at the same time.
        """
        root = self._load_or_create("events.xml")
        if root is None:
            return False
        event = self._find_event(root, vehicle_class_name)
        if event is None:
            event = ET.SubElement(root, "event", {"name": vehicle_class_name})
            ET.SubElement(event, "nominal").text = str(event_nominal)
            ET.SubElement(event, "min").text = str(event_min)
            ET.SubElement(event, "max").text = str(event_max)
            ET.SubElement(event, "lifetime").text = "1"
            ET.SubElement(event, "restock").text = "0"
            ET.SubElement(event, "saferadius").text = "0"
            ET.SubElement(event, "distanceradius").text = "0"
            ET.SubElement(event, "cleanupradius").text = "0"
            ET.SubElement(event, "spawnoffset").text = "0"
            ET.SubElement(event, "spawnrange").text = "0"
            ET.SubElement(event, "countoffset").text = "0"
            ET.SubElement(event, "flags", {"deletable": "0", "init_random": "0", "remove_damaged": "0"})
            ET.SubElement(event, "position").text = "fixed"
            ET.SubElement(event, "limit", {"mixed": "0", "nominal": str(limit_nominal)})
            ET.SubElement(event, "active").text = "1"
            children = ET.SubElement(event, "children")
            ET.SubElement(
                children,
                "child",
                {
                    "lootmax": "0",
                    "lootmin": "0",
                    "max": "1",
                    "min": "0",
                    "type": vehicle_class_name,
                },
            )
        else:
            nominal_elem = event.find("nominal")
            if nominal_elem is not None:
                nominal_elem.text = str(event_nominal)
            min_elem = event.find("min")
            if min_elem is not None:
                min_elem.text = str(event_min)
            max_elem = event.find("max")
            if max_elem is not None:
                max_elem.text = str(event_max)
            limit_elem = event.find("limit")
            if limit_elem is not None:
                limit_elem.set("nominal", str(limit_nominal))
        active_elem = event.find("active")
        if active_elem is None:
            active_elem = ET.SubElement(event, "active")
        active_elem.text = "1" if active else "0"
        return self._save("events.xml", root)

    def add_event_with_positions(
        self,
        event_name: str,
        class_name: str,
        positions: List[Dict[str, float]],
        nominal: int = 5,
        min_count: int = 2,
        lifetime: int = 3600,
        saferadius: int = 50,
        distanceradius: int = 100,
        cleanupradius: int = 200,
    ) -> bool:
        """Create or update an event in events.xml and cfgeventspawns.xml.

        This is used for exact-coordinate spawning. The event in events.xml
        references the class name as a child, and cfgeventspawns.xml holds
        the X/Z/Y coordinates.
        """
        if not positions:
            return False

        # events.xml
        root = self._load_or_create("events.xml")
        if root is None:
            return False
        event = self._find_event(root, event_name)
        if event is not None:
            root.remove(event)
        event = ET.SubElement(root, "event", {"name": event_name})
        ET.SubElement(event, "nominal").text = str(nominal)
        ET.SubElement(event, "min").text = str(min_count)
        ET.SubElement(event, "max").text = str(nominal)
        ET.SubElement(event, "lifetime").text = str(lifetime)
        ET.SubElement(event, "restock").text = "0"
        ET.SubElement(event, "saferadius").text = str(saferadius)
        ET.SubElement(event, "distanceradius").text = str(distanceradius)
        ET.SubElement(event, "cleanupradius").text = str(cleanupradius)
        ET.SubElement(event, "flags", {"lifecycle": "1", "coached": "0"})
        children = ET.SubElement(event, "children")
        ET.SubElement(
            children,
            "child",
            {
                "lootmax": "0",
                "lootmin": "0",
                "max": str(nominal),
                "min": str(min_count),
                "type": class_name,
            },
        )
        if not self._save("events.xml", root):
            return False

        # cfgeventspawns.xml
        spawns_root = self._load_or_create("cfgeventspawns.xml")
        if spawns_root is None:
            return False
        # Remove existing event block with the same name.
        for existing in list(spawns_root.findall("event")):
            if existing.get("name", "").lower() == event_name.lower():
                spawns_root.remove(existing)
        event_pos = ET.SubElement(spawns_root, "event", {"name": event_name})
        for pos in positions:
            attrs: Dict[str, str] = {
                "x": str(pos.get("x", 0.0)),
                "z": str(pos.get("z", 0.0)),
            }
            y = pos.get("y")
            if y is not None:
                attrs["y"] = str(y)
            a = pos.get("a")
            if a is not None:
                attrs["a"] = str(a)
            ET.SubElement(event_pos, "pos", attrs)
        return self._save("cfgeventspawns.xml", spawns_root)

    def is_vehicle_spawning_enabled(self, vehicle_class_name: str) -> Optional[bool]:
        """Return True/False if *vehicle_class_name* has an event in events.xml."""
        root = self._load("events.xml")
        if root is None:
            return None
        event = self._find_event(root, vehicle_class_name)
        if event is None:
            return None
        active = event.find("active")
        if active is None:
            return None
        text = (active.text or "").strip()
        if text in ("0", "1"):
            return text == "1"
        # Backwards compatibility with the old malformed min/max attributes.
        return active.get("min") == "1" or active.get("max") == "1"

    # ------------------------------------------------------------------
    # cfgspawnabletypes.xml helpers
    # ------------------------------------------------------------------
    def _find_spawnable_type(self, root: ET.Element, vehicle_class_name: str) -> Optional[ET.Element]:
        target = vehicle_class_name.lower()
        for type_elem in root.findall("type"):
            if type_elem.get("name", "").lower() == target:
                return type_elem
        return None

    def define_spawnable_type(
        self,
        vehicle_class_name: str,
        attachments: Optional[List[Tuple[str, float]]] = None,
        chance: float = 1.0,
    ) -> bool:
        """Define or update a spawnable type with optional attachments.

        Only ``<attachments>`` children are replaced; other tags such as
        ``<hoarder/>`` or ``<damage/>`` are preserved so the rewrite does
        not orphan existing spawnable-type configuration.
        """
        root = self._load_or_create("cfgspawnabletypes.xml")
        if root is None:
            return False
        type_elem = self._find_spawnable_type(root, vehicle_class_name)
        if type_elem is None:
            type_elem = ET.SubElement(root, "type", {"name": vehicle_class_name})
        else:
            # Replace only attachment definitions so we don't wipe unrelated
            # tags like <hoarder/>, <damage/>, <cargo/>, etc.
            for child in list(type_elem.findall("attachments")):
                type_elem.remove(child)

        if attachments:
            for item_name, item_chance in attachments:
                att = ET.SubElement(
                    type_elem,
                    "attachments",
                    {"chance": f"{item_chance:.2f}"},
                )
                ET.SubElement(att, "item", {"name": item_name, "chance": "1.00"})
        if chance < 1.0:
            # Remove any existing chance value before adding a new one.
            for existing in list(type_elem.findall("chance")):
                type_elem.remove(existing)
            ET.SubElement(type_elem, "chance", {"value": f"{chance:.2f}"})
        return self._save("cfgspawnabletypes.xml", root)

    def add_vehicle_attachments(self, vehicle_class_name: str) -> bool:
        """Add known wheel attachments for a vehicle if a template exists."""
        template = VEHICLE_TEMPLATES.get(vehicle_class_name)
        if not template:
            return False
        wheel_name, count = template
        attachments = [(wheel_name, 1.0)] * count
        return self.define_spawnable_type(vehicle_class_name, attachments)

    def has_vehicle_attachments(self, vehicle_class_name: str) -> bool:
        """Return True if cfgspawnabletypes.xml already has attachments for the vehicle."""
        root = self._load("cfgspawnabletypes.xml")
        if root is None:
            return False
        type_elem = self._find_spawnable_type(root, vehicle_class_name)
        if type_elem is None:
            return False
        return len(type_elem.findall("attachments")) > 0

    # ------------------------------------------------------------------
    # types.xml helpers
    # ------------------------------------------------------------------
    def _find_type(self, root: ET.Element, vehicle_class_name: str) -> Optional[ET.Element]:
        target = vehicle_class_name.lower()
        for type_elem in root.findall("type"):
            if type_elem.get("name", "").lower() == target:
                return type_elem
        return None

    def add_vehicle_to_types_xml(
        self,
        vehicle_class_name: str,
        nominal: int = 20,
        category: str = "vehicle",
        lifetime: int = 3888000,
        restock: int = 1800,
        min_count: int = 5,
        quantmin: int = -1,
        quantmax: int = -1,
        usage: str = "",
        value: str = "",
    ) -> bool:
        """Ensure a type entry exists in types.xml.

        The *usage* and *value* tags control which buildings/zones the item
        can spawn in. Match them to the tags declared in mapgroupproto.xml for
        the map being used.

        .. note::
            Vehicles, aircraft and boats are spawned by events.xml, so they
            should not receive a ``usage``/``value``.  Leaving these blank
            prevents them from being treated as dynamic loot and spawning
            inside houses.
        """
        root = self._load_or_create("types.xml")
        if root is None:
            return False
        type_elem = self._find_type(root, vehicle_class_name)
        if type_elem is None:
            type_elem = ET.SubElement(root, "type", {"name": vehicle_class_name})
        else:
            for child in list(type_elem):
                type_elem.remove(child)

        ET.SubElement(type_elem, "nominal").text = str(nominal)
        ET.SubElement(type_elem, "lifetime").text = str(lifetime)
        ET.SubElement(type_elem, "restock").text = str(restock)
        ET.SubElement(type_elem, "min").text = str(min_count)
        ET.SubElement(type_elem, "quantmin").text = str(quantmin)
        ET.SubElement(type_elem, "quantmax").text = str(quantmax)
        ET.SubElement(type_elem, "cost").text = "100"
        ET.SubElement(type_elem, "flags", {
            "count_in_map": "1",
            "count_in_player": "0",
            "count_in_cargo": "0",
            "count_in_hoarder": "0",
            "crafted": "0",
            "deloot": "0",
        })
        ET.SubElement(type_elem, "category", {"name": category})
        if usage:
            ET.SubElement(type_elem, "usage", {"name": usage})
        if value:
            ET.SubElement(type_elem, "value", {"name": value})
        return self._save("types.xml", root)

    def type_exists(self, vehicle_class_name: str) -> bool:
        """Return True if *vehicle_class_name* exists as a <type> in types.xml."""
        root = self._load("types.xml")
        if root is None:
            return False
        return self._find_type(root, vehicle_class_name) is not None

    def remove_event(self, class_name: str) -> bool:
        """Remove an <event> for *class_name* from events.xml."""
        root = self._load("events.xml")
        if root is None:
            return False
        target = class_name.lower()
        removed = False
        for event in list(root.findall("event")):
            if event.get("name", "").lower() == target:
                root.remove(event)
                removed = True
        if not removed:
            return True
        return self._save("events.xml", root)

    def remove_spawnable_type(self, class_name: str) -> bool:
        """Remove a <type> for *class_name* from cfgspawnabletypes.xml."""
        root = self._load("cfgspawnabletypes.xml")
        if root is None:
            return False
        target = class_name.lower()
        removed = False
        for type_elem in list(root.findall("type")):
            if type_elem.get("name", "").lower() == target:
                root.remove(type_elem)
                removed = True
        if not removed:
            return True
        return self._save("cfgspawnabletypes.xml", root)

    def remove_type(self, class_name: str) -> bool:
        """Remove a <type> for *class_name* from types.xml."""
        root = self._load("types.xml")
        if root is None:
            return False
        target = class_name.lower()
        removed = False
        for type_elem in list(root.findall("type")):
            if type_elem.get("name", "").lower() == target:
                root.remove(type_elem)
                removed = True
        if not removed:
            return True
        return self._save("types.xml", root)


class ModIntegrationWorkflow:
    """High-level workflow for integrating mod spawn requirements."""

    def __init__(self, mission_root: Path, workshop_dir: Optional[Path] = None):
        self.editor = XmlConfigEditor(mission_root)
        self.workshop_dir = workshop_dir
        self._limits: Optional[LimitsDefinition] = None
        self._economy_core: Optional[EconomyCore] = None

    @property
    def limits(self) -> LimitsDefinition:
        """Valid category/usage tokens declared by the mission."""
        if self._limits is None:
            self._limits = load_mission_limits(self.editor.mission_root)
        return self._limits

    @property
    def economy_core(self) -> EconomyCore:
        """CE file routing declared by cfgeconomycore.xml."""
        if self._economy_core is None:
            self._economy_core = parse_economy_core(self.editor.mission_root)
        return self._economy_core

    def is_valid_usage(self, usage: str) -> bool:
        """Return True if *usage* is declared in the mission limits."""
        if not usage:
            return True
        return usage in self.limits.usages

    def is_valid_value(self, value: str) -> bool:
        """Return True if *value* is a declared usage or tier token.

        DayZ commonly uses ``Tier1``..``Tier4`` or named usages as values.
        """
        if not value:
            return True
        if value.lower().startswith("tier") and value[4:].isdigit():
            return True
        return value in self.limits.usages

    def discover_spawnables(
        self,
        workshop_dir: Optional[Path] = None,
        categories: Optional[List[str]] = None,
    ) -> List[SpawnableClass]:
        """Return a sorted list of discovered SpawnableClass objects."""
        ws = workshop_dir or self.workshop_dir
        found = discover_spawnable_classes(self.editor.mission_root, ws)
        items = list(found.values())
        if categories:
            items = [s for s in items if s.category in categories]
        return sorted(items, key=lambda s: s.name)

    def discover_vehicles(
        self,
        workshop_dir: Optional[Path] = None,
    ) -> List[Tuple[str, str]]:
        """Return a sorted list of (vehicle_class_name, source) tuples."""
        spawnables = self.discover_spawnables(
            workshop_dir, categories=["vehicle", "air", "water"]
        )
        return [(s.name, s.source) for s in spawnables]

    def find_wheel_for_vehicle(self, vehicle_class_name: str) -> Optional[Tuple[str, int]]:
        """Look up a wheel class name/count for *vehicle_class_name*.

        Checks built-in templates first, then scans the configured workshop
        directory for a matching wheel class name. The scan is capped at a few
        seconds so a huge workshop cannot freeze the UI thread.
        """
        if vehicle_class_name in VEHICLE_TEMPLATES:
            return VEHICLE_TEMPLATES[vehicle_class_name]
        ws = self.workshop_dir
        if ws is None or not ws.exists():
            return None
        all_names = _collect_class_names_from_workshop(
            ws, timeout_seconds=8.0
        )
        wheel = _find_wheel_for_base(vehicle_class_name, all_names)
        if wheel is None:
            return None
        return (wheel, 4)

    def detect_actions(
        self,
        class_name: str,
        category: str = "vehicle",
    ) -> List[IntegrationAction]:
        """Return the list of changes needed for a spawnable mod class."""
        actions: List[IntegrationAction] = []

        if _is_likely_vehicle_part(class_name):
            actions.append(IntegrationAction(
                "validation",
                f"{class_name} looks like a vehicle part, not a whole spawnable. "
                "Quick Setup is meant for top-level class names only.",
            ))
            return actions

        if _is_likely_static_wreck(class_name):
            actions.append(IntegrationAction(
                "validation",
                f"{class_name} is a static wreck or scenery object and cannot be "
                "enabled as a spawnable.",
            ))
            return actions

        is_vehicle = category in ("vehicle", "air", "water")

        if is_vehicle:
            if not self.editor.file_exists("events.xml"):
                actions.append(IntegrationAction(
                    "events.xml",
                    f"events.xml is missing; cannot enable {class_name} spawning.",
                ))
            elif self.editor.is_vehicle_spawning_enabled(class_name) is not True:
                actions.append(IntegrationAction(
                    "events.xml",
                    f"Enable event spawning for {class_name}.",
                ))

        if is_vehicle and category == "vehicle":
            if not self.editor.file_exists("cfgspawnabletypes.xml"):
                actions.append(IntegrationAction(
                    "cfgspawnabletypes.xml",
                    f"cfgspawnabletypes.xml is missing; cannot define attachments for {class_name}.",
                ))
            elif not self.editor.has_vehicle_attachments(class_name):
                actions.append(IntegrationAction(
                    "cfgspawnabletypes.xml",
                    f"Define spawnable type/attachments for {class_name}.",
                ))

        if not self.editor.file_exists("types.xml"):
            actions.append(IntegrationAction(
                "types.xml",
                f"types.xml is missing; cannot define {class_name} entry.",
            ))
        elif not self.editor.type_exists(class_name):
            actions.append(IntegrationAction(
                "types.xml",
                f"Add {class_name} to types.xml.",
            ))

        return actions

    def integrate_spawnable_mod(
        self,
        class_name: str,
        spawn_count: int = 10,
        category: str = "vehicle",
        usage: Optional[str] = None,
        value: Optional[str] = None,
        lifetime: Optional[int] = None,
        restock: Optional[int] = None,
        quantmin: Optional[int] = None,
        quantmax: Optional[int] = None,
        locations: Optional[List[Dict[str, float]]] = None,
        event_min: int = 1,
        event_max: int = 1,
        min_count: Optional[int] = None,
    ) -> IntegrationResult:
        """Apply XML changes needed to enable a spawnable mod class.

        *spawn_count* controls the ``<nominal>`` value in types.xml and, for
        vehicles/air/water, the ``<limit nominal>`` value in events.xml.
        *min_count* controls the ``<min>`` value in types.xml for non-vehicle
        spawnables (defaults to a quarter of *spawn_count*).
        *lifetime*, *restock*, *quantmin* and *quantmax* control the matching
        types.xml child elements for loot spawnables.
        *usage* and *value* control the building zones/tiers the item spawns
        in.  When omitted, sensible defaults are chosen based on *category*;
        vehicles/air/water receive no usage/value because they are spawned
        through events.xml, which prevents them from being placed inside
        houses by Central Economy.

        *locations* is an optional list of ``{"x", "z", "y", "a"}`` dicts
        for exact-coordinate spawning via cfgeventspawns.xml.
        """
        actions: List[IntegrationAction] = []
        locations = locations or []

        if _is_likely_vehicle_part(class_name):
            actions.append(IntegrationAction(
                "validation",
                f"{class_name} looks like a vehicle part, not a whole spawnable. "
                "Quick Setup is meant for top-level class names only.",
                applied=False,
                error="Vehicle part selected",
            ))
            return IntegrationResult(
                success=False,
                actions=actions,
                backup_dir=self.editor.mission_root / "backups" / "integration",
            )

        if _is_likely_static_wreck(class_name):
            actions.append(IntegrationAction(
                "validation",
                f"{class_name} is a static wreck or scenery object and cannot be "
                "enabled as a spawnable.",
                applied=False,
                error="Static wreck selected",
            ))
            return IntegrationResult(
                success=False,
                actions=actions,
                backup_dir=self.editor.mission_root / "backups" / "integration",
            )

        def add_action(name: str, ok: bool, description: str, error: str = "") -> None:
            actions.append(IntegrationAction(
                name, description, applied=ok, error="" if ok else (error or "Failed to apply change"),
            ))

        is_vehicle = category in ("vehicle", "air", "water")

        # Vehicles/air/water are event-spawned and must never carry loot
        # usage/value tags.  Force them blank even if the caller passed a value.
        if is_vehicle:
            usage = ""
            value = ""
        elif usage is None:
            if category == "weapon":
                usage = "Military"
            else:
                usage = "Town"
        if value is None or is_vehicle:
            value = "" if is_vehicle else "Tier12"
        if lifetime is None:
            lifetime = 3888000 if is_vehicle else 7200
        if restock is None:
            restock = 1800 if is_vehicle else 0
        if quantmin is None:
            quantmin = -1 if is_vehicle else 30
        if quantmax is None:
            quantmax = -1 if is_vehicle else 80

        if locations:
            # Exact-coordinate spawning: write a proper event + cfgeventspawns.xml.
            event_name = f"DCM_{class_name}"
            ok = self.editor.add_event_with_positions(
                event_name=event_name,
                class_name=class_name,
                positions=locations,
                nominal=spawn_count,
                min_count=max(1, spawn_count // 4),
            )
            add_action(
                "cfgeventspawns.xml",
                ok,
                f"Add exact spawn positions for {class_name}",
            )
            add_action("events.xml", ok, f"Add coordinate event {event_name}")
        elif is_vehicle:
            ok = self.editor.enable_vehicle_spawning(
                class_name,
                active=True,
                limit_nominal=spawn_count,
                event_nominal=max(1, (event_min + event_max) // 2),
                event_min=event_min,
                event_max=event_max,
            )
            add_action("events.xml", ok, f"Enable event spawning for {class_name}")
        else:
            # Non-vehicle spawnables without coordinates rely on types.xml only.
            add_action("events.xml", True, f"No event spawn needed for {class_name}")

        if category == "vehicle":
            wheel_template = self.find_wheel_for_vehicle(class_name)
            if wheel_template:
                wheel_name, count = wheel_template
                ok = self.editor.define_spawnable_type(
                    class_name, [(wheel_name, 1.0)] * count
                )
                error = ""
            else:
                ok = self.editor.add_vehicle_attachments(class_name)
                if not ok:
                    ok = self.editor.define_spawnable_type(class_name, [])
                error = "No wheel template found; wheels must be configured manually"
            add_action(
                "cfgspawnabletypes.xml",
                ok,
                f"Define spawnable type for {class_name}",
                error=error,
            )
        else:
            # Air/water/weapon/gear/generic only need a types.xml entry.
            add_action("cfgspawnabletypes.xml", True, f"No attachments needed for {class_name}")

        types_category = "vehicle" if is_vehicle else category
        # Event-spawned vehicles (vehicle/air/water) must have nominal=0 in
        # types.xml so Central Economy does not dynamically spawn them as loot.
        # Only non-vehicle spawnables use the requested count in types.xml.
        if is_vehicle:
            types_nominal = 0
            types_min = 0
        else:
            types_nominal = spawn_count
            if min_count is None:
                types_min = max(1, spawn_count // 4)
            else:
                types_min = min_count
        ok = self.editor.add_vehicle_to_types_xml(
            class_name,
            nominal=types_nominal,
            min_count=types_min,
            category=types_category,
            lifetime=lifetime,
            restock=restock,
            quantmin=quantmin,
            quantmax=quantmax,
            usage=usage,
            value=value,
        )
        add_action("types.xml", ok, f"Add {class_name} to types.xml")

        success = all(a.applied for a in actions)
        return IntegrationResult(
            success=success,
            actions=actions,
            backup_dir=self.editor.mission_root / "backups" / "integration",
        )

    def integrate_vehicle_mod(
        self,
        vehicle_class_name: str,
        spawn_count: int = 10,
    ) -> IntegrationResult:
        """Backward-compatible wrapper for vehicle integration."""
        return self.integrate_spawnable_mod(
            vehicle_class_name, spawn_count=spawn_count, category="vehicle"
        )

    def random_remix(
        self,
        percentage: float = 25.0,
        spawn_count: int = 10,
        categories: Optional[List[str]] = None,
        seed: Optional[int] = None,
    ) -> IntegrationResult:
        """Randomly enable a subset of discovered spawnables.

        *percentage* (0-100) of discovered spawnables are selected and
        integrated with *spawn_count*.  Returns a combined result with a
        summary action.
        """
        if seed is not None:
            random.seed(seed)

        spawnables = self.discover_spawnables(categories=categories)
        if not spawnables:
            return IntegrationResult(
                success=True,
                actions=[IntegrationAction(
                    "summary",
                    "Random remix: no spawnables discovered.",
                    applied=True,
                )],
                backup_dir=self.editor.mission_root / "backups" / "integration",
            )

        count = max(1, int(len(spawnables) * percentage / 100.0))
        selected = random.sample(spawnables, min(count, len(spawnables)))

        all_actions: List[IntegrationAction] = []
        success = True
        for spawnable in selected:
            result = self.integrate_spawnable_mod(
                spawnable.name,
                spawn_count=spawn_count,
                category=spawnable.category,
            )
            all_actions.extend(result.actions)
            if not result.ok:
                success = False

        summary = IntegrationAction(
            "summary",
            f"Random remix: enabled {len(selected)} of {len(spawnables)} spawnables "
            f"({percentage:.0f}%) at count {spawn_count}.",
            applied=success,
        )
        all_actions.insert(0, summary)
        return IntegrationResult(
            success=success,
            actions=all_actions,
            backup_dir=self.editor.mission_root / "backups" / "integration",
        )

    def restore_defaults(self) -> IntegrationResult:
        """Remove all mod-added spawnables, keeping only vanilla templates."""
        vanilla_names = {name.lower() for name in VEHICLE_TEMPLATES}
        actions: List[IntegrationAction] = []

        for file_name, label in (
            ("events.xml", "event"),
            ("cfgspawnabletypes.xml", "spawnable type"),
            ("types.xml", "type"),
        ):
            path = self.editor.path_for(file_name)
            if path is None or not path.exists():
                actions.append(IntegrationAction(
                    file_name, f"{file_name} not present; nothing to restore.", applied=True
                ))
                continue

            text = path.read_text(encoding="utf-8", errors="ignore").strip()
            if not text:
                # Empty file is already at default state.
                actions.append(IntegrationAction(
                    file_name, f"{file_name} is empty; nothing to restore.", applied=True
                ))
                continue

            root = self.editor._load(file_name)
            if root is None:
                actions.append(IntegrationAction(
                    file_name, f"Could not parse {file_name}", applied=False, error="Parse error"
                ))
                continue

            removed = 0
            tag = "event" if file_name == "events.xml" else "type"
            for elem in list(root.findall(tag)):
                if elem.get("name", "").lower() not in vanilla_names:
                    root.remove(elem)
                    removed += 1

            ok = self.editor._save(file_name, root)
            actions.append(IntegrationAction(
                file_name,
                f"Removed {removed} mod {label}(s) from {file_name}",
                applied=ok,
            ))

        success = all(a.applied for a in actions)
        return IntegrationResult(
            success=success,
            actions=actions,
            backup_dir=self.editor.mission_root / "backups" / "integration",
        )
