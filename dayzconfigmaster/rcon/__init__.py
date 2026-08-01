# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
RCon client module for DayZConfigMaster.

Provides RCon protocol implementation for server management:
- Player list and management (kick, ban)
- Global messages
- Server status queries
"""

from .client import RConClient, RConError

__all__ = ['RConClient', 'RConError']