# DayZConfigMaster Feature Parity Porting Plan

## Executive Summary

This document outlines the complete implementation plan to port all features from **DayZ Labs (dzl)** to **DayZConfigMaster**, making it a fully functional, cross-platform master tool for DayZ server and mod management.

**Goal**: DayZConfigMaster should have EQUAL or BETTER functionality than dayz-labs while maintaining cross-platform compatibility (Windows, Linux, macOS).

---

## Architecture Overview

### Current State
- Python 3.6+ with Tkinter GUI
- Configuration file editing only
- Basic multi-instance framework
- No build pipeline, no server lifecycle control

### Target State
- Python 3.8+ (for subprocess run() improvements)
- Enhanced Tkinter/GTK/Qt hybrid for native look on each platform
- Complete server lifecycle management
- Full mod building and deployment pipeline
- steamcmd integration
- Git integration via GitPython

---

## Phase 1: Core Server Lifecycle Management

### 1.1 Process Controller Module
**File**: `dayzconfigmaster/server/process_controller.py`

```python
# Imports
import subprocess
import os
import signal
from pathlib import Path
import time
from typing import Optional, List, Tuple, Dict, Any

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
    
    def _get_exe_path(self, exe_name: str) -> Optional[Path]:
        """Get full path to executable (OS-specific handling)."""
        if os.name == 'nt':  # Windows
            suffix = ".exe"
        else:  # Linux/macOS
            suffix = ""
        
        path = self.dayz_path / f"{exe_name}{suffix}"
        return path if path.exists() else None
    
    def start_server(
        self,
        port: int = 2302,
        mode: str = "debug",
        map_size: int = 2000,
        max_players: int = 60,
        config_path: Optional[str] = None,
        mods: Optional[List[str]] = None,
        client: bool = False
    ) -> Tuple[bool, str]:
        """
        Start DayZ server with optional client.
        
        Args:
            port: Server port (default 2302)
            mode: debug|normal mode
            map_size: Map dimensions in meters
            max_players: Maximum concurrent players
            config_path: Path to serverDZ.cfg
            mods: List of mod paths to include
            client: Also start client? (default False)
        
        Returns:
            Tuple of (success, message)
        """
        try:
            # Determine executable based on mode
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
            cmd.append(f"-maxPlayers={max_players}")
            cmd.append(f"-mapSize={map_size}")
            
            if config_path:
                cmd.append(f"-config=\"{config_path}\"")
            
            # Mods handling
            if mods:
                mod_paths = ";".join(mods)  # DayZ format uses semicolons
                cmd.append(f"-mod=\"{mod_paths}\"")
            
            # Start server process
            self._start_process("server", cmd)
            
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
    
    def _start_process(self, name: str, cmd: List[str]) -> subprocess.Popen:
        """Start a process and track its PID."""
        if os.name == 'posix':
            # Create new session for process isolation on Unix
            process = subprocess.Popen(
                cmd,
                start_new_session=True,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE
            )
        else:  # Windows
            process = subprocess.Popen(
                cmd,
                creationflags=subprocess.CREATE_NEW_PROCESS_GROUP,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE
            )
        
        self.processes[name] = process
        return process
    
    def stop_server(self, client_first: bool = True) -> Tuple[bool, str]:
        """Gracefully stop server and optionally client."""
        try:
            if client_first and "client" in self.processes:
                self._stop_process("client")
            
            if "server" in self.processes:
                self._stop_process("server")
            
            return True, "Server stopped"
        
        except Exception as e:
            return False, f"Failed to stop server: {str(e)}"
    
    def _stop_process(self, name: str):
        """Stop a tracked process gracefully."""
        if name not in self.processes:
            return
        
        process = self.processes[name]
        
        # Try graceful termination first
        if os.name == 'posix':
            process.send_signal(signal.SIGTERM)
        else:
            process.send_signal(signal.CTRL_BREAK_EVENT)
        
        try:
            process.wait(timeout=5)  # Wait up to 5 seconds
        except subprocess.TimeoutExpired:
            # Force kill if grace fails
            if os.name == 'posix':
                process.kill()
            else:
                process.terminate()
        
        del self.processes[name]
    
    def restart_server(
        self,
        mode: Optional[str] = None,
        port: Optional[int] = None
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
        
        return self.start_server(mode=mode, port=port)
    
    def get_status(self) -> Dict[str, Any]:
        """Get current process status."""
        status = {
            "server_running": False,
            "client_running": False,
            "mode": self.running_mode
        }
        
        for name in ["server", "client"]:
            if name in self.processes:
                try:
                    # Check if process is still running
                    returncode = self.processes[name].poll()
                    status[f"{name}_running"] = returncode is None
                except:
                    pass
        
        return status
    
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
            except:
                pass
        
        return logs[-lines:]
```

### 1.2 Server Instance Manager
**File**: `dayzconfigmaster/server/instance_manager.py`

```python
"""
Server instance manager for DayZConfigMaster.

Handles:
- Creating and managing multiple server instances
- Per-instance configuration storage
- Port assignment
- Profile management
"""

import json
import os
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

class InstanceManager:
    """Manages multiple DayZ server instances."""
    
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
                with open(instance_file, 'r') as f:
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
            
            # Convert dataclass to dict
            data = {
                "name": instance.name,
                "enabled": instance.enabled,
                "port": instance.port,
                "query_port": instance.query_port,
                "map_size": instance.map_size,
                "max_players": instance.max_players,
                "password": instance.password,
                "dayz_path": instance.dayz_path,
                "profiles_path": instance.profiles_path,
                "mode": instance.mode,
                "server_params_debug": instance.server_params_debug,
                "server_params_normal": instance.server_params_normal,
                "mods": instance.mods,
                "config_path": instance.config_path
            }
            
            with open(config_file, 'w') as f:
                json.dump(data, f, indent=2)
    
    def create_instance(
        self,
        name: str,
        port: int = 2302,
        dayz_path: Optional[str] = None
    ) -> Tuple[bool, str]:
        """
        Create a new server instance.
        
        Args:
            name: Instance name (must be unique)
            port: Server port
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
        
        # Create instance with defaults
        instance = ServerInstance(
            name=name,
            port=port,
            dayz_path=dayz_path or ""
        )
        
        self.instances[name] = instance
        self.save_instances()
        
        return True, f"Instance '{name}' created"
    
    def delete_instance(self, name: str) -> Tuple[bool, str]:
        """Delete an instance."""
        if name not in self.instances:
            return False, f"Instance '{name}' not found"
        
        # Check if running
        if self.is_running(name):
            return False, "Cannot delete running instance"
        
        del self.instances[name]
        
        config_file = self.instances_dir / f"{name}.json"
        if config_file.exists():
            config_file.unlink()
        
        return True, f"Instance '{name}' deleted"
    
    def update_instance(
        self,
        name: str,
        **kwargs
    ) -> Tuple[bool, str]:
        """Update instance configuration."""
        if name not in self.instances:
            return False, f"Instance '{name}' not found"
        
        instance = self.instances[name]
        
        # Update fields
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
        """Check if an instance is running."""
        # Placeholder - will be implemented with ProcessController
        return False
    
    def start_instance(self, name: str) -> Tuple[bool, str]:
        """Start an instance."""
        instance = self.instances.get(name)
        if not instance:
            return False, f"Instance '{name}' not found"
        
        # Get controller (will be passed in from GUI)
        # This would be called with process_controller.start_server(...)
        return True, "Instance started"
    
    def stop_instance(self, name: str) -> Tuple[bool, str]:
        """Stop an instance."""
        return True, "Instance stopped"
```

