# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Unit tests for cfg_generator module."""

import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent))

from dayzconfigmaster.config.cfg_generator import (
    generate_server_cfg,
    generate_instance_cfgs,
    write_cfg_to_file,
)


def test_generate_server_cfg_basic():
    """Basic serverDZ.cfg generation."""
    cfg = generate_server_cfg(
        name="Test Server",
        port=2302,
        query_port=2303,
        max_players=60,
        map_name="chernarusplus",
        verify_signatures="2 (Verify)",
        force_same_build="1 (Yes)",
        mod_paths="@CF;@MasPuertas",
    )

    assert 'hostname = "Test Server";' in cfg
    assert "port = 2302;" in cfg
    assert "queryPort = 2303;" in cfg
    assert "maxPlayers = 60;" in cfg
    assert 'template="dayzOffline.chernarusplus";' in cfg
    assert "verifySignatures = 2;" in cfg
    assert "forceSameBuild = 1;" in cfg
    assert 'mod =' not in cfg
    print("test_generate_server_cfg_basic: PASSED")


def test_generate_server_cfg_no_mods_in_cfg():
    """Mod paths must not be written to serverDZ.cfg (DayZ uses -mod flag)."""
    cfg = generate_server_cfg(
        name="Test",
        mod_paths="CF,MasPuertas",
    )
    assert 'mod =' not in cfg
    print("test_generate_server_cfg_no_mods_in_cfg: PASSED")


def test_generate_instance_cfgs():
    """Multi-instance config generation."""
    import tkinter as tk

    base_config = {
        "max_players": 60,
        "password": "secret",
        "map_size": 2000,
        "verify_signatures": "2 (Verify)",
        "force_same_build": "1 (Yes)",
        "mod_paths": "@CF",
    }

    root = tk.Tk()
    root.withdraw()

    instances = [
        {
            "id": tk.StringVar(value="1"),
            "game_port": tk.StringVar(value="2302"),
            "query_port": tk.StringVar(value="2303"),
            "map": tk.StringVar(value="chernarusplus"),
            "profile": tk.StringVar(value="/dayz/server1/profile"),
            "mod_paths": tk.StringVar(value="@CF;@Mod1"),
        },
        {
            "id": tk.StringVar(value="2"),
            "game_port": tk.StringVar(value="2402"),
            "query_port": tk.StringVar(value="2403"),
            "map": tk.StringVar(value="enoch"),
            "profile": tk.StringVar(value="/dayz/server2/profile"),
            "mod_paths": tk.StringVar(value="@CF;@Mod2"),
        },
    ]

    configs = generate_instance_cfgs("MyServer", base_config, instances)

    assert 1 in configs
    assert 2 in configs
    assert 'hostname = "MyServer - chernarusplus";' in configs[1]
    assert 'hostname = "MyServer - enoch";' in configs[2]
    assert "port = 2302;" in configs[1]
    assert "port = 2402;" in configs[2]
    assert 'template="dayzOffline.chernarusplus";' in configs[1]
    assert 'template="dayzOffline.enoch";' in configs[2]

    root.destroy()
    print("test_generate_instance_cfgs: PASSED")


def test_generate_server_cfg_complete():
    """Complete serverDZ.cfg should include all standard DayZ fields."""
    cfg = generate_server_cfg(
        name="Test Server",
        port=2302,
        query_port=2303,
        max_players=60,
        password="connect",
        password_admin="admin",
        description="Test desc",
        enable_whitelist="0",
        map_name="enoch",
        server_time="SystemTime",
        server_time_acceleration=2.5,
        server_night_time_acceleration=10.0,
        server_time_persistent="0",
        disable_von="0",
        von_codec_quality=20,
        disable_3rd_person="0",
        disable_crosshair="0",
        disable_personal_light="1",
        lighting_config=0,
        shard_id="abc123",
        guaranteed_updates=1,
        login_queue_concurrent_players=5,
        login_queue_max_players=500,
        instance_id=3,
        storage_auto_fix="1",
        verify_signatures="2 (Verify)",
        force_same_build="1 (Yes)",
        mod_paths="@CF;@MasPuertas",
        profiles_path="./profile",
    )

    assert 'hostname = "Test Server";' in cfg
    assert 'password = "connect";' in cfg
    assert 'passwordAdmin = "admin";' in cfg
    assert 'description = "Test desc";' in cfg
    assert "enableWhitelist = 0;" in cfg
    assert "maxPlayers = 60;" in cfg
    assert "verifySignatures = 2;" in cfg
    assert "forceSameBuild = 1;" in cfg
    assert "disableVoN = 0;" in cfg
    assert "vonCodecQuality = 20;" in cfg
    assert 'shardId = "abc123";' in cfg
    assert "disable3rdPerson = 0;" in cfg
    assert "disableCrosshair = 0;" in cfg
    assert "disablePersonalLight = 1;" in cfg
    assert "lightingConfig = 0;" in cfg
    assert 'serverTime = "SystemTime";' in cfg
    assert "serverTimeAcceleration = 2.5;" in cfg
    assert "serverNightTimeAcceleration = 10.0;" in cfg
    assert "serverTimePersistent = 0;" in cfg
    assert "port = 2302;" in cfg
    assert "queryPort = 2303;" in cfg
    assert "guaranteedUpdates = 1;" in cfg
    assert "loginQueueConcurrentPlayers = 5;" in cfg
    assert "loginQueueMaxPlayers = 500;" in cfg
    assert "instanceId = 3;" in cfg
    assert "storageAutoFix = 1;" in cfg
    assert 'mod =' not in cfg
    assert 'profiles = "./profile";' in cfg
    assert 'template="dayzOffline.enoch";' in cfg
    print("test_generate_server_cfg_complete: PASSED")


def test_write_cfg_to_file():
    """Writing cfg content to file."""
    with tempfile.TemporaryDirectory() as tmpdir:
        filepath = Path(tmpdir) / "serverDZ.cfg"
        success, error = write_cfg_to_file("// test", str(filepath))
        assert success is True
        assert error is None
        assert filepath.read_text() == "// test"
    print("test_write_cfg_to_file: PASSED")


if __name__ == "__main__":
    test_generate_server_cfg_basic()
    test_generate_server_cfg_no_mods_in_cfg()
    test_generate_instance_cfgs()
    test_write_cfg_to_file()
    print("\nAll cfg_generator tests passed!")
