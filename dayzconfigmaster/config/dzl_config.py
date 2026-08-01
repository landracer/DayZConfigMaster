# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# DzlConfig class for DayZ server configuration composition
# Matches dayz-labs Profiles.ResolveActive pattern

"""
DzlConfig - Config composition system that merges global config with instance-specific settings.

This provides the same functionality as dayz-labs Profiles.ResolveActive:
- Global config.json (shared across all instances)
- Instance-specific config files
- Automatic merge with proper precedence (instance overrides global)
"""

import json
from pathlib import Path
from typing import Dict, Any, Optional, List


class DzlConfig:
    """
    Configuration manager that merges global and instance-specific settings.
    
    The configuration hierarchy is:
    1. Global defaults (built-in)
    2. Global config.json (projects_root/config.json)
    3. Instance-specific settings (projects_root/instances/{instance_name}.json)
    
    Precedence: Instance > Global > Defaults
    """
    
    # Default configuration values
    DEFAULT_CONFIG = {
        "dayz_path": "",
        "dayz_tools_path": "",
        "work_drive": "",
        "steamcmd_path": "",
        "signing_key_path": "",
        "verify_signatures": 2,
        "force_same_build": 1,
        "max_players": 60,
        "port": 2302,
        "query_port": 2303,
        "map_size": 2000,
        "game_mode": "Survival",
        "time_settings": "",
        "weather": 0.5,
        "fog_density": 0.5,
        "rain_intensity": 0.8,
        "wind_speed": 5.0,
        "temperature": 20.0,
        "damage_multiplier": 1.0,
        "hp_multiplier": 1.0,
        "loot_multiplier": 1.0,
        "zombie_multiplier": 1.0,
        "vehicle_multiplier": 1.0,
        "item_decay_time": 3600,
        "mod_paths": [],
        "auto_update": True,
        "build_cache_enabled": True,
        "console_visible": False
    }
    
    def __init__(self, projects_root: str):
        """
        Initialize configuration manager.
        
        Args:
            projects_root: Root directory containing config.json and instances/
        """
        self.projects_root = Path(projects_root)
        self.global_config_path = self.projects_root / "config.json"
        self.instances_dir = self.projects_root / "instances"
        
        # Load global config
        self._global_config = None
        self._load_global_config()
    
    def _load_global_config(self):
        """Load global configuration from config.json."""
        if not self.global_config_path.exists():
            self._global_config = {}
            return
        
        try:
            with open(self.global_config_path, 'r') as f:
                self._global_config = json.load(f)
        except (json.JSONDecodeError, IOError) as e:
            print(f"Warning: Could not load global config: {e}")
            self._global_config = {}
    
    def get_global_defaults(self) -> Dict[str, Any]:
        """Get the built-in default configuration values."""
        return dict(self.DEFAULT_CONFIG)
    
    def get_global_config(self) -> Dict[str, Any]:
        """
        Get merged global config (defaults + loaded config.json).
        
        Returns:
            Global configuration dictionary
        """
        result = dict(self.DEFAULT_CONFIG)
        
        # Override with loaded config
        for key, value in self._global_config.items():
            if key in result or isinstance(result.get(key), dict):
                result[key] = value
        
        return result
    
    def get_instance_settings(self, instance_name: str) -> Optional[Dict[str, Any]]:
        """
        Get instance-specific settings.
        
        Args:
            instance_name: Name of the instance
            
        Returns:
            Instance configuration or None if not found
        """
        instance_path = self.instances_dir / f"{instance_name}.json"
        
        if not instance_path.exists():
            return None
        
        try:
            with open(instance_path, 'r') as f:
                return json.load(f)
        except (json.JSONDecodeError, IOError) as e:
            print(f"Warning: Could not load instance config {instance_name}: {e}")
            return None
    
    def resolve_active(self, instance_name: Optional[str] = None) -> Dict[str, Any]:
        """
        Resolve active configuration for an instance.
        
        This matches dayz-labs Profiles.ResolveActive:
        1. Start with global defaults
        2. Apply global config.json overrides
        3. Apply instance-specific settings (if provided)
        
        Args:
            instance_name: Instance name to resolve (optional)
            
        Returns:
            Merged configuration dictionary
        """
        # Start with defaults
        result = dict(self.DEFAULT_CONFIG)
        
        # Apply global config
        for key, value in self._global_config.items():
            if key in result:
                result[key] = value
        
        # Apply instance settings if provided
        if instance_name:
            instance_settings = self.get_instance_settings(instance_name)
            if instance_settings:
                for key, value in instance_settings.items():
                    if key in result or isinstance(result.get(key), (dict, list)):
                        result[key] = value
        
        return result
    
    def get(self, key: str, default: Any = None) -> Any:
        """
        Get a configuration value from resolved config.
        
        Args:
            key: Configuration key
            default: Default value if not found
            
        Returns:
            Configuration value or default
        """
        config = self.get_global_config()
        return config.get(key, default)
    
    def set(self, key: str, value: Any) -> None:
        """
        Set a global configuration value.
        
        Args:
            key: Configuration key
            value: Value to set
        """
        self._global_config[key] = value
        self._save_global_config()
    
    def _save_global_config(self) -> bool:
        """Save global config to disk."""
        try:
            if not self.projects_root.exists():
                self.projects_root.mkdir(parents=True, exist_ok=True)
            
            with open(self.global_config_path, 'w') as f:
                json.dump(self._global_config, f, indent=2)
            
            return True
        except IOError as e:
            print(f"Error saving global config: {e}")
            return False
    
    def update_instance_settings(
        self,
        instance_name: str,
        settings: Dict[str, Any]
    ) -> bool:
        """
        Update instance-specific settings.
        
        Args:
            instance_name: Instance name
            settings: Settings to merge into instance config
            
        Returns:
            True if successful
        """
        existing = self.get_instance_settings(instance_name) or {}
        existing.update(settings)
        
        instance_path = self.instances_dir / f"{instance_name}.json"
        
        try:
            if not self.instances_dir.exists():
                self.instances_dir.mkdir(parents=True, exist_ok=True)
            
            with open(instance_path, 'w') as f:
                json.dump(existing, f, indent=2)
            
            return True
        except IOError as e:
            print(f"Error saving instance config: {e}")
            return False
    
    def delete_instance_settings(self, instance_name: str) -> bool:
        """
        Delete instance-specific settings.
        
        Args:
            instance_name: Instance name
            
        Returns:
            True if successful
        """
        instance_path = self.instances_dir / f"{instance_name}.json"
        
        try:
            if instance_path.exists():
                instance_path.unlink()
            return True
        except IOError as e:
            print(f"Error deleting instance config: {e}")
            return False
    
    def list_instances(self) -> List[str]:
        """List all configured instance names."""
        instances = []
        if self.instances_dir.exists():
            for f in self.instances_dir.glob("*.json"):
                instances.append(f.stem)
        return sorted(instances)


def create_default_config(projects_root: str) -> DzlConfig:
    """
    Create a new configuration with default values.
    
    Args:
        projects_root: Root directory for the project
        
    Returns:
        New DzlConfig instance with defaults
    """
    config = DzlConfig(projects_root)
    
    # Write defaults to config.json if not exists
    if not config.global_config_path.exists():
        config._global_config = dict(config.DEFAULT_CONFIG)
        config._save_global_config()
    
    return config