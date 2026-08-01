# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Map and Mod Parser for DayZ Workshop Items

This module distinguishes between:
- MAPS: Files/folders containing .map files or dayzOffline.[name]
- MODS: Folders with config.cpp but no map references
"""

import re
from pathlib import Path
from typing import Dict, List, Optional, Tuple
from dataclasses import dataclass, field


@dataclass
class WorkshopClassification:
    """Classification result for a local workshop item."""
    workshop_id: str
    item_name: str
    item_type: str  # 'map' or 'mod'
    path: str
    tags: List[str] = field(default_factory=list)


class MapModClassifier:
    """Classify workshop items as maps or mods."""

    # Keywords in meta.cpp/mod.cpp names that strongly indicate a map.
    _MAP_NAME_KEYWORDS = (
        "map", "island", "terrain", "world", "chiemsee", "deerisle",
        "chernarus", "livonia", "valning", "namalsk", "banov", "esseker",
        "rostow", "takistan", "raman", "alteria", "sarov", "stuart",
        "xzone", "chernobyl", "takistanplus", "deer isle", "winter chernarus",
        "winter livonia", "winter chiemsee", "winter valning", "summerchernarus",
        "vanillaplusplusmap", "namalsk island", "stuart island", "sarov map",
        "valning map", "raman map", "xzone || chernobyl || map", "sahinkaya",
    )

    def classify_workshop_item(self, item_path: Path) -> str:
        """
        Determine if a workshop folder is a MAP or MOD.

        Args:
            item_path: Path to the workshop item folder

        Returns:
            'map' or 'mod'
        """
        if not item_path.exists() or not item_path.is_dir():
            return "mod"

        # Check for .map files (definitive map indicator)
        try:
            map_files = list(item_path.rglob("*.map"))
            if map_files:
                return "map"
        except (OSError, PermissionError):
            pass

        # Check for dayzOffline.[name] pattern
        try:
            for item in item_path.iterdir():
                if item.is_dir() and item.name.startswith("dayzOffline."):
                    return "map"
        except (OSError, PermissionError):
            pass

        # Terrain/map-only workshop items often ship addons/world.pbo and
        # addons/surfaces.pbo without a dayzOffline folder or .map files.
        # Detect this PBO signature so new maps don't require a keyword entry.
        addons_dir = item_path / "addons"
        if addons_dir.is_dir():
            addon_names = {p.name.lower() for p in addons_dir.iterdir() if p.is_file()}
            if "world.pbo" in addon_names and "surfaces.pbo" in addon_names:
                return "map"

        # Check for config.cpp (mod indicator)
        config_file = item_path / "config.cpp"
        if config_file.exists():
            try:
                content = config_file.read_text(encoding='utf-8', errors='ignore')
                # If config doesn't reference a map, it's a mod
                if 'map' not in content.lower() and 'mpmissions' not in content.lower():
                    return "mod"
            except (OSError, PermissionError):
                pass

        # Heuristic: items with a map-suggesting name in mod.cpp/meta.cpp are maps.
        # This catches map-only workshop items that do not ship dayzOffline folders
        # or .map files but are still terrain/map content.
        name = self._read_mod_cpp_name(item_path) or self._read_meta_cpp_name(item_path) or ""
        lower_name = name.lower()
        if any(kw in lower_name for kw in self._MAP_NAME_KEYWORDS):
            return "map"

        # Default to mod if unsure
        return "mod"

    def classify(self, item_path: Path) -> WorkshopClassification:
        """Return a full classification object for a workshop item."""
        item_type = self.classify_workshop_item(item_path)
        return WorkshopClassification(
            workshop_id=item_path.name,
            item_name=self._extract_display_name(item_path, item_type),
            item_type=item_type,
            path=str(item_path)
        )

    def _extract_display_name(self, folder: Path, item_type: str) -> str:
        """Extract a human-readable name from a workshop folder."""
        # For maps, prefer meta.cpp/mod.cpp name, then dayzOffline.[name].
        if item_type == "map":
            map_name = self._read_meta_cpp_name(folder)
            if map_name:
                return map_name
            try:
                for item in folder.iterdir():
                    if item.is_dir() and item.name.startswith("dayzOffline."):
                        return item.name[len("dayzOffline."):]
            except (OSError, PermissionError):
                pass

        # For mods, read mod.cpp name field if present.
        if item_type == "mod":
            mod_name = self._read_mod_cpp_name(folder)
            if mod_name:
                return mod_name

        return folder.name

    def _read_meta_cpp_name(self, folder: Path) -> Optional[str]:
        """Read the 'name' field from a meta.cpp file inside the folder."""
        meta_cpp = folder / "meta.cpp"
        if not meta_cpp.exists():
            return None
        try:
            content = meta_cpp.read_text(encoding="utf-8", errors="ignore")
            match = re.search(
                r'^\s*name\s*=\s*["\'](.+?)["\']\s*;',
                content,
                re.IGNORECASE | re.MULTILINE,
            )
            if match:
                value = match.group(1).strip()
                if value and not value.startswith("$"):
                    return value
        except (OSError, PermissionError):
            pass
        return None

    def _read_mod_cpp_name(self, folder: Path) -> Optional[str]:
        """Read a human-readable name from mod.cpp or meta.cpp inside the folder.

        Prefers the ``name`` field, then ``tooltip``, then ``overview``.
        Falls back to ``meta.cpp`` if no ``mod.cpp`` is present.
        """
        mod_cpp = folder / "mod.cpp"
        if mod_cpp.exists():
            try:
                content = mod_cpp.read_text(encoding="utf-8", errors="ignore")
                for field in ("name", "tooltip", "overview"):
                    match = re.search(
                        rf'^\s*{field}\s*=\s*["\'](.+?)["\']\s*;',
                        content,
                        re.IGNORECASE | re.MULTILINE,
                    )
                    if match:
                        value = match.group(1).strip()
                        if value and not value.startswith("$"):
                            return value
            except (OSError, PermissionError):
                pass

        meta_cpp = folder / "meta.cpp"
        if meta_cpp.exists():
            try:
                content = meta_cpp.read_text(encoding="utf-8", errors="ignore")
                match = re.search(
                    r'^\s*name\s*=\s*["\'](.+?)["\']\s*;',
                    content,
                    re.IGNORECASE | re.MULTILINE,
                )
                if match:
                    value = match.group(1).strip()
                    if value and not value.startswith("$"):
                        return value
            except (OSError, PermissionError):
                pass

        return None


class WorkshopMapParser:
    """Parse workshop directories for map and mod information."""

    def __init__(self, workshop_content_path: Optional[str] = None):
        """
        Initialize parser.

        Args:
            workshop_content_path: Path to Steam Workshop content folder
                (e.g. /path/to/steamapps/workshop/content/221100)
        """
        self.path = Path(workshop_content_path) if workshop_content_path else None
        self.classifier = MapModClassifier()

    def set_path(self, workshop_content_path: str) -> None:
        """Update the workshop content path."""
        self.path = Path(workshop_content_path)

    def get_workshop_maps(self) -> Dict[str, Tuple[str, str]]:
        """
        Get all maps from workshop content.

        Returns:
            {workshop_id: (folder_name, item_name)}
        """
        maps = {}
        if not self.path or not self.path.exists():
            return maps

        for folder in self.path.iterdir():
            if not folder.is_dir():
                continue
            try:
                _ = int(folder.name)
            except ValueError:
                continue

            classification = self.classifier.classify(folder)
            if classification.item_type == "map":
                maps[folder.name] = (folder.name, classification.item_name)

        return maps

    def get_workshop_mods(self) -> Dict[str, Tuple[str, str]]:
        """
        Get all mods from workshop content.

        Returns:
            {workshop_id: (folder_name, item_name)}
        """
        mods = {}
        if not self.path or not self.path.exists():
            return mods

        for folder in self.path.iterdir():
            if not folder.is_dir():
                continue
            try:
                _ = int(folder.name)
            except ValueError:
                continue

            classification = self.classifier.classify(folder)
            if classification.item_type == "mod":
                mods[folder.name] = (folder.name, classification.item_name)

        return mods

    def classify_all(self) -> List[WorkshopClassification]:
        """Classify all workshop items in the content path."""
        results = []
        if not self.path or not self.path.exists():
            return results

        for folder in self.path.iterdir():
            if not folder.is_dir():
                continue
            try:
                _ = int(folder.name)
            except ValueError:
                continue

            results.append(self.classifier.classify(folder))

        return results


def parse_stock_maps(dayz_server_path: str) -> List[str]:
    """
    Parse stock maps from the DayZServer mpmissions directory.

    Args:
        dayz_server_path: Path to DayZServer installation

    Returns:
        List of map names
    """
    maps = []
    mpmissions = Path(dayz_server_path) / "mpmissions"
    if not mpmissions.exists():
        return maps

    for item in mpmissions.iterdir():
        if item.is_dir():
            map_name = item.name
            if map_name.startswith("dayzOffline."):
                map_name = map_name[len("dayzOffline."):]
            if map_name:
                maps.append(map_name)

    return sorted(set(maps), key=str.lower)


def parse_workshop_maps(workshop_content_path: str) -> Dict[str, Tuple[str, str]]:
    """
    Parse workshop maps from Steam Workshop content directory.

    Args:
        workshop_content_path: Path to workshop content folder

    Returns:
        {workshop_id: (folder_name, item_name)}
    """
    parser = WorkshopMapParser(workshop_content_path)
    return parser.get_workshop_maps()


def get_all_available_maps(
    dayz_server_path: Optional[str] = None,
    workshop_content_path: Optional[str] = None,
    fallback_defaults: Optional[List[str]] = None
) -> Tuple[List[str], Dict[str, Tuple[str, str]]]:
    """
    Get all available maps from stock and workshop sources.

    Args:
        dayz_server_path: Path to DayZServer installation
        workshop_content_path: Path to workshop content folder
        fallback_defaults: Default maps if no others found

    Returns:
        Tuple of (stock_maps, workshop_maps_dict)
    """
    stock_maps = []
    if dayz_server_path:
        stock_maps = parse_stock_maps(dayz_server_path)

    workshop_maps = {}
    if workshop_content_path:
        workshop_maps = parse_workshop_maps(workshop_content_path)

    if not stock_maps and not workshop_maps and fallback_defaults:
        stock_maps = fallback_defaults

    return stock_maps, workshop_maps
