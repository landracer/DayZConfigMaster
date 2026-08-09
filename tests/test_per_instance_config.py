# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Tests for per-instance configuration manager."""

from pathlib import Path

import pytest

from dayzconfigmaster.config.per_instance_config import (
    InstanceSpawnLoadout,
    ModSettingsOverride,
    PerInstanceConfigManager,
    SpawnableEntry,
)


@pytest.fixture
def instance_root(tmp_path: Path) -> Path:
    return tmp_path / "instance1"


def test_save_and_load_spawn_loadout(instance_root: Path) -> None:
    mgr = PerInstanceConfigManager(instance_root)
    loadout = InstanceSpawnLoadout()
    loadout.enabled.append(SpawnableEntry("TestAKM", "weapon", "Test Mod", 15))
    loadout.random_remix_percentage = 50
    loadout.random_remix_count = 12

    mgr.save_spawn_loadout(loadout)
    loaded = mgr.load_spawn_loadout()

    assert len(loaded.enabled) == 1
    assert loaded.enabled[0].name == "TestAKM"
    assert loaded.enabled[0].category == "weapon"
    assert loaded.enabled[0].spawn_count == 15
    assert loaded.random_remix_percentage == 50
    assert loaded.random_remix_count == 12
    assert loaded.last_updated is not None


def test_mod_settings_override_round_trip(instance_root: Path) -> None:
    mgr = PerInstanceConfigManager(instance_root)
    mgr.export_mod_settings_override(
        Path("/workshop/123/expansion/settings/GeneralSettings.json"),
        "ExpansionMod/Settings/GeneralSettings.json",
        '{"m_Version": 12}',
    )

    overrides = mgr.load_mod_settings_overrides()
    assert len(overrides) == 1
    override = overrides[str(Path("/workshop/123/expansion/settings/GeneralSettings.json"))]
    assert override.relative_target == "ExpansionMod/Settings/GeneralSettings.json"
    assert override.content == '{"m_Version": 12}'


def test_apply_mod_settings_overrides_writes_file(instance_root: Path) -> None:
    mgr = PerInstanceConfigManager(instance_root)
    mgr.export_mod_settings_override(
        Path("/workshop/123/expansion/settings/GeneralSettings.json"),
        "ExpansionMod/Settings/GeneralSettings.json",
        '{"test": true}',
    )

    ok, messages = mgr.apply_mod_settings_overrides(instance_root)
    assert ok is True
    assert any("Applied override" in m for m in messages)

    target = instance_root / "profiles" / "ExpansionMod" / "Settings" / "GeneralSettings.json"
    assert target.exists()
    assert target.read_text(encoding="utf-8") == '{"test": true}'


def test_apply_spawn_loadout_to_mission(instance_root: Path) -> None:
    mission = instance_root / "mpmissions" / "dayzOffline.alteria"
    mission.mkdir(parents=True)
    (mission / "types.xml").write_text("<types></types>")
    (mission / "events.xml").write_text("<events></events>")
    (mission / "cfgspawnabletypes.xml").write_text("<spawnabletypes></spawnabletypes>")

    workshop = instance_root / "workshop"
    mod = workshop / "12345"
    mod.mkdir(parents=True)
    (mod / "meta.cpp").write_text('name = "Test Mod";')
    (mod / "types.xml").write_text('<types><type name="TestAKM_01"/></types>')

    mgr = PerInstanceConfigManager(instance_root)
    loadout = InstanceSpawnLoadout()
    loadout.enabled.append(SpawnableEntry("TestAKM_01", "weapon", "Test Mod", 12))
    mgr.save_spawn_loadout(loadout)

    ok, messages = mgr.apply_spawn_loadout(mission, workshop)
    assert ok is True
    assert any("TestAKM_01" in m for m in messages)
    assert "TestAKM_01" in (mission / "types.xml").read_text()


def test_apply_spawn_loadout_empty_returns_message(instance_root: Path) -> None:
    mission = instance_root / "mpmissions" / "dayzOffline.alteria"
    mission.mkdir(parents=True)
    mgr = PerInstanceConfigManager(instance_root)

    ok, messages = mgr.apply_spawn_loadout(mission, None)
    assert ok is True
    assert any("No spawn loadout" in m for m in messages)


def test_apply_spawn_loadout_passes_usage_value_locations(instance_root: Path) -> None:
    mission = instance_root / "mpmissions" / "dayzOffline.alteria"
    mission.mkdir(parents=True)
    (mission / "types.xml").write_text("<types></types>")
    (mission / "events.xml").write_text("<events></events>")
    (mission / "cfgspawnabletypes.xml").write_text("<spawnabletypes></spawnabletypes>")
    (mission / "cfgeventspawns.xml").write_text("<eventposdef></eventposdef>")

    mgr = PerInstanceConfigManager(instance_root)
    loadout = InstanceSpawnLoadout()
    loadout.enabled.append(
        SpawnableEntry(
            "CustomBeacon",
            "gear",
            "Test Mod",
            3,
            usage="Military",
            value="Tier34",
            locations=[{"x": 100.0, "z": 200.0, "y": 10.0, "a": 45.0}],
        )
    )
    mgr.save_spawn_loadout(loadout)

    ok, messages = mgr.apply_spawn_loadout(mission, None)
    assert ok is True, messages

    types_text = (mission / "types.xml").read_text()
    assert "CustomBeacon" in types_text
    assert 'usage name="Military"' in types_text
    assert 'value name="Tier34"' in types_text

    events_text = (mission / "events.xml").read_text()
    assert "DCM_CustomBeacon" in events_text

    spawns_text = (mission / "cfgeventspawns.xml").read_text()
    assert "DCM_CustomBeacon" in spawns_text
    assert "100.0" in spawns_text
    assert "200.0" in spawns_text


def test_spawnable_entry_round_trip_vehicle_and_loot_counts() -> None:
    vehicle = SpawnableEntry(
        "LittleBird_Heli",
        "air",
        "Air Mod",
        spawn_count=5,
        event_min=2,
        event_max=4,
    )
    loot = SpawnableEntry(
        "AKM",
        "weapon",
        "Weapon Mod",
        spawn_count=25,
        min_count=12,
        usage="Military",
        value="Tier34",
        tier=2,
    )

    vehicle_dict = vehicle.to_dict()
    loot_dict = loot.to_dict()

    assert vehicle_dict["event_min"] == 2
    assert vehicle_dict["event_max"] == 4
    assert "min_count" in vehicle_dict

    assert loot_dict["min_count"] == 12
    assert loot_dict["tier"] == 2

    restored_vehicle = SpawnableEntry.from_dict(vehicle_dict)
    restored_loot = SpawnableEntry.from_dict(loot_dict)

    assert restored_vehicle.event_min == 2
    assert restored_vehicle.event_max == 4
    assert restored_loot.min_count == 12
    assert restored_loot.tier == 2


def test_apply_mission_scoped_mod_settings_override(instance_root: Path) -> None:
    mission = instance_root / "mpmissions" / "dayzOffline.alteria"
    mission.mkdir(parents=True)
    mgr = PerInstanceConfigManager(instance_root)
    mgr.export_mod_settings_override(
        Path("/src/expansion/missionsettings/MissionSettings.json"),
        "mpmissions/dayzOffline.alteria/expansion/MissionSettings.json",
        '{"m_Version": 12}',
    )

    ok, messages = mgr.apply_mod_settings_overrides(instance_root, mission)
    assert ok is True
    target = mission / "expansion" / "MissionSettings.json"
    assert target.exists()
    assert target.read_text(encoding="utf-8") == '{"m_Version": 12}'
