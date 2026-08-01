# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Configuration File Manager for GUI file I/O operations
"""
ConfigurationFileManager: Provides file loading/saving capabilities for the GUI.
"""

import tkinter as tk
from tkinter import filedialog, messagebox
from pathlib import Path
from typing import Optional, List, Dict, Any, Tuple
import json
import os

try:
    from ..config.parser import ConfigParser, detect_config_type
    from ..config.models import (
        EconomyConfig, LimitsConfig, RandomPresetConfig,
        SpawnableTypeConfig, TraderConfig, ServerConfig
    )
except ImportError:
    from dayzconfigmaster.config.parser import ConfigParser, detect_config_type
    from dayzconfigmaster.config.models import (
        EconomyConfig, LimitsConfig, RandomPresetConfig,
        SpawnableTypeConfig, TraderConfig, ServerConfig
    )


class ConfigFileManager:
    """
    Manages loading and saving of DayZ configuration files.
    
    Supports all major config types:
    - XML: economycore.xml, limitsdefinitions.xml, cfgrandompresets.xml, cfgspawnabletypes.xml
    - TXT: traderconfig.txt
    - CFG: serverDZ.cfg
    - JSON: cfggameplay.json
    """
    
    # Mapping of file extensions to supported config types
    CONFIG_EXTENSIONS = {
        '.xml': ['economycore', 'limitsdefinitions', 'cfgrandompresets', 'cfgspawnabletypes'],
        '.txt': ['traderconfig'],
        '.cfg': ['serverDZ'],
        '.json': ['cfggameplay']
    }
    
    # File descriptions for dialog filters
    FILE_FILTERS = {
        'xml': ('XML Configuration Files', '*.xml'),
        'txt': ('Text Configuration Files', '*.txt'),
        'cfg': ('CFG Configuration Files', '*.cfg'),
        'json': ('JSON Configuration Files', '*.json'),
        'all': ('All Configuration Files', '*.xml;*.txt;*.cfg;*.json')
    }
    
    def __init__(self, app_root: tk.Tk = None):
        """
        Initialize the configuration file manager.
        
        Args:
            app_root: Optional root window for dialogs
        """
        self.app_root = app_root
        self.parser = ConfigParser()
        self._last_directory: str = os.getcwd()
        self._current_file: Optional[str] = None
    
    @property
    def current_file(self) -> Optional[str]:
        """Get the currently loaded file path"""
        return self._current_file
    
    @property
    def last_directory(self) -> str:
        """Get the last accessed directory"""
        return self._last_directory
    
    @last_directory.setter
    def last_directory(self, value: str):
        """Set the last accessed directory for future dialogs"""
        self._last_directory = value
    
    def load_config_file(self, filepath: Optional[str] = None) -> Tuple[Optional[Any], Optional[str]]:
        """
        Load a configuration file.
        
        Args:
            filepath: Path to file (if None, opens dialog)
            
        Returns:
            Tuple of (config_object, error_message)
        """
        # If no path provided, open file dialog
        if not filepath:
            filepath = self._open_file_dialog()
            if not filepath:
                return None, "No file selected"
        
        path = Path(filepath)
        
        # Validate file exists
        if not path.exists():
            error = f"File not found: {filepath}"
            messagebox.showerror("Error", error) if self.app_root else print(error)
            return None, error
        
        # Parse the file
        config, error = self.parser.load_file(str(path))
        
        if error:
            msg = f"Failed to parse {path.name}: {error}"
            messagebox.showerror("Parse Error", msg) if self.app_root else print(msg)
            return None, error
        
        self._current_file = str(filepath)
        self.last_directory = str(path.parent)
        
        return config, None
    
    def save_config_file(self, config: Any, filepath: Optional[str] = None) -> Tuple[bool, Optional[str]]:
        """
        Save a configuration to file.
        
        Args:
            config: Configuration object to save
            filepath: Path to save (if None, opens dialog)
            
        Returns:
            Tuple of (success, error_message)
        """
        # If no path provided or using existing, prompt for new location if needed
        if not filepath and self._current_file:
            filepath = self._current_file
        elif not filepath:
            filepath = self._save_as_dialog()
            if not filepath:
                return False, "No file selected"
        
        # Ensure proper extension based on config type
        path = Path(filepath)
        
        # Determine extension based on config type
        ext = path.suffix.lower()
        if not ext:
            ext = '.xml'  # Default to XML
        
        success, error = self.parser.save_file(config, str(path))
        
        if success:
            self._current_file = str(path)
            return True, None
        else:
            msg = f"Failed to save configuration: {error}"
            messagebox.showerror("Save Error", msg) if self.app_root else print(msg)
            return False, error
    
    def _open_file_dialog(self) -> Optional[str]:
        """
        Open file dialog for selecting a config file.
        
        Returns:
            Selected file path or None
        """
        if not self.app_root:
            return None
        
        filetypes = [
            self.FILE_FILTERS['xml'],
            self.FILE_FILTERS['txt'],
            self.FILE_FILTERS['cfg'],
            self.FILE_FILTERS['json'],
            self.FILE_FILTERS['all']
        ]
        
        filepath = filedialog.askopenfilename(
            parent=self.app_root,
            title="Open Configuration File",
            initialdir=self._last_directory,
            filetypes=filetypes
        )
        
        return filepath if filepath else None
    
    def _save_as_dialog(self) -> Optional[str]:
        """
        Open save dialog for configuration.
        
        Returns:
            Selected file path or None
        """
        if not self.app_root:
            return None
        
        defaultext = '.xml'
        filetypes = [
            ('XML Files', '*.xml'),
            ('TXT Files', '*.txt'),
            ('CFG Files', '*.cfg'),
            ('JSON Files', '*.json'),
            ('All Files', '*.*')
        ]
        
        filepath = filedialog.asksaveasfilename(
            parent=self.app_root,
            title="Save Configuration As",
            initialdir=self._last_directory,
            defaultextension=defaultext,
            filetypes=filetypes
        )
        
        return filepath if filepath else None
    
    def get_config_type(self, filepath: str) -> Optional[str]:
        """
        Detect the configuration type from file.
        
        Args:
            filepath: Path to configuration file
            
        Returns:
            Config type string or None
        """
        return detect_config_type(filepath)
    
    def load_all_configs_in_directory(self, directory: str) -> Dict[str, Any]:
        """
        Load all supported config files in a directory.
        
        Args:
            directory: Directory path to scan
            
        Returns:
            Dictionary of {filename: config_object}
        """
        configs = {}
        dir_path = Path(directory)
        
        if not dir_path.exists():
            return configs
        
        # Supported file patterns
        patterns = ['*.xml', '*.txt', '*.cfg', '*.json']
        
        for pattern in patterns:
            for filepath in dir_path.glob(pattern):
                config, error = self.load_config_file(str(filepath))
                if config and not error:
                    configs[filepath.name] = config
        
        return configs
    
    def get_available_configs(self) -> List[str]:
        """
        Get list of available configuration file types.
        
        Returns:
            List of config type names
        """
        return [
            'Economy Core (economycore.xml)',
            'Limits Definitions (limitsdefinitions.xml)',
            'Random Presets (cfgrandompresets.xml)',
            'Spawnable Types (cfgspawnabletypes.xml)',
            'Trader Config (traderconfig.txt)',
            'Server Config (serverDZ.cfg)'
        ]


