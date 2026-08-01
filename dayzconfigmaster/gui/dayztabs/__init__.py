# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# DayZ configuration tabs

"""
DayZ configuration tab modules providing GUI components for each config type.

This module provides:
- BaseTab: Base class for all tab components
- ScrollableFrame: Scrollable frame with canvas and scrollbars
- WorkspaceTab: Overview and welcome screen
"""

from .base_tab import BaseTab, ScrollableFrame
from .workspace_tab import WorkspaceTab

__all__ = [
    "BaseTab",
    "ScrollableFrame",
    "WorkspaceTab",
]
