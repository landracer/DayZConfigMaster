# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Headless tests for server startup diagnostic helpers.

These used to require a Tk root and the full GUI app. They now exercise the
headless functions in :mod:`dayzconfigmaster.server.diagnostics` directly, so
CI runs without a display.
"""

import socket
import sys
import tempfile
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent))

from dayzconfigmaster.server.diagnostics import (
    is_server_port_bound,
    scan_server_log_for_errors,
    detect_mod_version_mismatches,
)


def test_port_bound_detects_listening_socket():
    port = 54321
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.bind(("127.0.0.1", port))
        sock.listen(1)
        assert is_server_port_bound(port) is True
    # After closing the socket, the port should be free.
    time.sleep(0.1)
    assert is_server_port_bound(port) is False
    print("test_port_bound_detects_listening_socket: PASSED")


def test_scan_server_log_finds_script_errors():
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
        errors = scan_server_log_for_errors(instance_root)
        # The scanner returns the first category of errors it finds.
        assert any("Can't compile" in e for e in errors)
    print("test_scan_server_log_finds_script_errors: PASSED")


def test_detect_mod_version_mismatches_finds_expansion_mismatch():
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
        warnings = detect_mod_version_mismatches(instance_root)
        assert len(warnings) == 1
        assert "1.9.58" in warnings[0]
        assert "1.9.70" in warnings[0]
    print("test_detect_mod_version_mismatches_finds_expansion_mismatch: PASSED")


def test_detect_mod_version_mismatches_ignores_matching_versions():
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
        warnings = detect_mod_version_mismatches(instance_root)
        assert warnings == []
    print("test_detect_mod_version_mismatches_ignores_matching_versions: PASSED")


if __name__ == "__main__":
    test_port_bound_detects_listening_socket()
    test_scan_server_log_finds_script_errors()
    test_detect_mod_version_mismatches_finds_expansion_mismatch()
    test_detect_mod_version_mismatches_ignores_matching_versions()
    print("\nAll server diagnostics tests passed!")
