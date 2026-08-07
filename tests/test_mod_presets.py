"""Tests for dayzconfigmaster.config.mod_presets."""

from pathlib import Path

import pytest

from dayzconfigmaster.config.mod_presets import ModPreset, ModPresetManager


@pytest.fixture
def manager(tmp_path: Path) -> ModPresetManager:
    """Return a preset manager backed by a temporary projects root."""
    return ModPresetManager(tmp_path)


class TestModPreset:
    def test_mod_string(self):
        preset = ModPreset(name="test", mods=["@CF", "@Expansion"])
        assert preset.mod_string == "@CF;@Expansion"

    def test_from_mod_string(self):
        preset = ModPreset.from_mod_string("test", "@CF; @Expansion ; ;")
        assert preset.mods == ["@CF", "@Expansion"]


class TestModPresetManager:
    def test_empty_manager(self, manager: ModPresetManager):
        assert manager.list_presets() == []
        assert manager.get_preset("missing") is None
        assert manager.apply_preset_to_string("missing") is None

    def test_save_and_load_preset(self, manager: ModPresetManager):
        ok, msg = manager.save_preset(
            "PvE", ["@CF", "@Expansion", "@VPPAdminTools"]
        )
        assert ok
        assert "Saved" in msg

        # Re-create manager to verify persistence.
        manager2 = ModPresetManager(manager.projects_root)
        assert manager2.list_presets() == ["PvE"]
        preset = manager2.get_preset("PvE")
        assert preset is not None
        assert preset.mods == ["@CF", "@Expansion", "@VPPAdminTools"]
        assert (
            manager2.apply_preset_to_string("PvE")
            == "@CF;@Expansion;@VPPAdminTools"
        )

    def test_save_preset_from_string(
        self, manager: ModPresetManager
    ):
        ok, msg = manager.save_preset_from_string(
            "Raid", "@CF; @RaidMod; @Weapons"
        )
        assert ok
        preset = manager.get_preset("Raid")
        assert preset is not None
        assert preset.mods == ["@CF", "@RaidMod", "@Weapons"]

    def test_update_existing_preset(self, manager: ModPresetManager):
        manager.save_preset("Test", ["@A"])
        manager.save_preset("Test", ["@B", "@C"])
        assert manager.get_preset("Test").mods == ["@B", "@C"]

    def test_delete_preset(self, manager: ModPresetManager):
        manager.save_preset("ToDelete", ["@A"])
        ok, msg = manager.delete_preset("ToDelete")
        assert ok
        assert "Deleted" in msg
        assert manager.list_presets() == []

    def test_delete_missing_preset(self, manager: ModPresetManager):
        ok, msg = manager.delete_preset("Missing")
        assert not ok
        assert "not found" in msg

    def test_save_empty_name(self, manager: ModPresetManager):
        ok, msg = manager.save_preset("   ", ["@A"])
        assert not ok
        assert "empty" in msg.lower()

    def test_to_dict(self, manager: ModPresetManager):
        manager.save_preset("One", ["@A", "@B"])
        assert manager.to_dict() == {"One": ["@A", "@B"]}

    def test_legacy_string_format(self, manager: ModPresetManager):
        # Older files may have stored strings instead of lists.
        path = manager.projects_root / "mod_presets.json"
        path.write_text('{"Legacy": "@A;@B"}', encoding="utf-8")
        manager2 = ModPresetManager(manager.projects_root)
        assert manager2.apply_preset_to_string("Legacy") == "@A;@B"

    def test_export_presets(self, manager: ModPresetManager):
        manager.save_preset("ExportMe", ["@A", "@B"])
        export_file = manager.projects_root / "export.json"
        ok, msg = manager.export_presets(export_file)
        assert ok
        assert "Exported" in msg
        assert export_file.exists()
        assert '"ExportMe"' in export_file.read_text(encoding="utf-8")

    def test_import_presets_merge(self, manager: ModPresetManager):
        manager.save_preset("Existing", ["@A"])
        import_file = manager.projects_root / "import.json"
        import_file.write_text(
            '{"Existing": ["@X"], "New": ["@Y", "@Z"]}',
            encoding="utf-8",
        )
        ok, msg = manager.import_presets(import_file, overwrite=False)
        assert ok
        assert manager.list_presets() == ["Existing", "New"]
        assert manager.get_preset("Existing").mods == ["@A"]
        assert manager.get_preset("New").mods == ["@Y", "@Z"]
        assert "skipped 1" in msg

    def test_import_presets_overwrite(self, manager: ModPresetManager):
        manager.save_preset("Existing", ["@A"])
        import_file = manager.projects_root / "import.json"
        import_file.write_text(
            '{"Existing": ["@X"], "New": ["@Y"]}',
            encoding="utf-8",
        )
        ok, msg = manager.import_presets(import_file, overwrite=True)
        assert ok
        assert manager.get_preset("Existing").mods == ["@X"]
        assert manager.get_preset("New").mods == ["@Y"]

    def test_import_presets_invalid_file(self, manager: ModPresetManager):
        import_file = manager.projects_root / "import.json"
        import_file.write_text("not json", encoding="utf-8")
        ok, msg = manager.import_presets(import_file)
        assert not ok
        assert "Failed" in msg

    def test_import_presets_missing_file(self, manager: ModPresetManager):
        ok, msg = manager.import_presets(
            manager.projects_root / "missing.json"
        )
        assert not ok
        assert "not found" in msg
