# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Setup wizard for DayZConfigMaster first-time configuration

"""
Setup Wizard - First-time setup experience for DayZConfigMaster.

Features:
- Detect DayZ installation path
- Detect DayZ Tools installation path
- Mount work drive (P: on Windows)
- Extract vanilla game data if needed
- Create default server instance
- Collect project folder location
"""

import os
import sys
import json
from pathlib import Path
from typing import Optional, Dict, Any


class SetupWizard:
    """
    Guide users through first-time configuration of DayZConfigMaster.
    
    This wizard:
    1. Detects existing DayZ/Tools installations
    2. Sets up work drive mounting
    3. Creates initial project structure
    4. Saves configuration for future use
    """
    
    def __init__(self, projects_root: str = None):
        """
        Initialize the setup wizard.
        
        Args:
            projects_root: Root directory for all DayZ projects
        """
        self.projects_root = Path(projects_root or ".")
        self.config_path = self.projects_root / "config.json"
        self.settings: Dict[str, Any] = {}
        self.is_windows = os.name == 'nt'
    
    def detect_dayz_installation(self) -> Optional[str]:
        """
        Detect DayZ installation path.
        
        Checks common locations:
        - Steam library (common for Windows)
        - Manual installations
        
        Returns:
            Path to DayZ installation or None if not found
        """
        possible_paths = []
        
        if self.is_windows:
            # Common Steam library locations
            steam_dirs = [
                Path(r"C:\Program Files\Steam\steamapps\common"),
                Path(r"C:\Program Files (x86)\Steam\steamapps\common"),
                Path.home() / "AppData" / "Local" / "Steam" / "steamapps" / "common",
            ]
            
            for steam_dir in steam_dirs:
                if steam_dir.exists():
                    dayz_paths = [
                        steam_dir / "DayZ",
                        steam_dir / "dayz",
                        steam_dir / "DayZ Dev",
                    ]
                    possible_paths.extend(dayz_paths)
        
        else:
            # Linux/macOS Steam locations
            steam_common = Path.home() / ".steam" / "steam" / "steamapps" / "common"
            if steam_common.exists():
                dayz_paths = [
                    steam_common / "DayZServer",  # Most common for DayZ server installation
                    steam_common / "dayz",
                    steam_common / "DayZ",
                ]
                possible_paths.extend(dayz_paths)
                
                # Also check DayZ Tools (with space in name on Linux)
                tools_paths = [
                    steam_common / "DayZ Tools",  # Space-separated path on Linux
                    steam_common / "dayz-tools",
                    steam_common / "DayZ-Tools",
                ]
                possible_paths.extend(tools_paths)
        
        # Check actual paths
        for path in possible_paths:
            if self._is_valid_dayz_install(path):
                return str(path)
        
        return None
    
    def detect_dayz_tools(self) -> Optional[str]:
        """
        Detect DayZ Tools installation path.
        
        Returns:
            Path to DayZ Tools or None if not found
        """
        possible_paths = []
        
        # First try Linux paths (common locations on Linux systems)
        steam_common = Path.home() / ".steam" / "steam" / "steamapps" / "common"
        if steam_common.exists():
            tools_paths = [
                steam_common / "DayZ Tools",   # Space-separated path
                steam_common / "dayz-tools",
                steam_common / "DayZ-Tools",
                steam_common / "DayZTools",
            ]
            possible_paths.extend(tools_paths)
        
        if self.is_windows:
            # Common Steam locations on Windows
            steam_dirs = [
                Path(r"C:\Program Files\Steam\steamapps\common"),
                Path(r"C:\Program Files (x86)\Steam\steamapps\common"),
            ]
            
            for steam_dir in steam_dirs:
                if steam_dir.exists():
                    tools_paths = [
                        steam_dir / "DayZ Tools",
                        steam_dir / "dayz-tools",
                        steam_dir / "DayZ-Tools",
                    ]
                    possible_paths.extend(tools_paths)
        
        # Check actual paths - look for key tool folders
        for path in possible_paths:
            bin_path = Path(path) / "Bin"
            if not bin_path.exists():
                continue
            
            # Check for AddonBuilder folder (key indicator of DayZ Tools)
            # On Linux, tools are in subfolders inside Bin/
            addon_builder = bin_path / "AddonBuilder"
            if addon_builder.exists() and addon_builder.is_dir():
                return str(path)
        
        return None
    
    def _is_valid_dayz_install(self, path: Path) -> bool:
        """
        Check if a path is a valid DayZ installation.
        
        Args:
            path: Path to check
            
        Returns:
            True if valid installation
        """
        if not path.exists():
            return False
        
        # For Linux/Unix systems - check for server executables and key folders
        if self.is_windows:
            # Windows: look for dayz.exe or DayZ.exe
            required = ["db", "bin", "dayz.exe"]
            for item in required:
                item_path = path / item
                if not (item_path.exists() or any(path.glob(f"*{item}"))):
                    return False
        else:
            # Linux/macOS: look for DayZServer executable and key folders
            has_server_exe = (path / "DayZServer").exists()
            has_dta_folder = (path / "dta").exists()
            has_mpmissions = (path / "mpmissions").exists()
            
            if not (has_server_exe and has_dta_folder and has_mpmissions):
                return False
        
        return True
    
    def detect_steam_workshop_path(self) -> Optional[str]:
        """
        Detect Steam Workshop content path for DayZ.
        
        Returns:
            Path to workshop content directory or None if not found
        """
        possible_paths = []
        
        # Common Steam library locations (Linux and Windows)
        steam_dirs = [
            Path.home() / ".steam" / "steam" / "steamapps",
            Path.home() / ".local" / "share" / "Steam" / "steamapps",
        ]
        
        if self.is_windows:
            steam_dirs.extend([
                Path(r"C:\Program Files\Steam\steamapps"),
                Path(r"C:\Program Files (x86)\Steam\steamapps"),
                Path.home() / "AppData" / "Local" / "Steam" / "steamapps",
            ])
        
        for steam_dir in steam_dirs:
            if steam_dir.exists():
                # DayZ app ID is 221100
                workshop_path = steam_dir / "workshop" / "content" / "221100"
                if workshop_path.exists() and workshop_path.is_dir():
                    return str(workshop_path)
        
        return None
    
    def mount_work_drive(self, target_path: str) -> bool:
        """
        Mount the work drive.
        
        Args:
            target_path: Path to mount as work drive
            
        Returns:
            True if successful
        """
        try:
            from ..workdrive.mounter import WorkDrive
            workdrive = WorkDrive(str(self.projects_root))
            
            # Unmount first if already mounted
            workdrive.unmount_work_drive()
            
            result = workdrive.mount_work_drive(target_path)
            return result.success
        
        except Exception as e:
            print(f"Work drive mount error: {e}")
            return False
    
    def create_default_instance(self) -> bool:
        """
        Create a default server instance.
        
        Returns:
            True if successful
        """
        try:
            from ..server.instance_manager import InstanceManager
            
            manager = InstanceManager(str(self.projects_root))
            
            # Check if default instance already exists
            existing = manager.get_instance("default")
            if existing:
                return True  # Already configured
            
            success, message = manager.create_instance(
                name="default",
                port=2302
            )
            
            print(f"Default instance: {message}")
            return success
        
        except Exception as e:
            print(f"Failed to create default instance: {e}")
            return False
    
    def extract_vanilla_data(self, dayz_path: str) -> bool:
        """
        Extract vanilla game data from DayZ installation.
        
        Args:
            dayz_path: Path to DayZ installation
            
        Returns:
            True if successful
        """
        try:
            # For now, just record the path
            # Full extraction would require more complex implementation
            print(f"Vanilla data path: {dayz_path}")
            return True
        
        except Exception as e:
            print(f"Failed to process vanilla data: {e}")
            return False
    
    def save_config(self) -> bool:
        """
        Save configuration to config.json.
        
        Returns:
            True if successful
        """
        try:
            # Create parent directories if needed
            self.projects_root.mkdir(parents=True, exist_ok=True)
            
            # Save settings
            with open(self.config_path, 'w') as f:
                json.dump(self.settings, f, indent=2)
            
            return True
        
        except Exception as e:
            print(f"Failed to save config: {e}")
            return False
    
    def default_workdrive_path(self) -> str:
        """
        Return the OS-appropriate default work drive location.

        Windows historically uses the ``P:`` drive letter; other platforms use a
        ``work`` folder under the projects root.
        """
        if self.is_windows:
            return "P:"
        return str(self.projects_root / "work")

    def apply_os_defaults(self) -> Dict[str, Any]:
        """
        Populate ``self.settings`` with sensible OS-based defaults.

        Auto-detects DayZ / DayZ Tools installations where possible and falls
        back to empty strings (never prompts). The user can change any of these
        later from the Preferences dialog.

        Returns:
            The resolved settings dictionary.
        """
        self.settings.setdefault('projects_root', str(self.projects_root))
        self.settings.setdefault('dayz_path', self.detect_dayz_installation() or "")
        self.settings.setdefault('dayz_tools_path', self.detect_dayz_tools() or "")
        self.settings.setdefault('workdrive_path', self.default_workdrive_path())
        self.settings.setdefault('steam_api_key', os.environ.get('STEAM_API_KEY', ''))
        # Auto-detect Steam workshop content path for DayZ (app ID 221100)
        self.settings.setdefault('steam_workshop_dir', self.detect_steam_workshop_path() or "")
        # Preserve theme setting if it exists
        return self.settings

    def run_automatic(self) -> bool:
        """
        Configure DayZConfigMaster non-interactively using OS-based defaults.

        This never blocks on CLI input so the GUI can open immediately. Missing
        installation paths are simply left blank for the user to fill in later
        from the Preferences dialog.

        Returns:
            True once defaults are written (always succeeds; individual
            best-effort steps may warn but never block startup).
        """
        self.apply_os_defaults()

        # Best-effort default instance; failure must not block startup.
        try:
            self.create_default_instance()
        except Exception as exc:  # noqa: BLE001 - startup must never crash here
            print(f"Note: could not create default instance ({exc})")

        if not self.save_config():
            print("Warning: could not save configuration; using in-memory defaults.")

        return True

    def run_interactive(self) -> bool:
        """
        Run the setup wizard interactively.
        
        Returns:
            True if setup completed successfully
        """
        print("=" * 60)
        print("DayZConfigMaster Setup Wizard")
        print("=" * 60)
        print()
        
        # Get project root
        default_projects = str(Path.home() / "Documents" / "DayZProjects")
        projects_root = input(
            f"Project folder location [{default_projects}]: "
        ).strip() or default_projects
        
        self.projects_root = Path(projects_root)
        self.settings['projects_root'] = projects_root
        print()
        
        # Detect DayZ
        print("Detecting DayZ installation...")
        dayz_path = self.detect_dayz_installation()
        
        if dayz_path:
            print(f"Found: {dayz_path}")
            use_detected = input("Use this path? (Y/n): ").strip().lower()
            if use_detected != 'n':
                self.settings['dayz_path'] = dayz_path
                print()
            else:
                custom_dayz = input("Enter DayZ path: ").strip()
                if custom_dayz:
                    self.settings['dayz_path'] = custom_dayz
        else:
            custom_dayz = input("DayZ installation not found. Enter path: ").strip()
            if custom_dayz:
                self.settings['dayz_path'] = custom_dayz
        
        print()
        
        # Detect DayZ Tools
        print("Detecting DayZ Tools...")
        tools_path = self.detect_dayz_tools()
        
        if tools_path:
            print(f"Found: {tools_path}")
            use_detected = input("Use this path? (Y/n): ").strip().lower()
            if use_detected != 'n':
                self.settings['dayz_tools_path'] = tools_path
                print()
            else:
                custom_tools = input("Enter DayZ Tools path: ").strip()
                if custom_tools:
                    self.settings['dayz_tools_path'] = custom_tools
        else:
            custom_tools = input("DayZ Tools not found. Enter path: ").strip()
            if custom_tools:
                self.settings['dayz_tools_path'] = custom_tools
        
        print()
        
        # Mount work drive
        workdrive_path = input(
            "Work drive mount point (P: on Windows, folder on Linux): "
        ).strip() or str(self.projects_root / "work")
        
        if self.mount_work_drive(workdrive_path):
            print(f"Mounted work drive to: {workdrive_path}")
        else:
            print("Warning: Work drive mount failed. Some features may not work.")
        
        print()
        
        # Create default instance
        print("Creating default server instance...")
        if self.create_default_instance():
            print("Default instance created successfully")
        else:
            print("Warning: Could not create default instance")
        
        print()
        
        # Save configuration
        if self.save_config():
            print(f"Configuration saved to: {self.config_path}")
        else:
            print("Warning: Could not save configuration")
        
        print()
        print("=" * 60)
        print("Setup complete!")
        print("=" * 60)
        
        return True
    
    def get_settings(self) -> Dict[str, Any]:
        """
        Get the current settings.
        
        Returns:
            Dictionary of settings
        """
        if self.config_path.exists():
            with open(self.config_path, 'r') as f:
                return json.load(f)
        return {}
    
    def is_setup_complete(self) -> bool:
        """
        Check if setup has been completed.
        
        Returns:
            True if config.json exists and has required settings
        """
        if not self.config_path.exists():
            return False
        
        try:
            with open(self.config_path, 'r') as f:
                config = json.load(f)
            
            # Check for required settings
            return 'dayz_path' in config or 'projects_root' in config
        
        except Exception:
            return False


def run_setup(projects_root: str = None) -> bool:
    """
    Convenience function to run setup.
    
    Args:
        projects_root: Root directory for projects (optional)
        
    Returns:
        True if setup completed
    """
    wizard = SetupWizard(projects_root)
    return wizard.run_interactive()


if __name__ == "__main__":
    import sys
    
    # Check if already set up
    wizard = SetupWizard()
    
    if wizard.is_setup_complete():
        print("Setup has already been completed.")
        response = input("Run setup again? (y/N): ").strip().lower()
        if response != 'y':
            print("Keeping existing configuration.")
            sys.exit(0)
    
    success = run_setup()
    sys.exit(0 if success else 1)