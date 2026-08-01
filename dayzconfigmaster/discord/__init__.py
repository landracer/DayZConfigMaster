# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Discord bot integration module for DayZConfigMaster.

Provides Discord integration for:
- Remote server management via commands
- Status updates and notifications
- Player list queries
"""

from .bot import DiscordBot, UserLevel

__all__ = ['DiscordBot', 'UserLevel']