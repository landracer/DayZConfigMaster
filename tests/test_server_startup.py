# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Unit tests for server startup hardening helpers."""

import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent))

from dayzconfigmaster.gui.app import DayzConfigMasterApp


def test_sanitize_ban_file_strips_comments_and_keeps_valid_steamids():
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        source = root / "ban.txt"
        source.write_text(
            "// Players added to the ban.txt won't be able to connect\n"
            "// Example line\n"
            "76561198120341761\n"
            "76561198956764064 // inline comment\n"
            "not-a-steamid\n"
            "1234567890\n"  # too short
            "76561198120341761\n"  # duplicate
        )
        dest = root / "instance" / "ban.txt"
        DayzConfigMasterApp._sanitize_ban_file(source, dest)

        lines = dest.read_text(encoding="utf-8").splitlines()
        assert lines == ["76561198120341761", "76561198956764064"]
        # Ensure single trailing newline.
        assert dest.read_bytes().endswith(b"\n")
        assert not dest.read_bytes().endswith(b"\n\n")
    print("test_sanitize_ban_file_strips_comments_and_keeps_valid_steamids: PASSED")


def test_sanitize_ban_file_creates_empty_when_source_missing():
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        dest = root / "instance" / "ban.txt"
        DayzConfigMasterApp._sanitize_ban_file(root / "missing.txt", dest)
        assert dest.read_text(encoding="utf-8") == "\n"
    print("test_sanitize_ban_file_creates_empty_when_source_missing: PASSED")


def test_sanitize_whitelist_file_removes_example_and_keeps_valid_ids():
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        source = root / "whitelist.txt"
        source.write_text(
            "// Players not included in the whitelist.txt won't be able to connect\n"
            "// Example of a character ID\n"
            "1111111111112222222222222333333333XXXXXXAAAA    //Example\n"
            "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGH // comment\n"
            "shortid\n"
            "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHI\n"
        )
        dest = root / "instance" / "whitelist.txt"
        DayzConfigMasterApp._sanitize_ban_file(source, dest, whitelist=True)

        lines = dest.read_text(encoding="utf-8").splitlines()
        assert lines == ["abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGH"]
    print("test_sanitize_whitelist_file_removes_example_and_keeps_valid_ids: PASSED")


def test_sanitize_ban_file_ensures_trailing_newline():
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        source = root / "ban.txt"
        source.write_bytes(b"76561198120341761")  # no trailing newline
        dest = root / "instance" / "ban.txt"
        DayzConfigMasterApp._sanitize_ban_file(source, dest)
        assert dest.read_bytes() == b"76561198120341761\n"
    print("test_sanitize_ban_file_ensures_trailing_newline: PASSED")


def test_deploy_mods_and_keys_creates_wrappers_and_copies_keys():
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        dayz_path = root / "DayZServer"
        dayz_path.mkdir()
        (dayz_path / "keys").mkdir()
        (dayz_path / "keys" / "dayz.bikey").write_text("base key")

        workshop = root / "workshop" / "content" / "221100"
        workshop.mkdir(parents=True)
        mod = workshop / "@CF"
        mod.mkdir()
        (mod / "keys").mkdir()
        (mod / "keys" / "cf.bikey").write_text("cf key")

        instance_root = root / "instance"
        instance_root.mkdir()

        # Minimal mock app with only the attributes _deploy_mods_and_keys needs.
        class MockApp:
            dayz_path_var = type("Var", (), {"get": lambda self: str(dayz_path)})()
            _get_workshop_directory = lambda self: str(workshop)
            _deploy_mods_and_keys = DayzConfigMasterApp._deploy_mods_and_keys

        app = MockApp()
        deployed = app._deploy_mods_and_keys(instance_root, "@CF")

        assert deployed == "@CF"
        assert (instance_root / "@CF").is_dir()
        assert (instance_root / "@CF" / "keys").is_symlink()
        assert (instance_root / "keys" / "dayz.bikey").exists()
        assert (instance_root / "keys" / "cf.bikey").exists()
        assert (instance_root / "keys" / "cf.bikey").read_text() == "cf key"
    print("test_deploy_mods_and_keys_creates_wrappers_and_copies_keys: PASSED")


def test_deploy_mods_and_keys_removes_stale_wrappers():
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        dayz_path = root / "DayZServer"
        dayz_path.mkdir()
        (dayz_path / "keys").mkdir()
        (dayz_path / "keys" / "dayz.bikey").write_text("base key")

        workshop = root / "workshop" / "content" / "221100"
        workshop.mkdir(parents=True)
        mod = workshop / "@CF"
        mod.mkdir()

        instance_root = root / "instance"
        instance_root.mkdir()
        stale_wrapper = instance_root / "@OldMod"
        stale_wrapper.mkdir()
        (stale_wrapper / "keys").symlink_to(workshop / "@OldMod", target_is_directory=True)

        class MockApp:
            dayz_path_var = type("Var", (), {"get": lambda self: str(dayz_path)})()
            _get_workshop_directory = lambda self: str(workshop)
            _deploy_mods_and_keys = DayzConfigMasterApp._deploy_mods_and_keys

        app = MockApp()
        deployed = app._deploy_mods_and_keys(instance_root, "@CF")

        assert deployed == "@CF"
        assert not stale_wrapper.exists()
        assert (instance_root / "@CF").is_dir()
    print("test_deploy_mods_and_keys_removes_stale_wrappers: PASSED")


if __name__ == "__main__":
    test_sanitize_ban_file_strips_comments_and_keeps_valid_steamids()
    test_sanitize_ban_file_creates_empty_when_source_missing()
    test_sanitize_whitelist_file_removes_example_and_keeps_valid_ids()
    test_sanitize_ban_file_ensures_trailing_newline()
    test_deploy_mods_and_keys_creates_wrappers_and_copies_keys()
    test_deploy_mods_and_keys_removes_stale_wrappers()
    print("\nAll server startup tests passed!")
