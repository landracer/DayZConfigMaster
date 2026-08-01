# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Server instance manager for DayZConfigMaster.

Handles:
- Creating and managing multiple server instances
- Per-instance configuration storage
- Automatic port assignment
- Profile management
"""

import json
import os
import signal
from pathlib import Path
from typing import Dict, List, Optional, Any
from dataclasses import dataclass, field, asdict


@dataclass
class ServerInstance:
    """A DayZ server instance configuration."""
    name: str = ""
    enabled: bool = True

    # Network settings
    port: int = 2302
    query_port: int = 2303

    # Game settings
    map_size: int = 2000
    max_players: int = 60
    password: str = ""

    # Path settings
    dayz_path: str = ""
    profiles_path: str = ""

    # Mode and params
    mode: str = "debug"  # debug|normal

    server_params_debug: List[str] = field(default_factory=lambda: [
        "-filePatching", "-dologs", "-adminLog", "-freezecheck"
    ])

    server_params_normal: List[str] = field(default_factory=lambda: [
        "-dologs", "-adminLog", "-freezecheck"
    ])

    # Mod paths (steam:// URLs or local paths)
    mods: List[str] = field(default_factory=list)

    # ServerDZ.cfg path
    config_path: str = "serverDZ.cfg"

    # Tracked process PID (populated when started)
    pid: Optional[int] = None


class InstanceManager:
    """Manages multiple DayZ server instances."""

    BASE_PORT = 2302

    def __init__(self, projects_root: str):
        """
        Initialize instance manager.

        Args:
            projects_root: Root directory for all instances and mods
        """
        self.projects_root = Path(projects_root)
        self.instances_dir = self.projects_root / "instances"
        self.config_path = self.projects_root / "config.json"

        # Load instances from disk
        self.instances: Dict[str, ServerInstance] = {}
        self._load_instances()

    def _load_instances(self):
        """Load all instances from disk."""
        if not self.instances_dir.exists():
            self.instances_dir.mkdir(parents=True)
            return

        for instance_file in self.instances_dir.glob("*.json"):
            try:
                with open(instance_file, "r") as f:
                    data = json.load(f)

                instance = ServerInstance(**data)
                self.instances[instance.name] = instance
            except Exception as e:
                print(f"Error loading instance {instance_file}: {e}")

    def save_instances(self):
        """Save all instances to disk."""
        if not self.instances_dir.exists():
            self.instances_dir.mkdir(parents=True)

        for name, instance in self.instances.items():
            config_file = self.instances_dir / f"{name}.json"
            data = asdict(instance)

            with open(config_file, "w") as f:
                json.dump(data, f, indent=2)

    def _used_ports(self) -> List[int]:
        """Return all ports currently assigned to instances."""
        return [instance.port for instance in self.instances.values()]

    def next_free_port(self, start: int = BASE_PORT) -> int:
        """Return the first free port at/after ``start``."""
        used = set(self._used_ports())
        port = start
        while port in used:
            port += 1
        return port

    def create_instance(
        self,
        name: str,
        port: Optional[int] = None,
        dayz_path: Optional[str] = None
    ) -> tuple:
        """
        Create a new server instance.

        Args:
            name: Instance name (must be unique)
            port: Server port (auto-assigned if omitted)
            dayz_path: Path to DayZ installation

        Returns:
            Tuple of (success, message)
        """
        if not name:
            return False, "Instance name is required"

        if name in self.instances:
            return False, f"Instance '{name}' already exists"

        # Validate path if provided
        if dayz_path and not Path(dayz_path).exists():
            return False, f"DayZ path not found: {dayz_path}"

        # Auto-assign port if not provided
        assigned_port = port if port is not None else self.next_free_port()

        instance = ServerInstance(
            name=name,
            port=assigned_port,
            query_port=assigned_port + 1,
            dayz_path=dayz_path or ""
        )

        self.instances[name] = instance
        self.save_instances()

        return True, f"Instance '{name}' created on port {assigned_port}"

    def delete_instance(self, name: str) -> tuple:
        """Delete an instance."""
        if name not in self.instances:
            return False, f"Instance '{name}' not found"

        if self.is_running(name):
            return False, "Cannot delete running instance"

        del self.instances[name]

        config_file = self.instances_dir / f"{name}.json"
        if config_file.exists():
            config_file.unlink()

        return True, f"Instance '{name}' deleted"

    def update_instance(self, name: str, **kwargs) -> tuple:
        """Update instance configuration."""
        if name not in self.instances:
            return False, f"Instance '{name}' not found"

        instance = self.instances[name]

        for key, value in kwargs.items():
            if hasattr(instance, key):
                setattr(instance, key, value)

        self.save_instances()
        return True, f"Instance '{name}' updated"

    def get_instance(self, name: str) -> Optional[ServerInstance]:
        """Get instance by name."""
        return self.instances.get(name)

    def list_instances(self) -> List[ServerInstance]:
        """List all instances."""
        return list(self.instances.values())

    def is_running(self, name: str) -> bool:
        """Check if an instance's tracked PID is still alive."""
        instance = self.instances.get(name)
        if not instance or not instance.pid:
            return False

        try:
            os.kill(instance.pid, 0)
            return True
        except (OSError, ProcessLookupError):
            instance.pid = None
            self.save_instances()
            return False

    def set_pid(self, name: str, pid: Optional[int]) -> bool:
        """Update the tracked PID for an instance."""
        instance = self.instances.get(name)
        if not instance:
            return False

        instance.pid = pid
        self.save_instances()
        return True

    def start_instance(self, name: str) -> tuple:
        """Start an instance."""
        instance = self.instances.get(name)
        if not instance:
            return False, f"Instance '{name}' not found"

        return True, f"Instance '{name}' ready to start on port {instance.port}"

    def stop_instance(self, name: str) -> tuple:
        """Stop an instance."""
        if name not in self.instances:
            return False, f"Instance '{name}' not found"

        instance = self.instances[name]
        if instance.pid:
            try:
                os.kill(instance.pid, signal.SIGTERM)
            except (OSError, ProcessLookupError):
                pass
            instance.pid = None
            self.save_instances()

        return True, f"Instance '{name}' stopped"