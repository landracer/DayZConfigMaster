# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Tab grouping module for DayZ configuration editor
"""
TabGroupManager: Groups tabs into logical categories for better organization.
"""

import tkinter as tk
from tkinter import ttk

from typing import Dict, List, Optional
from dataclasses import dataclass


@dataclass
class TabGroup:
    """Represents a group of related configuration tabs"""
    name: str
    description: str
    tab_names: List[str]
    icon: str = "folder"


class TabGroupManager:
    """
    Manages grouping and organization of configuration tabs.
    
    Groups related tabs together to provide a cleaner interface
    with fewer top-level tabs but more organized content.
    """
    
    # Default group definitions based on DayZ server configuration needs
    DEFAULT_GROUPS: List[Dict] = [
        {
            "name": "Economy Settings",
            "description": "Configure game economy, spawn rules, and item distribution",
            "tabs": ["Economy Core", "Limits Definitions", "Random Presets", "Spawnable Types"]
        },
        {
            "name": "Server Management",
            "description": "Main server configuration and instance management",
            "tabs": ["Server Config", "Multi-Instance Manager", "House Cleaning"]
        },
        {
            "name": "Trading System",
            "description": "Configure traders, prices, and trade settings",
            "tabs": ["Trader Config"]
        },
        {
            "name": "Advanced Configuration",
            "description": "Gameplay mechanics, environment, and optimization settings",
            "tabs": ["Advanced Settings"]
        },
        {
            "name": "Modding & Build",
            "description": "Workshop management, mod building, and signing",
            "tabs": ["Mod Builder", "Workshop Manager"]
        },
        {
            "name": "Server Control",
            "description": "Start/stop server, view logs, and diagnostics",
            "tabs": ["Server Control"]
        }
    ]
    
    def __init__(self):
        """Initialize the tab group manager"""
        self._groups: List[TabGroup] = []
        self._tab_to_group: Dict[str, str] = {}
        
        self._load_default_groups()
    
    def _load_default_groups(self):
        """Load the default tab group definitions"""
        for group_data in self.DEFAULT_GROUPS:
            group = TabGroup(
                name=group_data["name"],
                description=group_data["description"],
                tab_names=group_data.get("tabs", [])
            )
            self._groups.append(group)
            
            # Map each tab to its group
            for tab_name in group.tab_names:
                self._tab_to_group[tab_name] = group.name
    
    def get_groups(self) -> List[TabGroup]:
        """Get all configured groups"""
        return list(self._groups)
    
    def get_group_for_tab(self, tab_name: str) -> Optional[str]:
        """
        Get the group that contains a specific tab.
        
        Args:
            tab_name: Name of the tab
            
        Returns:
            Group name or None if not found
        """
        return self._tab_to_group.get(tab_name)
    
    def get_tabs_in_group(self, group_name: str) -> List[str]:
        """
        Get all tabs in a specific group.
        
        Args:
            group_name: Name of the group
            
        Returns:
            List of tab names in the group
        """
        for group in self._groups:
            if group.name == group_name:
                return list(group.tab_names)
        return []
    
    def add_group(self, name: str, description: str, tabs: List[str], icon: str = "folder"):
        """Add a new tab group"""
        group = TabGroup(
            name=name,
            description=description,
            tab_names=list(tabs),
            icon=icon
        )
        self._groups.append(group)
        
        # Update tab-to-group mapping
        for tab_name in tabs:
            self._tab_to_group[tab_name] = name
    
    def remove_group(self, group_name: str) -> bool:
        """
        Remove a tab group.
        
        Args:
            group_name: Name of the group to remove
            
        Returns:
            True if removed, False if not found
        """
        for i, group in enumerate(self._groups):
            if group.name == group_name:
                self._groups.pop(i)
                
                # Update tab-to-group mapping for affected tabs
                for tab_name in group.tab_names:
                    del self._tab_to_group[tab_name]
                
                return True
        
        return False
    
    def add_tab_to_group(self, group_name: str, tab_name: str) -> bool:
        """
        Add a tab to an existing group.
        
        Args:
            group_name: Name of the group
            tab_name: Name of the tab to add
            
        Returns:
            True if added successfully
        """
        for group in self._groups:
            if group.name == group_name:
                if tab_name not in group.tab_names:
                    group.tab_names.append(tab_name)
                    self._tab_to_group[tab_name] = group_name
                return True
        
        # Group doesn't exist, create it first
        self.add_group(group_name, f"Auto-created group for {tab_name}", [tab_name])
        return True
    
    def remove_tab_from_group(self, tab_name: str) -> bool:
        """
        Remove a tab from its current group.
        
        Args:
            tab_name: Name of the tab
            
        Returns:
            True if removed successfully
        """
        if tab_name in self._tab_to_group:
            group_name = self._tab_to_group[tab_name]
            
            for group in self._groups:
                if group.name == group_name and tab_name in group.tab_names:
                    group.tab_names.remove(tab_name)
                    del self._tab_to_group[tab_name]
                    
                    # Clean up empty groups
                    if not group.tab_names:
                        self._groups.remove(group)
                    
                    return True
        
        return False
    
    def get_summary(self) -> Dict[str, List[str]]:
        """
        Get a summary of all groups and their tabs.
        
        Returns:
            Dictionary mapping group names to tab lists
        """
        return {group.name: list(group.tab_names) for group in self._groups}
    
    def consolidate_tabs(
        self,
        current_tab_order: List[str],
        max_groups: int = 5
    ) -> Dict[str, List[str]]:
        """
        Automatically consolidate tabs into groups.
        
        Args:
            current_tab_order: Current tab order from the notebook
            max_groups: Maximum number of groups to create
            
        Returns:
            Dictionary mapping group names to tab lists in display order
        """
        # Map tabs to their logical groups
        result = {}
        
        for group_data in self.DEFAULT_GROUPS[:max_groups]:
            matched_tabs = []
            
            for tab_name in current_tab_order:
                if any(
                    tab.lower() in tab_name.lower() or 
                    tab_name.lower() in tab.lower()
                    for tab in group_data.get("tabs", [])
                ):
                    matched_tabs.append(tab_name)
            
            if matched_tabs:
                result[group_data["name"]] = matched_tabs
        
        return result


