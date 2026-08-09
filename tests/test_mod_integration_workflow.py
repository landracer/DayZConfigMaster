# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Unit tests for the mod integration workflow (headless)."""

import tempfile
from pathlib import Path

from dayzconfigmaster.config.mod_integration import (
    XmlConfigEditor,
    ModIntegrationWorkflow,
    VEHICLE_TEMPLATES,
    discover_vehicle_classes,
)


def test_enable_vehicle_spawning_creates_event():
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        (root / "events.xml").write_text("<events></events>")
        editor = XmlConfigEditor(root)

        assert editor.is_vehicle_spawning_enabled("OffroadHatchback") is None
        assert editor.enable_vehicle_spawning("OffroadHatchback", active=True)
        assert editor.is_vehicle_spawning_enabled("OffroadHatchback") is True

        assert editor.enable_vehicle_spawning("OffroadHatchback", active=False)
        assert editor.is_vehicle_spawning_enabled("OffroadHatchback") is False


def test_define_spawnable_type_with_attachments():
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        (root / "cfgspawnabletypes.xml").write_text("<spawnabletypes></spawnabletypes>")
        editor = XmlConfigEditor(root)

        assert editor.define_spawnable_type(
            "OffroadHatchback",
            attachments=[("HatchbackWheel", 1.0)] * 4,
        )
        assert editor.has_vehicle_attachments("OffroadHatchback")

        text = (root / "cfgspawnabletypes.xml").read_text()
        assert text.count("HatchbackWheel") == 4


def test_add_vehicle_to_types_xml():
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        (root / "types.xml").write_text("<types></types>")
        editor = XmlConfigEditor(root)

        assert editor.add_vehicle_to_types_xml("OffroadHatchback", nominal=10)
        assert editor.type_exists("OffroadHatchback")

        text = (root / "types.xml").read_text()
        assert "OffroadHatchback" in text
        assert "<nominal>10</nominal>" in text


def test_workflow_detects_missing_changes():
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        (root / "events.xml").write_text("<events></events>")
        (root / "cfgspawnabletypes.xml").write_text("<spawnabletypes></spawnabletypes>")
        (root / "types.xml").write_text("<types></types>")
        workflow = ModIntegrationWorkflow(root)

        actions = workflow.detect_actions("OffroadHatchback")
        assert len(actions) == 3
        files = {a.file_name for a in actions}
        assert files == {"events.xml", "cfgspawnabletypes.xml", "types.xml"}


def test_workflow_integrates_vehicle_mod():
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        (root / "events.xml").write_text("<events></events>")
        (root / "cfgspawnabletypes.xml").write_text("<spawnabletypes></spawnabletypes>")
        (root / "types.xml").write_text("<types></types>")
        workflow = ModIntegrationWorkflow(root)

        result = workflow.integrate_vehicle_mod("OffroadHatchback")
        assert result.ok
        assert len(result.actions) == 3

        # No further changes needed.
        assert not workflow.detect_actions("OffroadHatchback")


def test_workflow_unknown_vehicle_still_applies():
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        (root / "events.xml").write_text("<events></events>")
        (root / "cfgspawnabletypes.xml").write_text("<spawnabletypes></spawnabletypes>")
        (root / "types.xml").write_text("<types></types>")
        workflow = ModIntegrationWorkflow(root)

        result = workflow.integrate_vehicle_mod("UnknownVehicle")
        # events.xml and types.xml should still succeed; attachments fail
        # because there is no template.
        events_action = next(a for a in result.actions if a.file_name == "events.xml")
        types_action = next(a for a in result.actions if a.file_name == "types.xml")
        assert events_action.applied
        assert types_action.applied


def test_discover_vehicle_classes_includes_vanilla_templates():
    found = discover_vehicle_classes()
    assert "OffroadHatchback" in found
    assert found["OffroadHatchback"] == "Vanilla template"


def test_discover_vehicle_classes_from_mission_xml():
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        (root / "types.xml").write_text(
            "<types><type name=\"ModdedTruck\"/><type name=\"PlainRock\"/>"
            "<type name=\"OffroadHatchback\"/></types>"
        )
        (root / "events.xml").write_text(
            "<events><event name=\"ModdedHeli\"/><event name=\"AnimalSpawn\"/></events>"
        )
        found = discover_vehicle_classes(mission_root=root)

        assert "ModdedTruck" in found
        assert found["ModdedTruck"] == "Mission: types.xml"
        assert "ModdedHeli" in found
        assert found["ModdedHeli"] == "Mission: events.xml"
        assert "PlainRock" not in found
        assert "AnimalSpawn" not in found
        # Vanilla template source should not be overwritten by mission XML.
        assert found["OffroadHatchback"] == "Vanilla template"


