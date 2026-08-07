# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Tests for per-instance storage_1 backup management."""

import time
from pathlib import Path

from dayzconfigmaster.backups.instance_storage import InstanceStorageBackupManager


def test_backup_and_list(tmp_path: Path) -> None:
    storage = tmp_path / "storage_1"
    storage.mkdir()
    (storage / "players.db").write_text("current")

    mgr = InstanceStorageBackupManager(tmp_path, 1, "ChernarusPlus")
    ok, msg = mgr.create_backup(storage)
    assert ok is True
    assert (tmp_path / "backups" / "storage_1" / "ChernarusPlus").exists()

    backups = mgr.list_backups()
    assert len(backups) == 1
    assert (backups[0].path / "players.db").read_text() == "current"


def test_ensure_backup_creates_when_none(tmp_path: Path) -> None:
    storage = tmp_path / "storage_1"
    storage.mkdir()
    (storage / "players.db").write_text("current")

    mgr = InstanceStorageBackupManager(tmp_path, 1, "ChernarusPlus")
    ok, msg = mgr.ensure_backup_before_start(storage, max_age_minutes=60)
    assert ok is True
    assert mgr.list_backups()


def test_ensure_backup_skips_recent(tmp_path: Path) -> None:
    storage = tmp_path / "storage_1"
    storage.mkdir()
    (storage / "players.db").write_text("current")

    mgr = InstanceStorageBackupManager(tmp_path, 1, "ChernarusPlus")
    mgr.create_backup(storage)
    ok, msg = mgr.ensure_backup_before_start(storage, max_age_minutes=60)
    assert ok is True
    assert "recent backup exists" in msg
    assert len(mgr.list_backups()) == 1


def test_ensure_backup_creates_when_old(tmp_path: Path) -> None:
    storage = tmp_path / "storage_1"
    storage.mkdir()
    (storage / "players.db").write_text("current")

    mgr = InstanceStorageBackupManager(tmp_path, 1, "ChernarusPlus")
    # Create a fake old backup by naming the directory manually.
    old_backup = mgr.backup_dir / "20200101_000000"
    old_backup.mkdir(parents=True)
    (old_backup / "players.db").write_text("old")

    ok, msg = mgr.ensure_backup_before_start(storage, max_age_minutes=60)
    assert ok is True
    assert "recent backup exists" not in msg
    assert len(mgr.list_backups()) == 2


def test_restore_backup(tmp_path: Path) -> None:
    storage = tmp_path / "storage_1"
    storage.mkdir()
    (storage / "players.db").write_text("current")

    mgr = InstanceStorageBackupManager(tmp_path, 1, "ChernarusPlus")
    mgr.create_backup(storage)

    (storage / "players.db").write_text("corrupted")
    backup = mgr.list_backups()[0]
    ok, msg = mgr.restore_backup(backup.path, storage)
    assert ok is True
    assert (storage / "players.db").read_text() == "current"


def test_prune_by_count(tmp_path: Path) -> None:
    storage = tmp_path / "storage_1"
    storage.mkdir()
    (storage / "players.db").write_text("data")

    mgr = InstanceStorageBackupManager(tmp_path, 1, "ChernarusPlus")
    # Create five backups with distinct timestamps by naming directories manually.
    for i in range(5):
        name = f"2020010{i+1}_000000"
        backup_path = mgr.backup_dir / name
        backup_path.mkdir(parents=True)
        (backup_path / "players.db").write_text(f"data{i}")

    assert len(mgr.list_backups()) == 5
    deleted, _ = mgr.prune(max_count=2)
    assert deleted == 3
    assert len(mgr.list_backups()) == 2
