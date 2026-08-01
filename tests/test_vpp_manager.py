# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Unit tests for dayzconfigmaster.banlist.vpp_manager."""

import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent))

from dayzconfigmaster.banlist.vpp_manager import VppAdminTools


def _write_log(vpp: VppAdminTools, text: str) -> None:
    log_dir = vpp.vpp_path / "Logging"
    log_dir.mkdir(parents=True, exist_ok=True)
    log_file = log_dir / "Log_2026-7-17_2-31-43.txt"
    log_file.write_text(text, encoding="utf-8")


def test_is_available_detects_vpp(tmp_path):
    vpp = VppAdminTools(tmp_path)
    assert not vpp.is_available()
    vpp.vpp_path.mkdir(parents=True)
    vpp.ban_path.write_text('{"m_BanList": []}', encoding="utf-8")
    assert vpp.is_available()


def test_get_connected_players_from_log(tmp_path):
    vpp = VppAdminTools(tmp_path)
    vpp.vpp_path.mkdir(parents=True)
    vpp.ban_path.write_text('{"m_BanList": []}', encoding="utf-8")
    log_text = (
        "2:31:43 | [PermissionManager] Adding Super Admin\n"
        "2:32:10 | Player \"Survivor\" (steamId=76561198773616855) connected to server!\n"
        "2:35:22 | Player \"Bandit\" (steamId=76561198012345678) connected to server!\n"
        "2:40:00 | Player \"Bandit\" (steamId=76561198012345678) disconnected from server.\n"
    )
    _write_log(vpp, log_text)
    players = vpp.get_connected_players()
    assert len(players) == 1
    assert players[0].name == "Survivor"
    assert players[0].steam_id == "76561198773616855"


def test_get_bans_returns_entries(tmp_path):
    vpp = VppAdminTools(tmp_path)
    vpp.vpp_path.mkdir(parents=True)
    vpp.ban_path.write_text(
        json.dumps({
            "m_BanList": [
                {
                    "playerName": "Cheater",
                    "Steam64Id": "76561198000000001",
                    "GUID": "abc123",
                    "banReason": "Aimbot",
                    "issuedBy": "Admin",
                    "expirationDate": {
                        "Hour": 0, "Minute": 0, "Year": 0, "Month": 0, "Day": 0,
                        "Permanent": 1,
                    },
                }
            ]
        }),
        encoding="utf-8",
    )
    bans = vpp.get_bans()
    assert len(bans) == 1
    assert bans[0].steam_id == "76561198000000001"
    assert bans[0].player_name == "Cheater"
    assert bans[0].permanent is True


def test_add_and_remove_ban(tmp_path):
    vpp = VppAdminTools(tmp_path)
    vpp.vpp_path.mkdir(parents=True)
    vpp.ban_path.write_text('{"m_BanList": []}', encoding="utf-8")

    vpp.add_ban("76561198000000002", name="Griefer", reason="Toxic", permanent=False)
    bans = vpp.get_bans()
    assert len(bans) == 1
    assert bans[0].steam_id == "76561198000000002"
    assert bans[0].permanent is False

    # Re-adding same Steam64 replaces the entry.
    vpp.add_ban("76561198000000002", name="Griefer2", reason="Toxic2")
    bans = vpp.get_bans()
    assert len(bans) == 1
    assert bans[0].player_name == "Griefer2"
    assert bans[0].permanent is True

    assert vpp.remove_ban("76561198000000002") is True
    assert vpp.get_bans() == []
    assert vpp.remove_ban("76561198000000002") is False


def test_ensure_super_admin_appends_once(tmp_path):
    vpp = VppAdminTools(tmp_path)
    vpp.vpp_path.mkdir(parents=True)
    assert vpp.ensure_super_admin("76561198000000003") is True
    assert vpp.ensure_super_admin("76561198000000003") is False
    assert vpp.ensure_super_admin("76561198000000004") is True

    sa_path = vpp.vpp_path / "Permissions" / "SuperAdmins" / "SuperAdmins.txt"
    lines = [line.strip() for line in sa_path.read_text(encoding="utf-8").splitlines() if line.strip()]
    assert lines == ["76561198000000003", "76561198000000004"]
