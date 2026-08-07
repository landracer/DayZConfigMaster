# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Cross-instance storage_1 backup discovery.

Allows an instance to locate backups for a given map across all sibling
instances under the same projects root, so map data can follow the map when
instances swap configurations.
"""

from __future__ import annotations

from pathlib import Path
from typing import List, Optional

from .instance_storage import InstanceStorageBackupManager, StorageBackupInfo


def _all_instance_roots(projects_root: Path) -> List[Path]:
    """Return all candidate instance roots under projects_root."""
    roots: List[Path] = []
    candidates = [
        projects_root / "instances",
        projects_root / ".." / "instances",
    ]
    for candidate in candidates:
        try:
            candidate = candidate.resolve()
        except OSError:
            continue
        if not candidate.exists():
            continue
        for item in candidate.iterdir():
            if item.is_dir():
                roots.append(item)
    return roots


def find_map_backups_across_instances(
    projects_root: Path,
    map_name: str,
    exclude_instance_root: Optional[Path] = None,
) -> List[StorageBackupInfo]:
    """Find all storage_1 backups for a map across every instance.

    Results are sorted newest first. The caller's own instance is included
    unless explicitly excluded.
    """
    results: List[StorageBackupInfo] = []
    if not map_name:
        return results

    seen_paths = set()
    for instance_root in _all_instance_roots(projects_root):
        if exclude_instance_root is not None:
            try:
                if instance_root.resolve() == exclude_instance_root.resolve():
                    continue
            except OSError:
                continue

        mgr = InstanceStorageBackupManager(instance_root, 0, map_name)
        for info in mgr.list_backups():
            try:
                resolved = info.path.resolve()
            except OSError:
                resolved = info.path
            if resolved in seen_paths:
                continue
            seen_paths.add(resolved)
            results.append(info)

    results.sort(key=lambda b: b.timestamp, reverse=True)
    return results
