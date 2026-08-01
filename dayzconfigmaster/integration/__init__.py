# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Integration module for DayZ server management.

Connects all modules together:
- EventScheduler with ProcessController restarts
- Backups with scheduled events
- RCon client with Discord commands
"""

from .manager import ServerManager, create_default_servers

__all__ = ['ServerManager', 'create_default_servers']