def test_discover_vehicle_classes_from_workshop_mod():
    with tempfile.TemporaryDirectory() as tmpdir:
        workshop = Path(tmpdir) / "workshop"
        mod = workshop / "123456789"
        mod.mkdir(parents=True)
        (mod / "meta.cpp").write_text('name = "Awesome Vehicles";')
        (mod / "types.xml").write_text(
            "<types><type name=\"AwesomeCar\"/><type name=\"AwesomeHat\"/></types>"
        )

        found = discover_vehicle_classes(workshop_dir=workshop)
        assert "AwesomeCar" in found
        assert found["AwesomeCar"] == "Awesome Vehicles"
        assert "AwesomeHat" not in found


def test_workflow_discovers_vehicles():
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        (root / "events.xml").write_text("<events></events>")
        (root / "cfgspawnabletypes.xml").write_text("<spawnabletypes></spawnabletypes>")
        (root / "types.xml").write_text(
            "<types><type name=\"MissionCar\"/></types>"
        )
        workflow = ModIntegrationWorkflow(root)
        discovered = workflow.discover_vehicles()

        names = {name for name, _ in discovered}
        assert "MissionCar" in names
        assert "OffroadHatchback" in names


def test_workflow_discover_spawnables_filters_by_category():
    with tempfile.TemporaryDirectory() as tmpdir:
        mission = Path(tmpdir) / "mission"
        mission.mkdir(parents=True)
        workshop = Path(tmpdir) / "workshop"
        mod = workshop / "12345"
        mod.mkdir(parents=True)
        (mod / "meta.cpp").write_text('name = "Mixed Mod";')
        (mod / "types.xml").write_text(
            "<types>"
            '<type name="Dodge_ChallengerSRT"/>'
            '<type name="Dodge_ChallengerSRT_Wheel"/>'
            '<type name="LittleBird_Heli"/>'
            '<type name="AKM"/>'
            '<type name="TacticalVest"/>'
            "</types>"
        )
        (mission / "events.xml").write_text("<events></events>")
        (mission / "cfgspawnabletypes.xml").write_text("<spawnabletypes></spawnabletypes>")
        (mission / "types.xml").write_text("<types></types>")

        workflow = ModIntegrationWorkflow(mission, workshop_dir=workshop)

        only_air = workflow.discover_spawnables(categories=["air"])
        names = {s.name for s in only_air}
        assert "LittleBird_Heli" in names
        assert "Dodge_ChallengerSRT" not in names
        assert "AKM" not in names

        only_weapons = workflow.discover_spawnables(categories=["weapon"])
        names = {s.name for s in only_weapons}
        assert "AKM" in names
        assert "Dodge_ChallengerSRT" not in names

        vehicles_and_gear = workflow.discover_spawnables(categories=["vehicle", "gear"])
        names = {s.name for s in vehicles_and_gear}
        assert "Dodge_ChallengerSRT" in names
        assert "TacticalVest" in names
        assert "AKM" not in names


def test_discover_spawnables_finds_aircraft_by_name_and_parts():
    from dayzconfigmaster.config.mod_integration import discover_spawnable_classes

    with tempfile.TemporaryDirectory() as tmpdir:
        workshop = Path(tmpdir) / "workshop"
        mod = workshop / "12345"
        mod.mkdir(parents=True)
        (mod / "meta.cpp").write_text('name = "Air Mod";')
        (mod / "types.xml").write_text(
            "<types>"
            '<type name="UH1H"/>'
            '<type name="UH1H_Camo"/>'
            '<type name="UH1H_Rotor"/>'
            '<type name="MH6J"/>'
            '<type name="LittleBird"/>'
            '<type name="C130"/>'
            '<type name="SpeedBoat"/>'
            '<type name="SpeedBoat_Hull"/>'
            "</types>"
        )

        found = discover_spawnable_classes(workshop_dir=workshop)
        assert found["UH1H"].category == "air"
        assert found["MH6J"].category == "air"
        assert found["LittleBird"].category == "air"
        assert found["C130"].category == "air"
        # Parts identify a water base even without a water keyword in the base name.
        assert found["SpeedBoat"].category == "water"
        # Skin/part variants must not be returned as separate spawnables.
        assert "UH1H_Camo" not in found
        assert "UH1H_Rotor" not in found
        assert "SpeedBoat_Hull" not in found


