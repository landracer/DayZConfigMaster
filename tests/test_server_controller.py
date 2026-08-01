# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Headless unit tests for dayzconfigmaster.server.controller."""

import tempfile
from pathlib import Path

import pytest

from dayzconfigmaster.server.controller import DayzServerController, ServerInstance


def _make_controller(tmpdir: str) -> DayzServerController:
    return DayzServerController(
        dayz_server_path=str(tmpdir),
        projects_root=str(tmpdir),
    )


def test_default_instance_root():
    with tempfile.TemporaryDirectory() as tmpdir:
        ctrl = _make_controller(tmpdir)
        assert ctrl.default_instance_root("3") == Path(tmpdir) / "instances" / "server3"


def test_resolve_instance_root_uses_configured_root():
    with tempfile.TemporaryDirectory() as tmpdir:
        ctrl = _make_controller(tmpdir)
        inst = ServerInstance(instance_id="1", root_folder="/custom/path")
        assert ctrl.resolve_instance_root(inst) == Path("/custom/path")


def test_resolve_instance_root_falls_back_to_default():
    with tempfile.TemporaryDirectory() as tmpdir:
        ctrl = _make_controller(tmpdir)
        inst = ServerInstance(instance_id="2")
        assert ctrl.resolve_instance_root(inst) == ctrl.default_instance_root("2")


def test_is_port_bound_true():
    """A listening socket should be reported as bound."""
    import socket
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.bind(("127.0.0.1", 0))
        port = sock.getsockname()[1]
        sock.listen(1)
        with tempfile.TemporaryDirectory() as tmpdir:
            ctrl = _make_controller(tmpdir)
            assert ctrl.is_port_bound(port) is True


def test_is_port_bound_false():
    """An unused port should not be reported as bound."""
    with tempfile.TemporaryDirectory() as tmpdir:
        ctrl = _make_controller(tmpdir)
        # Port 0 is special; use a high port unlikely to be in use.
        assert ctrl.is_port_bound(54321) is False


def test_validate_ports_fails_on_invalid_port():
    with tempfile.TemporaryDirectory() as tmpdir:
        ctrl = _make_controller(tmpdir)
        inst = ServerInstance(game_port="abc")
        ok, msg = ctrl.validate_ports(inst)
        assert not ok
        assert "integers" in msg


def test_build_command_finds_executable():
    with tempfile.TemporaryDirectory() as tmpdir:
        root = Path(tmpdir)
        exe = root / "DayZServer_x64"
        exe.write_text("#!/bin/sh\necho ok")
        exe.chmod(0o755)
        ctrl = DayzServerController(dayz_server_path=str(tmpdir))
        inst = ServerInstance(instance_id="1")
        cmd, cwd = ctrl.build_command(inst)
        assert cmd[0] == str(exe)
        assert cwd == ctrl.default_instance_root("1")


def test_build_command_raises_when_executable_missing():
    with tempfile.TemporaryDirectory() as tmpdir:
        ctrl = DayzServerController(dayz_server_path=str(tmpdir))
        with pytest.raises(FileNotFoundError):
            ctrl.build_command(ServerInstance())


def test_prepare_instance_files_creates_directories():
    with tempfile.TemporaryDirectory() as tmpdir:
        ctrl = _make_controller(tmpdir)
        inst = ServerInstance(instance_id="1")
        ok, msg = ctrl.prepare_instance_files(inst)
        assert ok
        root = ctrl.resolve_instance_root(inst)
        assert (root / "profiles").is_dir()
        assert (root / "keys").is_dir()


if __name__ == "__main__":
    test_default_instance_root()
    test_resolve_instance_root_uses_configured_root()
    test_resolve_instance_root_falls_back_to_default()
    test_is_port_bound_true()
    test_is_port_bound_false()
    test_validate_ports_fails_on_invalid_port()
    test_build_command_finds_executable()
    test_build_command_raises_when_executable_missing()
    test_prepare_instance_files_creates_directories()
    print("All server controller tests passed!")
