# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Tests for GUI diagnostic helpers (port check, log scan, version mismatch)."""

import sys
import tempfile
import time
from pathlib import Path

try:
    import tkinter as tk
except ImportError:
    tk = None

sys.path.insert(0, str(Path(__file__).parent.parent))


def _make_app():
    if tk is None:
        raise RuntimeError("tkinter not available")
    root = tk.Tk()
    root.withdraw()
    from dayzconfigmaster.gui.app import DayzConfigMasterApp
    return root, DayzConfigMasterApp(root)


def test_port_bound_detects_listening_socket():
    if tk is None:
        return
    import socket
    root, app = _make_app()
    try:
        port = 54321
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            sock.bind(("127.0.0.1", port))
            sock.listen(1)
            assert app._check_server_ports_bound(port) is True
        # After closing the socket, the port should be free.
        time.sleep(0.1)
        assert app._check_server_ports_bound(port) is False
    finally:
        root.destroy()
    print("test_port_bound_detects_listening_socket: PASSED")


def test_scan_server_log_finds_script_errors():
    if tk is None:
        return
    root, app = _make_app()
    try:
        with tempfile.TemporaryDirectory() as tmpdir:
            instance_root = Path(tmpdir)
            profiles = instance_root / "profiles"
            profiles.mkdir()
            rpt = profiles / "DayZServer_2026-07-13_00-00-00.RPT"
            rpt.write_text(
                "23:01:16  SCRIPT    (E): Can't compile \"Mission\" script module!\n"
                "DayZExpansion/PlayerList/...c(13): Unknown type 'ExpansionViewController'\n"
                "23:01:16  SCRIPT    (E): Failed to load mission scripts!\n"
            )
            errors = app._scan_server_log_for_errors(instance_root)
            # The scanner returns the first category of errors it finds.
            assert any("Can't compile" in e for e in errors)
    finally:
        root.destroy()
    print("test_scan_server_log_finds_script_errors: PASSED")


def test_detect_mod_version_mismatches_finds_expansion_mismatch():
    if tk is None:
        return
    root, app = _make_app()
    try:
        with tempfile.TemporaryDirectory() as tmpdir:
            instance_root = Path(tmpdir)
            (instance_root / "@2116151222").mkdir()
            (instance_root / "@2291785308").mkdir()
            (instance_root / "@2116151222" / "meta.cpp").write_text(
                'name = "DayZ-Expansion";\nversion = "1.9.58";\n'
            )
            (instance_root / "@2291785308" / "meta.cpp").write_text(
                'name = "DayZ-Expansion-Core";\nversion = "1.9.70";\n'
            )
            warnings = app._detect_mod_version_mismatches(instance_root)
            assert len(warnings) == 1
            assert "1.9.58" in warnings[0]
            assert "1.9.70" in warnings[0]
    finally:
        root.destroy()
    print("test_detect_mod_version_mismatches_finds_expansion_mismatch: PASSED")


def test_detect_mod_version_mismatches_ignores_matching_versions():
    if tk is None:
        return
    root, app = _make_app()
    try:
        with tempfile.TemporaryDirectory() as tmpdir:
            instance_root = Path(tmpdir)
            (instance_root / "@2116151222").mkdir()
            (instance_root / "@2291785308").mkdir()
            (instance_root / "@2116151222" / "meta.cpp").write_text(
                'name = "DayZ-Expansion";\nversion = "1.9.70";\n'
            )
            (instance_root / "@2291785308" / "meta.cpp").write_text(
                'name = "DayZ-Expansion-Core";\nversion = "1.9.70";\n'
            )
            warnings = app._detect_mod_version_mismatches(instance_root)
            assert warnings == []
    finally:
        root.destroy()
    print("test_detect_mod_version_mismatches_ignores_matching_versions: PASSED")


if __name__ == "__main__":
    test_port_bound_detects_listening_socket()
    test_scan_server_log_finds_script_errors()
    test_detect_mod_version_mismatches_finds_expansion_mismatch()
    test_detect_mod_version_mismatches_ignores_matching_versions()
    print("\nAll GUI diagnostics tests passed!")
