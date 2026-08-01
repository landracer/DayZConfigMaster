# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# GUI module for DayZ configuration editor
"""
GUI module providing tkinter-based user interface components.

This module provides:
- Main application window
- Scrollable frame utilities
- System tray icon with hidden window support
- Tab modules for each configuration type
- Base template save/restore functionality
"""

from .app import DayzConfigMasterApp
from .system_tray import SystemTrayIcon, create_hidden_window, run_with_system_tray
from .templates import TemplateManager, create_default_templates
from .config_manager import ConfigFileManager, TabConfigManager, create_default_templates as create_config_templates
from .validation_manager import ValidationManager, ValidationRule, ValidationErrorUI
from .tab_groups import TabGroupManager, TabGroup, TabContainer, create_tab_groups

__all__ = [
    "DayzConfigMasterApp",
    "SystemTrayIcon",
    "create_hidden_window",
    "run_with_system_tray",
    "TemplateManager",
    "create_default_templates",
    "ConfigFileManager",
    "TabConfigManager",
    "create_config_templates",
    "ValidationManager",
    "ValidationRule",
    "ValidationErrorUI",
    "TabGroupManager",
    "TabGroup",
    "TabContainer",
    "create_tab_groups"
]