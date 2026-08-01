# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Unit tests for workshop mod update logic in the GUI."""

import sys
import tempfile
from pathlib import Path
from unittest.mock import MagicMock, patch

sys.path.insert(0, str(Path(__file__).parent.parent))

from dayzconfigmaster.workshop.api import WorkshopManager


def test_update_workshop_mods_collects_selected_ids():
    """Selected workshop mods should be collected by display name."""
    from dayzconfigmaster.gui.app import DayzConfigMasterApp

    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        app = DayzConfigMasterApp.__new__(DayzConfigMasterApp)
        # A MagicMock root must never crash the app or recurse infinitely.
        app.root = MagicMock()
        app._mod_workshop_id_by_name = {
            "Community Framework": "1559212036",
            "MasPuertas": "123456789",
            "Local Mod": "",
        }
        app._selected_mods = {"Community Framework", "Local Mod"}
        app._get_workshop_directory = lambda: str(root / "workshop")
        app._get_projects_root = lambda: str(root)
        (root / "workshop").mkdir()

        collected = []

        def fake_download(item_id):
            collected.append(item_id)
            return True, "ok"

        with patch.object(app, "_refresh_mod_list"):
            with patch.object(app, "_refresh_all_map_combos"):
                with patch("dayzconfigmaster.gui.app.messagebox"):
                    with patch("dayzconfigmaster.workshop.api.WorkshopManager") as mock_manager:
                        instance = MagicMock(spec=WorkshopManager)
                        instance.download_item = fake_download
                        mock_manager.return_value = instance
                        app._update_workshop_mods()

        assert collected == ["1559212036"]
    print("test_update_workshop_mods_collects_selected_ids: PASSED")


def test_update_workshop_mods_warns_without_workshop_dir():
    """A missing workshop directory should show a warning and not download."""
    from dayzconfigmaster.gui.app import DayzConfigMasterApp

    app = DayzConfigMasterApp.__new__(DayzConfigMasterApp)
    app._mod_workshop_id_by_name = {"Community Framework": "1559212036"}
    app._selected_mods = {"Community Framework"}
    app._get_workshop_directory = lambda: None

    with patch("dayzconfigmaster.gui.app.messagebox") as mock_msg:
        with patch("dayzconfigmaster.workshop.api.WorkshopManager") as mock_manager:
            app._update_workshop_mods()
            assert mock_msg.showwarning.called
            mock_manager.assert_not_called()
    print("test_update_workshop_mods_warns_without_workshop_dir: PASSED")


def test_update_workshop_mods_info_when_none_selected():
    """An empty selection should show an info dialog and not download."""
    from dayzconfigmaster.gui.app import DayzConfigMasterApp

    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        app = DayzConfigMasterApp.__new__(DayzConfigMasterApp)
        app._mod_workshop_id_by_name = {"Community Framework": "1559212036"}
        app._selected_mods = set()
        app._get_workshop_directory = lambda: str(root / "workshop")
        (root / "workshop").mkdir()

        with patch("dayzconfigmaster.gui.app.messagebox") as mock_msg:
            with patch("dayzconfigmaster.workshop.api.WorkshopManager") as mock_manager:
                app._update_workshop_mods()
                assert mock_msg.showinfo.called
                mock_manager.assert_not_called()
    print("test_update_workshop_mods_info_when_none_selected: PASSED")


def test_magicmock_root_does_not_crash_update():
    """A MagicMock root must not cause infinite recursion or memory exhaustion."""
    from dayzconfigmaster.gui.app import DayzConfigMasterApp

    app = DayzConfigMasterApp.__new__(DayzConfigMasterApp)
    app.root = MagicMock()
    assert app._safe_toplevel() is None
    assert app._safe_after(0, lambda: None) is None


if __name__ == "__main__":
    from unittest.mock import patch

    test_update_workshop_mods_collects_selected_ids()
    test_update_workshop_mods_warns_without_workshop_dir()
    test_update_workshop_mods_info_when_none_selected()
    test_magicmock_root_does_not_crash_update()
    print("\nAll workshop mod update tests passed!")
