# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Validation module for DayZ configuration fields

"""
Validation module providing field validation rules and error handling.

This module separates validation logic from GUI components, following
the principle of separation of concerns.
"""

from typing import Dict, List, Tuple, Optional, Union, Any
import re  # For regex pattern matching


class ValidationError(Exception):
    """Custom exception for validation errors"""
    pass


class FieldValidator:
    """
    Validator class with comprehensive field rules for DayZ server configuration.
    
    Provides validation for:
    - Server settings (port, max players, map size)
    - Time and weather settings
    - Multipliers and limits
    - String fields (length checks)
    - Numeric ranges
    """
    
    def __init__(self):
        # Define all valid ranges and default values with comprehensive explanations
        self.field_rules = {
            "Server Port": {"min": 1024, "max": 65535, "default": 2302, "explanation": "Port for server connections (1024-65535)"},
            "Query Port": {"min": 1024, "max": 65535, "default": 2303, "explanation": "Port for server browser queries (1024-65535)"},
            "Max Players": {"min": 1, "max": 1000, "default": 60, "explanation": "Maximum concurrent players (1-1000)"},
            "Map Size": {"min": 1000, "max": 100000, "default": 2000, "explanation": "Map dimensions in meters (1000-100000m)"},
            "Time Settings": {"format": "YYYY/M/D/Hour/Minute", "example": "2020/4/1/08/00", "explanation": "Date and time in format YYYY/M/D/Hour/Minute"},
            "Weather (0-1)": {"min": 0.0, "max": 1.0, "default": 0.5, "explanation": "Global weather intensity (0.0-1.0)"},
            "Fog Density": {"min": 0.0, "max": 1.0, "default": 0.5, "explanation": "Fog density level (0.0-1.0)"},
            "Rain Intensity": {"min": 0.0, "max": 1.0, "default": 0.8, "explanation": "Rain intensity level (0.0-1.0)"},
            "Wind Speed": {"min": 0.0, "max": 20.0, "default": 5.0, "explanation": "Wind speed in m/s (0.0-20.0)"},
            "Temperature": {"min": -40.0, "max": 50.0, "default": 20.0, "explanation": "Base temperature in Celsius (-40 to 50°C)"},
            
            # Multipliers
            "Base Damage Multiplier": {"min": 0.1, "max": 10.0, "default": 1.0, "explanation": "Damage multiplier for all damage types (0.1-10.0)"},
            "Hit Point Multiplier": {"min": 0.1, "max": 10.0, "default": 1.0, "explanation": "Health damage multiplier (0.1-10.0)"},
            "Loot Spawn Multiplier": {"min": 0.1, "max": 5.0, "default": 1.0, "explanation": "Loot spawn rate multiplier (0.1-5.0)"},
            "Zombie Spawn Multiplier": {"min": 0.1, "max": 5.0, "default": 1.0, "explanation": "Zombie spawn rate multiplier (0.1-5.0)"},
            "Vehicle Spawn Multiplier": {"min": 0.1, "max": 5.0, "default": 1.0, "explanation": "Vehicle spawn rate multiplier (0.1-5.0)"},
            
            # Item settings
            "Item Decay Time": {"min": 0, "max": 86400, "default": 3600, "explanation": "Time in seconds before items decay (0-86400s)"},
            
            # String fields
            "Server Name": {"min_length": 1, "max_length": 50, "default": "DayZ Server", "explanation": "Display name for your server (1-50 chars)"},
            "Server Password": {"min_length": 0, "max_length": 32, "default": "", "explanation": "Password to join server (empty = public)"},
            
            # Game settings
            "Game Mode": {"valid_values": ["Survival", "Custom", "Modded"], "default": "Survival", "explanation": "Server game mode type"},
            "Season": {"valid_values": ["Spring", "Summer", "Autumn", "Winter"], "default": "Summer", "explanation": "Current season"},
            
            # Mod settings
            "Verify Signatures": {"valid_values": ["0", "1", "2"], "default": "2", "explanation": "Signature verification level (0=Off, 1=Debug, 2=Verify)"},
            "Force Same Build": {"valid_values": ["0", "1"], "default": "1", "explanation": "Force clients to use same build (0=No, 1=Yes)"},
        }
    
    def validate(self, field_name: str, value: Any) -> Tuple[bool, Optional[str]]:
        """
        Validate a field value against its rules.
        
        Args:
            field_name: The name of the field to validate
            value: The value to validate
            
        Returns:
            Tuple of (is_valid, message)
            - is_valid: True if valid, False otherwise
            - message: Explanation string or None if no issues
        """
        if field_name not in self.field_rules:
            return True, None
        
        rule = self.field_rules[field_name]
        
        # Check format (for date/time fields)
        if "format" in rule and isinstance(value, str):
            if value:
                return True, f"Format: {rule['format']} | Example: {rule.get('example', 'N/A')}"
            return True, None
        
        # Check string length
        if isinstance(value, str):
            if "min_length" in rule and len(value) < rule["min_length"]:
                return False, f"Too short (min {rule['min_length']} chars). Default: {rule.get('default', 'N/A')}"
            if "max_length" in rule and len(value) > rule["max_length"]:
                return False, f"Too long (max {rule['max_length']} chars). Default: {rule.get('default', 'N/A')}"
        
        # Check valid values
        if "valid_values" in rule:
            str_value = str(value)
            if value and str_value not in rule["valid_values"]:
                return False, f"Invalid value. Must be one of: {', '.join(rule['valid_values'])}. Default: {rule.get('default', 'N/A')}"
        
        # Check numeric ranges
        try:
            num_value = float(value) if value else 0
            
            if "min" in rule and num_value < rule["min"]:
                return False, f"Below minimum ({rule['min']}). Default: {rule['default']}"
            if "max" in rule and num_value > rule["max"]:
                return False, f"Above maximum ({rule['max']}). Default: {rule['default']}"
            
            # Build explanation with all available info
            explain_parts = [f"{k}: {v}" for k, v in rule.items() if k not in ["min", "max"]]
            return True, " | ".join(explain_parts)
        except (ValueError, TypeError):
            if value:  # Only error if not empty
                return False, f"Must be a number. Default: {rule.get('default', 'N/A')}"
            else:
                return True, None
    
    def get_field_info(self, field_name: str) -> Dict[str, Any]:
        """Get full information about a field"""
        return self.field_rules.get(field_name, {})
    
    def is_required(self, field_name: str) -> bool:
        """Check if a field is required (has a default value)"""
        rule = self.field_rules.get(field_name, {})
        return "default" in rule or rule.get("required", False)