def test_discover_vehicle_classes_from_workshop_subfolder():
    with tempfile.TemporaryDirectory() as tmpdir:
        workshop = Path(tmpdir) / "workshop"
        mod = workshop / "123456789"
        types_dir = mod / "Types"
        types_dir.mkdir(parents=True)
        (mod / "meta.cpp").write_text('name = "Awesome Vehicles";')
        (types_dir / "awesome_types.xml").write_text(
            "<types>"
            '<type name="AwesomeCar_Black"/>'
            '<type name="AwesomeCar_Blue"/>'
            '<type name="AwesomeCar_Wheel"/>'
            '<type name="AwesomeCar_Hood"/>'
            "</types>"
        )

        found = discover_vehicle_classes(workshop_dir=workshop)
        assert "AwesomeCar" in found
        assert found["AwesomeCar"] == "Awesome Vehicles"
        assert "AwesomeCar_Hood" not in found


def test_discover_vehicle_classes_fuzzy_wheel_mapping():
    with tempfile.TemporaryDirectory() as tmpdir:
        workshop = Path(tmpdir) / "workshop"
        mod = workshop / "123456789"
        mod.mkdir(parents=True)
        (mod / "meta.cpp").write_text('name = "Fuzzy Motors";')
        (mod / "types.xml").write_text(
            "<types>"
            '<type name="Ford_Mustang_Shelby_GT500_Black"/>'
            '<type name="Ford_Mustang_Shelby_GT500_Blue"/>'
            '<type name="FordShelbyGT500_Wheel"/>'
            "</types>"
        )

        found = discover_vehicle_classes(workshop_dir=workshop)
        assert "Ford_Mustang_Shelby_GT500" in found
        assert found["Ford_Mustang_Shelby_GT500"] == "Fuzzy Motors"


def test_workflow_rejects_vehicle_part():
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        (root / "events.xml").write_text("<events></events>")
        (root / "cfgspawnabletypes.xml").write_text("<spawnabletypes></spawnabletypes>")
        (root / "types.xml").write_text("<types></types>")
        workflow = ModIntegrationWorkflow(root)

        actions = workflow.detect_actions("CarRadiator")
        assert len(actions) == 1
        assert actions[0].file_name == "validation"

        result = workflow.integrate_vehicle_mod("CarRadiator")
        assert not result.ok
        assert any(a.file_name == "validation" for a in result.actions)


def test_enable_vehicle_spawning_uses_dayz_format():
    """Regression: events.xml entries must use text content, not min/max attrs."""
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        (root / "events.xml").write_text("<events></events>")
        editor = XmlConfigEditor(root)

        assert editor.enable_vehicle_spawning("OffroadHatchback", active=True)
        text = (root / "events.xml").read_text()

        assert "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" in text
        assert "<nominal>1</nominal>" in text
        assert "<active>1</active>" in text
        assert 'type="OffroadHatchback"' in text
        # Old malformed format must not be present.
        assert "<nominal min=" not in text
        assert "<active min=" not in text


def test_editor_prefers_db_economy_files():
    """XmlConfigEditor must edit db/types.xml and db/events.xml when present."""
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        db = root / "db"
        db.mkdir()
        (db / "types.xml").write_text("<types></types>")
        (db / "events.xml").write_text("<events></events>")
        (root / "cfgspawnabletypes.xml").write_text("<spawnabletypes></spawnabletypes>")

        editor = XmlConfigEditor(root)
        assert editor.path_for("types.xml") == db / "types.xml"
        assert editor.path_for("events.xml") == db / "events.xml"
        assert editor.path_for("cfgspawnabletypes.xml") == root / "cfgspawnabletypes.xml"

        # Applying a vehicle should write to db/ files, not create root files.
        workflow = ModIntegrationWorkflow(root)
        result = workflow.integrate_vehicle_mod("OffroadHatchback", spawn_count=5)
        assert result.ok
        assert not (root / "types.xml").exists()
        assert not (root / "events.xml").exists()
        db_types_text = (db / "types.xml").read_text()
        db_events_text = (db / "events.xml").read_text()
        assert "<nominal>0</nominal>" in db_types_text
        assert "OffroadHatchback" in db_events_text
        assert 'nominal="5"' in db_events_text


