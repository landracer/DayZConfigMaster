# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Focused incremental backup for DayZ instance storage data.

Backs up only the files that matter for player progress and world state:
- players.db
- storage_1/data (Central Economy cache)
- mpmissions/<mission>/storage_1/data (mission-specific persistence)

Uses hard-link based incremental backups (rsync --link-dest) so multiple
snapshots share unchanged blocks.  Older backups are purged by retention.
"""

import shutil
import subprocess
from dataclasses import dataclass
from datetime import datetime, timedelta
from pathlib import Path
from typing import List, Optional, Tuple


@dataclass
class StorageBackupResult:
    """Result of a storage backup operation."""

    success: bool
    backup_path: Optional[Path]
    size_bytes: int
    message: str
    purged: List[Path]


class StorageBackupManager:
    """Create and rotate incremental backups of DayZ instance storage."""

    def __init__(
        self,
        instance_root: Path,
        backup_root: Optional[Path] = None,
        retention_hours: int = 168,  # 7 days
    ):
        self.instance_root = Path(instance_root)
        self.backup_root = Path(backup_root) if backup_root else self.instance_root / "backups" / "storage"
        self.retention_hours = retention_hours

    def _rsync_available(self) -> bool:
        return shutil.which("rsync") is not None

    def _latest_backup(self) -> Optional[Path]:
        if not self.backup_root.exists():
            return None
        candidates = [p for p in self.backup_root.iterdir() if p.is_dir()]
        if not candidates:
            return None
        return max(candidates, key=lambda p: p.stat().st_mtime)

    def _purge_old_backups(self) -> List[Path]:
        if not self.backup_root.exists():
            return []
        cutoff = datetime.now() - timedelta(hours=self.retention_hours)
        purged: List[Path] = []
        for entry in self.backup_root.iterdir():
            if not entry.is_dir():
                continue
            try:
                mtime = datetime.fromtimestamp(entry.stat().st_mtime)
            except OSError:
                continue
            if mtime < cutoff:
                try:
                    shutil.rmtree(entry)
                    purged.append(entry)
                except OSError:
                    pass
        return purged

    def _collect_source_paths(self) -> List[Path]:
        """Return the list of storage-related files/directories to back up."""
        sources: List[Path] = []
        candidates = [
            self.instance_root / "players.db",
            self.instance_root / "storage_1" / "players.db",
            self.instance_root / "profiles" / "players.db",
            self.instance_root / "storage_1" / "data",
        ]
        # Also find storage_1/data under each mission folder.
        mpmissions = self.instance_root / "mpmissions"
        if mpmissions.exists():
            for mission_dir in mpmissions.iterdir():
                storage_data = mission_dir / "storage_1" / "data"
                if storage_data.exists():
                    candidates.append(storage_data)

        for src in candidates:
            if src.exists():
                sources.append(src)
        return sources

    def create_backup(self, name: Optional[str] = None) -> StorageBackupResult:
        """Create an incremental storage backup.

        Args:
            name: Optional backup folder name (defaults to timestamp).

        Returns:
            StorageBackupResult describing the outcome.
        """
        sources = self._collect_source_paths()
        if not sources:
            return StorageBackupResult(
                success=True,
                backup_path=None,
                size_bytes=0,
                message="No storage data found to backup.",
                purged=[],
            )

        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        backup_name = name or f"storage_{timestamp}"
        backup_path = self.backup_root / backup_name

        try:
            self.backup_root.mkdir(parents=True, exist_ok=True)
        except OSError as exc:
            return StorageBackupResult(
                success=False,
                backup_path=None,
                size_bytes=0,
                message=f"Could not create backup directory: {exc}",
                purged=[],
            )

        latest = self._latest_backup()
        purged = self._purge_old_backups()

        if self._rsync_available():
            # Use paths relative to the instance root so the backup mirrors the
            # instance layout without embedding absolute paths.
            rel_sources = [str(src.relative_to(self.instance_root)) for src in sources]
            cmd = ["rsync", "-a", "--delete", "--relative"]
            if latest is not None:
                cmd.extend(["--link-dest", str(latest)])
            cmd.extend(rel_sources)
            cmd.append(str(backup_path) + "/")
            try:
                result = subprocess.run(
                    cmd,
                    capture_output=True,
                    text=True,
                    check=False,
                    cwd=self.instance_root,
                )
                if result.returncode not in (0, 24):  # 24 = partial transfer due to vanished source files
                    return StorageBackupResult(
                        success=False,
                        backup_path=None,
                        size_bytes=0,
                        message=f"rsync failed (code {result.returncode}): {result.stderr}",
                        purged=purged,
                    )
            except Exception as exc:
                return StorageBackupResult(
                    success=False,
                    backup_path=None,
                    size_bytes=0,
                    message=f"rsync error: {exc}",
                    purged=purged,
                )
        else:
            # Fallback: plain copytree if rsync is unavailable.
            for src in sources:
                dst = backup_path / src.relative_to(self.instance_root)
                try:
                    if src.is_dir():
                        shutil.copytree(src, dst, dirs_exist_ok=True)
                    else:
                        dst.parent.mkdir(parents=True, exist_ok=True)
                        shutil.copy2(src, dst)
                except OSError as exc:
                    return StorageBackupResult(
                        success=False,
                        backup_path=None,
                        size_bytes=0,
                        message=f"Copy failed for {src}: {exc}",
                        purged=purged,
                    )

        size_bytes = sum(
            f.stat().st_size for f in backup_path.rglob("*") if f.is_file()
        )
        return StorageBackupResult(
            success=True,
            backup_path=backup_path,
            size_bytes=size_bytes,
            message=f"Created {backup_path.name} ({self._human_size(size_bytes)})",
            purged=purged,
        )

    @staticmethod
    def _human_size(size_bytes: int) -> str:
        for unit in ("B", "KB", "MB", "GB", "TB"):
            if size_bytes < 1024:
                return f"{size_bytes:.1f}{unit}"
            size_bytes /= 1024
        return f"{size_bytes:.1f}PB"


def backup_instance_storage(
    instance_root: Path,
    retention_hours: int = 168,
) -> StorageBackupResult:
    """Convenience function to back up a single instance's storage."""
    manager = StorageBackupManager(instance_root, retention_hours=retention_hours)
    return manager.create_backup()
