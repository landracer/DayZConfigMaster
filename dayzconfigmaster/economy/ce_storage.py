# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Central Economy storage helpers.

DayZ caches parsed economy XML in ``storage_1/data/*.bin`` files.  When the
source XML changes (mod integration, nominal repair, manual edits, etc.) the
server often restores the old binary cache and ignores new types.  These
helpers detect that situation and clear the cached binaries after backing them
up.
"""

from __future__ import annotations

import hashlib
import json
import shutil
from datetime import datetime
from pathlib import Path
from typing import Dict, Optional, Tuple


# Files whose content directly feeds the CE binary cache.  The cache is also
# sensitive to mapgroupproto/cfglimitsdefinition changes, so include them.
ECONOMY_XML_FILES = [
    "db/types.xml",
    "db/events.xml",
    "db/globals.xml",
    "mapgroupproto.xml",
    "cfglimitsdefinition.xml",
    "cfgspawnabletypes.xml",
    "cfgeconomycore.xml",
]

_HASH_STATE_FILENAME = "ce_economy_hashes.json"


def _file_hash(path: Path) -> str:
    """Return a short sha256 hash of a file's contents, or empty if missing."""
    if not path.exists():
        return ""
    try:
        return hashlib.sha256(path.read_bytes()).hexdigest()[:16]
    except OSError:
        return ""


def get_economy_xml_hashes(mission_dir: Path) -> Dict[str, str]:
    """Return a mapping of economy XML paths (relative to mission) to hashes."""
    return {
        rel_path: _file_hash(mission_dir / rel_path)
        for rel_path in ECONOMY_XML_FILES
    }


def _hash_state_path(instance_root: Path) -> Path:
    return instance_root / "dcm_config" / _HASH_STATE_FILENAME


def get_stored_economy_hashes(instance_root: Path) -> Dict[str, str]:
    """Return the last-saved economy XML hashes for this instance."""
    path = _hash_state_path(instance_root)
    if not path.exists():
        return {}
    try:
        data = json.loads(path.read_text(encoding="utf-8"))
        if isinstance(data, dict):
            return data
    except (json.JSONDecodeError, OSError):
        pass
    return {}


def store_economy_hashes(instance_root: Path, hashes: Dict[str, str]) -> None:
    """Persist economy XML hashes so stale-cache detection works next start."""
    path = _hash_state_path(instance_root)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(
        json.dumps(hashes, indent=2, ensure_ascii=False, sort_keys=True),
        encoding="utf-8",
    )


def find_ce_storage_data_dir(
    instance_root: Path, mission_target_name: str
) -> Optional[Path]:
    """Return ``storage_1/data`` for the mission if it exists."""
    data_dir = instance_root / "mpmissions" / mission_target_name / "storage_1" / "data"
    return data_dir if data_dir.exists() else None


def has_ce_storage_data(instance_root: Path, mission_target_name: str) -> bool:
    """Return True if cached CE ``.bin`` files are present."""
    data_dir = find_ce_storage_data_dir(instance_root, mission_target_name)
    if not data_dir:
        return False
    return any(data_dir.iterdir()) if data_dir.exists() else False


def backup_and_clear_ce_storage(
    instance_root: Path,
    mission_target_name: str,
    backup_label: Optional[str] = None,
    only_bin_files: bool = False,
) -> Tuple[bool, str, Optional[Path]]:
    """Move ``storage_1/data/*`` to a backup folder and return the backup path.

    DayZ keeps recovery copies with extensions ``.001``/``.002``; by default
    those are moved too so the server cannot restore an old cache.  Set
    ``only_bin_files=True`` to clear only the primary ``.bin`` files while
    leaving recovery copies and player-related files untouched.
    """
    data_dir = find_ce_storage_data_dir(instance_root, mission_target_name)
    if not data_dir:
        return True, "No CE storage data directory found", None

    files = list(data_dir.iterdir())
    if not files:
        return True, "CE storage data directory is empty", None

    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    label = f"{backup_label}_" if backup_label else ""
    backup_dir = (
        instance_root
        / "backups"
        / "ce_storage"
        / f"{label}{mission_target_name}_{timestamp}"
    )
    backup_dir.mkdir(parents=True, exist_ok=True)

    moved = 0
    for src in sorted(files):
        if not src.is_file():
            continue
        if only_bin_files and src.suffix.lower() != ".bin":
            continue
        shutil.move(str(src), str(backup_dir / src.name))
        moved += 1

    return True, f"Backed up and cleared {moved} CE storage files to {backup_dir}", backup_dir


def economy_storage_needs_refresh(
    instance_root: Path, mission_target_name: str
) -> Tuple[bool, Dict[str, str], Dict[str, str]]:
    """Return (stale, current_hashes, previous_hashes).

    ``stale`` is True when cached bins exist and any tracked economy XML file
    has changed since the hashes were last stored.  It is also treated as
    stale when no previous hashes exist but cached bins do, because we cannot
    prove the cache matches the current XML (e.g. after a manual edit).
    """
    mission_dir = instance_root / "mpmissions" / mission_target_name
    current = get_economy_xml_hashes(mission_dir)
    previous = get_stored_economy_hashes(instance_root)
    has_data = has_ce_storage_data(instance_root, mission_target_name)

    if not has_data:
        return False, current, previous

    if not previous:
        return True, current, previous

    if current == previous:
        return False, current, previous

    return True, current, previous
