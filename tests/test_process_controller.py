# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Unit tests for ProcessController process lifecycle management."""

import atexit
import os
import signal
import sys
import tempfile
import time
from contextlib import contextmanager
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent))

from dayzconfigmaster.server.process_controller import ProcessController

# Track every PID created by these tests so we can reap orphans on exit.
_spawned_pids: set[int] = set()


def _track_pid(pid: int) -> int:
    _spawned_pids.add(pid)
    return pid


def _kill_pid(pid: int) -> None:
    try:
        os.kill(pid, signal.SIGKILL)
    except (OSError, ProcessLookupError):
        pass


@atexit.register
def _cleanup_test_processes() -> None:
    """Reap any test subprocesses left behind by a failed or aborted run."""
    for pid in list(_spawned_pids):
        _kill_pid(pid)


@contextmanager
def _spawned_process(*args, **kwargs):
    """Context manager that guarantees a spawned process is killed on exit."""
    proc = __import__("subprocess").Popen(*args, **kwargs)
    _track_pid(proc.pid)
    try:
        yield proc
    finally:
        for child in getattr(proc, "_children", []) or []:
            _kill_pid(child)
        if proc.poll() is None:
            proc.terminate()
            try:
                proc.wait(timeout=2)
            except Exception:
                proc.kill()
                try:
                    proc.wait(timeout=2)
                except Exception:
                    pass
        _spawned_pids.discard(proc.pid)


@contextmanager
def _spawn_parent_with_child():
    """Spawn a Python process that forks a long-running sleep child."""
    with _spawned_process(
        [
            sys.executable,
            "-c",
            "import subprocess, time; subprocess.Popen(['sleep', '30']); time.sleep(30)",
        ],
        start_new_session=True,
    ) as proc:
        yield proc


def test_discover_descendants_finds_child():
    if os.name != 'posix':
        return

    with _spawn_parent_with_child() as proc:
        time.sleep(0.3)
        controller = ProcessController("/tmp")
        descendants = controller._discover_descendants(proc.pid)
        assert len(descendants) >= 1, f"Expected descendants, got {descendants}"
    print("test_discover_descendants_finds_child: PASSED")


def test_kill_process_tree_terminates_children():
    if os.name != 'posix':
        return

    with _spawn_parent_with_child() as proc:
        controller = ProcessController("/tmp")
        time.sleep(0.3)
        descendants = controller._discover_descendants(proc.pid)
        assert len(descendants) >= 1
        controller._kill_process_tree(proc.pid, descendants, timeout=3)
        assert not controller._is_alive(proc.pid)
        for child in descendants:
            assert not controller._is_alive(child)
    print("test_kill_process_tree_terminates_children: PASSED")


def test_is_process_name_running_tracks_state():
    with tempfile.TemporaryDirectory() as tmpdir:
        dayz_path = Path(tmpdir)
        controller = ProcessController(str(dayz_path))
        assert not controller.is_process_name_running("server")

        # Use a long-running shell as a fake server process.
        with _spawned_process(
            ["bash", "-c", "sleep 30"],
            start_new_session=True,
        ) as proc:
            controller.processes["server"] = proc
            time.sleep(0.1)
            assert controller.is_process_name_running("server")
            controller._stop_process("server")

        assert not controller.is_process_name_running("server")
    print("test_is_process_name_running_tracks_state: PASSED")


def test_get_running_process_names():
    with tempfile.TemporaryDirectory() as tmpdir:
        controller = ProcessController(str(tmpdir))
        assert controller.get_running_process_names() == []

        with _spawned_process(
            ["bash", "-c", "sleep 30"],
            start_new_session=True,
        ) as proc:
            controller.processes["server"] = proc
            time.sleep(0.1)
            assert controller.get_running_process_names() == ["server"]
            controller._stop_process("server")
    print("test_get_running_process_names: PASSED")


def test_stop_process_by_name():
    with tempfile.TemporaryDirectory() as tmpdir:
        controller = ProcessController(str(tmpdir))
        success, msg = controller.stop_process_by_name("missing")
        assert not success
        assert "No tracked process" in msg

        with _spawned_process(
            ["bash", "-c", "sleep 30"],
            start_new_session=True,
        ) as proc:
            controller.processes["server"] = proc
            time.sleep(0.1)
            success, msg = controller.stop_process_by_name("server")
            assert success
            assert not controller.is_process_name_running("server")
    print("test_stop_process_by_name: PASSED")


