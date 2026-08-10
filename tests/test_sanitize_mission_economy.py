# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Tests for the standalone mission economy sanitizer."""

from pathlib import Path

import pytest

from tools.sanitize_mission_economy import sanitize


@pytest.fixture
def instance_root(tmp_path: Path) -> Path:
    return tmp_path / "instance1"


def test_sanitize_quarantines_corrupt_root_types(instance_root: Path) -> None:
    mission = instance_root / "mpmissions" / "dayzOffline.enoch"
    mission.mkdir(parents=True)
    db_types = mission / "db" / "types.xml"
    db_types.parent.mkdir(parents=True)
    db_types.write_text("<types></types>")

    root_types = mission / "types.xml"
    root_types.write_text(
        '<types>'
        '<type name="AKM"><category name="weapon"/><usage name="Town"/><value name="Tier12"/></type>'
        '<type name="Jeans"><category name="gear"/><usage name="Town"/></type>'
        '<type name="Sedan"><category name="vehicle"/><usage name="Town"/></type>'
        '</types>'
    )

    sanitize(instance_root)

    assert not root_types.exists()
    assert (mission / "types.xml.corrupt").exists()


def test_sanitize_quarantines_shadowing_root_events(instance_root: Path) -> None:
    mission = instance_root / "mpmissions" / "dayzOffline.enoch"
    mission.mkdir(parents=True)
    db_events = mission / "db" / "events.xml"
    db_events.parent.mkdir(parents=True)
    db_events.write_text("<events></events>")

    root_events = mission / "events.xml"
    root_events.write_text("<events><event name=\"VehicleA\"/></events>")

    sanitize(instance_root)

    assert not root_events.exists()
    assert (mission / "events.xml.shadow").exists()


def test_sanitize_skips_backup_folders(instance_root: Path) -> None:
    backup = instance_root / "mpmissions" / "dayzOffline.enoch-Original"
    backup.mkdir(parents=True)
    (backup / "types.xml").write_text(
        '<types><type name="AKM"><category name="weapon"/></type></types>'
    )

    sanitize(instance_root)

    assert (backup / "types.xml").exists()