def test_define_spawnable_type_preserves_non_attachment_tags():
    """Regression: re-defining a type must not wipe <hoarder/> or <damage/>."""
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        (root / "cfgspawnabletypes.xml").write_text(
            "<spawnabletypes>"
            '<type name="CarTent"><hoarder /></type>'
            '<type name="NVGoggles"><damage min="0.0" max="0.32" /></type>'
            "</spawnabletypes>"
        )
        editor = XmlConfigEditor(root)

        assert editor.define_spawnable_type("CarTent", [("HatchbackWheel", 1.0)])
        assert editor.define_spawnable_type("NVGoggles", chance=0.5)

        text = (root / "cfgspawnabletypes.xml").read_text()
        assert "<hoarder" in text
        assert '<damage min="0.0" max="0.32"' in text
        assert "HatchbackWheel" in text
        assert 'value="0.50"' in text


def test_workflow_uses_mod_wheel_template():
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        workshop = Path(tmpdir) / "workshop"
        mod = workshop / "3369325490"
        (mod / "Types").mkdir(parents=True)
        (mod / "meta.cpp").write_text('name = "4K Vehicles";')
        (mod / "Types" / "types.xml").write_text(
            "<types>"
            '<type name="Audi_RS6_ABT_Black"/>'
            '<type name="Audi_RS6_ABT_Blue"/>'
            '<type name="Audi_RS6_ABT_Wheel"/>'
            "</types>"
        )
        (root / "events.xml").write_text("<events></events>")
        (root / "cfgspawnabletypes.xml").write_text("<spawnabletypes></spawnabletypes>")
        (root / "types.xml").write_text("<types></types>")

        workflow = ModIntegrationWorkflow(root, workshop_dir=workshop)
        wheel = workflow.find_wheel_for_vehicle("Audi_RS6_ABT")
        assert wheel == ("Audi_RS6_ABT_Wheel", 4)

        result = workflow.integrate_vehicle_mod("Audi_RS6_ABT")
        assert result.ok
        cfg_text = (root / "cfgspawnabletypes.xml").read_text()
        assert cfg_text.count("Audi_RS6_ABT_Wheel") == 4


def test_discover_vehicle_classes_excludes_weapons_and_items():
    with tempfile.TemporaryDirectory() as tmpdir:
        workshop = Path(tmpdir) / "workshop"
        mod = workshop / "999999999"
        mod.mkdir(parents=True)
        (mod / "meta.cpp").write_text('name = "Mixed Mod";')
        (mod / "types.xml").write_text(
            "<types>"
            '<type name="AKM"/>'
            '<type name="AK74"/>'
            '<type name="A2M14"/>'
            '<type name="Glock19"/>'
            '<type name="CombatBoots"/>'
            '<type name="CarBattery"/>'
            '<type name="Audi_RS6_ABT_Black"/>'
            '<type name="Audi_RS6_ABT_Wheel"/>'
            "</types>"
        )

        found = discover_vehicle_classes(workshop_dir=workshop)
        assert "Audi_RS6_ABT" in found
        assert "AKM" not in found
        assert "AK74" not in found
        assert "A2M14" not in found
        assert "Glock19" not in found
        assert "CombatBoots" not in found
        assert "CarBattery" not in found


def test_discover_spawnable_classes_categories():
    from dayzconfigmaster.config.mod_integration import discover_spawnable_classes

    with tempfile.TemporaryDirectory() as tmpdir:
        workshop = Path(tmpdir) / "workshop"
        mod = workshop / "12345"
        mod.mkdir(parents=True)
        (mod / "meta.cpp").write_text('name = "Mixed Mod";')
        (mod / "types.xml").write_text(
            "<types>"
            '<type name="Dodge_ChallengerSRT"/>'
            '<type name="Dodge_ChallengerSRT_Wheel"/>'
            '<type name="LittleBird_Heli"/>'
            '<type name="AKM"/>'
            '<type name="M4A1"/>'
            '<type name="TacticalVest"/>'
            '<type name="CombatBoots"/>'
            '<type name="Loadout_NATO"/>'
            '<type name="AKM_Mag"/>'
            '<type name="Ammo_762x39"/>'
            '<type name="CarBattery"/>'
            "</types>"
        )

        found = discover_spawnable_classes(workshop_dir=workshop)
        assert found["Dodge_ChallengerSRT"].category == "vehicle"
        assert found["LittleBird_Heli"].category == "air"
        assert found["AKM"].category == "weapon"
        assert found["M4A1"].category == "weapon"
        assert found["TacticalVest"].category == "gear"
        assert found["CombatBoots"].category == "gear"
        assert found["Loadout_NATO"].category == "gear"
        assert "AKM_Mag" not in found
        assert "Ammo_762x39" not in found
        assert "CarBattery" not in found


