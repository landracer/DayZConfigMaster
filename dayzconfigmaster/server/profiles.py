# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Profile manager for DayZ server configurations

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
    ) -> tuple:
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
        from datetime import datetime
        
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
    
    def delete_profile(self, name: str) -> tuple:
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
        """Get currently active profile from JSON state file."""
        import json
        state_file = self.projects_root / ".active_profile.json"
        if not state_file.exists():
            return None
        try:
            with open(state_file, 'r') as f:
                data = json.load(f)
                return data.get("active_profile")
        except (json.JSONDecodeError, OSError):
            return None
    
    def set_active_profile(self, name: str) -> tuple:
        """Set the active profile. Validates name exists in profiles directory."""
        import json
        if not self.profiles_dir.exists():
            return False, "Profiles directory does not exist"
        
        # Check if profile exists
        profile_file = self.profiles_dir / f"{name}.json"
        if not profile_file.exists():
            return False, f"Profile '{name}' does not exist"
        
        # Save active state to JSON file
        try:
            with open(self.projects_root / ".active_profile.json", 'w') as f:
                json.dump({"active_profile": name}, f, indent=2)
            return True, f"Active profile set to '{name}'"
        except OSError as e:
            return False, f"Failed to save active profile: {e}"
