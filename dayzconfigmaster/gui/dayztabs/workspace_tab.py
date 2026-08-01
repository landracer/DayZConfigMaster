# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Workspace tab for DayZ configuration editor

"""
WorkspaceTab: Overview and welcome screen for the application.
"""

import tkinter as tk
from tkinter import ttk


class WorkspaceTab(ttk.Frame):
    """Workspace overview tab with welcome message and quick access."""
    
    def __init__(self, parent, notebook, **kwargs):
        super().__init__(parent, **kwargs)
        
        self.notebook = notebook
        
        # Configure row/column weights for proper resizing
        self.columnconfigure(0, weight=1)
        self.rowconfigure(1, weight=1)
        
        self._create_ui()
    
    def _create_ui(self):
        """Create the workspace tab UI."""
        # Welcome message
        welcome_label = ttk.Label(
            self,
            text="DayzConfigMaster - Advanced DayZ Server Configuration Editor",
            font=("Arial", 16, "bold")
        )
        welcome_label.pack(pady=20)
        
        description_label = ttk.Label(
            self,
            text="Comprehensive DayZ server configuration editor with full support for all configuration files\n"
                 "and advanced features for modded servers.",
            font=("Arial", 11),
            justify=tk.CENTER
        )
        description_label.pack(pady=10)
        
        # Quick overview of supported files
        files_frame = ttk.LabelFrame(self, text="Supported Configuration Files", padding=10)
        files_frame.pack(fill=tk.X, padx=20, pady=10)
        
        supported_files = [
            "economycore.xml - Core game configuration",
            "limitsdefinitions.xml - Item and limit definitions (categories, usages, values)",
            "cfgrandompresets.xml - Random item presets with advanced settings",
            "cfgspawnabletypes.xml - Spawnable item types with categories and flags",
            "traderconfig.txt - Trader configurations with full category support",
            "serverDZ.cfg - Main server configuration file",
            "cfggameplay.json - Gameplay settings",
            "cfgenvironment.xml - Environmental settings"
        ]
        
        for file_desc in supported_files:
            ttk.Label(files_frame, text=file_desc).pack(anchor=tk.W)
        
        # DayZ features overview
        features_frame = ttk.LabelFrame(self, text="DayZ Server Features Supported", padding=10)
        features_frame.pack(fill=tk.X, padx=20, pady=10)
        
        features = [
            "Full Economy System Control",
            "Advanced Spawn Limit Management",
            "Random Preset Customization",
            "Item Category and Usage Control",
            "Trader Configuration with Categories",
            "Server Performance Settings",
            "Mod Support and Expansion Settings"
        ]
        
        for feature in features:
            ttk.Label(features_frame, text=feature).pack(anchor=tk.W)