class TabContainer:
    """
    A container widget that displays tabs grouped by category.
    
    Uses a nested notebook structure where outer notebook contains
    groups and inner notebooks contain the individual tabs within each group.
    """
    
    def __init__(self, parent):
        self.parent = parent
        self.group_manager = TabGroupManager()
        
        # Create main container frame
        self.container = ttk.Frame(parent)
        
        # Outer notebook for groups
        self.outer_notebook = ttk.Notebook(self.container)
        self.outer_notebook.pack(fill=tk.BOTH, expand=True)
    
    def add_group(self, group_name: str, tabs: List[str], content_widgets: Dict[str, tk.Widget]):
        """
        Add a group with its tabs to the container.
        
        Args:
            group_name: Name of the group
            tabs: List of tab names to add
            content_widgets: Dictionary mapping tab names to their widgets
        """
        # Create frame for this group
        group_frame = ttk.Frame(self.outer_notebook)
        
        # Create inner notebook for individual tabs within this group
        inner_notebook = ttk.Notebook(group_frame)
        inner_notebook.pack(fill=tk.BOTH, expand=True)
        
        # Add each tab to the inner notebook
        for tab_name in tabs:
            if tab_name in content_widgets:
                widget = content_widgets[tab_name]
                inner_notebook.add(widget, text=tab_name)
        
        # Add the group frame to the outer notebook
        self.outer_notebook.add(group_frame, text=group_name)


def create_tab_groups() -> TabGroupManager:
    """
    Create a default tab group manager with all standard groups.
    
    Returns:
        Configured TabGroupManager instance
    """
    return TabGroupManager()