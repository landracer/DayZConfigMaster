# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Unit tests for the Mod Settings tab and cfgspawnabletypes repair."""

import shutil
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent))


def _make_app():
    import tkinter as tk
    from tkinter import messagebox
    from unittest.mock import patch

    with patch.object(messagebox, "askyesno", return_value=False), \
         patch.object(messagebox, "askokcancel", return_value=True), \
         patch.object(messagebox, "showinfo"), \
         patch.object(messagebox, "showwarning"), \
         patch.object(messagebox, "showerror"):
        from dayzconfigmaster.gui.app import DayzConfigMasterApp
        root = tk.Tk()
        root.withdraw()
        app = DayzConfigMasterApp(root)
        return root, app


def test_detect_mod_settings_finds_mission_configs():
    with tempfile.TemporaryDirectory() as tmpdir:
        mission = Path(tmpdir) / "dayzOffline.enoch"
        mission.mkdir()
        (mission / "cfgspawnabletypes.xml").write_text("<spawnabletypes/>")
        (mission / "db").mkdir()
        (mission / "db" / "types.xml").write_text("<types/>")
        (mission / "expansion" / "settings").mkdir(parents=True)
        (mission / "expansion" / "settings" / "MapSettings.json").write_text("{}")

        root, app = _make_app()
        app._get_current_mission_root = lambda: mission
        app._get_workshop_directory = lambda: None
        files = app._detect_mod_settings_files()
        root.destroy()

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

        root, app = _make_app()
        app._get_current_mission_root = lambda: None
        app._get_workshop_directory = lambda: str(workshop)
        files = app._detect_mod_settings_files()
        root.destroy()

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

        root, app = _make_app()
        app._get_current_mission_root = lambda: mission
        app._repair_cfg_spawnable_types()
        root.destroy()

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

        root, app = _make_app()
        app._get_current_mission_root = lambda: mission
        app._repair_cfg_spawnable_types()
        root.destroy()

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
