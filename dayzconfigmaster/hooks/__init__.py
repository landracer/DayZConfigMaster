# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Hooks manager module for DayZConfigMaster.

Provides hook execution system:
- beforeStart hooks - run before server starts
- afterStart hooks - run after server is ready
- missionChanged hooks - run when mission files change
"""

from .manager import HookManager, HookType

__all__ = ['HookManager', 'HookType']