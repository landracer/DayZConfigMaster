# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Tests for aircraft lifetime normalization."""

import re

import pytest
from pathlib import Path

from dayzconfigmaster.economy.aircraft_lifetime import (
    MAX_VEHICLE_LIFETIME,
    AircraftLifetimeResult,
    ensure_aircraft_lifetime,
)


SAMPLE_TYPES = """<?xml version="1.0" encoding="UTF-8"?>
<types>
    <type name="RFFS_MH6M">
        <nominal>10</nominal>
        <lifetime>3</lifetime>
        <restock>1800</restock>
        <min>5</min>
    </type>
    <type name="ExpansionHelicopterUh1h">
        <nominal>5</nominal>
        <lifetime>3600</lifetime>
        <restock>1800</restock>
        <min>2</min>
    </type>
    <type name="C130J">
        <nominal>1</nominal>
        <lifetime>3888000</lifetime>
        <restock>3600</restock>
        <min>1</min>
    </type>
    <type name="OffroadHatchback">
        <nominal>20</nominal>
        <lifetime>604800</lifetime>
        <restock>1800</restock>
        <min>10</min>
    </type>
    <type name="AKM">
        <nominal>30</nominal>
        <lifetime>7200</lifetime>
        <restock>0</restock>
        <min>15</min>
    </type>
</types>
"""


@pytest.fixture
def mission_db(tmp_path: Path) -> Path:
    db_dir = tmp_path / "db"
    db_dir.mkdir()
    types_path = db_dir / "types.xml"
    types_path.write_text(SAMPLE_TYPES, encoding="utf-8")
    return types_path


def test_ensure_aircraft_lifetime_updates_short_lifetimes(mission_db: Path):
    result = ensure_aircraft_lifetime(mission_db)

    assert result.success
    assert result.changed_count == 2
    assert "RFFS_MH6M" in result.changed
    assert "ExpansionHelicopterUh1h" in result.changed
    assert "C130J" in result.skipped
    assert "OffroadHatchback" not in result.changed
    assert "AKM" not in result.changed

    # Verify the file was actually updated.
    content = mission_db.read_text(encoding="utf-8")
    assert re.search(
        rf"<lifetime>\s*{MAX_VEHICLE_LIFETIME}\s*</lifetime>", content
    )
    assert not re.search(r"<lifetime>\s*3\s*</lifetime>", content)
    assert not re.search(r"<lifetime>\s*3600\s*</lifetime>", content)

    # Non-air entries must be untouched.
    assert re.search(r"<lifetime>\s*604800\s*</lifetime>", content)
    assert re.search(r"<lifetime>\s*7200\s*</lifetime>", content)


def test_ensure_aircraft_lifetime_creates_backup(mission_db: Path):
    result = ensure_aircraft_lifetime(mission_db)

    assert result.success
    assert result.backup_path is not None
    assert result.backup_path.exists()
    backup_text = result.backup_path.read_text(encoding="utf-8")
    assert re.search(r"<lifetime>\s*3\s*</lifetime>", backup_text)


def test_ensure_aircraft_lifetime_no_change_when_all_max(mission_db: Path):
    # First run normalizes everything.
    ensure_aircraft_lifetime(mission_db)

    # Second run should report no changes.
    result = ensure_aircraft_lifetime(mission_db)
    assert result.success
    assert result.changed_count == 0
    assert "C130J" in result.skipped


def test_ensure_aircraft_lifetime_missing_file(tmp_path: Path):
    missing = tmp_path / "db" / "types.xml"
    result = ensure_aircraft_lifetime(missing)

    assert not result.success
    assert "not found" in result.error
    assert result.changed_count == 0


def test_ensure_aircraft_lifetime_custom_max(mission_db: Path):
    result = ensure_aircraft_lifetime(mission_db, max_lifetime=1000)

    assert result.success
    # Only RFFS_MH6M was below the custom cap.
    assert "RFFS_MH6M" in result.changed
    assert "ExpansionHelicopterUh1h" in result.skipped
    assert "C130J" in result.skipped

    content = mission_db.read_text(encoding="utf-8")
    assert re.search(r"<lifetime>\s*1000\s*</lifetime>", content)


def test_result_dataclass_properties():
    result = AircraftLifetimeResult(
        success=True,
        types_path=Path("/tmp/types.xml"),
        changed=["a", "b"],
        skipped=["c"],
    )
    assert result.changed_count == 2
    assert result.skipped_count == 1