def test_integrate_weapon_only_writes_types_xml():
    from dayzconfigmaster.config.mod_integration import ModIntegrationWorkflow

    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        (root / "events.xml").write_text("<events></events>")
        (root / "cfgspawnabletypes.xml").write_text("<spawnabletypes></spawnabletypes>")
        (root / "types.xml").write_text("<types></types>")
        workflow = ModIntegrationWorkflow(root)

        result = workflow.integrate_spawnable_mod("AKM", spawn_count=25, category="weapon")
        assert result.ok
        files = {a.file_name for a in result.actions if a.applied}
        assert "types.xml" in files
        assert "events.xml" in files
        assert "cfgspawnabletypes.xml" in files

        types_text = (root / "types.xml").read_text()
        assert "<nominal>25</nominal>" in types_text
        assert 'name="weapon"' in types_text
        assert 'usage name="Military"' in types_text
        assert 'value name="Tier12"' in types_text
        assert "<lifetime>7200</lifetime>" in types_text
        assert "AKM" in types_text


def test_integrate_aircraft_writes_events_and_types():
    from dayzconfigmaster.config.mod_integration import ModIntegrationWorkflow

    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        (root / "events.xml").write_text("<events></events>")
        (root / "cfgspawnabletypes.xml").write_text("<spawnabletypes></spawnabletypes>")
        (root / "types.xml").write_text("<types></types>")
        workflow = ModIntegrationWorkflow(root)

        result = workflow.integrate_spawnable_mod("LittleBird_Heli", spawn_count=5, category="air")
        assert result.ok
        events_text = (root / "events.xml").read_text()
        assert "LittleBird_Heli" in events_text
        assert 'nominal="5"' in events_text
        types_text = (root / "types.xml").read_text()
        # Aircraft are event-spawned: types.xml nominal must be 0.
        assert "<nominal>0</nominal>" in types_text
        assert "<min>0</min>" in types_text
        assert "<lifetime>3888000</lifetime>" in types_text
        assert 'usage name=' not in types_text
        assert 'value name=' not in types_text


def test_integrate_vehicle_no_usage_or_value_by_default():
    from dayzconfigmaster.config.mod_integration import ModIntegrationWorkflow

    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        (root / "events.xml").write_text("<events></events>")
        (root / "cfgspawnabletypes.xml").write_text("<spawnabletypes></spawnabletypes>")
        (root / "types.xml").write_text("<types></types>")
        workflow = ModIntegrationWorkflow(root)

        result = workflow.integrate_vehicle_mod("OffroadHatchback", spawn_count=8)
        assert result.ok

        types_text = (root / "types.xml").read_text()
        # Vehicles are event-spawned: types.xml nominal must be 0.
        assert "<nominal>0</nominal>" in types_text
        assert "<min>0</min>" in types_text
        assert "<lifetime>3888000</lifetime>" in types_text
        assert 'usage name=' not in types_text
        assert 'value name=' not in types_text

        events_text = (root / "events.xml").read_text()
        assert 'nominal="8"' in events_text


def test_integrate_watercraft_no_usage_or_value_by_default():
    from dayzconfigmaster.config.mod_integration import ModIntegrationWorkflow

    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        (root / "events.xml").write_text("<events></events>")
        (root / "cfgspawnabletypes.xml").write_text("<spawnabletypes></spawnabletypes>")
        (root / "types.xml").write_text("<types></types>")
        workflow = ModIntegrationWorkflow(root)

        result = workflow.integrate_spawnable_mod("SpeedBoat", spawn_count=3, category="water")
        assert result.ok

        types_text = (root / "types.xml").read_text()
        # Watercraft are event-spawned: types.xml nominal must be 0.
        assert "<nominal>0</nominal>" in types_text
        assert "<min>0</min>" in types_text
        assert 'usage name=' not in types_text
        assert 'value name=' not in types_text


