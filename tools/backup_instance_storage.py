#!/usr/bin/env python3
# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Command-line tool to back up DayZ instance storage data.

This is intended to be run from cron for scheduled incremental backups.

Examples:
    # Back up instance 1 with default 7-day retention
    python3 tools/backup_instance_storage.py /home/sysadmin/Documents/DayZProjects/instances/server1

    # Back up with 12-hour retention (keeps only the last 12 hours)
    python3 tools/backup_instance_storage.py /home/sysadmin/Documents/DayZProjects/instances/server1 --retention 12

    # Back up all instances under a projects root
    python3 tools/backup_instance_storage.py /home/sysadmin/Documents/DayZProjects/instances --all

Cron examples:
    # Every 12 hours
    0 */12 * * * cd /home/sysadmin/Documents/git/DayzConfigMaster && /usr/bin/python3 tools/backup_instance_storage.py /home/sysadmin/Documents/DayZProjects/instances/server1

    # Every day at 3 AM for all instances
    0 3 * * * cd /home/sysadmin/Documents/git/DayzConfigMaster && /usr/bin/python3 tools/backup_instance_storage.py /home/sysadmin/Documents/DayZProjects/instances --all
"""

import argparse
import sys
from pathlib import Path

# Allow running the tool directly from the repo without installing the package.
REPO_ROOT = Path(__file__).resolve().parent.parent
if str(REPO_ROOT) not in sys.path:
    sys.path.insert(0, str(REPO_ROOT))


def backup_one(instance_root: Path, retention_hours: int) -> bool:
    from dayzconfigmaster.backups.storage_backup import StorageBackupManager

    manager = StorageBackupManager(
        instance_root,
        backup_root=instance_root / "backups" / "storage",
        retention_hours=retention_hours,
    )
    result = manager.create_backup()
    status = "OK" if result.success else "FAILED"
    print(f"[{status}] {instance_root}: {result.message}")
    if result.purged:
        for p in result.purged:
            print(f"  purged old backup: {p.name}")
    return result.success


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Back up DayZ instance storage (players.db + storage_1/data)."
    )
    parser.add_argument("path", help="Path to instance root or instances directory")
    parser.add_argument(
        "--all",
        action="store_true",
        help="Back up every server* subdirectory under PATH",
    )
    parser.add_argument(
        "--retention",
        type=int,
        default=168,
        help="Retention window in hours (default: 168 = 7 days)",
    )
    args = parser.parse_args()

    target = Path(args.path)
    if not target.exists():
        print(f"Path does not exist: {target}", file=sys.stderr)
        return 1

    if args.all:
        if not target.is_dir():
            print(f"--all requires a directory: {target}", file=sys.stderr)
            return 1
        ok = True
        for entry in sorted(target.iterdir()):
            if entry.is_dir() and entry.name.startswith("server"):
                if not backup_one(entry, args.retention):
                    ok = False
        return 0 if ok else 1

    return 0 if backup_one(target, args.retention) else 1


if __name__ == "__main__":
    sys.exit(main())