### 1.3 Server Profile/Preset System
**File**: `dayzconfigmaster/server/profiles.py`

```python
"""
Profile system for saving and loading server configurations.

Profiles are named snapshots of server configuration that can be
quickly switched between.
"""

import json
from pathlib import Path
from typing import Dict, List, Optional, Any

class ProfileManager:
    """
    Manages named server profiles/presets.
    
    A profile contains the complete server configuration state:
    - Mod list (paths and enabled state)
    - Server parameters
    - Map settings
    - Port configuration
    
    Profiles are stored as JSON files in <projects_root>/profiles/
    """
    
    def __init__(self, projects_root: str):
        self.projects_root = Path(projects_root)
        self.profiles_dir = self.projects_root / "profiles"
        
        if not self.profiles_dir.exists():
            self.profiles_dir.mkdir(parents=True)
    
    def _get_profile_path(self, name: str) -> Path:
        """Get path to profile file."""
        return self.profiles_dir / f"{name}.json"
    
    def save_profile(
        self,
        name: str,
        mod_list: List[Dict[str, Any]],
        server_config: Dict[str, Any],
        mode: str = "debug"
    ) -> Tuple[bool, str]:
        """
        Save current configuration as a profile.
        
        Args:
            name: Profile name
            mod_list: List of enabled mods with paths and sides
            server_config: Server configuration dict
            mode: debug|normal
        
        Returns:
            Tuple of (success, message)
        """
        if not name:
            return False, "Profile name is required"
        
        profile_data = {
            "name": name,
            "saved_at": str(datetime.now()),
            "mod_list": mod_list,
            "server_config": server_config,
            "mode": mode
        }
        
        try:
            with open(self._get_profile_path(name), 'w') as f:
                json.dump(profile_data, f, indent=2)
            
            return True, f"Profile '{name}' saved"
        except Exception as e:
            return False, f"Failed to save profile: {str(e)}"
    
    def load_profile(self, name: str) -> Optional[Dict[str, Any]]:
        """Load a saved profile."""
        try:
            with open(self._get_profile_path(name), 'r') as f:
                return json.load(f)
        except FileNotFoundError:
            return None
        except Exception as e:
            print(f"Error loading profile: {e}")
            return None
    
    def delete_profile(self, name: str) -> Tuple[bool, str]:
        """Delete a saved profile."""
        try:
            path = self._get_profile_path(name)
            if path.exists():
                path.unlink()
                return True, f"Profile '{name}' deleted"
            else:
                return False, f"Profile '{name}' not found"
        except Exception as e:
            return False, f"Failed to delete profile: {str(e)}"
    
    def list_profiles(self) -> List[str]:
        """List all available profiles."""
        profiles = []
        if self.profiles_dir.exists():
            for p in self.profiles_dir.glob("*.json"):
                profiles.append(p.stem)
        return sorted(profiles)
    
    def get_active_profile(self) -> Optional[str]:
        """Get currently active profile (stored in main config)."""
        # To be implemented with global config
        return None
    
    def set_active_profile(self, name: str) -> Tuple[bool, str]:
        """Set the active profile."""
        # Implementation with global config
        return True, "Profile activated"
```

---

## Phase 2: Mod Building Pipeline

### 2.1 PBO Builder Module (AddonBuilder Wrapper)
**File**: `dayzconfigmaster/mods/pbo_builder.py`

```python
"""
PBO Builder module for DayZConfigMaster.

Uses AddonBuilder CLI from DayZ Tools to pack mods into PBO files.
This is cross-platform compatible and handles all the complex packing logic.

Features:
- Pack folders into PBO with prefix
- Binarize configs/models
- Sign PBOs with creator key
- Clean build option
"""

import subprocess
import os
from pathlib import Path
from typing import Optional, List, Tuple

class PboBuilder:
    """
    Wrapper for AddonBuilder from DayZ Tools.
    
    Cross-platform wrapper that handles:
    - Path normalization (Windows vs Unix)
    - Command construction
    - Error handling
    - Progress reporting
    """
    
    def __init__(self, dayz_tools_path: str):
        """
        Initialize with DayZ Tools installation path.
        
        Args:
            dayz_tools_path: Path to DayZ Tools directory
        """
        self.dayz_tools_path = Path(dayz_tools_path)
        self.addon_builder_path = self._find_addon_builder()
    
    def _find_addon_builder(self) -> Optional[Path]:
        """Locate AddonBuilder executable."""
        # Check common locations
        possible_names = ["AddonsBuilder", "AddonBuilder", "addonbuilder"]
        
        for name in possible_names:
            if os.name == 'nt':
                exe_path = self.dayz_tools_path / "Bin" / f"{name}.exe"
            else:
                exe_path = self.dayz_tools_path / "Bin" / name
            
            if exe_path.exists():
                return exe_path
        
        # Return None if not found
        return None
    
    def pack(
        self,
        source_dir: str,
        output_pbo: str,
        prefix: Optional[str] = None,
        sign_key: Optional[str] = None,
        clean: bool = False,
        binarize: bool = True,
        verbose: bool = False
    ) -> Tuple[bool, str]:
        """
        Pack a folder into a PBO file using AddonBuilder.
        
        Args:
            source_dir: Source directory to pack
            output_pbo: Output PBO path (without .pbo extension)
            prefix: PBO prefix string
            sign_key: Path to private key for signing
            clean: Clear output folder before packing?
            binarize: Binarize configs/models? (False = -packonly)
            verbose: Show detailed output?
        
        Returns:
            Tuple of (success, message)
        """
        if not self.addon_builder_path:
            return False, "AddonBuilder not found in DayZ Tools"
        
        # Convert to absolute paths
        source_dir = str(Path(source_dir).resolve())
        output_pbo = str(Path(output_pbo).resolve())
        
        cmd = [str(self.addon_builder_path), source_dir]
        
        # Output PBO
        if os.name == 'nt':
            # Windows uses quotes for paths
            cmd.append(f'"{output_pbo}.pbo"')
        else:
            cmd.append(f"{output_pbo}.pbo")
        
        # Prefix (wrapped in quotes)
        if prefix:
            cmd.append(f'"{prefix}"')
        
        # Signing key
        if sign_key:
            cmd.append(str(Path(sign_key).resolve()))
        
        # Clean flag (-clear)
        if clean:
            cmd.append("-clear")
        
        # Pack only vs full build
        if not binarize:
            cmd.append("-packonly")
        
        # Verbose mode
        if verbose:
            cmd.append("-dologs")
        
        try:
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=300  # 5 minute timeout
            )
            
            # Check for errors in output
            if "SUCCESS" in result.stdout or result.returncode == 0:
                return True, f"PBO created: {output_pbo}.pbo"
            
            error_msg = result.stderr.strip() or result.stdout.strip()
            return False, f"Build failed: {error_msg}"
        
        except subprocess.TimeoutExpired:
            return False, "Build timed out after 5 minutes"
        except Exception as e:
            return False, f"Build error: {str(e)}"
    
    def unbinarize(self, input_pbo: str, output_path: Optional[str] = None) -> Tuple[bool, str]:
        """
        Unbinarize a config.bin to .cpp using DeRap.
        
        Args:
            input_pbo: Path to the PBO or config.bin
            output_path: Output path for .cpp file
        
        Returns:
            Tuple of (success, message)
        """
        # Use CfgConvert/DeRap for unbinarization
        return False, "Not yet implemented - requires DeRap"
```