def test_random_remix_selects_subset():
    from dayzconfigmaster.config.mod_integration import ModIntegrationWorkflow

    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        workshop = Path(tmpdir) / "workshop"
        mod = workshop / "12345"
        mod.mkdir(parents=True)
        (mod / "meta.cpp").write_text('name = "Big Mod";')
        xml = "<types>"
        for i in range(20):
            xml += f'<type name="TestAKM_{i:02d}"/>'
        xml += "</types>"
        (mod / "types.xml").write_text(xml)
        (root / "events.xml").write_text("<events></events>")
        (root / "cfgspawnabletypes.xml").write_text("<spawnabletypes></spawnabletypes>")
        (root / "types.xml").write_text("<types></types>")

        workflow = ModIntegrationWorkflow(root, workshop_dir=workshop)
        result = workflow.random_remix(percentage=50, spawn_count=10, categories=["weapon"], seed=42)
        assert result.ok
        summary = next(a for a in result.actions if a.file_name == "summary")
        assert "enabled 10 of 21" in summary.description


def test_restore_defaults_removes_mod_entries_keeps_vanilla():
    from dayzconfigmaster.config.mod_integration import ModIntegrationWorkflow

    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        workshop = Path(tmpdir) / "workshop"
        mod = workshop / "12345"
        mod.mkdir(parents=True)
        (mod / "meta.cpp").write_text('name = "Big Mod";')
        (mod / "types.xml").write_text(
            "<types>"
            '<type name="Dodge_ChallengerSRT"/><type name="Dodge_ChallengerSRT_Wheel"/>'
            '<type name="AKM"/>'
            "</types>"
        )
        (root / "events.xml").write_text("<events></events>")
        (root / "cfgspawnabletypes.xml").write_text("<spawnabletypes></spawnabletypes>")
        (root / "types.xml").write_text("<types></types>")

        workflow = ModIntegrationWorkflow(root, workshop_dir=workshop)
        workflow.integrate_vehicle_mod("OffroadHatchback", spawn_count=10)
        workflow.integrate_spawnable_mod("Dodge_ChallengerSRT", spawn_count=15, category="vehicle")
        workflow.integrate_spawnable_mod("AKM", spawn_count=25, category="weapon")

        result = workflow.restore_defaults()
        assert result.ok

        events_text = (root / "events.xml").read_text()
        types_text = (root / "types.xml").read_text()
        cfg_text = (root / "cfgspawnabletypes.xml").read_text()

        assert "OffroadHatchback" in events_text
        assert "OffroadHatchback" in types_text
        assert "OffroadHatchback" in cfg_text
        assert "Dodge_ChallengerSRT" not in events_text
        assert "AKM" not in types_text


def test_integrate_spawnable_mod_with_custom_usage_value():
    from dayzconfigmaster.config.mod_integration import ModIntegrationWorkflow

    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        (root / "events.xml").write_text("<events></events>")
        (root / "cfgspawnabletypes.xml").write_text("<spawnabletypes></spawnabletypes>")
        (root / "types.xml").write_text("<types></types>")
        workflow = ModIntegrationWorkflow(root)

        result = workflow.integrate_spawnable_mod(
            "AKM",
            spawn_count=25,
            category="weapon",
            usage="Military",
            value="Tier34",
        )
        assert result.ok

        types_text = (root / "types.xml").read_text()
        assert 'usage name="Military"' in types_text
        assert 'value name="Tier34"' in types_text


def test_integrate_spawnable_mod_with_exact_locations():
    from dayzconfigmaster.config.mod_integration import ModIntegrationWorkflow

    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        (root / "events.xml").write_text("<events></events>")
        (root / "cfgspawnabletypes.xml").write_text("<spawnabletypes></spawnabletypes>")
        (root / "types.xml").write_text("<types></types>")
        (root / "cfgeventspawns.xml").write_text("<eventposdef></eventposdef>")
        workflow = ModIntegrationWorkflow(root)

        locations = [
            {"x": 1234.5, "z": 6789.0, "y": 120.0, "a": 45.0},
            {"x": 4321.0, "z": 9876.5, "y": 120.0, "a": 90.0},
        ]
        result = workflow.integrate_spawnable_mod(
            "CustomBeacon",
            spawn_count=2,
            category="gear",
            usage="Town",
            value="Tier12",
            locations=locations,
        )
        assert result.ok

        events_text = (root / "events.xml").read_text()
        assert "CustomBeacon" in events_text
        assert "DCM_CustomBeacon" in events_text

        spawns_text = (root / "cfgeventspawns.xml").read_text()
        assert "DCM_CustomBeacon" in spawns_text
        assert "1234.5" in spawns_text
        assert "6789.0" in spawns_text


