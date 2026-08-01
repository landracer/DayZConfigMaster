# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Headless discovery of mod settings files.

Scans Steam Workshop mod folders and the active mission directory for
configuration files that the interactive mod settings editor can open.
"""

import re
from pathlib import Path
from typing import Dict, List, Optional, Set, Tuple


# Mission-side config locations that get friendly display names.
KNOWN_MISSION_PATTERNS: Dict[str, List[str]] = {
    "Mission Core": ["*.xml", "*.json"],
    "Mission Database": ["db/*.xml", "db/*.json"],
    "DayZ Expansion": ["expansion/**/*.json", "expansion/**/*.xml"],
    "TraderPlus": ["TraderPlus/*.json", "TraderPlus/*.xml"],
}

# File extensions we consider editable mod settings.
SETTINGS_SUFFIXES = {".json", ".xml", ".cpp", ".hpp", ".txt"}

# Folders inside a workshop mod that are never user-editable settings.
WORKSHOP_IGNORED_FOLDERS = {"keys", "addons"}
WORKSHOP_IGNORED_FILES = {"meta.cpp", "mod.cpp"}


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
                # Some meta files use Steam localization tokens like "$STR...";
                # fall back to the folder id in that case.
                if value and not value.startswith(("$", "#")):
                    return value
        except (OSError, PermissionError):
            pass
    return None


def _sanitize_display_name(name: str, fallback: str) -> str:
    """Return a non-empty display name, falling back to *fallback* if needed."""
    if name and str(name).strip():
        return str(name).strip()
    return fallback.strip() or "Unknown"


def _is_workshop_mod_id(name: str) -> bool:
    """Return True if *name* looks like a numeric Steam Workshop item id."""
    try:
        int(name)
        return True
    except ValueError:
        return False


def detect_mod_settings_files(
    workshop_dir: Optional[str],
    mission_root: Optional[Path],
) -> List[Tuple[str, str, Path]]:
    """Scan workshop mod folders and the mission folder for settings files.

    Returns a list of ``(mod_display_name, relative_file_path, file_path)``
    tuples. The same physical file is never returned twice.
    """
    results: List[Tuple[str, str, Path]] = []
    seen_paths: Set[Path] = set()

    # ------------------------------------------------------------------
    # 1) Workshop mod folders.
    # ------------------------------------------------------------------
    if workshop_dir:
        content_path = Path(workshop_dir)
        if content_path.exists():
            for folder in sorted(content_path.iterdir()):
                if not folder.is_dir() or not _is_workshop_mod_id(folder.name):
                    continue

                display_name = _sanitize_display_name(
                    _read_workshop_display_name(folder), folder.name
                )
                for path in sorted(folder.rglob("*")):
                    if not path.is_file():
                        continue
                    if path.suffix.lower() not in SETTINGS_SUFFIXES:
                        continue
                    rel_parts = path.relative_to(folder).parts
                    if rel_parts[0].lower() in WORKSHOP_IGNORED_FOLDERS:
                        continue
                    if path.name.lower() in WORKSHOP_IGNORED_FILES:
                        continue
                    if path in seen_paths:
                        continue
                    seen_paths.add(path)
                    rel_path = path.relative_to(folder).as_posix()
                    results.append((display_name, rel_path, path))

    # ------------------------------------------------------------------
    # 2) Active mission folder.
    # ------------------------------------------------------------------
    if mission_root and mission_root.exists():
        for mod_name, patterns in KNOWN_MISSION_PATTERNS.items():
            for pattern in patterns:
                for path in sorted(mission_root.glob(pattern)):
                    if path.is_file() and path not in seen_paths:
                        seen_paths.add(path)
                        rel_path = path.relative_to(mission_root).as_posix()
                        results.append((mod_name, rel_path, path))

        # Catch any other mod config folders not in the known list.
        for path in sorted(mission_root.rglob("*")):
            if not path.is_file() or path.suffix.lower() not in SETTINGS_SUFFIXES:
                continue
            if path in seen_paths:
                continue
            rel = path.relative_to(mission_root)
            if len(rel.parts) == 1:
                continue
            if rel.parts[0].lower() == "db":
                continue
            seen_paths.add(path)
            folder = _sanitize_display_name(rel.parts[0], "Mission")
            rel_path = rel.as_posix()
            results.append((folder, rel_path, path))

    return results
