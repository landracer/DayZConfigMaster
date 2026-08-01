# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Process controller module providing cross-platform process management
for DayZ servers and clients.

All code governed by the SOVEREIGN INDIVIDUAL LICENSE.
"""

import logging
import shutil
import subprocess
import os
import shlex
import signal
import threading
from pathlib import Path
import time
from typing import Optional, List, Tuple, Dict, Any, Callable

logger = logging.getLogger(__name__)


class ProcessController:
    """
    Cross-platform process management for DayZ servers and clients.
    
    Handles:
    - Server start/stop/restart with proper parameters
    - Client start in debug/normal mode
    - PID tracking to avoid conflicts
    - Graceful shutdown handling
    """
    
    # Default executables (cross-platform compatible names)
    SERVER_DEBUG_EXE = "DayZDiag_x64"  # Will be .exe on Windows, no extension on Linux/macOS
    SERVER_NORMAL_EXE = "DayZServer_x64"
    # Linux Steam install uses the plain "DayZServer" binary name.
    SERVER_NORMAL_EXE_LINUX = "DayZServer"
    SERVER_DEBUG_EXE_LINUX = "DayZDiag"
    CLIENT_DEBUG_EXE = "DayZDiag_x64"
    CLIENT_NORMAL_EXE = "DayZ_x64"
    
    def __init__(self, dayz_path: str):
        """
        Initialize controller with DayZ installation path.
        
        Args:
            dayz_path: Path to DayZ installation directory
        """
        self.dayz_path = Path(dayz_path)
        self.processes: Dict[str, subprocess.Popen] = {}
        self.running_mode: Optional[str] = None  # debug|normal
        # Maps tracked process name to the actual DayZ PID when launched via
        # a terminal emulator (terminal PID != DayZ PID).
        self._dayz_pids: Dict[str, int] = {}        # Maps tracked process name to all descendant PIDs discovered at start
        # time (e.g. enfmain children spawned by the DayZ server binary).
        self._child_pids: Dict[str, List[int]] = {}
        # Memory watchdog threads and stop events, keyed by process name.
        self._memory_monitors: Dict[str, threading.Thread] = {}
        self._memory_monitor_stop_events: Dict[str, threading.Event] = {}
        self._memory_monitor_callbacks: Dict[str, Callable[[Dict[str, Any]], None]] = {}
    def _get_exe_path(self, exe_name: str) -> Optional[Path]:
        """Get full path to executable (OS-specific handling)."""
        if os.name == 'nt':  # Windows
            suffix = ".exe"
        else:  # Linux/macOS
            suffix = ""

        path = self.dayz_path / f"{exe_name}{suffix}"
        if path.exists():
            return path

        # On Linux, Steam installs often use the plain "DayZServer"/"DayZDiag" names.
        if os.name != 'nt':
            if exe_name == self.SERVER_NORMAL_EXE:
                fallback = self.dayz_path / self.SERVER_NORMAL_EXE_LINUX
                if fallback.exists():
                    return fallback
            elif exe_name == self.SERVER_DEBUG_EXE:
                fallback = self.dayz_path / self.SERVER_DEBUG_EXE_LINUX
                if fallback.exists():
                    return fallback

        return None
    
    def start_server(
        self,
        port: int = 2302,
        query_port: Optional[int] = None,
        steam_port: Optional[int] = None,
        mode: str = "debug",
        map_size: int = 2000,
        max_players: int = 60,
        config_path: Optional[str] = None,
        profiles_path: Optional[str] = None,
        mods: Optional[List[str]] = None,
        client: bool = False,
        terminal: bool = False,
        cwd: Optional[Path] = None,
        process_name: str = "server",
        exe_path: Optional[Path] = None,
        env: Optional[Dict[str, str]] = None,
    ) -> Tuple[bool, str]:
        """
        Start DayZ server with optional client.
        
        Args:
            port: Server port (default 2302)
            query_port: Steam query port (defaults to port+1 if omitted)
            steam_port: Steam P2P/matchmaking port (auto-bound if omitted)
            mode: debug|normal mode
            map_size: Map dimensions in meters
            max_players: Maximum concurrent players
            config_path: Path to serverDZ.cfg
            profiles_path: Path to the server's -profiles folder
            mods: List of mod paths to include
            client: Also start client? (default False)
            process_name: Name used to track this server process (default "server")
            exe_path: Optional explicit executable path. Used for multi-instance
                setups where each instance must run its own copied binary so
                Steam/BattlEye treat them as distinct processes.
            env: Optional environment dictionary. Used to isolate per-instance
                Steam/IPC state by giving each instance its own HOME directory.
        
        Returns:
            Tuple of (success, message)
        """
        try:
            # Determine executable based on mode
            if exe_path is not None:
                exe_path = Path(exe_path)
                if not exe_path.exists():
                    return False, f"Executable not found: {exe_path}"
            else:
                exe_name = self.SERVER_DEBUG_EXE if mode == "debug" else self.SERVER_NORMAL_EXE
                exe_path = self._get_exe_path(exe_name)
                
                if not exe_path:
                    return False, f"Executable not found: {exe_name}"
            
            # Build command
            cmd = [str(exe_path)]
            
            # Common parameters
            if os.name == 'nt':
                cmd.extend(["-filePatching", "-dologs", "-adminLog", "-freezecheck"])
            else:
                cmd.extend(["-dologs", "-adminLog", "-freezecheck"])
            
            # Server-specific parameters
            cmd.append(f"-port={port}")
            if query_port is not None:
                cmd.append(f"-queryPort={query_port}")
            if steam_port is not None:
                cmd.append(f"-steamPort={steam_port}")
            cmd.append(f"-maxPlayers={max_players}")
            cmd.append(f"-mapSize={map_size}")

            if config_path:
                # DayZ parses -config= literally; do not wrap the value in quotes.
                cmd.append(f"-config={config_path}")

            # DayZ requires an explicit -profiles folder so mods like ZomBerry
            # can locate their config and the server writes logs consistently.
            if profiles_path:
                cmd.append(f"-profiles={profiles_path}")

            # Mods handling - use semicolon separator for DayZ
            if mods:
                mod_paths = ";".join(mods)
                cmd.append(f"-mod={mod_paths}")

            # Start server process from the instance directory (or DayZ install directory
            # if no instance cwd was supplied) so it can find config, addons/, dta/, etc.
            process_cwd = cwd if cwd is not None else self.dayz_path
            self._start_process(process_name, cmd, cwd=process_cwd, terminal=terminal, env=env)
            
            # Start client if requested
            if client:
                self.start_client(mode=mode, connect_port=port)
            
            self.running_mode = mode
            return True, f"Server started in {mode} mode on port {port}"
        
        except Exception as e:
            return False, f"Failed to start server: {str(e)}"
    
    def start_client(
        self,
        mode: str = "debug",
        connect_port: Optional[int] = None
    ) -> Tuple[bool, str]:
        """Start DayZ client."""
        try:
            exe_name = self.CLIENT_DEBUG_EXE if mode == "debug" else self.CLIENT_NORMAL_EXE
            exe_path = self._get_exe_path(exe_name)
            
            if not exe_path:
                return False, f"Executable not found: {exe_name}"
            
            cmd = [str(exe_path)]
            
            # Platform-specific parameters
            if os.name == 'nt':
                cmd.extend(["-window", "-nosplash", "-filePatching", "-doLogs", "-scriptDebug=true"])
            else:
                cmd.extend(["-window", "-nosplash"])
            
            if connect_port:
                cmd.append(f"-connect=127.0.0.1:{connect_port}")
            
            self._start_process("client", cmd)
            return True, "Client started"
        
        except Exception as e:
            return False, f"Failed to start client: {str(e)}"
    
    def _start_process(
        self,
        name: str,
        cmd: List[str],
        cwd: Optional[Path] = None,
        terminal: bool = False,
        env: Optional[Dict[str, str]] = None,
    ) -> subprocess.Popen:
        """Start a process and track its PID."""
        kwargs: Dict[str, Any] = {
            "start_new_session": True,
        }
        if cwd is not None:
            kwargs["cwd"] = str(cwd)
        if env is not None:
            kwargs["env"] = env

        if terminal and os.name == 'posix':
            # Launch in a visible terminal so the user can see server output live.
            # xterm is the fallback; konsole is common on KDE (your environment).
            terminal_emulators = [
                ["konsole", "--separate", "--noclose", "-e"],
                ["gnome-terminal", "--", "bash", "-c"],
                ["xterm", "-hold", "-e"],
            ]
            for term in terminal_emulators:
                exe = shutil.which(term[0])
                if exe:
                    if term[0] == "gnome-terminal":
                        # gnome-terminal needs the command as a single quoted string.
                        # Shell-quote each argument so semicolons inside -mod= values
                        # are not interpreted as command separators by bash.
                        full_cmd = term + [" ".join(shlex.quote(str(arg)) for arg in cmd) + "; exec bash"]
                    else:
                        full_cmd = term + cmd
                    process = subprocess.Popen(full_cmd, **kwargs)
                    break
            else:
                # No terminal emulator found; fall back to background launch
                kwargs["stdout"] = subprocess.PIPE
                kwargs["stderr"] = subprocess.PIPE
                process = subprocess.Popen(cmd, **kwargs)
        else:
            kwargs["stdout"] = subprocess.PIPE
            kwargs["stderr"] = subprocess.PIPE
            if os.name == 'posix':
                process = subprocess.Popen(cmd, **kwargs)
            else:  # Windows
                kwargs["creationflags"] = subprocess.CREATE_NEW_PROCESS_GROUP
                process = subprocess.Popen(cmd, **kwargs)
        
        self.processes[name] = process

        # For terminal launches the tracked PID is the terminal emulator, not
        # the DayZ server. Try to discover the real child PID so get_status()
        # reports the server's liveness instead of the terminal's.
        if terminal and os.name == 'posix':
            dayz_pid = self._resolve_child_pid(process.pid)
            if dayz_pid:
                self._dayz_pids[name] = dayz_pid

        # Discover and remember the whole process tree so we can clean up every
        # child (including enfmain and other Enfusion subprocesses) on stop.
        root_pid = self._dayz_pids.get(name, process.pid)
        self._child_pids[name] = self._discover_descendants(root_pid)

        return process

    def _resolve_child_pid(self, parent_pid: int) -> Optional[int]:
        """Discover the DayZ server PID spawned by a terminal emulator.

        Tries /proc/<pid>/task/<pid>/children first, then falls back to
        pgrep -P. Returns None if no child can be found.
        """
        try:
            children_file = Path(f"/proc/{parent_pid}/task/{parent_pid}/children")
            if children_file.exists():
                children = children_file.read_text().strip().split()
                for child_pid in children:
                    try:
                        child_cmd = Path(f"/proc/{child_pid}/cmdline").read_bytes()
                        cmdline = child_cmd.replace(b"\x00", b" ").decode("utf-8", errors="ignore")
                        if "DayZ" in cmdline or "dayz" in cmdline.lower():
                            return int(child_pid)
                    except (OSError, ValueError):
                        continue
                # No DayZ child found by name; return the first child if any.
                if children:
                    return int(children[0])
        except (OSError, ValueError):
            pass

        try:
            result = subprocess.run(
                ["pgrep", "-P", str(parent_pid)],
                capture_output=True,
                text=True,
                timeout=2,
            )
            if result.returncode == 0:
                for line in result.stdout.strip().splitlines():
                    child_pid = line.strip()
                    if not child_pid:
                        continue
                    try:
                        child_cmd = Path(f"/proc/{child_pid}/cmdline").read_bytes()
                        cmdline = child_cmd.replace(b"\x00", b" ").decode("utf-8", errors="ignore")
                        if "DayZ" in cmdline or "dayz" in cmdline.lower():
                            return int(child_pid)
                    except (OSError, ValueError):
                        continue
                # Fall back to first child.
                first = result.stdout.strip().splitlines()[0].strip()
                return int(first) if first else None
        except (subprocess.TimeoutExpired, IndexError, ValueError, FileNotFoundError):
            pass

        return None

    def _discover_descendants(self, pid: int) -> List[int]:
        """Recursively discover all descendant PIDs of a given PID on Linux.

        Returns a leaf-to-root ordered list so that children are listed before
        their parents. This ordering is useful when terminating a tree.
        """
        descendants: List[int] = []
        if os.name != 'posix':
            return descendants

        try:
            pid = int(pid)
        except (TypeError, ValueError):
            return descendants

        def collect(current: int, output: List[int]) -> None:
            children_file = Path(f"/proc/{current}/task/{current}/children")
            if not children_file.exists():
                return
            try:
                children = [int(c) for c in children_file.read_text().strip().split() if c]
            except (OSError, ValueError):
                return
            for child in children:
                collect(child, output)
                output.append(child)

        collect(pid, descendants)
        return descendants

    def _kill_pid(self, pid: int, sig: int = signal.SIGTERM) -> bool:
        """Send a signal to a PID, returning True if it was delivered."""
        try:
            os.kill(pid, sig)
            return True
        except (OSError, ProcessLookupError):
            return False

    def _kill_process_tree(
        self,
        root_pid: int,
        descendants: List[int],
        timeout: float = 5.0,
    ) -> None:
        """Terminate a process and all descendants gracefully, then forcefully."""
        # Terminate leaves first so children do not outlive the parent.
        for child_pid in descendants:
            self._kill_pid(child_pid, signal.SIGTERM)

        # Give leaves a moment to exit before terminating the root.
        time.sleep(0.2)
        self._kill_pid(root_pid, signal.SIGTERM)

        # Wait for everything to die, escalating to SIGKILL if necessary.
        deadline = time.time() + timeout
        all_pids = list(descendants) + [root_pid]
        while time.time() < deadline:
            still_alive = [p for p in all_pids if self._is_alive(p)]
            if not still_alive:
                return
            for pid in still_alive:
                self._kill_pid(pid, signal.SIGKILL)
            time.sleep(0.1)

    def _is_alive(self, pid: int) -> bool:
        """Return True if a PID is still alive and not a zombie."""
        try:
            os.kill(pid, 0)
        except (OSError, ProcessLookupError):
            return False

        # On Linux, a zombie process still has a PID but is already dead.
        # Treat zombies as not-alive so we do not loop trying to kill them.
        if os.name == 'posix':
            try:
                for line in Path(f"/proc/{pid}/status").read_text().splitlines():
                    if line.startswith("State:") and "Z" in line:
                        return False
            except (OSError, IndexError):
                pass

        return True
    
    def _get_process_rss_kb(self, pid: int) -> int:
        """Return the resident set size (RSS) of a PID in kilobytes.

        Reads /proc/<pid>/status on Linux. Returns 0 if the process has
        exited or the value cannot be read.
        """
        if os.name != 'posix':
            return 0
        try:
            for line in Path(f"/proc/{pid}/status").read_text().splitlines():
                if line.startswith("VmRSS:"):
                    parts = line.split()
                    return int(parts[1]) if len(parts) >= 2 else 0
        except (OSError, ValueError):
            pass
        return 0

    def _get_tree_rss_kb(self, root_pid: int) -> Tuple[int, Dict[int, int]]:
        """Return total RSS in KB for a process tree and a per-PID breakdown."""
        per_pid: Dict[int, int] = {}
        if os.name != 'posix':
            return 0, per_pid

        try:
            root_pid = int(root_pid)
        except (TypeError, ValueError):
            return 0, per_pid

        pids_to_check = [root_pid] + self._discover_descendants(root_pid)
        total = 0
        for pid in pids_to_check:
            rss = self._get_process_rss_kb(pid)
            if rss > 0:
                per_pid[pid] = rss
                total += rss
        return total, per_pid

    def start_memory_monitor(
        self,
        process_name: str,
        limit_bytes: int = 10 * 1024 * 1024 * 1024,
        log_path: Optional[Path] = None,
        interval_seconds: float = 5.0,
        callback: Optional[Callable[[Dict[str, Any]], None]] = None,
    ) -> None:
        """Start a background watchdog that kills the process if RSS exceeds limit.

        Args:
            process_name: Tracked process name (e.g. "server").
            limit_bytes: Memory threshold. The process tree is terminated when
                total RSS exceeds this value.
            log_path: Optional file to append memory samples to.
            interval_seconds: Sampling interval.
            callback: Optional callable invoked on every sample with a dict
                containing ``timestamp``, ``total_kb``, ``limit_kb``,
                ``per_pid``, ``killed`` and ``process_name``.
        """
        self.stop_memory_monitor(process_name)

        stop_event = threading.Event()
        self._memory_monitor_stop_events[process_name] = stop_event
        if callback is not None:
            self._memory_monitor_callbacks[process_name] = callback

        def monitor() -> None:
            if log_path is not None:
                log_path.parent.mkdir(parents=True, exist_ok=True)

            while not stop_event.is_set():
                try:
                    process = self.processes.get(process_name)
                    if process is None:
                        break

                    root_pid = self._dayz_pids.get(process_name, process.pid)
                    if not self._is_alive(root_pid):
                        break

                    total_kb, per_pid = self._get_tree_rss_kb(root_pid)
                    total_bytes = total_kb * 1024
                    killed = total_bytes > limit_bytes and limit_bytes > 0

                    sample = {
                        "timestamp": time.strftime("%Y-%m-%d %H:%M:%S"),
                        "process_name": process_name,
                        "total_kb": total_kb,
                        "total_mb": total_kb // 1024,
                        "total_gb": total_kb / (1024 * 1024),
                        "limit_kb": limit_bytes // 1024,
                        "limit_gb": limit_bytes / (1024 * 1024 * 1024),
                        "per_pid": per_pid,
                        "killed": killed,
                    }

                    if log_path is not None:
                        try:
                            with log_path.open("a", encoding="utf-8") as fh:
                                fh.write(
                                    f"{sample['timestamp']} | {process_name} | "
                                    f"{sample['total_gb']:.2f} GB / {sample['limit_gb']:.2f} GB | "
                                    f"pids={list(per_pid.keys())}\n"
                                )
                                if killed:
                                    fh.write(
                                        f"{sample['timestamp']} | KILLING {process_name}: "
                                        f"memory limit exceeded ({sample['total_gb']:.2f} GB)\n"
                                    )
                        except OSError as exc:
                            logger.debug("Memory monitor log write error: %s", exc)

                    cb = self._memory_monitor_callbacks.get(process_name)
                    if cb is not None:
                        try:
                            cb(sample)
                        except Exception as exc:
                            logger.debug("Memory monitor callback error: %s", exc)

                    if killed:
                        logger.warning(
                            "Memory limit exceeded for %s: %.2f GB / %.2f GB. Killing process tree.",
                            process_name,
                            sample["total_gb"],
                            sample["limit_gb"],
                        )
                        descendants = self._child_pids.get(process_name, [])
                        self._kill_process_tree(root_pid, descendants)
                        break
                except Exception as exc:
                    logger.debug("Memory monitor error: %s", exc)

                stop_event.wait(interval_seconds)

        thread = threading.Thread(target=monitor, name=f"memory-monitor-{process_name}", daemon=True)
        self._memory_monitors[process_name] = thread
        thread.start()

    def stop_memory_monitor(self, process_name: Optional[str] = None) -> None:
        """Stop the memory watchdog for a process (or all processes if None)."""
        names = [process_name] if process_name else list(self._memory_monitors.keys())
        for name in names:
            event = self._memory_monitor_stop_events.pop(name, None)
            if event is not None:
                event.set()
            self._memory_monitor_callbacks.pop(name, None)
            self._memory_monitors.pop(name, None)

    def get_memory_sample(self, process_name: str) -> Optional[Dict[str, Any]]:
        """Return the current memory usage for a tracked process tree."""
        process = self.processes.get(process_name)
        if process is None:
            return None
        root_pid = self._dayz_pids.get(process_name, process.pid)
        if not self._is_alive(root_pid):
            return None
        total_kb, per_pid = self._get_tree_rss_kb(root_pid)
        return {
            "timestamp": time.strftime("%Y-%m-%d %H:%M:%S"),
            "process_name": process_name,
            "total_kb": total_kb,
            "total_mb": total_kb // 1024,
            "total_gb": total_kb / (1024 * 1024),
            "per_pid": per_pid,
        }

    def stop_server(self, client_first: bool = True) -> Tuple[bool, str]:
        """Gracefully stop server and optionally client."""
        try:
            self.stop_memory_monitor("server")
            if client_first and "client" in self.processes:
                self._stop_process("client")
            
            if "server" in self.processes:
                self._stop_process("server")
            
            return True, "Server stopped"
        
        except Exception as e:
            return False, f"Failed to stop server: {str(e)}"
    
    def _stop_process(self, name: str):
        """Stop a tracked process and its entire descendant tree."""
        if name not in self.processes:
            return

        # Stop the memory watchdog first so it doesn't fight the shutdown.
        self.stop_memory_monitor(name)

        process = self.processes[name]
        descendants = self._child_pids.pop(name, [])
        dayz_pid = self._dayz_pids.pop(name, None)

        # Determine the root of the tree to kill. For terminal launches this is
        # the discovered DayZ child; otherwise it is the tracked process itself.
        root_pid = dayz_pid if dayz_pid is not None else process.pid

        # Kill the whole tree (leaves first) so enfmain and other Enfusion
        # children cannot outlive the server and keep ports/files locked.
        self._kill_process_tree(root_pid, descendants)

        # Also terminate the tracked process if it is still alive (e.g. the
        # terminal emulator that launched the DayZ child).
        if process.poll() is None:
            try:
                if os.name == 'posix':
                    process.send_signal(signal.SIGTERM)
                else:
                    process.send_signal(signal.CTRL_BREAK_EVENT)
                process.wait(timeout=3)
            except subprocess.TimeoutExpired:
                try:
                    process.kill()
                except Exception:
                    pass
        
        del self.processes[name]
    
    def restart_server(
        self,
        mode: Optional[str] = None,
        port: Optional[int] = None,
        query_port: Optional[int] = None,
        steam_port: Optional[int] = None,
    ) -> Tuple[bool, str]:
        """Restart server with optional parameter changes."""
        # Get current params if not specified
        if mode is None:
            mode = self.running_mode or "debug"
        
        if port is None:
            port = 2302  # Default
        
        # Stop then start
        success, msg = self.stop_server()
        if not success:
            return False, f"Failed to stop server: {msg}"
        
        time.sleep(1)  # Brief pause before restart
        
        return self.start_server(
            mode=mode,
            port=port,
            query_port=query_port,
            steam_port=steam_port,
        )
    
    def get_status(self) -> Dict[str, Any]:
        """Get current process status."""
        status = {
            "server_running": False,
            "client_running": False,
            "mode": self.running_mode
        }
        
        for name in ["server", "client"]:
            if name not in self.processes:
                continue
            status[f"{name}_running"] = self._is_name_alive(name)
        
        return status

    def get_all_status(self) -> Dict[str, Dict[str, Any]]:
        """Get status for every tracked process by name."""
        status: Dict[str, Dict[str, Any]] = {}
        for name, process in self.processes.items():
            try:
                is_running = self._is_name_alive(name)
                status[name] = {
                    "running": is_running,
                    "pid": process.pid,
                    "dayz_pid": self._dayz_pids.get(name),
                    "mode": self.running_mode,
                }
            except Exception as exc:
                logger.debug("Process check error for %s: %s", name, exc)
                status[name] = {"running": False, "pid": None, "dayz_pid": None, "mode": self.running_mode}
        return status

    def _is_name_alive(self, name: str) -> bool:
        """Return True if the tracked process (and its DayZ child) is alive."""
        if name not in self.processes:
            return False
        process = self.processes[name]
        if process.poll() is not None:
            return False
        if name in self._dayz_pids:
            return self._is_alive(self._dayz_pids[name])
        return True

    def is_process_name_running(self, process_name: str) -> bool:
        """Return True if a process with the given tracked name is running."""
        return self._is_name_alive(process_name)

    def get_running_process_names(self) -> List[str]:
        """Return the names of all currently tracked running processes."""
        return [name for name in self.processes if self._is_name_alive(name)]

    def stop_process_by_name(self, process_name: str) -> Tuple[bool, str]:
        """Stop a specific tracked process by name."""
        if process_name not in self.processes:
            return False, f"No tracked process named {process_name}"
        try:
            self._stop_process(process_name)
            return True, f"Stopped {process_name}"
        except Exception as exc:
            return False, f"Failed to stop {process_name}: {exc}"

    def kill_orphaned_dayz_processes(
        self,
        instance_root: Path,
        config_path: str,
        port: int,
    ) -> List[str]:
        """Kill any existing DayZ server processes for this instance.

        Scans /proc on Linux for processes whose cwd matches the instance root
        and whose command line references the config file and port. This
        prevents a previously lost process (e.g. enfmain) from keeping the
        instance alive after the GUI thinks it is stopped.

        Returns:
            List of human-readable messages describing killed processes.
        """
        messages: List[str] = []
        if os.name != 'posix':
            return messages

        instance_root = instance_root.resolve()
        config_name = Path(config_path).name
        port_flag = f"-port={port}"

        for proc_dir in Path("/proc").iterdir():
            if not proc_dir.name.isdigit():
                continue
            pid = int(proc_dir.name)
            if pid == os.getpid():
                continue

            try:
                cmdline_path = proc_dir / "cmdline"
                if not cmdline_path.exists():
                    continue
                cmdline_bytes = cmdline_path.read_bytes()
                if not cmdline_bytes:
                    continue
                cmdline = cmdline_bytes.replace(b"\x00", b" ").decode("utf-8", errors="ignore")

                # Only consider DayZ server processes.
                if "DayZ" not in cmdline and "dayz" not in cmdline.lower():
                    continue
                if "-server" not in cmdline.lower() and "DayZServer" not in cmdline:
                    # DayZDiag can also run as a server; keep going if it has server flags.
                    if port_flag not in cmdline and "-config=" not in cmdline:
                        continue

                # Match cwd to the instance root.
                cwd_link = proc_dir / "cwd"
                if cwd_link.exists():
                    try:
                        cwd = cwd_link.resolve()
                    except OSError:
                        continue
                    if cwd != instance_root:
                        continue
                else:
                    continue

                # Match config and port in command line.
                if config_name not in cmdline or port_flag not in cmdline:
                    continue

                # Kill the process and any descendants.
                descendants = self._discover_descendants(pid)
                self._kill_process_tree(pid, descendants)
                messages.append(f"Killed orphaned DayZ process {pid} for {config_path}")
            except (OSError, PermissionError, ValueError):
                continue

        return messages
    
    def get_logs(self, lines: int = 100) -> List[str]:
        """Get recent log entries from process output."""
        logs = []
        
        for name, process in self.processes.items():
            try:
                # Read stderr if available
                line = process.stderr.readline()
                while line and len(logs) < lines:
                    logs.append(line.decode('utf-8', errors='ignore').strip())
                    line = process.stderr.readline()
            except Exception as exc:
                logger.debug("Log read error for %s: %s", name, exc)
        
        return logs[-lines:]