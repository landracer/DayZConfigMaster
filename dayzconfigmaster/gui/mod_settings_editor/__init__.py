# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Interactive Mod Settings Editor for DayZ Config Master.

This module provides:
- Parsing of DayZ mod configuration formats (config.cpp, JSON, XML)
- Interactive UI widgets with drag sliders, min/max controls
- Support for DayZ Expansion Bundle and other major mods

Usage:
    from dayzconfigmaster.gui.mod_settings_editor import ModConfigEditor
    
    editor = ModConfigEditor(parent_frame, config_file_path)
    editor.render()
"""

from .parser import ConfigParser, SettingType, SettingField
from .widgets import (
    SliderWidget,
    NumberInputWidget,
    BoolWidget,
    StringWidget,
    SelectWidget,
    ColorWidget,
    create_widget_for_setting,
)

__all__ = [
    'ConfigParser',
    'SettingType',
    'SettingField',
    'SliderWidget',
    'NumberInputWidget',
    'BoolWidget',
    'StringWidget',
    'SelectWidget',
    'ColorWidget',
    'create_widget_for_setting',
]
