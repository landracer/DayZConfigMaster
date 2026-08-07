# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Tests for cross-instance storage_1 backup discovery."""

from pathlib import Path

import pytest

from dayzconfigmaster.backups.cross_instance_storage import find_map_backups_across_instances
from dayzconfigmaster.backups.instance_storage import InstanceStorageBackupManager


@pytest.fixture
def projects_root(tmp_path: Path) -> Path:
    return tmp_path / "DayZProjects"


def test_finds_backups_across_instances(projects_root: Path) -> None:
    instances = projects_root / "instances"
    server1 = instances / "server1"
    server2 = instances / "server2"

    # Create a DeerIsle backup in server1.
    mgr1 = InstanceStorageBackupManager(server1, 1, "DeerIsle")
    storage1 = server1 / "mpmissions" / "dayzOffline.deerisle" / "storage_1"
    storage1.mkdir(parents=True)
    (storage1 / "players.db").write_text("old")
    mgr1.create_backup(storage1)

    # Create a newer DeerIsle backup in server2.
    mgr2 = InstanceStorageBackupManager(server2, 2, "DeerIsle")
    storage2 = server2 / "mpmissions" / "dayzOffline.deerisle" / "storage_1"
    storage2.mkdir(parents=True)
    (storage2 / "players.db").write_text("newer")
    mgr2.create_backup(storage2)

    results = find_map_backups_across_instances(projects_root, "DeerIsle")
    assert len(results) == 2
    assert "newer" in results[0].path.joinpath("players.db").read_text()


def test_excludes_requested_instance(projects_root: Path) -> Path:
    instances = projects_root / "instances"
    server1 = instances / "server1"
    server2 = instances / "server2"

    for root in (server1, server2):
        mgr = InstanceStorageBackupManager(root, 0, "Enoch")
        storage = root / "mpmissions" / "dayzOffline.enoch" / "storage_1"
        storage.mkdir(parents=True)
        mgr.create_backup(storage)

    results = find_map_backups_across_instances(
        projects_root, "Enoch", exclude_instance_root=server1
    )
    assert len(results) == 1
    assert server2.name in str(results[0].path)


def test_empty_when_no_backups(projects_root: Path) -> None:
    assert find_map_backups_across_instances(projects_root, "Enoch") == []