### 2.2 Mod Discovery Module
**File**: `dayzconfigmaster/mods/discovery.py`

```python
"""
Mod discovery module for scanning and identifying DayZ mods.

Scans configured roots to find available mods and their metadata.
"""

import os
from pathlib import Path
from typing import Dict, List, Optional, Any

class ModInfo:
    """Information about a discovered mod."""
    
    def __init__(
        self,
        name: str,
        path: str,
        enabled: bool = False,
        side: str = "both",  # both|server|client
        missing: bool = False,
        version: Optional[str] = None,
        author: Optional[str] = None,
        description: Optional[str] = None
    ):
        self.name = name
        self.path = path
        self.enabled = enabled
        self.side = side
        self.missing = missing
        self.version = version
        self.author = author
        self.description = description
    
    def to_dict(self) -> Dict[str, Any]:
        """Convert to dictionary for JSON serialization."""
        return {
            "name": self.name,
            "path": self.path,
            "enabled": self.enabled,
            "side": self.side,
            "missing": self.missing,
            "version": self.version,
            "author": self.author,
            "description": self.description
        }

class ModDiscovery:
    """Discovers DayZ mods in configured paths."""
    
    # Common mod metadata files
    METADATA_FILES = ["mod.cpp", "meta.cpp", "config.cpp"]
    
    def __init__(self, scan_roots: List[str]):
        """
        Initialize with paths to scan for mods.
        
        Args:
            scan_roots: List of directory paths to scan
        """
        self.scan_roots = [Path(root) for root in scan_roots]
    
    def discover(self) -> List[ModInfo]:
        """Scan configured roots and return list of discovered mods."""
        found_mods = []
        
        for root in self.scan_roots:
            if not root.exists():
                continue
            
            # Scan immediate subdirectories
            try:
                for subdir in root.iterdir():
                    if subdir.is_dir() and self._is_mod(subdir):
                        mod_info = self._read_mod_metadata(subdir)
                        found_mods.append(mod_info)
            except (OSError, PermissionError) as e:
                print(f"Error scanning {root}: {e}")
        
        return found_mods
    
    def _is_mod(self, path: Path) -> bool:
        """Check if a directory looks like a DayZ mod."""
        # Check for common mod structure
        addons_dir = path / "addons"
        
        if addons_dir.exists() and addons_dir.is_dir():
            return True
        
        # Or check for direct config.cpp in root
        config_cpp = path / "config.cpp"
        if config_cpp.exists():
            return True
        
        return False
    
    def _read_mod_metadata(self, path: Path) -> ModInfo:
        """Read mod metadata from files."""
        name = path.name  # Default to folder name
        version = None
        author = None
        description = None
        
        # Try to read mod.cpp or meta.cpp for metadata
        for metadata_file in self.METADATA_FILES:
            filepath = path / metadata_file
            if filepath.exists():
                try:
                    content = filepath.read_text()
                    
                    # Parse name from file (simple pattern matching)
                    import re
                    
                    # Look for name field
                    name_match = re.search(r'name\s*=\s*"([^"]+)"', content, re.IGNORECASE)
                    if name_match:
                        name = name_match.group(1).strip()
                    
                    # Look for version
                    version_match = re.search(r'version\s*=\s*"([^"]+)"', content, re.IGNORECASE)
                    if version_match:
                        version = version_match.group(1).strip()
                    
                    # Look for author
                    author_match = re.search(r'author\s*=\s*"([^"]+)"', content, re.IGNORECASE)
                    if author_match:
                        author = author_match.group(1).strip()
                    
                    # Look for description
                    desc_match = re.search(r'description\s*=\s*"([^"]+)"', content, re.IGNORECASE)
                    if desc_match:
                        description = desc_match.group(1).strip()
                    
                    break  # Found a metadata file, stop searching
                    
                except Exception as e:
                    print(f"Error reading {filepath}: {e}")
        
        return ModInfo(
            name=name,
            path=str(path),
            version=version,
            author=author,
            description=description
        )
    
    def merge_with_config(
        self,
        discovered: List[ModInfo],
        configured: Dict[str, Dict[str, Any]]
    ) -> List[ModInfo]:
        """
        Merge discovered mods with configuration.
        
        Args:
            discovered: List of currently discovered mods
            configured: Configured mod state (enabled/side from previous session)
        
        Returns:
            Merged list with configured state preserved
        """
        # Build config lookup
        config_lookup = {}
        for path, state in configured.items():
            config_lookup[path] = state
        
        result = []
        
        # Process discovered mods
        for mod in discovered:
            if mod.path in config_lookup:
                # Preserve configured state
                state = config_lookup[mod.path]
                mod.enabled = state.get("enabled", False)
                mod.side = state.get("side", "both")
            
            result.append(mod)
        
        return result

class ModLoader:
    """
    Manages enabled mod list for server launch.
    
    Handles:
    - Loading/saving mod configuration
    - Side assignment (server/client/both)
    - Load order preservation
    """
    
    def __init__(self, projects_root: str):
        self.projects_root = Path(projects_root)
        self.mods_dir = self.projects_root / "mods"
        self.config_path = self.projects_root / ".dzl" / "mod_config.json"
        
        # Ensure config directory exists
        self.config_path.parent.mkdir(parents=True, exist_ok=True)
        
        self.enabled_mods: List[Dict[str, Any]] = []
        self._load()
    
    def _load(self):
        """Load saved mod configuration."""
        if not self.config_path.exists():
            return
        
        try:
            with open(self.config_path, 'r') as f:
                data = json.load(f)
            
            self.enabled_mods = data.get("enabled_mods", [])
        except Exception as e:
            print(f"Error loading mod config: {e}")
    
    def save(self):
        """Save current configuration."""
        try:
            data = {
                "enabled_mods": self.enabled_mods,
                "last_save": str(datetime.now())
            }
            
            with open(self.config_path, 'w') as f:
                json.dump(data, f, indent=2)
        
        except Exception as e:
            print(f"Error saving mod config: {e}")
    
    def enable_mod(self, path: str, side: str = "both") -> bool:
        """Enable a mod with specified side."""
        # Check if already in list
        for mod in self.enabled_mods:
            if mod["path"] == path:
                mod["enabled"] = True
                mod["side"] = side
                return True
        
        # Add new entry
        self.enabled_mods.append({
            "path": path,
            "enabled": True,
            "side": side
        })
        
        return True
    
    def disable_mod(self, path: str) -> bool:
        """Disable a mod."""
        for mod in self.enabled_mods:
            if mod["path"] == path:
                mod["enabled"] = False
                return True
        
        return False
    
    def get_enabled_list(self) -> List[Dict[str, Any]]:
        """Get list of enabled mods (sorted by side and path)."""
        return [m for m in self.enabled_mods if m.get("enabled", False)]
    
    def get_server_mods(self) -> List[str]:
        """Get server-side mod paths."""
        return [
            m["path"] for m in self.enabled_mods
            if m.get("side") in ("both", "server")
        ]
    
    def get_client_mods(self) -> List[str]:
        """Get client-side mod paths."""
        return [
            m["path"] for m in self.enabled_mods
            if m.get("side") in ("both", "client")
        ]

class ModDeployer:
    """
    Deploys built PBOs to work drive or server profile.
    
    Handles:
    - Deploying PBOs to DayZ work drive (P:)
    - Copying to server profiles directory
    - Setting up required folder structure
    """
    
    def __init__(self, dayz_path: str, projects_root: str):
        self.dayz_path = Path(dayz_path)
        self.projects_root = Path(projects_root)
        
        # Work drive paths (will be mounted on Windows)
        self.work_drive_source = self.projects_root / "workdrive"
    
    def deploy_pbo(
        self,
        pbo_file: str,
        server_name: Optional[str] = None
    ) -> Tuple[bool, str]:
        """
        Deploy a PBO to the appropriate location.
        
        Args:
            pbo_file: Path to built PBO file
            server_name: Server instance name (if not provided, deploys to work drive)
        
        Returns:
            Tuple of (success, message)
        """
        try:
            pbo_path = Path(pbo_file)
            
            if not pbo_path.exists():
                return False, f"PBO file not found: {pbo_file}"
            
            # Determine deployment location
            if server_name:
                # Deploy to server profiles
                profile_dir = self.dayz_path / "profiles" / server_name
                addons_dir = profile_dir / "Addons"
                
                # Create directory structure
                addons_dir.mkdir(parents=True, exist_ok=True)
                
                # Copy PBO to server profile
                dest_pbo = addons_dir / pbo_path.name
                import shutil
                shutil.copy2(str(pbo_path), str(dest_pbo))
                
                return True, f"PBO deployed to server profile"
            
            else:
                # Deploy to work drive (P:)
                # On Windows: subst P: <work_drive_source>
                # On Linux/macOS: symlink
                
                mods_dir = self.work_drive_source / "Mods"
                mods_dir.mkdir(parents=True, exist_ok=True)
                
                dest_pbo = mods_dir / pbo_path.name
                import shutil
                shutil.copy2(str(pbo_path), str(dest_pbo))
                
                return True, f"PBO deployed to work drive"
        
        except Exception as e:
            return False, f"Deployment error: {str(e)}"
    
    def setup_work_drive(self) -> Tuple[bool, str]:
        """
        Set up the work drive mount/symlink.
        
        Returns:
            Tuple of (success, message)
        """
        try:
            self.work_drive_source.mkdir(parents=True, exist_ok=True)
            
            if os.name == 'nt':
                # Windows: Use subst command
                import subprocess
                cmd = ["subst", "P:", str(self.work_drive_source)]
                result = subprocess.run(cmd, capture_output=True)
                
                if result.returncode == 0:
                    return True, "Work drive mounted (P:)"
                else:
                    return False, f"Failed to mount work drive: {result.stderr.decode()}"
            
            else:
                # Linux/macOS: Use symlink
                work_drive = Path("/p")  # Standard Unix mount point
                
                if not work_drive.exists():
                    work_drive.mkdir(parents=True)
                
                link_path = work_drive / "mods"
                if link_path.exists():
                    link_path.unlink()
                
                link_path.symlink_to(self.work_drive_source / "Mods")
                
                return True, "Work drive symlink created (/p/mods)"
        
        except Exception as e:
            return False, f"Work drive setup error: {str(e)}"

```