def test_parse_limits_definition():
    from dayzconfigmaster.config.mod_integration import parse_limits_definition

    with tempfile.TemporaryDirectory() as tmpdir:
        path = Path(tmpdir) / "cfglimitsdefinition.xml"
        path.write_text(
            '<listsdefinition>'
            '<categories><category name="weapons"/><category name="vehicles"/></categories>'
            '<usageflags><usage name="Town"/><usage name="Military"/></usageflags>'
            '</listsdefinition>'
        )
        limits = parse_limits_definition(path)
        assert limits.categories == {"weapons", "vehicles"}
        assert limits.usages == {"Town", "Military"}


def test_parse_economy_core():
    from dayzconfigmaster.config.mod_integration import parse_economy_core

    with tempfile.TemporaryDirectory() as tmpdir:
        mission = Path(tmpdir)
        db = mission / "db" / "mods"
        db.mkdir(parents=True)
        (mission / "cfgeconomycore.xml").write_text(
            '<economycore>'
            '<ce folder="db">'
            '<file name="types.xml" type="types"/>'
            '<file name="cfgspawnabletypes.xml" type="spawnabletypes"/>'
            '</ce>'
            '<ce folder="db/mods">'
            '<file name="bossk_types.xml" type="types"/>'
            '<file name="bossk_events.xml" type="events"/>'
            '</ce>'
            '</economycore>'
        )
        eco = parse_economy_core(mission)
        assert eco.types_files == [mission / "db" / "types.xml", db / "bossk_types.xml"]
        assert eco.spawnabletypes_files == [mission / "db" / "cfgspawnabletypes.xml"]
        assert eco.events_files == [db / "bossk_events.xml"]


def test_discover_spawnable_classes_uses_economy_core():
    from dayzconfigmaster.config.mod_integration import discover_spawnable_classes

    with tempfile.TemporaryDirectory() as tmpdir:
        mission = Path(tmpdir)
        db = mission / "db"
        db.mkdir(parents=True)
        (mission / "cfglimitsdefinition.xml").write_text(
            '<listsdefinition>'
            '<categories><category name="weapons"/><category name="vehicles"/></categories>'
            '<usageflags><usage name="Town"/></usageflags>'
            '</listsdefinition>'
        )
        (mission / "cfgeconomycore.xml").write_text(
            '<economycore><ce folder="db">'
            '<file name="mod_types.xml" type="types"/>'
            '</ce></economycore>'
        )
        (db / "mod_types.xml").write_text(
            '<types>'
            '<type name="UH1H"><category name="vehicles"/></type>'
            '<type name="AKM"><category name="weapons"/><usage name="Town"/></type>'
            '<type name="BadItem"><category name="notallowed"/></type>'
            '</types>'
        )

        found = discover_spawnable_classes(mission_root=mission)
        assert found["UH1H"].category == "air"
        assert found["UH1H"].declared_categories == ("vehicles",)
        assert found["AKM"].category == "weapon"
        assert found["AKM"].declared_usages == ("Town",)
        assert found["BadItem"].invalid_categories == ("notallowed",)


def test_air_keywords_do_not_match_clothing():
    from dayzconfigmaster.config.mod_integration import _classify_spawnable

    clothing_names = [
        "FirefighterJacket",
        "Firefighter_Pants",
        "BomberJacket",
        "Bomber_Jacket_Camo",
        "Chopper_Knife",
        "Chopper_Axe",
    ]
    lookup = set(clothing_names)
    for name in clothing_names:
        cls = _classify_spawnable(name, lookup)
        assert cls is None or cls.category != "air", f"{name} was wrongly classified as air"

    # Real aircraft must still be detected.
    aircraft_names = ["F16C", "UH1H", "A10_Warthog", "C130J_Hercules"]
    lookup.update(aircraft_names)
    for name in aircraft_names:
        cls = _classify_spawnable(name, lookup)
        assert cls is not None and cls.category == "air", f"{name} should be air"


