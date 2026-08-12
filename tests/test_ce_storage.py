# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Tests for Central Economy storage cache management."""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent))

from dayzconfigmaster.economy.ce_storage import (
    backup_and_clear_ce_storage,
    economy_storage_needs_refresh,
    get_economy_xml_hashes,
    get_stored_economy_hashes,
    has_ce_storage_data,
    store_economy_hashes,
)


def test_hashes_detect_changed_types_xml(tmp_path: Path):
    instance_root = tmp_path / "instance"
    mission_dir = instance_root / "mpmissions" / "dayzOffline.enoch"
    db_dir = mission_dir / "db"
    db_dir.mkdir(parents=True)
    (db_dir / "types.xml").write_text("<types><type name=\"AKM\"/></types>")
    for name in ("events.xml", "globals.xml"):
        (db_dir / name).write_text(f"<{name[:-4]}></{name[:-4]}>")
    for name in ("mapgroupproto.xml", "cfglimitsdefinition.xml", "cfgspawnabletypes.xml", "cfgeconomycore.xml"):
        (mission_dir / name).write_text(f"<{name[:-4]}></{name[:-4]}>")

    hashes = get_economy_xml_hashes(mission_dir)
    store_economy_hashes(instance_root, hashes)
    assert get_stored_economy_hashes(instance_root) == hashes

    # No storage data yet -> not stale.
    stale, current, previous = economy_storage_needs_refresh(
        instance_root, "dayzOffline.enoch"
    )
    assert not stale

    # Create a fake bin file.
    data_dir = mission_dir / "storage_1" / "data"
    data_dir.mkdir(parents=True)
    (data_dir / "types.bin").write_bytes(b"old")
    assert has_ce_storage_data(instance_root, "dayzOffline.enoch")

    # Unknown prior state with existing bins -> treat as stale.
    store_economy_hashes(instance_root, {})
    stale, _current, _previous = economy_storage_needs_refresh(
        instance_root, "dayzOffline.enoch"
    )
    assert stale

    # After storing current hashes, unchanged XML -> not stale.
    store_economy_hashes(instance_root, current)
    stale, _current, _previous = economy_storage_needs_refresh(
        instance_root, "dayzOffline.enoch"
    )
    assert not stale

    # Change types.xml -> stale again.
    (db_dir / "types.xml").write_text("<types><type name=\"M4A1\"/></types>")
    stale, _current, _previous = economy_storage_needs_refresh(
        instance_root, "dayzOffline.enoch"
    )
    assert stale


def test_backup_and_clear_moves_bins(tmp_path: Path):
    instance_root = tmp_path / "instance"
    mission_dir = instance_root / "mpmissions" / "dayzOffline.enoch"
    data_dir = mission_dir / "storage_1" / "data"
    data_dir.mkdir(parents=True)
    (data_dir / "types.bin").write_bytes(b"a")
    (data_dir / "types.001").write_bytes(b"b")

    ok, msg, backup_dir = backup_and_clear_ce_storage(
        instance_root, "dayzOffline.enoch"
    )
    assert ok
    assert backup_dir is not None
    assert (backup_dir / "types.bin").exists()
    assert (backup_dir / "types.001").exists()
    assert not any(data_dir.iterdir())
    assert "2" in msg