### 2.3 Build Service Module
**File**: `dayzconfigmaster/build/service.py`

```python
"""
Build service for managing mod builds.

Coordinates the build pipeline:
1. Preflight checks
2. PBO building
3. Signing (if enabled)
4. Deployment to work drive/server profiles
"""

import os
from pathlib import Path
from typing import Dict, List, Optional, Tuple

class BuildService:
    """Manages the complete mod build process."""
    
    def __init__(
        self,
        projects_root: str,
        dayz_tools_path: str,
        key_path: Optional[str] = None
    ):
        """
        Initialize build service.
        
        Args:
            projects_root: Root directory for all projects
            dayz_tools_path: Path to DayZ Tools installation
            key_path: Path to signing key directory (optional)
        """
        self.projects_root = Path(projects_root)
        self.dayz_tools_path = Path(dayz_tools_path)
        self.key_path = Path(key_path) if key_path else None
        
        # Import modules
        from dayzconfigmaster.mods.pbo_builder import PboBuilder
        from dayzconfigmaster.modss.discovery import ModLoader, ModDeployer
        from dayzconfigmaster.build.preflight import PreflightEngine
        
        self.builder = PboBuilder(str(dayz_tools_path))
        self.loader = ModLoader(projects_root)
        self.deployer = ModDeployer("", projects_root)
        self.preflight_engine = PreflightEngine()
    
    def preflight(
        self,
        mod_name: str,
        mod_dir: Optional[str] = None
    ) -> Dict[str, Any]:
        """
        Run preflight checks on a mod project.
        
        Args:
            mod_name: Name of the mod to check
            mod_dir: Path to mod directory (optional)
        
        Returns:
            Preflight report dict with findings and severity
        """
        if not mod_dir:
            mod_dir = self.projects_root / "mods" / mod_name
        
        return self.preflight_engine.run(str(mod_dir))
    
    def build(
        self,
        mod_name: str,
        clean: bool = False,
        binarize: bool = True,
        sign: bool = False,
        force: bool = False
    ) -> Dict[str, Any]:
        """
        Build a mod project.
        
        Args:
            mod_name: Name of the mod to build
            clean: Clear output before building?
            binarize: Binarize configs/models?
            sign: Sign with creator key?
            force: Force rebuild even if unchanged?
        
        Returns:
            Build result dict with success/failure and messages
        """
        mod_dir = self.projects_root / "mods" / mod_name
        
        if not mod_dir.exists():
            return {
                "success": False,
                "error": f"Mod directory not found: {mod_dir}"
            }
        
        # Run preflight (errors block build)
        preflight_report = self.preflight(mod_name, str(mod_dir))
        
        if not preflight_report.get("ok", True):
            return {
                "success": False,
                "error": "Preflight check failed",
                "findings": preflight_report.get("findings", [])
            }
        
        # Determine output path
        build_output = self.projects_root / "build" @f"{mod_name}"
        
        # Determine prefix from $PBOPREFIX$ file
        prefix_file = mod_dir / "$PBOPREFIX$"
        prefix = ""
        if prefix_file.exists():
            prefix = prefix_file.read_text().strip()
        
        # Build the PBO
        success, message = self.builder.pack(
            source_dir=str(mod_dir),
            output_pbo=str(build_output),
            prefix=prefix,
            sign_key=str(self.key_path) if sign and self.key_path else None,
            clean=clean,
            binarize=binarize
        )
        
        result = {
            "success": success,
            "message": message
        }
        
        if success:
            # Deploy to work drive
            pbo_file = str(build_output) + ".pbo"
            self.deployer.deploy_pbo(pbo_file)
            
            result["deployed"] = True
        
        return result
    
    def generate_key(self, key_name: Optional[str] = None) -> Tuple[bool, str]:
        """
        Generate a signing key pair.
        
        Args:
            key_name: Name for the key (defaults to creator name)
        
        Returns:
            Tuple of (success, message)
        """
        if not self.key_path:
            return False, "No key path configured"
        
        # Use DSCreateKey or equivalent
        # For now, just create placeholder structure
        private_key = self.key_path / f"{key_name}.biprivatekey"
        public_key = self.key_path / f"{key_name}.bikey"
        
        if not self.key_path.exists():
            self.key_path.mkdir(parents=True)
        
        # TODO: Implement actual key generation using DSCreateKey
        
        return True, "Keys generated successfully"

```

