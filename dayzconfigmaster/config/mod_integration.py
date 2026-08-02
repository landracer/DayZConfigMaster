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

import re
import shutil
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
        return self.success and not any(a.error for a in self.actions)


SUPPORTED_FILES = (
    "events.xml",
    "cfgspawnabletypes.xml",
    "types.xml",
    "cfgeventspawns.xml",
)


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


# Heuristic keywords used when scanning XML files for candidate vehicle class
# names.  This is intentionally broad so mod-added helicopters, boats, bikes,
# and armored vehicles are also surfaced.
_VEHICLE_KEYWORDS = (
    "car", "truck", "van", "offroad", "hatchback", "sedan", "suv", "bus",
    "bike", "motor", "moto", "quad", "atv", "uaz", "v3s", "gunter",
    "sarka", "olga", "ada", "m1025", "humvee", "tank", "apc", "heli",
    "helicopter", "plane", "boat", "ship", "vehicle",
)
_VEHICLE_RE = re.compile(
    "|".join(re.escape(k) for k in _VEHICLE_KEYWORDS),
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
    """Parse *path* and return class names that look like vehicles."""
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
            if name and _looks_like_vehicle(name):
                names.append(name)

    # <event name="..."> appears in events.xml.
    if tag == "events":
        for elem in root.iter("event"):
            name = elem.get("name")
            if name and _looks_like_vehicle(name):
                names.append(name)

    return names


def discover_vehicle_classes(
    mission_root: Optional[Path] = None,
    workshop_dir: Optional[Path] = None,
) -> Dict[str, str]:
    """Return a mapping of vehicle class name to a human-readable source.

    Discovers candidate vehicle class names from:
      * built-in vanilla templates (e.g. OffroadHatchback)
      * the active mission's XML files
      * workshop mod folders that ship XML fragments

    The returned dict is stable: vanilla templates are included first, then
    mission-derived names, then workshop-derived names.  Workshop entries are
    keyed by the mod's display name (read from meta.cpp/mod.cpp) when possible.
    """
    results: Dict[str, str] = {}

    # 1) Vanilla templates are always available as a baseline.
    for name in VEHICLE_TEMPLATES:
        results[name] = "Vanilla template"

    # 2) Active mission XML files.
    if mission_root is not None and mission_root.exists():
        for xml_name in ("types.xml", "cfgspawnabletypes.xml", "events.xml"):
            path = mission_root / xml_name
            if not path.exists():
                continue
            for name in _extract_vehicle_names_from_xml(path):
                if name not in results:
                    results[name] = f"Mission: {xml_name}"

    # 3) Workshop mod folders.
    if workshop_dir is not None and workshop_dir.exists():
        for folder in sorted(workshop_dir.iterdir()):
            if not folder.is_dir() or not _is_workshop_mod_id(folder.name):
                continue
            display_name = _read_workshop_display_name(folder) or folder.name
            for xml_name in ("types.xml", "cfgspawnabletypes.xml", "events.xml"):
                path = folder / xml_name
                if not path.exists():
                    continue
                for name in _extract_vehicle_names_from_xml(path):
                    if name not in results:
                        results[name] = display_name

    return results


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


class XmlConfigEditor:
    """Edit core DayZ server XML configuration files."""

    supported_files = list(SUPPORTED_FILES)

    def __init__(self, mission_root: Path):
        self.mission_root = Path(mission_root)
        self._paths: Dict[str, Path] = {
            name: self.mission_root / name for name in SUPPORTED_FILES
        }

    def path_for(self, name: str) -> Optional[Path]:
        """Return the on-disk path for a supported XML file."""
        return self._paths.get(name)

    def file_exists(self, name: str) -> bool:
        path = self.path_for(name)
        return bool(path and path.exists())

    def read_text(self, name: str) -> Optional[str]:
        path = self.path_for(name)
        if not path or not path.exists():
            return None
        try:
            return path.read_text(encoding="utf-8")
        except OSError:
            return None

    def _load(self, name: str) -> Optional[ET.Element]:
        text = self.read_text(name)
        if text is None:
            return None
        try:
            return ET.fromstring(text)
        except ET.ParseError:
            return None

    def _save(self, name: str, root: ET.Element) -> bool:
        path = self.path_for(name)
        if path is None:
            return False
        try:
            _indent_xml(root)
            tree = ET.ElementTree(root)
            backup = _backup_path(path)
            if path.exists():
                shutil.copy2(path, backup)
            tree.write(path, encoding="utf-8", xml_declaration=True)
            return True
        except OSError:
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

    def enable_vehicle_spawning(self, vehicle_class_name: str, active: bool = True) -> bool:
        """Enable or disable a vehicle event in events.xml."""
        root = self._load("events.xml")
        if root is None:
            return False
        event = self._find_event(root, vehicle_class_name)
        if event is None:
            event = ET.SubElement(root, "event", {"name": vehicle_class_name})
            ET.SubElement(event, "nominal", {"min": "1", "max": "1"})
            ET.SubElement(event, "min", {"min": "1", "max": "1"})
            ET.SubElement(event, "max", {"min": "1", "max": "1"})
            ET.SubElement(event, "lifetime", {"min": "1", "max": "1"})
            ET.SubElement(event, "restock", {"min": "0", "max": "0"})
            ET.SubElement(event, "saferadius", {"min": "0", "max": "0"})
            ET.SubElement(event, "distanceradius", {"min": "0", "max": "0"})
            ET.SubElement(event, "spawnoffset", {"min": "0", "max": "0"})
            ET.SubElement(event, "spawnrange", {"min": "0", "max": "0"})
            ET.SubElement(event, "countoffset", {"min": "0", "max": "0"})
            ET.SubElement(event, "flags", {"deletable": "0", "init_random_spawn": "0"})
            ET.SubElement(event, "position", {"fixed": "0"})
            ET.SubElement(event, "limit", {"mixed": "0", "nominal": "1"})
            ET.SubElement(event, "active", {"min": "1", "max": "1"})
            ET.SubElement(event, "children")
        active_elem = event.find("active")
        if active_elem is None:
            active_elem = ET.SubElement(event, "active", {"min": "1", "max": "1"})
        min_v = "1" if active else "0"
        max_v = "1" if active else "0"
        active_elem.set("min", min_v)
        active_elem.set("max", max_v)
        return self._save("events.xml", root)

    def is_vehicle_spawning_enabled(self, vehicle_class_name: str) -> Optional[bool]:
        root = self._load("events.xml")
        if root is None:
            return None
        event = self._find_event(root, vehicle_class_name)
        if event is None:
            return None
        active = event.find("active")
        if active is None:
            return None
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
        """Define or update a spawnable type with optional attachments."""
        root = self._load("cfgspawnabletypes.xml")
        if root is None:
            return False
        type_elem = self._find_spawnable_type(root, vehicle_class_name)
        if type_elem is None:
            type_elem = ET.SubElement(root, "type", {"name": vehicle_class_name})
        else:
            # Clear existing attachments so we can recreate them deterministically.
            for child in list(type_elem):
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
    ) -> bool:
        """Ensure a vehicle type exists in types.xml."""
        root = self._load("types.xml")
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
        ET.SubElement(type_elem, "usage", {"name": "Town"})
        ET.SubElement(type_elem, "value", {"name": "Tier12"})
        return self._save("types.xml", root)

    def type_exists(self, vehicle_class_name: str) -> bool:
        root = self._load("types.xml")
        if root is None:
            return False
        return self._find_type(root, vehicle_class_name) is not None