def test_integrate_vehicle_mod_uses_event_min_max():
    """Vehicle event entries must use caller-supplied event_min and event_max."""
    from dayzconfigmaster.config.mod_integration import ModIntegrationWorkflow

    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        (root / "events.xml").write_text("<events></events>")
        (root / "cfgspawnabletypes.xml").write_text("<spawnabletypes></spawnabletypes>")
        (root / "types.xml").write_text("<types></types>")
        workflow = ModIntegrationWorkflow(root)

        result = workflow.integrate_spawnable_mod(
            "LittleBird_Heli",
            spawn_count=5,
            category="air",
            event_min=2,
            event_max=4,
        )
        assert result.ok

        events_text = (root / "events.xml").read_text()
        assert "LittleBird_Heli" in events_text
        assert '<min>2</min>' in events_text
        assert '<max>4</max>' in events_text
        # event_nominal defaults to the midpoint of event_min/event_max.
        assert '<nominal>3</nominal>' in events_text
        assert 'nominal="5"' in events_text

        types_text = (root / "types.xml").read_text()
        assert "<nominal>0</nominal>" in types_text
        assert "<min>0</min>" in types_text


def test_integrate_weapon_mod_uses_min_count():
    """Loot entries must use caller-supplied min count in types.xml."""
    from dayzconfigmaster.config.mod_integration import ModIntegrationWorkflow

    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        (root / "events.xml").write_text("<events></events>")
        (root / "cfgspawnabletypes.xml").write_text("<spawnabletypes></spawnabletypes>")
        (root / "types.xml").write_text("<types></types>")
        workflow = ModIntegrationWorkflow(root)

        result = workflow.integrate_spawnable_mod(
            "AKM",
            spawn_count=25,
            category="weapon",
            usage="Military",
            value="Tier34",
            min_count=12,
        )
        assert result.ok

        types_text = (root / "types.xml").read_text()
        assert "<nominal>25</nominal>" in types_text
        assert "<min>12</min>" in types_text
        # Vehicle event min/max must not leak into loot types.xml entries.
        assert "<max>" not in types_text


def test_integrate_weapon_mod_uses_lifetime_restock_quant():
    """Loot entries must use caller-supplied lifetime, restock, quantmin, quantmax."""
    from dayzconfigmaster.config.mod_integration import ModIntegrationWorkflow

    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        (root / "events.xml").write_text("<events></events>")
        (root / "cfgspawnabletypes.xml").write_text("<spawnabletypes></spawnabletypes>")
        (root / "types.xml").write_text("<types></types>")
        workflow = ModIntegrationWorkflow(root)

        result = workflow.integrate_spawnable_mod(
            "AKM",
            spawn_count=25,
            category="weapon",
            usage="Military",
            value="Tier3",
            lifetime=14400,
            restock=1800,
            quantmin=50,
            quantmax=90,
        )
        assert result.ok

        types_text = (root / "types.xml").read_text()
        assert "<lifetime>14400</lifetime>" in types_text
        assert "<restock>1800</restock>" in types_text
        assert "<quantmin>50</quantmin>" in types_text
        assert "<quantmax>90</quantmax>" in types_text


if __name__ == "__main__":
    test_enable_vehicle_spawning_creates_event()
    test_define_spawnable_type_with_attachments()
    test_add_vehicle_to_types_xml()
    test_workflow_detects_missing_changes()
    test_workflow_integrates_vehicle_mod()
    test_workflow_unknown_vehicle_still_applies()
    test_discover_vehicle_classes_includes_vanilla_templates()
    test_discover_vehicle_classes_from_mission_xml()
    test_discover_vehicle_classes_from_workshop_mod()
    test_workflow_discovers_vehicles()
    test_discover_vehicle_classes_from_workshop_subfolder()
    test_discover_vehicle_classes_fuzzy_wheel_mapping()
    test_workflow_rejects_vehicle_part()
    test_workflow_uses_mod_wheel_template()
    test_discover_vehicle_classes_excludes_weapons_and_items()
    test_discover_spawnable_classes_categories()
    test_integrate_weapon_only_writes_types_xml()
    test_integrate_aircraft_writes_events_and_types()
    test_random_remix_selects_subset()
    test_restore_defaults_removes_mod_entries_keeps_vanilla()
    test_parse_limits_definition()
    test_parse_economy_core()
    test_discover_spawnable_classes_uses_economy_core()
    test_air_keywords_do_not_match_clothing()
    print("All mod integration workflow tests passed!")
