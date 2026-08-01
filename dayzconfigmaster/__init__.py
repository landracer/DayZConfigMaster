# DayZConfigMaster - DayZ Server Configuration Editor Package
# 
# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
DayZConfigMaster: A comprehensive, cross-platform DayZ server configuration editor.

This package provides:
- Modular GUI built with tkinter (cross-platform compatible)
- XML/JSON/TXT parser for DayZ config files
- Data models for economy, limits, presets, and trader configurations
- Validation rules for all DayZ server settings

New Features (Ported from DayZ Labs):
- Process control for starting/stopping servers (Python subprocess)
- Instance management for multi-server setups (JSON configuration)
- Profile/preset system for quick configuration switching
- PBO building via AddonBuilder CLI wrapper
- Mod discovery from configured paths
- SteamCMD integration for Workshop downloads
- Git management via GitPython
- Log tailing and diagnostics engine

Cross-Platform Support:
- Windows: Full native support with all features
- Linux: Full support (with work drive symlinks instead of subst)
- macOS: Full support (with work drive symlinks)

This software is governed by the SOVEREIGN INDIVIDUAL LICENSE. See LICENSE file for details.
"""

from .config.parser import ConfigParser, detect_config_type
from .config.validator import FieldValidator
from .config.models import (
    EconomyConfig,
    LimitsConfig,
    RandomPresetConfig,
    SpawnableTypeConfig,
    TraderConfig,
    ServerConfig
)

# New modules ported from DayZ Labs
from .server.process_controller import ProcessController
from .server.instance_manager import InstanceManager, ServerInstance
from .server.profiles import ProfileManager

from .mods.pbo_builder import PboBuilder
from .mods.discovery import ModDiscovery, ModInfo, ModLoader, ModDeployer

from .workshop.steamcmd import BaseSteamCmd as SteamCmd, ConsoleVisibleSteamCmd
from .workshop.browser import WorkshopBrowser, WorkshopItem

from .git.manager import GitManager

from .logs.tailer import LogTailer
from .logs.diagnostics import DiagnosticsEngine, DiagnosticRule

from .build.service import BuildService
from .build.preflight import PreflightEngine

# New features (port from DayZ Labs)
from .config.dzl_config import DzlConfig, create_default_config
from .server.pid_tracker import PIDTracker, PIDEntry, cleanup_stale_pids
from .build.cache import BuildCache
from .workdrive.mounter import WorkDrive, MountResult
from .workshop.steamcmd import ConsoleVisibleSteamCmd, BaseSteamCmd
from .workshop.api import SteamWorkshopAPI, WorkshopManager, WorkshopItemInfo
from .mcp.server import MCPServer, create_server
from .update.manager import UpdateManager, check_for_updates

__all__ = [
    # Original exports
    "ConfigParser",
    "FieldValidator", 
    "EconomyConfig",
    "LimitsConfig",
    "RandomPresetConfig",
    "SpawnableTypeConfig",
    "TraderConfig",
    "ServerConfig",
    "ProcessController",
    "InstanceManager",
    "ServerInstance",
    "ProfileManager",
    "PboBuilder",
    "ModDiscovery",
    "ModInfo",
    "ModLoader",
    "ModDeployer",
    "SteamCmd",
    "WorkshopBrowser",
    "WorkshopItem",
    "GitManager",
    "LogTailer",
    "DiagnosticsEngine",
    "DiagnosticRule",
    "BuildService",
    "PreflightEngine",
    # New features
    "DzlConfig",
    "create_default_config",
    "PIDTracker",
    "PIDEntry",
    "cleanup_stale_pids",
    "BuildCache",
    "WorkDrive",
    "MountResult",
    "ConsoleVisibleSteamCmd",
    "BaseSteamCmd",
    "MCPServer",
    "create_server",
    # Workshop API (new)
    "SteamWorkshopAPI",
    "WorkshopManager",
    "WorkshopItemInfo",
    # Auto-update
    "UpdateManager",
    "check_for_updates",
]

__version__ = "2.1.0"