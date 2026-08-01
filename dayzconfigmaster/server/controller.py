# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Headless DayZ server controller.

This module extracts the core server startup, stopping, and instance-management
logic out of `gui/app.py` so it can be unit-tested and reused without Tkinter.
The GUI layer (`DayzConfigMasterApp`) should delegate to this controller and
only handle widget updates and user prompts.
"""

from __future__ import annotations

import shutil
import socket
import subprocess
import time
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple


@dataclass
class ServerInstance:
    """Configuration for a single DayZ server instance."""
    instance_id: str = "1"
    config_file: str = "serverDZ.cfg"
    root_folder: str = ""
    game_port: str = "2302"
    query_port: str = "2303"
    steam_port: str = "27016"
    map_name: str = ""
    profile_path: str = ""
    mod_paths: str = ""
    mission_path: str = ""

    def to_dict(self) -> Dict[str, Any]:
        return {
            "id": self.instance_id,
            "config_file": self.config_file,
            "root_folder": self.root_folder,
            "game_port": self.game_port,
            "query_port": self.query_port,
            "steam_port": self.steam_port,
            "map": self.map_name,
            "profile": self.profile_path,
            "mod_paths": self.mod_paths,
            "mission_path": self.mission_path,
        }


@dataclass
class ServerStartResult:
    """Result of starting a DayZ server instance."""
    success: bool
    instance_id: str
    message: str = ""
    pid: Optional[int] = None


@dataclass
class ServerStopResult:
    """Result of stopping a DayZ server instance."""
    success: bool
    instance_id: str
    message: str = ""


class DayzServerController:
    """Headless controller for DayZ server instances."""

    def __init__(
        self,
        dayz_server_path: str,
        projects_root: Optional[str] = None,
        process_controller: Optional[Any] = None,
    ):
        self.dayz_server_path = Path(dayz_server_path) if dayz_server_path else Path()
        self.projects_root = Path(projects_root) if projects_root else Path.home() / "Documents" / "DayZProjects"
        self._process_controller = process_controller
        self._running: Dict[str, subprocess.Popen] = {}

    # ------------------------------------------------------------------
    # Instance root helpers
    # ------------------------------------------------------------------
    def default_instance_root(self, instance_id: str) -> Path:
        """Return the default on-disk root for an instance."""
        return self.projects_root / "instances" / f"server{instance_id}"

    def resolve_instance_root(self, instance: ServerInstance) -> Path:
        """Resolve the instance root, falling back to the default path."""
        if instance.root_folder:
            return Path(instance.root_folder)
        return self.default_instance_root(instance.instance_id)

    # ------------------------------------------------------------------
    # Port checks
    # ------------------------------------------------------------------
    def is_port_bound(self, port: int) -> bool:
        """Return True if *port* is already listening on any interface."""
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
                sock.settimeout(0.5)
                sock.bind(("0.0.0.0", port))
                return False
        except OSError:
            return True
        except Exception:
            return False

    def validate_ports(self, instance: ServerInstance) -> Tuple[bool, str]:
        """Check that the configured ports are available."""
        try:
            ports = {
                "game": int(instance.game_port),
                "query": int(instance.query_port),
                "steam": int(instance.steam_port),
            }
        except ValueError:
            return False, "Ports must be integers"

        for name, port in ports.items():
            if self.is_port_bound(port):
                return False, f"{name} port {port} is already in use"
        return True, ""

    # ------------------------------------------------------------------
    # Executable / command building
    # ------------------------------------------------------------------
    def _find_executable(self) -> Optional[Path]:
        """Locate the DayZ server executable."""
        suffix = ".exe" if __import__("os").name == "nt" else ""
        for name in ("DayZServer_x64", "DayZServer", "DayZDiag_x64", "DayZDiag"):
            candidate = self.dayz_server_path / f"{name}{suffix}"
            if candidate.exists():
                return candidate
        return None

    def build_command(self, instance: ServerInstance) -> Tuple[List[str], Path]:
        """Build the command line and working directory for an instance."""
        root = self.resolve_instance_root(instance)
        exe = self._find_executable()
        if exe is None:
            raise FileNotFoundError(f"No DayZ server executable found in {self.dayz_server_path}")

        mods = instance.mod_paths.strip()
        mod_arg = f'-mod="{mods}"' if mods else ""

        # Prefer absolute config path inside the instance root.
        config_path = root / instance.config_file
        if not config_path.exists():
            config_path = self.dayz_server_path / instance.config_file

        cmd = [
            str(exe),
            f"-port={instance.game_port}",
            f"-queryPort={instance.query_port}",
            f"-steamPort={instance.steam_port}",
            f"-config={config_path}",
            f"-profiles={instance.profile_path or root / 'profiles'}",
            f"-instanceId={instance.instance_id}",
        ]
        if mod_arg:
            cmd.append(mod_arg)
        if instance.mission_path:
            cmd.append(f'-mission={instance.mission_path}')

        return cmd, root

    # ------------------------------------------------------------------
    # Start / stop
    # ------------------------------------------------------------------
    def start_instance(self, instance: ServerInstance) -> ServerStartResult:
        """Start a single DayZ server instance."""
        ok, msg = self.validate_ports(instance)
        if not ok:
            return ServerStartResult(False, instance.instance_id, msg)

        root = self.resolve_instance_root(instance)
        root.mkdir(parents=True, exist_ok=True)

        try:
            cmd, cwd = self.build_command(instance)
        except FileNotFoundError as exc:
            return ServerStartResult(False, instance.instance_id, str(exc))

        try:
            proc = subprocess.Popen(cmd, cwd=str(cwd))
            self._running[instance.instance_id] = proc
            return ServerStartResult(True, instance.instance_id, "Server started", pid=proc.pid)
        except Exception as exc:
            return ServerStartResult(False, instance.instance_id, f"Failed to start: {exc}")

    def stop_instance(self, instance_id: str, timeout: float = 30.0) -> ServerStopResult:
        """Stop a running DayZ server instance."""
        proc = self._running.pop(instance_id, None)
        if proc is None and self._process_controller is not None:
            try:
                self._process_controller.stop_process(f"server_instance_{instance_id}")
                return ServerStopResult(True, instance_id, "Stopped via process controller")
            except Exception as exc:
                return ServerStopResult(False, instance_id, str(exc))

        if proc is None:
            return ServerStopResult(False, instance_id, "Instance is not running")

        try:
            proc.terminate()
            proc.wait(timeout=timeout)
            return ServerStopResult(True, instance_id, "Stopped gracefully")
        except subprocess.TimeoutExpired:
            proc.kill()
            return ServerStopResult(True, instance_id, "Killed after timeout")
        except Exception as exc:
            return ServerStopResult(False, instance_id, f"Error stopping: {exc}")

    def stop_all(self) -> List[ServerStopResult]:
        """Stop every tracked instance."""
        results: List[ServerStopResult] = []
        for instance_id in list(self._running.keys()):
            results.append(self.stop_instance(instance_id))
        return results

    def is_running(self, instance_id: str) -> bool:
        """Return True if the instance process is still alive."""
        proc = self._running.get(instance_id)
        if proc is None:
            return False
        return proc.poll() is None

    # ------------------------------------------------------------------
    # Deployment hooks
    # ------------------------------------------------------------------
    def prepare_instance_files(
        self,
        instance: ServerInstance,
        mission_path: Optional[Path] = None,
    ) -> Tuple[bool, str]:
        """Ensure the instance directory and key files exist."""
        root = self.resolve_instance_root(instance)
        try:
            root.mkdir(parents=True, exist_ok=True)
            (root / "profiles").mkdir(parents=True, exist_ok=True)
            (root / "keys").mkdir(parents=True, exist_ok=True)
            if mission_path and mission_path.exists():
                target = root / "mpmissions" / mission_path.name
                target.parent.mkdir(parents=True, exist_ok=True)
                if target.exists() or target.is_symlink():
                    target.unlink()
                shutil.copytree(mission_path, target, dirs_exist_ok=True)
            return True, "Instance files prepared"
        except Exception as exc:
            return False, f"Failed to prepare instance files: {exc}"