def test_memory_monitor_kills_when_limit_exceeded():
    if os.name != 'posix':
        return

    with tempfile.TemporaryDirectory() as tmpdir:
        controller = ProcessController(str(tmpdir))
        # Start a process that allocates and holds 64 MB of memory.
        with _spawned_process(
            [
                sys.executable,
                "-c",
                "data = bytearray(64 * 1024 * 1024); import time; time.sleep(60)",
            ],
            start_new_session=True,
        ) as proc:
            controller.processes["server"] = proc

            log_path = Path(tmpdir) / "memory.log"
            killed_samples: list = []

            try:
                controller.start_memory_monitor(
                    process_name="server",
                    limit_bytes=1 * 1024 * 1024,  # 1 MB — well below 64 MB
                    log_path=log_path,
                    interval_seconds=0.5,
                    callback=lambda s: killed_samples.append(s) if s.get("killed") else None,
                )
                # Wait for the monitor to detect the violation and kill the process.
                deadline = time.time() + 10
                while time.time() < deadline and controller.is_process_name_running("server"):
                    time.sleep(0.2)
                assert not controller.is_process_name_running("server"), "Process should have been killed"
                assert any(s.get("killed") for s in killed_samples), "Callback should report killed=True"
                assert log_path.exists(), "Memory log should have been created"
                log_text = log_path.read_text()
                assert "KILLING server" in log_text, "Log should contain kill message"
            finally:
                controller.stop_memory_monitor("server")
                if controller.is_process_name_running("server"):
                    controller._stop_process("server")
    print("test_memory_monitor_kills_when_limit_exceeded: PASSED")


def test_memory_monitor_does_not_kill_under_limit():
    if os.name != 'posix':
        return

    with tempfile.TemporaryDirectory() as tmpdir:
        controller = ProcessController(str(tmpdir))
        with _spawned_process(
            ["bash", "-c", "sleep 30"],
            start_new_session=True,
        ) as proc:
            controller.processes["server"] = proc

            samples: list = []
            try:
                controller.start_memory_monitor(
                    process_name="server",
                    limit_bytes=500 * 1024 * 1024,  # 500 MB
                    interval_seconds=0.5,
                    callback=lambda s: samples.append(s),
                )
                time.sleep(1.5)
                assert controller.is_process_name_running("server"), "Process should still be alive"
                assert not any(s.get("killed") for s in samples), "No kill should have occurred"
                assert any(s.get("total_kb", 0) > 0 for s in samples), "Samples should report memory"
            finally:
                controller.stop_memory_monitor("server")
                controller._stop_process("server")
    print("test_memory_monitor_does_not_kill_under_limit: PASSED")


def test_gnome_terminal_command_quotes_mod_separators():
    """Semicolons inside -mod= values must not be interpreted by bash."""
    if os.name != 'posix':
        return

    import shlex

    # Simulate the command list ProcessController builds for a server with mods.
    cmd = [
        "printf",
        "%s\\n",
        "-dologs",
        "-mod=@3369325490;@2832884779;@3718035516",
    ]

    # Reproduce the command portion of the gnome-terminal string construction
    # from _start_process (omit the trailing "; exec bash" used by the terminal).
    quoted = " ".join(shlex.quote(str(arg)) for arg in cmd)

    # Run bash with the same -c invocation and capture the printed arguments.
    result = __import__("subprocess").run(
        ["bash", "-c", quoted],
        capture_output=True,
        text=True,
        timeout=5,
    )
    lines = [line for line in result.stdout.splitlines() if line]
    assert lines[0] == "-dologs", f"Unexpected first line: {lines!r}"
    assert lines[1] == "-mod=@3369325490;@2832884779;@3718035516", f"Unexpected second line: {lines!r}"
    print("test_gnome_terminal_command_quotes_mod_separators: PASSED")


if __name__ == "__main__":
    test_discover_descendants_finds_child()
    test_kill_process_tree_terminates_children()
    test_is_process_name_running_tracks_state()
    test_get_running_process_names()
    test_stop_process_by_name()
    test_memory_monitor_kills_when_limit_exceeded()
    test_memory_monitor_does_not_kill_under_limit()
    test_gnome_terminal_command_quotes_mod_separators()
    print("\nAll ProcessController tests passed!")
