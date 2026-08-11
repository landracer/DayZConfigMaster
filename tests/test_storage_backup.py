# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

import tempfile
from pathlib import Path

import pytest

from dayzconfigmaster.backups.storage_backup import StorageBackupManager


def test_storage_backup_collects_players_db_and_mission_storage():
    with tempfile.TemporaryDirectory() as tmp:
        instance_root = Path(tmp)
        players_db = instance_root / "players.db"
        players_db.write_text("player data")

        mission_data = instance_root / "mpmissions" / "dayzOffline.enoch" / "storage_1" / "data"
        mission_data.mkdir(parents=True)
        (mission_data / "dynamic_000.bin").write_text("ce cache")

        backup_root = instance_root / "backups" / "storage"
        manager = StorageBackupManager(
            instance_root,
            backup_root=backup_root,
            retention_hours=168,
        )
        result = manager.create_backup(name="test1")

        assert result.success
        assert result.backup_path is not None
        assert (result.backup_path / "players.db").exists()
        assert (result.backup_path / "mpmissions" / "dayzOffline.enoch" / "storage_1" / "data" / "dynamic_000.bin").exists()


def test_storage_backup_purges_old_backups():
    with tempfile.TemporaryDirectory() as tmp:
        instance_root = Path(tmp)
        (instance_root / "players.db").write_text("data")

        backup_root = instance_root / "backups" / "storage"
        manager = StorageBackupManager(
            instance_root,
            backup_root=backup_root,
            retention_hours=1,
        )

        old = backup_root / "old"
        old.mkdir(parents=True)
        (old / "players.db").write_text("old")

        import time
        time.sleep(0.05)

        result = manager.create_backup(name="new")
        assert result.success
        assert old not in result.purged  # mtime too recent

        # Force old backup mtime into the past
        old_epoch = 0
        Path(old / "players.db").touch()
        import os
        os.utime(old, (old_epoch, old_epoch))

        result = manager.create_backup(name="newer")
        assert result.success
        assert old in result.purged


def test_storage_backup_no_data_returns_empty_message():
    with tempfile.TemporaryDirectory() as tmp:
        instance_root = Path(tmp)
        manager = StorageBackupManager(instance_root)
        result = manager.create_backup()
        assert result.success
        assert result.backup_path is None
        assert "No storage data" in result.message