class TabConfigManager:
    """
    Manages configuration for individual tabs in the GUI.
    
    Each tab can have its own config file manager with specific
    validation and UI integration.
    """
    
    def __init__(self, parent_manager: ConfigFileManager, tab_name: str):
        """
        Initialize a tab-specific config manager.
        
        Args:
            parent_manager: Parent ConfigFileManager instance
            tab_name: Name of this tab
        """
        self.parent = parent_manager
        self.tab_name = tab_name
        self.current_config: Optional[Any] = None
        self._original_state: Dict[str, Any] = {}
    
    def load(self, filepath: Optional[str] = None) -> Tuple[bool, Optional[str]]:
        """Load configuration for this tab"""
        config, error = self.parent.load_config_file(filepath)
        if config:
            self.current_config = config
            return True, None
        return False, error
    
    def save(self, filepath: Optional[str] = None) -> Tuple[bool, Optional[str]]:
        """Save configuration for this tab"""
        if not self.current_config:
            error = "No active configuration to save"
            print(error)
            return False, error
        return self.parent.save_config_file(self.current_config, filepath)
    
    def get_editor_widgets(self) -> Dict[str, Any]:
        """
        Get all editor widgets for this tab.
        
        Returns:
            Dictionary of widget names to their values
        """
        if not self.current_config:
            return {}
        
        # Extract data based on config type
        if isinstance(self.current_config, EconomyConfig):
            return {
                'classes': dict(self.current_config.classes),
                'defaults': dict(self.current_config.defaults)
            }
        elif isinstance(self.current_config, TraderConfig):
            traders = [
                {'name': t.name, 'category': t.category, 'items': t.items}
                for t in self.current_config.traders
            ]
            return {'traders': traders}
        
        return {}


def create_default_templates() -> Dict[str, str]:
    """
    Create default configuration templates.
    
    Returns:
        Dictionary of {template_name: content_string}
    """
    templates = {}
    
    # Default economy core template
    templates['economycore.xml'] = '''<?xml version="1.0" encoding="UTF-8"?>
<economy>
  <classes>
    <class name="Item" value="1"/>
    <class name="Weapon" value="2"/>
    <class name="Vehicle" value="3"/>
    <class name="Clothing" value="4"/>
    <class name="Food" value="5"/>
    <class name="Tool" value="6"/>
  </classes>
  <defaults>
    <default name="max_items" value="1000"/>
    <default name="max_weapons" value="500"/>
    <default name="max_vehicles" value="100"/>
  </defaults>
</economy>'''
    
    # Default trader config template
    templates['traderconfig.txt'] = '''// Trader Configuration File
// Generated by DayzConfigMaster

Trader1 {
    Name = "General Store"
    Category = "General"
    Items = [
        "Food",
        "Weapons",
        "Ammo"
    ]
    MinDistance = 50.0
    MaxDistance = 200.0
    CanBeRobbed = true
    
    PriceMultiplier = 1.0
    BuyPriceModifier = -0.1
    SellPriceModifier = 0.1
}

Trader2 {
    Name = "Medical Supplies"
    Category = "Medical"
    Items = [
        "Medkit",
        "Painkillers",
        "Bandages"
    ]
    MinDistance = 30.0
    MaxDistance = 150.0
    CanBeRobbed = true
    
    PriceMultiplier = 1.2
    BuyPriceModifier = -0.15
    SellPriceModifier = 0.15
}
'''
    
    return templates