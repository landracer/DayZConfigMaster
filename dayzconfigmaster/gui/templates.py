# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Base Template save/restore for DayZConfigMaster

"""
Base Templates - Save and restore configuration templates.

Features:
- Save current config as template
- Load template to restore config
- Compare templates with active config
- Export/import templates
"""

import json
import os
from pathlib import Path
from typing import Dict, Any, Optional, List
from datetime import datetime


class TemplateManager:
    """
    Manages configuration templates for DayZ servers.
    
    Templates can be saved from the current configuration and restored later.
    """
    
    def __init__(self, projects_root: str):
        """
        Initialize template manager.
        
        Args:
            projects_root: Root directory for all projects
        """
        self.projects_root = Path(projects_root)
        self.templates_dir = self.projects_root / "templates"
        self.templates_dir.mkdir(parents=True, exist_ok=True)
    
    def save_template(
        self,
        name: str,
        description: str = "",
        config_data: Dict[str, Any] = None
    ) -> bool:
        """
        Save current configuration as a template.
        
        Args:
            name: Template name (must be unique)
            description: Optional description
            config_data: Config data to save (uses default if None)
        
        Returns:
            True if successful
        """
        # Validate name
        safe_name = "".join(c for c in name if c.isalnum() or c in " -_").strip()
        safe_name = safe_name.replace(" ", "_")
        
        if not safe_name:
            return False
        
        template_file = self.templates_dir / f"{safe_name}.json"
        
        template_data = {
            "name": safe_name,
            "display_name": name,
            "description": description,
            "created_at": datetime.now().isoformat(),
            "version": "1.0",
            "data": config_data or {}
        }
        
        try:
            with open(template_file, 'w') as f:
                json.dump(template_data, f, indent=2)
            return True
        except Exception as e:
            print(f"Failed to save template: {e}")
            return False
    
    def load_template(self, name: str) -> Optional[Dict[str, Any]]:
        """
        Load a template by name.
        
        Args:
            name: Template name
        
        Returns:
            Template data dict or None if not found
        """
        safe_name = "".join(c for c in name if c.isalnum() or c in " -_").strip()
        safe_name = safe_name.replace(" ", "_")
        
        template_file = self.templates_dir / f"{safe_name}.json"
        
        if not template_file.exists():
            return None
        
        try:
            with open(template_file, 'r') as f:
                data = json.load(f)
            
            return data.get("data", {})
        except Exception as e:
            print(f"Failed to load template: {e}")
            return None
    
    def delete_template(self, name: str) -> bool:
        """
        Delete a template.
        
        Args:
            name: Template name
        
        Returns:
            True if deleted
        """
        safe_name = "".join(c for c in name if c.isalnum() or c in " -_").strip()
        safe_name = safe_name.replace(" ", "_")
        
        template_file = self.templates_dir / f"{safe_name}.json"
        
        try:
            template_file.unlink()
            return True
        except Exception as e:
            print(f"Failed to delete template: {e}")
            return False
    
    def list_templates(self) -> List[Dict[str, Any]]:
        """
        List all available templates.
        
        Returns:
            List of template metadata dicts
        """
        templates = []
        
        for template_file in self.templates_dir.glob("*.json"):
            try:
                with open(template_file, 'r') as f:
                    data = json.load(f)
                
                templates.append({
                    "name": data.get("name", ""),
                    "display_name": data.get("display_name", ""),
                    "description": data.get("description", ""),
                    "created_at": data.get("created_at", ""),
                    "version": data.get("version", "")
                })
            except Exception:
                continue
        
        # Sort by creation date (newest first)
        templates.sort(key=lambda x: x.get("created_at", ""), reverse=True)
        
        return templates
    
    def compare_with_active(
        self,
        template_name: str,
        active_config: Dict[str, Any]
    ) -> Dict[str, Any]:
        """
        Compare a template with the current active configuration.
        
        Args:
            template_name: Name of template to compare
            active_config: Current active configuration
        
        Returns:
            Dict with 'changed' keys showing differences
        """
        template_data = self.load_template(template_name)
        
        if not template_data:
            return {"error": "Template not found"}
        
        # Simple comparison - find keys that differ
        changes = {
            "added": [],
            "removed": [],
            "modified": []
        }
        
        template_keys = set(template_data.keys())
        active_keys = set(active_config.keys())
        
        # Added in template (not in active)
        for key in template_keys - active_keys:
            changes["added"].append({"key": key, "value": template_data[key]})
        
        # Removed from template (in active but not template)
        for key in active_keys - template_keys:
            changes["removed"].append({"key": key})
        
        # Modified values
        for key in template_keys & active_keys:
            if template_data[key] != active_config.get(key):
                changes["modified"].append({
                    "key": key,
                    "old_value": active_config[key],
                    "new_value": template_data[key]
                })
        
        return changes
    
    def export_template(self, name: str, output_path: str) -> bool:
        """
        Export a template to an external file.
        
        Args:
            name: Template name
            output_path: Output file path
        
        Returns:
            True if exported successfully
        """
        template_data = self.load_template(name)
        
        if not template_data:
            return False
        
        try:
            with open(output_path, 'w') as f:
                json.dump(template_data, f, indent=2)
            return True
        except Exception as e:
            print(f"Failed to export template: {e}")
            return False
    
    def import_template(self, input_path: str, name_override: str = None) -> bool:
        """
        Import a template from an external file.
        
        Args:
            input_path: Input file path
            name_override: Optional name override
        
        Returns:
            True if imported successfully
        """
        try:
            with open(input_path, 'r') as f:
                data = json.load(f)
            
            # Use override or existing name
            template_name = name_override or data.get("name", "imported")
            safe_name = "".join(c for c in template_name if c.isalnum() or c in " -_").strip()
            safe_name = safe_name.replace(" ", "_")
            
            # Ensure we have data
            if "data" not in data:
                return False
            
            template_data = {
                "name": safe_name,
                "display_name": data.get("display_name", template_name),
                "description": data.get("description", ""),
                "created_at": datetime.now().isoformat(),
                "version": data.get("version", "1.0"),
                "data": data["data"]
            }
            
            output_file = self.templates_dir / f"{safe_name}.json"
            with open(output_file, 'w') as f:
                json.dump(template_data, f, indent=2)
            
            return True
        except Exception as e:
            print(f"Failed to import template: {e}")
            return False


