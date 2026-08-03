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
    print("All mod integration workflow tests passed!")
