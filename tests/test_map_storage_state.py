# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Tests for map-aware instance storage state tracking."""

from pathlib import Path

import pytest

from dayzconfigmaster.backups.map_storage_state import (
    InstanceMapStorageState,
    InstanceMapStorageTracker,
)


@pytest.fixture
def instance_root(tmp_path: Path) -> Path:
    return tmp_path / "server1"


def test_load_missing_returns_default(instance_root: Path) -> None:
    tracker = InstanceMapStorageTracker(instance_root)
    state = tracker.load()
    assert state.last_run_map == ""
    assert state.last_run_at is None


def test_record_run_persists_map(instance_root: Path) -> None:
    tracker = InstanceMapStorageTracker(instance_root)
    tracker.record_run("Enoch", Path("/backups/storage/enoch/20260806_120000"))

    loaded = tracker.load()
    assert loaded.last_run_map == "Enoch"
    assert loaded.last_run_at is not None
    assert loaded.last_backup_path == "/backups/storage/enoch/20260806_120000"


def test_state_round_trip(instance_root: Path) -> None:
    tracker = InstanceMapStorageTracker(instance_root)
    state = InstanceMapStorageState(instance_id=1, last_run_map="DeerIsle")
    tracker.save(state)

    loaded = tracker.load()
    assert loaded.last_run_map == "DeerIsle"
    assert loaded.instance_id == 1
