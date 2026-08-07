# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Tests for storage_1 fresh-start reset."""

from pathlib import Path

from dayzconfigmaster.backups.instance_storage import InstanceStorageBackupManager


def test_start_fresh_clears_storage(tmp_path: Path) -> None:
    instance_root = tmp_path / "server1"
    storage = instance_root / "mpmissions" / "dayzOffline.enoch" / "storage_1"
    storage.mkdir(parents=True)
    (storage / "players.db").write_text("data")
    (storage / "profile.bin").write_text("binary")

    mgr = InstanceStorageBackupManager(instance_root, 1, "Enoch")
    ok, msg = mgr.start_fresh(storage)
    assert ok is True
    assert storage.exists()
    assert not (storage / "players.db").exists()
    assert not (storage / "profile.bin").exists()

    # A safety backup should have been created.
    backups = mgr.list_backups()
    assert len(backups) == 1
    assert (backups[0].path / "players.db").exists()


def test_start_fresh_on_missing_storage(tmp_path: Path) -> None:
    instance_root = tmp_path / "server1"
    storage = instance_root / "mpmissions" / "dayzOffline.enoch" / "storage_1"

    mgr = InstanceStorageBackupManager(instance_root, 1, "Enoch")
    ok, msg = mgr.start_fresh(storage)
    assert ok is True
    assert storage.exists()
    assert mgr.list_backups() == []