def create_default_templates(projects_root: str) -> Dict[str, Any]:
    """
    Create default templates for common configurations.
    
    Args:
        projects_root: Root directory for all projects
    
    Returns:
        Dict of created template names and descriptions
    """
    manager = TemplateManager(projects_root)
    defaults_created = {}
    
    # Survival template
    survival_template = {
        "economy": {
            "max_items": 1000,
            "max_weapons": 500,
            "max_vehicles": 100
        },
        "limits_definitions": {
            "categories": ["weapons", "tools", "clothes"],
            "usages": ["Town", "Village", "Farm"]
        },
        "server_config": {
            "game_mode": "Survival",
            "max_players": 60,
            "day_time_speed": 24.0
        }
    }
    
    manager.save_template(
        name="survival_default",
        description="Default survival server configuration",
        config_data=survival_template
    )
    defaults_created["survival_default"] = "Default survival server configuration"
    
    # PVP template
    pvp_template = {
        "economy": {
            "max_items": 1500,
            "max_weapons": 800,
            "max_vehicles": 150
        },
        "limits_definitions": {
            "categories": ["weapons", "tools", "clothes"],
            "usages": ["Military", "Town"]
        },
        "server_config": {
            "game_mode": "Custom",
            "max_players": 100,
            "day_time_speed": 48.0
        }
    }
    
    manager.save_template(
        name="pvp_hardcore",
        description="Hardcore PVP server configuration",
        config_data=pvp_template
    )
    defaults_created["pvp_hardcore"] = "Hardcore PVP server configuration"
    
    # Roleplay template
    rp_template = {
        "economy": {
            "max_items": 500,
            "max_weapons": 200,
            "max_vehicles": 50
        },
        "limits_definitions": {
            "categories": ["clothes", "tools", "food"],
            "usages": ["Town", "Village"]
        },
        "server_config": {
            "game_mode": "Custom",
            "max_players": 40,
            "day_time_speed": 24.0
        }
    }
    
    manager.save_template(
        name="roleplay_casual",
        description="Roleplay casual server configuration",
        config_data=rp_template
    )
    defaults_created["roleplay_casual"] = "Roleplay casual server configuration"
    
    return defaults_created