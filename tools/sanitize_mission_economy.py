#!/usr/bin/env python3
# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Quarantine stale/corrupt root-level economy overrides.

Broken spawn generators and older DCM builds sometimes leave a mission-root
``types.xml`` and ``events.xml`` behind.  When ``cfgeconomycore.xml`` has no
``<ce>`` section (the stock DayZ layout), the server loads ``db/types.xml``
and ``db/events.xml`` by default.  A root ``types.xml`` in that situation is
ignored for direct loading but is still merged by DCM's aircraft lifetime
logic, re-introducing bogus vehicle/wreck spawns into the loot economy.

Run this before starting a server manually:

    python3 tools/sanitize_mission_economy.py /path/to/instances/server1

The script moves offending root files to ``*.xml.corrupt`` / ``*.xml.shadow``
backups and leaves the ``db/`` economy files intact.
"""

from __future__ import annotations

import argparse
import shutil
import sys
from pathlib import Path
from xml.etree import ElementTree as ET


def _quarantine(root_file: Path, db_file: Path, label: str) -> bool:
    if not root_file.exists():
        return False
    if not db_file.exists():
        print(f"[KEEP] root {label}: no db/{label} found; mission may rely on root file")
        return False

    try:
        tree = ET.parse(str(root_file))
    except ET.ParseError:
        backup = root_file.with_suffix(".xml.corrupt")
        if backup.exists():
            backup.unlink()
        shutil.move(str(root_file), str(backup))
        print(f"[FIX] unparseable root {label} quarantined -> {backup.name}")
        return True

    root_el = tree.getroot()
    bad_categories = {"weapon", "vehicle", "gear"}
    bad_count = sum(
        1
        for t in root_el.findall("type")
        if (cat := t.find("category")) is not None and cat.get("name") in bad_categories
    )

    if bad_count >= 3:
        backup = root_file.with_suffix(".xml.corrupt")
        if backup.exists():
            backup.unlink()
        shutil.move(str(root_file), str(backup))
        print(f"[FIX] corrupted root {label} ({bad_count} bad categories) quarantined -> {backup.name}")
        return True

    backup = root_file.with_suffix(".xml.shadow")
    if backup.exists():
        backup.unlink()
    shutil.move(str(root_file), str(backup))
    print(f"[FIX] shadowing root {label} quarantined -> {backup.name}")
    return True


def _is_real_mission_dir(path: Path) -> bool:
    """Return True for actual map mission folders, skipping backup copies."""
    if not path.is_dir():
        return False
    name = path.name
    # Only process DayZ mission folders.
    if not name.startswith("dayzOffline."):
        return False
    # Skip backup / snapshot copies.
    lower = name.lower()
    skip_markers = ("-original", "-bak", "-backup", "-pre-", "-post-", "-old", "-copy")
    return not any(marker in lower for marker in skip_markers)


def sanitize(instance_root: Path) -> int:
    fixed = 0
    mpmissions = instance_root / "mpmissions"
    if not mpmissions.exists():
        print(f"[SKIP] no mpmissions folder in {instance_root}")
        return 0

    for mission_dir in mpmissions.iterdir():
        if not _is_real_mission_dir(mission_dir):
            continue
        changed = _quarantine(
            mission_dir / "types.xml",
            mission_dir / "db" / "types.xml",
            f"{mission_dir.name}/types.xml",
        )
        changed |= _quarantine(
            mission_dir / "events.xml",
            mission_dir / "db" / "events.xml",
            f"{mission_dir.name}/events.xml",
        )
        if changed:
            fixed += 1

    if not fixed:
        print("[OK] no shadowing/corrupt root economy files found")
    return fixed


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("instance_root", type=Path, help="Path to a server instance root")
    args = parser.parse_args(argv)

    if not args.instance_root.exists():
        print(f"[ERROR] instance root does not exist: {args.instance_root}", file=sys.stderr)
        return 1

    sanitize(args.instance_root)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
