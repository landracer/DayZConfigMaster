# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Auto-update package for DayZConfigMaster

"""
Auto-update module providing self-updating functionality.

Features:
- Check for GitHub releases
- Download and verify updates
- Install updates silently or with user confirmation
- Velopack integration support (Windows native updater)
"""

from .manager import UpdateManager, check_for_updates

__all__ = ['UpdateManager', 'check_for_updates']