### 2.4 Preflight Engine Module
**File**: `dayzconfigmaster/build/preflight.py`

```python
"""
Preflight engine for validating mod projects before building.

Checks:
- Config file syntax (CfgPatches, CfgMods)
- Required addons dependencies
- Missing asset references
- Path hygiene (lowercase, no absolute paths)
"""

import os
from pathlib import Path
import re
from typing import Dict, List, Optional, Any

class PreflightResult:
    """Results from a preflight check."""
    
    def __init__(self):
        self.errors: List[Dict[str, Any]] = []
        self.warnings: List[Dict[str, Any]] = []
        self.infos: List[Dict[str, Any]] = []
    
    def add_error(self, rule_id: str, message: str, file: Optional[str] = None, line: int = 0):
        """Add an error finding."""
        self.errors.append({
            "rule": rule_id,
            "message": message,
            "file": file,
            "line": line,
            "severity": "error"
        })
    
    def add_warning(self, rule_id: str, message: str, file: Optional[str] = None, line: int = 0):
        """Add a warning finding."""
        self.warnings.append({
            "rule": rule_id,
            "message": message,
            "file": file,
            "line": line,
            "severity": "warning"
        })
    
    def add_info(self, rule_id: str, message: str, file: Optional[str] = None):
        """Add an info finding."""
        self.infos.append({
            "rule": rule_id,
            "message": message,
            "file": file,
            "severity": "info"
        })
    
    def is_ok(self) -> bool:
        """Check if preflight passed (no errors)."""
        return len(self.errors) == 0
    
    def to_dict(self) -> Dict[str, Any]:
        """Convert to dictionary."""
        return {
            "ok": self.is_ok(),
            "errors": self.errors,
            "warnings": self.warnings,
            "infos": self.infos
        }

class PreflightEngine:
    """Main preflight engine that runs all checks."""
    
    def __init__(self):
        from dayzconfigmaster.build.preflight_rules import ConfigRules, FilesystemRules
        
        self.config_rules = ConfigRules()
        self.filesystem_rules = FilesystemRules()
    
    def run(self, mod_dir: str) -> Dict[str, Any]:
        """
        Run all preflight checks on a mod directory.
        
        Args:
            mod_dir: Path to mod directory
        
        Returns:
            Preflight report dict
        """
        result = PreflightResult()
        
        mod_path = Path(mod_dir)
        
        if not mod_path.exists():
            result.add_error("mod-missing", f"Mod directory does not exist: {mod_dir}")
            return result.to_dict()
        
        # Run config rules
        self.config_rules.check_prefix_file(mod_path, result)
        self.config_rules.check_configs(mod_path, result)
        
        # Run filesystem rules
        self.filesystem_rules.check(mod_path, result)
        
        return result.to_dict()

class ConfigRules:
    """Configuration file validation rules."""
    
    def __init__(self):
        self.required_classes = ["CfgPatches", "CfgMods"]
    
    def check_prefix_file(self, mod_path: Path, result: PreflightResult):
        """Check for $PBOPREFIX$ file and validity."""
        prefix_file = mod_path / "$PBOPREFIX$"
        
        if not prefix_file.exists():
            result.add_info(
                "prefix-missing",
                "No $PBOPREFIX$ file found. PBO name will be used as prefix.",
                str(prefix_file)
            )
            return
        
        try:
            content = prefix_file.read_text().strip()
            
            # Check for drive letters (should not contain absolute paths)
            if re.match(r'^[A-Z]:', content, re.IGNORECASE):
                result.add_warning(
                    "prefix-drive",
                    f"PBO prefix contains drive letter: {content}",
                    str(prefix_file)
                )
            
            # Check for proper format
            if len(content) == 0:
                result.add_warning(
                    "prefix-empty",
                    "$PBOPREFIX$ file is empty",
                    str(prefix_file)
                )
        except Exception as e:
            result.add_error(
                "prefix-read-error",
                f"Failed to read $PBOPREFIX$: {str(e)}",
                str(prefix_file)
            )
    
    def check_configs(self, mod_path: Path, result: PreflightResult):
        """Check config.cpp files for required classes."""
        config_files = list(mod_path.rglob("config.cpp"))
        
        if not config_files:
            result.add_error(
                "config-missing",
                "No config.cpp found in mod directory"
            )
            return
        
        for config_file in config_files:
            self._check_config(config_file, mod_path, result)
    
    def _check_config(self, config_file: Path, mod_path: Path, result: PreflightResult):
        """Check a single config file."""
        try:
            content = config_file.read_text()
            
            # Check for CfgPatches
            if "class CfgPatches" not in content:
                result.add_error(
                    "cfgpatches-missing",
                    "CfgPatches class not found. Required for addon registration.",
                    str(config_file)
                )
                return
            
            # Check for requiredAddons in each patch
            patches = re.findall(r'class\s+([A-Za-z0-9_]+)\s*\{[^}]*}', content, re.IGNORECASE | re.DOTALL)
            
            for patch_name in patches:
                self._check_patch(patch_name, config_file, mod_path, result)
        
        except Exception as e:
            result.add_error(
                "config-read-error",
                f"Failed to read {config_file}: {str(e)}",
                str(config_file)
            )
    
    def _check_patch(self, patch_name: str, config_file: Path, mod_path: Path, result: PreflightResult):
        """Check a CfgPatches entry."""
        # TODO: Implement proper parsing of patches
        pass

class FilesystemRules:
    """File system validation rules."""
    
    def check(self, mod_path: Path, result: PreflightResult):
        """Run filesystem checks."""
        self._check_lowercase(mod_path, result)
        self._check_absolute_paths(mod_path, result)
        self._check_banned_patterns(mod_path, result)
    
    def _check_lowercase(self, mod_path: Path, result: PreflightResult):
        """Check that paths don't contain uppercase letters (DayZ requirement)."""
        for file in mod_path.rglob("*"):
            if file.is_file():
                rel_path = str(file.relative_to(mod_path))
                
                # DayZ requires all lowercase paths
                if any(c.isupper() for c in rel_path):
                    result.add_warning(
                        "uppercase-path",
                        f"Path contains uppercase: {rel_path}",
                        str(file)
                    )
    
    def _check_absolute_paths(self, mod_path: Path, result: PreflightResult):
        """Check that configs don't contain absolute paths."""
        for file in mod_path.rglob("*.cpp"):
            try:
                content = file.read_text()
                
                # Check for Windows-style absolute paths
                if re.search(r'^[A-Z]:\\', content, re.MULTILINE | re.IGNORECASE):
                    result.add_warning(
                        "absolute-path-windows",
                        f"Windows absolute path found in {file}",
                        str(file)
                    )
                
                # Check for Unix-style absolute paths (shouldn't be in config)
                if re.search(r'^/', content, re.MULTILINE):
                    result.add_warning(
                        "absolute-path-unix",
                        f"Unix absolute path found in {file}",
                        str(file)
                    )
            
            except Exception:
                pass
    
    def _check_banned_patterns(self, mod_path: Path, result: PreflightResult):
        """Check for banned file patterns."""
        banned_extensions = [".pyc", ".pyo"]
        
        for ext in banned_extensions:
            for file in mod_path.rglob(f"*{ext}"):
                result.add_info(
                    "banned-file",
                    f"Binary Python file: {file}",
                    str(file)
                )

```