class ModIntegrationWorkflow:
    """High-level workflow for integrating a mod's XML requirements."""

    def __init__(self, mission_root: Path):
        self.editor = XmlConfigEditor(mission_root)

    def discover_vehicles(
        self,
        workshop_dir: Optional[Path] = None,
    ) -> List[Tuple[str, str]]:
        """Return a sorted list of (vehicle_class_name, source) tuples."""
        found = discover_vehicle_classes(self.editor.mission_root, workshop_dir)
        return sorted(found.items())

    def detect_actions(self, vehicle_class_name: str) -> List[IntegrationAction]:
        """Return the list of changes needed for a vehicle mod."""
        actions: List[IntegrationAction] = []

        if not self.editor.file_exists("events.xml"):
            actions.append(IntegrationAction(
                "events.xml",
                f"events.xml is missing; cannot enable {vehicle_class_name} spawning.",
            ))
        elif self.editor.is_vehicle_spawning_enabled(vehicle_class_name) is not True:
            actions.append(IntegrationAction(
                "events.xml",
                f"Enable vehicle spawning for {vehicle_class_name}.",
            ))

        if not self.editor.file_exists("cfgspawnabletypes.xml"):
            actions.append(IntegrationAction(
                "cfgspawnabletypes.xml",
                f"cfgspawnabletypes.xml is missing; cannot define attachments for {vehicle_class_name}.",
            ))
        elif not self.editor.has_vehicle_attachments(vehicle_class_name):
            actions.append(IntegrationAction(
                "cfgspawnabletypes.xml",
                f"Define spawnable type/attachments for {vehicle_class_name}.",
            ))

        if not self.editor.file_exists("types.xml"):
            actions.append(IntegrationAction(
                "types.xml",
                f"types.xml is missing; cannot define {vehicle_class_name} entry.",
            ))
        elif not self.editor.type_exists(vehicle_class_name):
            actions.append(IntegrationAction(
                "types.xml",
                f"Add {vehicle_class_name} to types.xml.",
            ))

        return actions

    def integrate_vehicle_mod(self, vehicle_class_name: str) -> IntegrationResult:
        """Apply all XML changes needed to enable a vehicle mod."""
        actions: List[IntegrationAction] = []

        def add_action(name: str, ok: bool, description: str) -> None:
            actions.append(IntegrationAction(
                name, description, applied=ok, error="" if ok else "Failed to apply change",
            ))

        ok = self.editor.enable_vehicle_spawning(vehicle_class_name, active=True)
        add_action("events.xml", ok, f"Enable vehicle spawning for {vehicle_class_name}")

        ok = self.editor.add_vehicle_attachments(vehicle_class_name)
        if not ok:
            ok = self.editor.define_spawnable_type(vehicle_class_name, [])
        add_action("cfgspawnabletypes.xml", ok, f"Define spawnable type for {vehicle_class_name}")

        ok = self.editor.add_vehicle_to_types_xml(vehicle_class_name)
        add_action("types.xml", ok, f"Add {vehicle_class_name} to types.xml")

        success = all(a.applied for a in actions)
        return IntegrationResult(
            success=success,
            actions=actions,
            backup_dir=self.editor.mission_root / "backups" / "integration",
        )
