# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Server management modules for DayZConfigMaster

"""
Server management package providing:
- Process control for starting/stopping servers
- Instance management for multi-server setups
- Profile/preset system for quick configuration switching
"""

from .process_controller import ProcessController
from .instance_manager import InstanceManager
from .profiles import ProfileManager
from .pid_tracker import PIDTracker, PIDEntry, cleanup_stale_pids

__all__ = [
    'ProcessController',
    'InstanceManager',
    'ProfileManager',
    'PIDTracker',
    'PIDEntry',
    'cleanup_stale_pids'
]