# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Parser module for DayZ configuration files

"""
Parser module providing file I/O operations for DayZ configuration types.

Supports parsing and writing:
- XML files (economycore.xml, limitsdefinitions.xml, etc.)
- JSON files (cfggameplay.json, cfgeffectarea.json)
- TXT files (traderconfig.txt)
- CFG files (serverDZ.cfg)
"""

import os
from typing import Dict, Any, Optional
from pathlib import Path

# Import models for type hints
try:
    from .models import (
        EconomyConfig,
        LimitsConfig,
        RandomPresetConfig,
        SpawnableTypeConfig,
        TraderConfig,
        ServerConfig,
    )
except ImportError:
    # Fallback for direct execution
    from models import (
        EconomyConfig,
        LimitsConfig,
        RandomPresetConfig,
        SpawnableTypeConfig,
        TraderConfig,
        ServerConfig,
    )


class ConfigParser:
    """
    Parser class for DayZ configuration files.
    
    Provides methods to:
    - Load configuration from various file formats
    - Save configurations to files
    - Validate parsed data
    """
    
    # File extensions and their corresponding parsers
    SUPPORTED_EXTENSIONS = {
        '.xml': 'xml',
        '.json': 'json', 
        '.txt': 'text',
        '.cfg': 'config'
    }
    
    def __init__(self):
        self._last_error: Optional[str] = None
    
    @property
    def last_error(self) -> Optional[str]:
        """Get the last error message"""
        return self._last_error
    
    def load_file(self, filepath: str) -> tuple:
        """
        Load a configuration file and parse it.
        
        Args:
            filepath: Path to the configuration file
            
        Returns:
            Tuple of (config_object, error_message)
            - config_object: Parsed config model or None on error
            - error_message: Error string if failed, None if successful
        """
        path = Path(filepath)
        
        if not path.exists():
            self._last_error = f"File not found: {filepath}"
            return None, self._last_error
        
        ext = path.suffix.lower()
        
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read()
            
            parser_method = self.get_parser(ext)
            if parser_method:
                config = parser_method(content)
                return config, None
            
            self._last_error = f"Unsupported file type: {ext}"
            return None, self._last_error
            
        except Exception as e:
            self._last_error = f"Error parsing file: {str(e)}"
            return None, self._last_error
    
    def get_parser(self, ext: str):
        """Get the appropriate parser method for a file extension"""
        parsers = {
            '.xml': self.parse_xml,
            '.json': self.parse_json,
            '.txt': self.parse_txt,
            '.cfg': self.parse_cfg,
        }
        return parsers.get(ext)
    
    def save_file(self, config: Any, filepath: str) -> tuple:
        """
        Save a configuration object to a file.
        
        Args:
            config: Configuration model instance
            filepath: Path to save the file
            
        Returns:
            Tuple of (success, error_message)
        """
        path = Path(filepath)
        ext = path.suffix.lower()
        
        try:
            # Ensure parent directory exists
            path.parent.mkdir(parents=True, exist_ok=True)
            
            serializer_method = self.get_serializer(ext)
            if serializer_method:
                content = serializer_method(config)
                
                with open(filepath, 'w', encoding='utf-8') as f:
                    f.write(content)
                return True, None
            
            self._last_error = f"Unsupported file type: {ext}"
            return False, self._last_error
            
        except Exception as e:
            self._last_error = f"Error saving file: {str(e)}"
            return False, self._last_error
    
    def get_serializer(self, ext: str):
        """Get the appropriate serializer method for a file extension"""
        serializers = {
            '.xml': self.to_xml,
            '.json': self.to_json,
            '.txt': self.to_txt,
            '.cfg': self.to_cfg,
        }
        return serializers.get(ext)
    
    def parse_xml(self, content: str) -> Optional[Any]:
        """Parse XML content and return appropriate config object"""
        # Try to determine config type by looking at root element
        import re
        
        # Check for economycore.xml format
        if '<economy>' in content:
            return EconomyConfig.from_xml(content)
        
        # Check for limitsdefinitions.xml format
        elif '<limitsdefinitions>' in content:
            return LimitsConfig.from_xml(content)
        
        # Check for cfgrandompresets.xml format  
        elif '<randompresets>' in content:
            return RandomPresetConfig.from_xml(content)
        
        # Check for cfgspawnabletypes.xml format
        elif '<spawnabletypes>' in content:
            return SpawnableTypeConfig.from_xml(content)
        
        self._last_error = "Unknown XML format - could not determine config type"
        return None
    
    def parse_json(self, content: str) -> Optional[Dict[str, Any]]:
        """Parse JSON content"""
        import json
        
        try:
            return json.loads(content)
        except json.JSONDecodeError as e:
            self._last_error = f"JSON parse error: {str(e)}"
            return None
    
    def parse_txt(self, content: str) -> Optional[TraderConfig]:
        """Parse TXT content (traderconfig.txt format)"""
        return TraderConfig.from_txt(content)
    
    def parse_cfg(self, content: str) -> Optional[ServerConfig]:
        """Parse CFG content (serverDZ.cfg format)"""
        return ServerConfig.from_cfg(content)
    
    def to_xml(self, config: Any) -> str:
        """Convert a config object to XML string"""
        if hasattr(config, 'to_xml'):
            return config.to_xml()
        raise TypeError(f"Config object {type(config)} does not support XML serialization")
    
    def to_json(self, data: Dict[str, Any]) -> str:
        """Convert data to JSON string"""
        import json
        return json.dumps(data, indent=2)
    
    def to_txt(self, config: Any) -> str:
        """Convert a config object to TXT string"""
        if hasattr(config, 'to_txt'):
            return config.to_txt()
        raise TypeError(f"Config object {type(config)} does not support TXT serialization")
    
    def to_cfg(self, config: Any) -> str:
        """Convert a config object to CFG string"""
        if hasattr(config, 'to_cfg'):
            return config.to_cfg()
        raise TypeError(f"Config object {type(config)} does not support CFG serialization")


def load_config(filepath: str) -> tuple:
    """
    Convenience function to load a configuration file.
    
    Args:
        filepath: Path to the configuration file
        
    Returns:
        Tuple of (config_object, error_message)
    """
    parser = ConfigParser()
    return parser.load_file(filepath)


def save_config(config: Any, filepath: str) -> tuple:
    """
    Convenience function to save a configuration.
    
    Args:
        config: Configuration model instance
        filepath: Path to save the file
        
    Returns:
        Tuple of (success, error_message)
    """
    parser = ConfigParser()
    return parser.save_file(config, filepath)


# File type detection helper
def detect_config_type(filepath: str) -> Optional[str]:
    """
    Detect the configuration file type based on filename or content.
    
    Args:
        filepath: Path to the configuration file
        
    Returns:
        Config type string (e.g., 'serverDZ', 'economycore', etc.)
    """
    path = Path(filepath)
    ext = path.suffix.lower()
    
    # Check by extension
    if ext == '.cfg':
        return 'serverDZ'
    elif ext == '.xml':
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read(500)  # Read first 500 chars for detection
            
            if '<economy>' in content:
                return 'economycore'
            elif '<limitsdefinitions>' in content:
                return 'limitsdefinitions'
            elif '<randompresets>' in content:
                return 'cfgrandompresets'
            elif '<spawnabletypes>' in content:
                return 'cfgspawnabletypes'
        except Exception:
            pass
        return 'unknown_xml'
    
    return None