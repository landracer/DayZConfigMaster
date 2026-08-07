# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Per-instance storage_1 backup management.

Backups are stored inside each instance folder so everything for a server
instance stays together:

    instances/server<N>/backups/storage_1/<map_name>/<timestamp>/
"""

from __future__ import annotations

import shutil
from dataclasses import dataclass
from datetime import datetime, timedelta, timezone
from pathlib import Path
from typing import List, Optional, Tuple


@dataclass(frozen=True)
class StorageBackupInfo:
    """Metadata about a single storage_1 backup."""

    path: Path
    timestamp: datetime
    size_bytes: int

    @property
    def age(self) -> timedelta:
        return datetime.now(timezone.utc) - self.timestamp


class InstanceStorageBackupManager:
    """Create, list, restore and prune storage_1 backups for one instance."""

    def __init__(
        self,
        instance_root: Path,
        instance_id: int,
        map_name: str,
    ):
        self.instance_root = Path(instance_root)
        self.instance_id = instance_id
        self.map_name = map_name or "unknown"
        self.backup_dir = (
            self.instance_root
            / "backups"
            / "storage_1"
            / self.map_name.replace(".", "_")
        )

    def _ensure_dir(self) -> None:
        self.backup_dir.mkdir(parents=True, exist_ok=True)

    @staticmethod
    def _parse_timestamp(name: str) -> Optional[datetime]:
        try:
            return datetime.strptime(name, "%Y%m%d_%H%M%S").replace(
                tzinfo=timezone.utc
            )
        except ValueError:
            return None

    def list_backups(self) -> List[StorageBackupInfo]:
        """Return all storage_1 backups, newest first."""
        if not self.backup_dir.exists():
            return []

        results: List[StorageBackupInfo] = []
        for entry in self.backup_dir.iterdir():
            if not entry.is_dir():
                continue
            ts = self._parse_timestamp(entry.name)
            if ts is None:
                continue
            size = sum(
                f.stat().st_size for f in entry.rglob("*") if f.is_file()
            )
            results.append(StorageBackupInfo(path=entry, timestamp=ts, size_bytes=size))

        results.sort(key=lambda b: b.timestamp, reverse=True)
        return results

    def create_backup(self, storage_path: Path) -> Tuple[bool, str]:
        """Create a dated copy of storage_path under the instance backup dir."""
        storage_path = Path(storage_path)
        if not storage_path.exists():
            return False, f"storage_1 folder not found: {storage_path}"

        self._ensure_dir()
        timestamp = datetime.now(timezone.utc).strftime("%Y%m%d_%H%M%S")
        backup_path = self.backup_dir / timestamp

        if backup_path.exists():
            return False, f"backup already exists: {backup_path}"

        try:
            shutil.copytree(
                str(storage_path),
                str(backup_path),
                symlinks=True,
                ignore=shutil.ignore_patterns("*.log", "*.rpt", "*.tmp"),
            )
            return True, str(backup_path)
        except Exception as exc:
            return False, f"storage backup failed: {exc}"

    def ensure_backup_before_start(
        self,
        storage_path: Path,
        max_age_minutes: int = 60,
        force: bool = False,
    ) -> Tuple[bool, str]:
        """Create a backup if none exists or the newest is older than max_age.

        This is the recommended hook to call before starting an instance: the
        storage files are intact from the previous run and not being written.
        """
        if force:
            return self.create_backup(storage_path)

        backups = self.list_backups()
        if not backups:
            return self.create_backup(storage_path)

        cutoff = datetime.now(timezone.utc) - timedelta(minutes=max_age_minutes)
        if backups[0].timestamp < cutoff:
            return self.create_backup(storage_path)

        return True, f"recent backup exists: {backups[0].path.name}"

    def restore_backup(
        self,
        backup_path: Path,
        target_storage_path: Path,
        dry_run: bool = False,
    ) -> Tuple[bool, str]:
        """Restore a storage_1 backup to the live storage path."""
        backup_path = Path(backup_path)
        target_storage_path = Path(target_storage_path)

        if not backup_path.exists():
            return False, f"backup not found: {backup_path}"
        if not backup_path.is_dir():
            return False, f"backup is not a directory: {backup_path}"

        if dry_run:
            return True, f"would restore {backup_path.name} to {target_storage_path}"

        # Safety: never overwrite the backup itself.
        try:
            backup_path.resolve().relative_to(self.backup_dir.resolve())
        except ValueError:
            return False, "refusing to restore from outside the backup directory"

        try:
            if target_storage_path.exists():
                shutil.rmtree(target_storage_path)
            shutil.copytree(str(backup_path), str(target_storage_path), symlinks=True)
            return True, f"restored {backup_path.name} to {target_storage_path}"
        except Exception as exc:
            return False, f"restore failed: {exc}"

    def delete_backup(self, backup_path: Path) -> Tuple[bool, str]:
        """Delete a single backup directory."""
        backup_path = Path(backup_path)
        if not backup_path.exists():
            return False, f"backup not found: {backup_path}"
        try:
            backup_path.resolve().relative_to(self.backup_dir.resolve())
        except ValueError:
            return False, "refusing to delete backup outside the backup directory"

        try:
            shutil.rmtree(backup_path)
            return True, f"deleted {backup_path.name}"
        except Exception as exc:
            return False, f"delete failed: {exc}"

    def start_fresh(self, storage_path: Path) -> Tuple[bool, str]:
        """Reset storage_path to a clean, empty state for a new map run.

        A safety backup of the current storage is created first if it exists.
        """
        storage_path = Path(storage_path)

        if storage_path.exists():
            safety_ok, safety_msg = self.create_backup(storage_path)
            if not safety_ok:
                return False, f"failed to create safety backup before fresh start: {safety_msg}"
            try:
                shutil.rmtree(storage_path)
            except Exception as exc:
                return False, f"failed to remove old storage: {exc}"

        try:
            storage_path.mkdir(parents=True, exist_ok=True)
            # DayZ expects some files to exist or it will create them. A minimal
            # empty directory is enough for a true fresh start.
            return True, f"started fresh storage at {storage_path}"
        except Exception as exc:
            return False, f"failed to create fresh storage directory: {exc}"

    def prune(
        self,
        max_count: Optional[int] = None,
        max_age_days: Optional[int] = None,
    ) -> Tuple[int, List[str]]:
        """Remove old backups exceeding count or age limits.

        Returns (number_deleted, messages).
        """
        backups = self.list_backups()
        if not backups:
            return 0, ["no backups to prune"]

        to_delete: List[StorageBackupInfo] = []

        if max_count is not None and len(backups) > max_count:
            to_delete.extend(backups[max_count:])

        if max_age_days is not None:
            cutoff = datetime.now(timezone.utc) - timedelta(days=max_age_days)
            for b in backups:
                if b.timestamp < cutoff and b not in to_delete:
                    to_delete.append(b)

        messages: List[str] = []
        deleted = 0
        for b in to_delete:
            ok, msg = self.delete_backup(b.path)
            if ok:
                deleted += 1
            messages.append(msg)

        if not messages:
            messages.append("nothing to prune")
        return deleted, messages


def storage_backup_manager_for_instance(
    instance_root: Path,
    instance_id: int,
    map_name: str,
) -> InstanceStorageBackupManager:
    """Factory helper for the common case."""
    return InstanceStorageBackupManager(instance_root, instance_id, map_name)