---

## Phase 3: Steam Workshop Integration

### 3.1 steamcmd Wrapper Module
**File**: `dayzconfigmaster/workshop/steamcmd.py`

```python
"""
SteamCMD wrapper for downloading Workshop items.

Handles:
- Downloading Workshop items via steamcmd
- Installing and managing steamcmd
- Anonymous and authenticated downloads
"""

import subprocess
import os
from pathlib import Path
from typing import Optional, List, Tuple

class SteamCmd:
    """Wrapper for SteamCMD."""
    
    def __init__(self, install_path: str):
        """
        Initialize with SteamCMD installation path.
        
        Args:
            install_path: Path to SteamCMD directory
        """
        self.install_path = Path(install_path)
        self.steamcmd_exe = self._find_steamcmd()
    
    def _find_steamcmd(self) -> Optional[Path]:
        """Locate steamcmd executable."""
        if os.name == 'nt':
            possible_names = ["steamcmd.exe", "SteamCMD.exe"]
        else:
            possible_names = ["steamcmd.sh", "steamcmd"]
        
        for name in possible_names:
            exe_path = self.install_path / name
            if exe_path.exists():
                return exe_path
        
        return None
    
    def download_workshop_item(
        self,
        item_id: str,
        install_dir: Optional[str] = None,
        login: Optional[str] = None,
        password: Optional[str] = None
    ) -> Tuple[bool, str]:
        """
        Download a Workshop item using steamcmd.
        
        Args:
            item_id: Workshop item ID (numeric)
            install_dir: Install directory (defaults to SteamApps/workshop/content/221100)
            login: Steam username (optional, for authenticated downloads)
            password: Steam password (optional)
        
        Returns:
            Tuple of (success, message)
        """
        if not self.steamcmd_exe:
            return False, "steamcmd.exe not found"
        
        # Build command
        cmd = [str(self.steamcmd_exe)]
        
        # Login (anonymous if no credentials provided)
        if login:
            cmd.extend(["+login", login])
        else:
            cmd.extend(["+login", "anonymous"])
        
        # Install directory
        if install_dir:
            cmd.extend(["+force_install_dir", str(Path(install_dir).resolve())])
        
        # Download item
        cmd.extend([
            "+workshop_download_item",
            "221100",  # DayZ app ID
            str(item_id),
            "+quit"
        ])
        
        try:
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=600  # 10 minute timeout for downloads
            )
            
            if "Success" in result.stdout or result.returncode == 0:
                return True, f"Downloaded item {item_id}"
            
            error_msg = result.stderr.strip() or result.stdout.strip()
            return False, f"Download failed: {error_msg}"
        
        except subprocess.TimeoutExpired:
            return False, "Download timed out"
        except Exception as e:
            return False, f"Download error: {str(e)}"
    
    def update_workshop_item(
        self,
        item_id: str,
        install_dir: Optional[str] = None
    ) -> Tuple[bool, str]:
        """Update a Workshop item to latest version."""
        # SteamCMD automatically updates on download
        return self.download_workshop_item(item_id, install_dir)

```

### 3.2 Workshop Browser Module
**File**: `dayzconfigmaster/workshop/browser.py`

```python
"""
Workshop browser for searching and downloading community mods.

Uses:
- Web scraping of Steam Community browse page (no API key required)
- GetPublishedFileDetails endpoint for detailed item info
"""

import requests
from pathlib import Path
from typing import Dict, List, Optional, Any

class WorkshopItem:
    """Information about a Workshop item."""
    
    def __init__(
        self,
        id: str,
        title: str,
        description: str = "",
        preview_url: str = "",
        subscribers: int = 0,
        created: int = 0,
        updated: int = 0,
        tags: List[str] = None,
        file_size: int = 0
    ):
        self.id = id
        self.title = title
        self.description = description
        self.preview_url = preview_url
        self.subscribers = subscribers
        self.created = created
        self.updated = updated
        self.tags = tags or []
        self.file_size = file_size
    
    def to_dict(self) -> Dict[str, Any]:
        """Convert to dictionary."""
        return {
            "id": self.id,
            "title": self.title,
            "description": self.description,
            "preview_url": self.preview_url,
            "subscribers": self.subscribers,
            "created": self.created,
            "updated": self.updated,
            "tags": self.tags,
            "file_size": self.file_size
        }

class WorkshopBrowser:
    """Browse and search DayZ Workshop items."""
    
    BASE_URL = "https://steamcommunity.com/workshop/browse"
    APP_ID = "221100"  # DayZ
    
    def __init__(self):
        self.session = requests.Session()
        self.session.headers.update({
            "User-Agent": "Mozilla/5.0 (DayZConfigMaster)"
        })
    
    def browse(
        self,
        search_text: str = "",
        sort_by: str = "trend",
        time_frame: int = -1,  # -1 = all time
        page: int = 1,
        count: int = 20,
        tags: List[str] = None
    ) -> Tuple[bool, List[WorkshopItem]]:
        """
        Browse Workshop items.
        
        Args:
            search_text: Search query
            sort_by: Sort method (trend, toprated, mostrecent)
            time_frame: Time frame in days (-1 for all time)
            page: Page number
            count: Items per page
            tags: Filter by tags
        
        Returns:
            Tuple of (success, list of items)
        """
        # Build URL
        url = f"{self.BASE_URL}/?appid={self.APP_ID}&section=readytouseitems"
        
        if sort_by == "trend" and time_frame != -1:
            url += f"&days={time_frame}"
        
        if search_text:
            url += f"&searchtext={requests.utils.quote(search_text)}"
        
        url += f"&p={page}"
        
        if tags:
            for tag in tags:
                url += f"&requiredtags%5B%5D={requests.utils.quote(tag)}"
        
        try:
            response = self.session.get(url, timeout=30)
            
            if response.status_code != 200:
                return False, []
            
            items = self._parse_browse_page(response.text)
            return True, items
        
        except Exception as e:
            print(f"Browse error: {e}")
            return False, []
    
    def _parse_browse_page(self, html: str) -> List[WorkshopItem]:
        """Parse browse page HTML for item IDs and titles."""
        import re
        
        # Pattern to find Workshop items
        pattern = r'filedetails/\?id=(\d+)".*?<img\s+src="([^"]+)".*?alt="([^"]+)"'
        
        matches = re.findall(pattern, html, re.DOTALL | re.IGNORECASE)
        
        items = []
        
        for match in matches:
            item_id, preview_url, title = match
            
            # Fetch detailed info for this item
            details = self.get_details(item_id)
            
            if details:
                items.append(details)
            else:
                # Fallback to basic info
                items.append(WorkshopItem(
                    id=item_id,
                    title=title.strip(),
                    preview_url=preview_url
                ))
        
        return items
    
    def get_details(self, item_id: str) -> Optional[WorkshopItem]:
        """Get detailed information about a Workshop item."""
        # Use the keyless GetPublishedFileDetails endpoint
        url = "https://api.steamcommunity.com/ISteamRemoteStorage/GetPublishedFileDetails"
        
        data = {
            "items": [{"publishedfileid": str(item_id)}]
        }
        
        try:
            response = self.session.post(url, json=data, timeout=30)
            
            if response.status_code != 200:
                return None
            
            result = response.json()
            
            if "response" not in result or "publishedfiledetails" not in result["response"]:
                return None
            
            details = result["response"]["publishedfiledetails"][0]
            
            # Parse tags
            tags = []
            for tag_data in details.get("tags", []):
                if isinstance(tag_data, dict) and "tag" in tag_data:
                    tags.append(tag_data["tag"])
            
            return WorkshopItem(
                id=str(details.get("publishedfileid", item_id)),
                title=details.get("title", ""),
                description=details.get("file_description", "")[:500],  # Limit to 500 chars
                preview_url=details.get("preview_url", ""),
                subscribers=int(details.get("subscriptions", 0)),
                created=int(details.get("time_created", 0)),
                updated=int(details.get("time_updated", 0)),
                tags=tags,
                file_size=int(details.get("file_size", 0))
            )
        
        except Exception as e:
            print(f"Get details error: {e}")
            return None
    
    def subscribe(self, item_id: str) -> Tuple[bool, str]:
        """
        Subscribe to a Workshop item.
        
        Note: This requires a Steam Web API access token for true API access.
        For now, opens the workshop page in browser.
        
        Args:
            item_id: Workshop item ID
        
        Returns:
            Tuple of (success, message)
        """
        # Open in browser - user can subscribe there
        import webbrowser
        webbrowser.open(f"https://steamcommunity.com/sharedfiles/filedetails/?id={item_id}")
        
        return True, "Opened workshop page"

```

