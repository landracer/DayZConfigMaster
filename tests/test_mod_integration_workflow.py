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


if __name__ == "__main__":
    test_enable_vehicle_spawning_creates_event()
    test_define_spawnable_type_with_attachments()
    test_add_vehicle_to_types_xml()
    test_workflow_detects_missing_changes()
    test_workflow_integrates_vehicle_mod()
    test_workflow_unknown_vehicle_still_applies()
    print("All mod integration workflow tests passed!")
