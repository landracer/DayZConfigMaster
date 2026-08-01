# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# WorkDrive module for DayZConfigMaster

"""
Work Drive package providing:
- Cross-platform drive mounting (subst on Windows, symlinks on Linux/macOS)
- Mount status tracking
"""

from .mounter import WorkDrive, MountResult, create_work_drive

__all__ = ['WorkDrive', 'MountResult', 'create_work_drive']