---

## Phase 4: Git Integration

### 4.1 Git Manager Module
**File**: `dayzconfigmaster/references/git/manager.py`

```python
"""
Git manager for DayZConfigMaster.

Uses GitPython library to provide Git integration.
"""

import os
from pathlib import Path
from typing import Dict, List, Optional, Any

class GitManager:
    """Manages Git operations for mod projects."""
    
    def __init__(self, projects_root: str):
        """
        Initialize with projects root path.
        
        Args:
            projects_root: Root directory containing mod projects
        """
        self.projects_root = Path(projects_root)
    
    def _get_repo(self, mod_name: str) -> Optional["git.Repo"]:
        """Get GitPython Repo object for a mod."""
        try:
            import git
            
            mod_dir = self.projects_root / "mods" / mod_name
            if not mod_dir.exists():
                return None
            
            repo_path = mod_dir
            return git.Repo(repo_path)
        
        except ImportError:
            print("GitPython not installed. Install with: pip install GitPython")
            return None
        
        except Exception as e:
            print(f"Error getting repo for {mod_name}: {e}")
            return None
    
    def init(self, mod_name: str) -> Tuple[bool, str]:
        """Initialize a new Git repository."""
        try:
            import git
            
            mod_dir = self.projects_root / "mods" / mod_name
            if not mod_dir.exists():
                return False, f"Mod directory not found: {mod_dir}"
            
            if (mod_dir / ".git").exists():
                return False, "Repository already initialized"
            
            # Initialize repository
            repo = git.Repo.init(mod_dir)
            
            # Create .gitignore for DayZ mods
            gitignore_content = """
# Python cache
__pycache__/
*.py[cod]
*$py.class

# IDE files
.idea/
.vscode/
*.swp
*.swo

# Build outputs
*.pbo
*.pbi
*.bikinfi

# Private keys (NEVER commit these!)
*.biprivatekey
*.key

# Logs
*.log
server.log
DayZDiag_x64.log

# OS files
.DS_Store
Thumbs.db
"""
            
            gitignore_path = mod_dir / ".gitignore"
            gitignore_path.write_text(gitignore_content)
            
            # Initial commit
            repo.index.add(["."])
            repo.index.commit("Initial commit")
            
            return True, "Repository initialized"
        
        except ImportError:
            return False, "GitPython not installed. Install with: pip install GitPython"
        
        except Exception as e:
            return False, f"Failed to initialize repository: {str(e)}"
    
    def status(self, mod_name: str) -> Dict[str, Any]:
        """Get Git status for a mod."""
        repo = self._get_repo(mod_name)
        if not repo:
            return {"error": "Repository not found"}
        
        try:
            import git
            
            # Get diff stats
            staged = [d.a_path for d in repo.index.diff(repo.head.commit)]
            unstaged = repo.git.diff(name_only=True).splitlines()
            untracked = repo.untracked_files
            
            # Get branch info
            branch = repo.active_branch.name if not repo.head.is_detached else "detached"
            
            # Check if ahead/behind remote
            try:
                origin = repo.remotes.origin
                ahead = len(list(repo.iter_commits(f"origin/{branch}..{branch}")))
                behind = len(list(repo.iter_commits(f"{branch}..origin/{branch}")))
                
                has_remote = True
            except Exception:
                ahead = 0
                behind = 0
                has_remote = False
            
            return {
                "ok": True,
                "branch": branch,
                "ahead": ahead,
                "behind": behind,
                "dirty": bool(staged or unstaged or untracked),
                "has_remote": has_remote,
                "staged": staged,
                "unstaged": unstaged,
                "untracked": untracked
            }
        
        except Exception as e:
            return {"ok": False, "error": str(e)}
    
    def commit(self, mod_name: str, message: str, stage_all: bool = True) -> Tuple[bool, str]:
        """Commit changes to the repository."""
        repo = self._get_repo(mod_name)
        if not repo:
            return False, f"Repository not found for {mod_name}"
        
        try:
            import git
            
            if stage_all:
                repo.index.add(["."])
            
            # Create commit
            repo.index.commit(message)
            
            return True, "Changes committed"
        
        except Exception as e:
            return False, f"Commit failed: {str(e)}"
    
    def push(self, mod_name: str) -> Tuple[bool, str]:
        """Push changes to remote repository."""
        repo = self._get_repo(mod_name)
        if not repo:
            return False, f"Repository not found for {mod_name}"
        
        try:
            # Find remote
            origin = None
            for remote in repo.remotes:
                if remote.name == "origin":
                    origin = remote
                    break
            
            if not origin:
                return False, "No remote configured"
            
            # Push to remote
            origin.push()
            
            return True, "Pushed to remote"
        
        except Exception as e:
            return False, f"Push failed: {str(e)}"
    
    def pull(self, mod_name: str) -> Tuple[bool, str]:
        """Pull changes from remote repository."""
        repo = self._get_repo(mod_name)
        if not repo:
            return False, f"Repository not found for {mod_name}"
        
        try:
            origin = None
            for remote in repo.remotes:
                if remote.name == "origin":
                    origin = remote
                    break
            
            if not origin:
                return False, "No remote configured"
            
            origin.pull()
            
            return True, "Pulled from remote"
        
        except Exception as e:
            return False, f"Pull failed: {str(e)}"
    
    def create_branch(self, mod_name: str, branch_name: str) -> Tuple[bool, str]:
        """Create a new branch."""
        repo = self._get_repo(mod_name)
        if not repo:
            return False, f"Repository not found for {mod_name}"
        
        try:
            import git
            
            # Create and checkout branch
            repo.create_head(branch_name)
            
            return True, f"Branch '{branch_name}' created"
        
        except Exception as e:
            return False, f"Failed to create branch: {str(e)}"
    
    def list_branches(self, mod_name: str) -> List[str]:
        """List all branches for a repository."""
        repo = self._get_repo(mod_name)
        if not repo:
            return []
        
        try:
            return [b.name for b in repo.branches]
        
        except Exception as e:
            print(f"Error listing branches: {e}")
            return []

```

