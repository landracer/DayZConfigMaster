# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Headless unit tests for mod settings discovery and cfgspawnabletypes repair.

These used to spin up the full Tk GUI. They now exercise the headless helpers
so the suite passes in CI without a display.
"""

import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent))

from dayzconfigmaster.mods.settings_discovery import detect_mod_settings_files
from dayzconfigmaster.config.spawnabletypes_repair import repair_cfg_spawnable_types


def test_detect_mod_settings_finds_mission_configs():
    with tempfile.TemporaryDirectory() as tmpdir:
        mission = Path(tmpdir) / "dayzOffline.enoch"
        mission.mkdir()
        (mission / "cfgspawnabletypes.xml").write_text("<spawnabletypes/>")
        (mission / "db").mkdir()
        (mission / "db" / "types.xml").write_text("<types/>")
        (mission / "expansion" / "settings").mkdir(parents=True)
        (mission / "expansion" / "settings" / "MapSettings.json").write_text("{}")

        files = detect_mod_settings_files(workshop_dir=None, mission_root=mission)

        by_mod = {}
        for mod, name, _ in files:
            by_mod.setdefault(mod, []).append(name)

        assert "cfgspawnabletypes.xml" in by_mod.get("Mission Core", [])
        assert "db/types.xml" in by_mod.get("Mission Database", [])
        assert "expansion/settings/MapSettings.json" in by_mod.get("DayZ Expansion", [])
    print("test_detect_mod_settings_finds_mission_configs: PASSED")


def test_detect_mod_settings_finds_workshop_mod_configs():
    with tempfile.TemporaryDirectory() as tmpdir:
        workshop = Path(tmpdir) / "workshop" / "content" / "221100"
        workshop.mkdir(parents=True)
        mod_folder = workshop / "3369325490"
        mod_folder.mkdir()
        (mod_folder / "meta.cpp").write_text('name = "4KBOSSKVehicles";')
        (mod_folder / "TraderPlus").mkdir()
        (mod_folder / "TraderPlus" / "Outdated_TraderPlusIDsConfig.json").write_text("{}")
        types_dir = mod_folder / "Types"
        types_dir.mkdir(parents=True)
        (types_dir / "4kbosskTypes.xml").write_text("<types/>")
        config_dir = mod_folder / "Config"
        config_dir.mkdir(parents=True)
        (config_dir / "UserSettings.cpp").write_text("class Cfg {};")
        # Should be ignored.
        (mod_folder / "addons" / "foo.pbo").mkdir(parents=True)
        (mod_folder / "Keys" / "foo.bikey").mkdir(parents=True)

        files = detect_mod_settings_files(workshop_dir=str(workshop), mission_root=None)

        by_mod = {}
        for mod, rel, _ in files:
            by_mod.setdefault(mod, []).append(rel)

        assert "4KBOSSKVehicles" in by_mod
        assert "TraderPlus/Outdated_TraderPlusIDsConfig.json" in by_mod["4KBOSSKVehicles"]
        assert "Types/4kbosskTypes.xml" in by_mod["4KBOSSKVehicles"]
        assert "Config/UserSettings.cpp" in by_mod["4KBOSSKVehicles"]
        assert "addons/foo.pbo" not in [r for rels in by_mod.values() for r in rels]
        assert "Keys/foo.bikey" not in [r for rels in by_mod.values() for r in rels]
    print("test_detect_mod_settings_finds_workshop_mod_configs: PASSED")


def test_repair_cfg_spawnable_types_fixes_invalid_comments():
    with tempfile.TemporaryDirectory() as tmpdir:
        mission = Path(tmpdir) / "dayzOffline.enoch"
        mission.mkdir()
        bad = (
            '<?xml version="1.0"?>\n<spawnabletypes>\n'
            '  <!-- -----------------------------MUMMY------------------------------->\n'
            '  <type name="OffroadHatchback">\n\t</type>\n'
            '</spawnabletypes>\n'
        )
        (mission / "cfgspawnabletypes.xml").write_text(bad)

        result = repair_cfg_spawnable_types(mission / "cfgspawnabletypes.xml")
        assert result.success
        assert result.changed

        from xml.etree import ElementTree as ET
        text = (mission / "cfgspawnabletypes.xml").read_text()
        ET.fromstring(text)
        assert "MUMMY" in text  # sanitized but still readable
        assert "-----------------------------" not in text
    print("test_repair_cfg_spawnable_types_fixes_invalid_comments: PASSED")


def test_repair_cfg_spawnable_types_adds_missing_wheels():
    with tempfile.TemporaryDirectory() as tmpdir:
        mission = Path(tmpdir) / "dayzOffline.enoch"
        mission.mkdir()
        bad = (
            '<?xml version="1.0"?>\n<spawnabletypes>\n'
            '  <type name="OffroadHatchback">\n\t</type>\n'
            '</spawnabletypes>\n'
        )
        (mission / "cfgspawnabletypes.xml").write_text(bad)

        result = repair_cfg_spawnable_types(mission / "cfgspawnabletypes.xml")
        assert result.success
        assert result.changed

        text = (mission / "cfgspawnabletypes.xml").read_text()
        block = text.split('<type name="OffroadHatchback">')[1].split("</type>")[0]
        assert block.count("HatchbackWheel") == 4
    print("test_repair_cfg_spawnable_types_adds_missing_wheels: PASSED")


if __name__ == "__main__":
    test_detect_mod_settings_finds_mission_configs()
    test_detect_mod_settings_finds_workshop_mod_configs()
    test_repair_cfg_spawnable_types_fixes_invalid_comments()
    test_repair_cfg_spawnable_types_adds_missing_wheels()
    print("\nAll mod settings tests passed!")
