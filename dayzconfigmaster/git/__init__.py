# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Git module for DayZConfigMaster

"""
Git management package providing:
- Repository initialization
- Status, commit, push, pull operations
"""

from .manager import GitManager

__all__ = ['GitManager']