---

## Phase 5: Log Management System

### 5.1 Log Tailer Module
**File**: `dayzconfigmaster/logs/tailer.py`

```python
"""
Log tailing module for real-time log monitoring.

Provides:
- Real-time log file watching
- Multiple log type support (script, rpt, adm, client)
- Auto-scroll and display management
"""

import os
import time
from pathlib import Path
from typing import List, Optional

class LogTailer:
    """Tails DayZ server logs in real-time."""
    
    def __init__(self, profiles_path: str):
        """
        Initialize with profiles directory path.
        
        Args:
            profiles_path: Path to DayZ profiles directory
        """
        self.profiles_path = Path(profiles_path)
        
        # Log file paths by type
        self.log_types = {
            "script": "DayZDiag_x64_script.log",
            "rpt": "DayZDiag_x64_rpt.log",  # Extended report log
            "adm": "admin.log",
            "client": "DayZDiag_x64_client.log"
        }
    
    def get_log_path(self, log_type: str) -> Optional[Path]:
        """Get path to log file for a given type."""
        if log_type not in self.log_types:
            return None
        
        # Try to find the profile directory
        if self.profiles_path.exists():
            # Look for most recent profile directory
            profiles = [p for p in self.profiles_path.iterdir() if p.is_dir()]
            
            if profiles:
                return profiles[0] / self.log_types[log_type]
        
        # Fallback to standard DayZ paths
        possible_paths = [
            Path(os.environ.get("HOME", "~")) / ".dayz" / self.log_types[log_type],
            Path.home() / "DayZ" / self.log_types[log_type],
        ]
        
        for path in possible_paths:
            if path.exists():
                return path
        
        return None
    
    def tail(
        self,
        log_type: str,
        lines: int = 50
    ) -> List[str]:
        """
        Get the last N lines of a log file.
        
        Args:
            log_type: Type of log (script, rpt, adm, client)
            lines: Number of lines to return
        
        Returns:
            Last N lines of the log
        """
        path = self.get_log_path(log_type)
        
        if not path or not path.exists():
            return [f"Log file not found for {log_type}"]
        
        try:
            with open(path, 'r', encoding='utf-8', errors='ignore') as f:
                # Read all lines
                all_lines = f.readlines()
                
                # Return last N lines
                return [line.rstrip() for line in all_lines[-lines:]]
        
        except Exception as e:
            return [f"Error reading log: {str(e)}"]
    
    def watch(
        self,
        log_type: str,
        callback,
        update_interval: float = 1.0
    ):
        """
        Watch a log file and call callback on new lines.
        
        This is intended to be run in a background thread.
        
        Args:
            log_type: Type of log to watch
            callback: Function to call with new lines
            update_interval: Poll interval in seconds
        """
        path = self.get_log_path(log_type)
        
        if not path or not path.exists():
            callback([f"Log file not found for {log_type}"])
            return
        
        try:
            # Get initial line count
            with open(path, 'r') as f:
                lines = f.readlines()
            
            while True:  # Loop until stopped
                time.sleep(update_interval)
                
                # Check if file was modified
                current_size = os.path.getsize(path)
                
                with open(path, 'r') as f:
                    new_lines = f.readlines()
                
                # Get new lines since last check
                if len(new_lines) > len(lines):
                    new_content = new_lines[len(lines):]
                    callback([line.rstrip() for line in new_content])
                    lines = new_lines
        
        except Exception as e:
            callback([f"Error watching log: {str(e)}"])

```

### 5.2 Diagnostics Engine Module
**File**: `dayzconfigmaster/logs/diagnostics.py`

```python
"""
Diagnostics engine for analyzing DayZ logs.

Identifies common failure patterns and suggests fixes.
"""

import re
from typing import Dict, List, Optional

class DiagnosticRule:
    """A diagnostic rule with pattern matching."""
    
    def __init__(
        self,
        id: str,
        name: str,
        patterns: List[str],
        cause: str,
        fix: str,
        severity: str = "warning"
    ):
        """
        Initialize a diagnostic rule.
        
        Args:
            id: Unique rule ID
            name: Human-readable name
            patterns: Regex patterns to match in logs
            cause: Description of the problem
            fix: Suggested solution
            severity: error|warning|info
        """
        self.id = id
        self.name = name
        self.patterns = [re.compile(p, re.IGNORECASE) for p in patterns]
        self.cause = cause
        self.fix = fix
        self.severity = severity
    
    def check(self, log_content: str) -> Optional[Dict[str, str]]:
        """Check if rule matches the log content."""
        for pattern in self.patterns:
            if pattern.search(log_content):
                return {
                    "rule": self.id,
                    "name": self.name,
                    "cause": self.cause,
                    "fix": self.fix,
                    "severity": self.severity
                }
        
        return None

class DiagnosticsEngine:
    """Engine for running diagnostic rules on logs."""
    
    def __init__(self):
        # Define all diagnostic rules
        self.rules = [
            DiagnosticRule(
                id="verification-missing-bisign",
                name="Missing .bisign File",
                patterns=[
                    r"VE_MISSING_BISIGN.*?([A-Za-z0-9_./]+)",
                    r"File ([A-Za-z0-9_/.]+) missing bisignature"
                ],
                cause="PBO is signed but .bisign file is missing",
                fix="Re-sign the PBO with your creator key"
            ),
            
            DiagnosticRule(
                id="verification-patched-pbo",
                name="Modified/Unsigned PBO",
                patterns=[
                    r"VE_PATCHED_PBO.*?([A-Za-z0-9_./]+)",
                    r"File ([A-Za-z0-9_/.]+) was modified after signing"
                ],
                cause="PBO content was changed after signing",
                fix="Rebuild and re-sign the PBO without editing config.bin"
            ),
            
            DiagnosticRule(
                id="mod-version-skew",
                name="Mod Version Mismatch",
                patterns=[
                    r"version mismatch for mod ([A-Za-z0-9_]+)",
                    r"Different versions of mod ([A-Za-z0-9_]+)"
                ],
                cause="Different clients have different mod versions",
                fix="Ensure all players use the same mod version"
            ),
            
            DiagnosticRule(
                id="file-patching-mismatch",
                name="File Patching Mismatch",
                patterns=[
                    r"FilePatching mismatch.*?([A-Za-z0-9_./]+)",
                    r"File ([A-Za-z0-9_/.]+) not found at expected path"
                ],
                cause="-filePatching flag mismatch between server and client",
                fix="Ensure -filePatching is enabled in both server and client launch parameters"
            ),
            
            DiagnosticRule(
                id="addon-builder-failed",
                name="AddonBuilder Failure",
                patterns=[
                    r"AddonBuilder.*failed",
                    r"Error building PBO",
                    r"Unable to create archive"
                ],
                cause="Addo",