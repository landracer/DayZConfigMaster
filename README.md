# DayZConfigMaster

SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
https://github.com/landracer/DayZConfigMaster/LICENSE

A comprehensive cross-platform DayZ server configuration editor with full feature parity to dayz-labs.

## Overview

This software is governed by the SOVEREIGN INDIVIDUAL LICENSE (see LICENSE file for details).

DayZConfigMaster is a Python-based GUI application designed to provide a complete solution for configuring DayZ server settings. It supports all major DayZ configuration files and provides an intuitive interface for managing server parameters, spawn limits, trader configurations, and more.

This project has been ported from dayz-labs (.NET/WPF) with cross-platform support and enhanced features including:

- **25+ CLI commands** matching dayz-labs functionality
- **Robust Workshop API** with Steam Web API integration
- **Cross-platform process management** (Windows, Linux, macOS)

## Features

### Configuration Management
- **Complete DayZ Configuration Support**:
  - Economy Core (economycore.xml)
  - Limits Definitions (limitsdefinitions.xml) with Categories/Usages/Values
  - Random Presets (cfgrandompresets.xml)
  - Spawnable Types (cfgspawnabletypes.xml)
  - Trader Configuration (traderconfig.txt)
  - Main Server Configuration (serverDZ.cfg)
  - Gameplay Settings (cfggameplay.json)
  - Environmental Settings (cfgenvironment.xml)

- **Multi-Instance Server Management**:
  - Run multiple server instances on same hardware
  - Configure different maps per instance (chernarusplus, enoch, namalsk, etc.)
  - Automatic port allocation for each instance
  - Isolated storage/database paths per instance

- **House-Cleaning & Scheduling**:
  - Memory leak management guidance
  - Restart scheduling with staggered instances
  - Memory monitoring with logMemory = 1
  - Linux start.sh generation for multi-instance setups

### Modding & Workshop Integration
- **Mod Configuration**:
  - Mod configuration wizard
  - Steam Workshop integration support
  - Popular mod list with requirements
  - Signature verification settings

- **Workshop Cleanup** (`Cleanup Unsubscribed` button):
  - Removes stale workshop folders for mods you no longer use
  - Uses a two-pass detection strategy: first checks Steam's `.acf` subscription file, then falls back to comparing against your GUI's active mod list (handles the case where Steam hasn't synced the ACF file yet)
  - Active/selected mods are always protected from deletion
  - See the [Server Config — Mods Tab](wiki/06-configuration/server-config-mods.md#cleanup-unsubscribed---how-it-works) wiki page for full details and best practices.

- **Workshop API (NEW)**:
  - `SteamWorkshopAPI` - Robust Steam Web API client
  - `WorkshopManager` - Unified download interface using SteamCMD and browser fallback

### Advanced DayZ Features
- **Full Category Support**: weapons, tools, clothes, containers, food, vehiclesparts, explosives
- **Complete Usage Definitions**: Military, Police, Hunting, Town, Village, Farm, Industrial, Medic
- **Tier-based Value System**: Tier1 to Tier4 with proper zone mappings
- **Advanced Trader Configuration** with custom categories
- **Server Performance and Gameplay Settings**
- **Environmental Control Parameters**

### User-Friendly Interface
- Tabbed interface for easy navigation
- Comprehensive treeview displays for structured data
- Text editors for complex configurations
- Form-based input for server settings
- Advanced settings tabs with categorized controls
- Input validation and user guidance

## Server Control (Unified)

The **Server Control** tab is the central hub for all DayZ server management. It is organized into inner tabs:

| Inner Tab | Description |
|-----------|-------------|
| **General Settings** | Single Server and Multi-Instance start/stop/restart controls |
| **Mod Selection** | Per-instance mod path configuration |
| **Ban List & Players** | RCon-based player management and bans |
| **Logs & Diagnostics** | Real-time log viewer and automated issue detection |

**Key Features:**
- 🌐 Automatic map discovery from stock and workshop directories
- 🔍 Real-time log viewer with multiple log types (script, rpt, adm, client)
- 🔍 Diagnostics analysis for common server issues
- 🚀 Start/stop/restart controls for single or multiple instances
- 🗂️ Maps and mods are automatically separated in the Server Config tab

## Command Line Interface

DayZConfigMaster includes a powerful CLI with 25+ subcommands:

```bash
# Server management
dzl server start --instance myserver
dzl server stop --instance myserver
dzl server status --instance myserver
dzl server logs --tail --lines 100

# Build system
dzl build prebuild -m my_mod
dzl build build -m my_mod --clean
dzl build sign -m my_mod

# Workshop management (NEW)
dzl workshop download --id 123456789
dzl workshop search "mod name"
dzl workshop auth

# Instance management
dzl instance create --name production --port 2304
dzl instance list
dzl instance enable --name staging

# Configuration
dzl config get --key max_players
dzl config set --key max_players --value 100
```

### Full CLI Command List

| Category | Commands |
|----------|----------|
| **Server** | start, stop, restart, status, logs (5 commands) |
| **Build** | prebuild, build, sign, cache (4 commands) |
| **Config** | get, set, list, validate (4 commands) |
| **Workshop** | download, update, search, auth (4 commands) |
| **Instance** | create, delete, enable, disable, list (5 commands) |
| **Preset** | save, load, list, delete (4 commands) |

## Requirements

- Python 3.8+
- Tkinter (usually included with Python)
- requests library for workshop API
- GitPython (optional - for git integration)

### Installation

```bash
cd /path/to/DayzConfigMaster
python3 -m pip install -r requirements.txt
python3 main.py
```

## Development Setup

1. Clone the repository:
```bash
git clone https://github.com/landracer/DayZConfigMaster.git
cd DayZConfigMaster
```

2. Install dependencies:
```bash
pip install -r requirements.txt
```

3. Run the application:
```bash
python3 main.py
```

### Running CLI Without GUI

```bash
# Check for updates
dzl update check

# Workshop download with verbose output
dzl workshop download --id 123456789 --visible

# System info in JSON format
dzl utils info --json
```

## Project Structure

```
DayzConfigMaster/
├── dayzconfigmaster/    # Core package
│   ├── config/         # Configuration parsing and validation
│   ├── gui/            # GUI components and tabs
│   ├── server/         # Process and instance management
│   ├── build/          # PBO building and caching
│   ├── workshop/       # Steam Workshop API integration
│   ├── cli/            # Command-line interface (dzl command)
│   ├── git/            # Git management via GitPython
│   ├── economy/        # Central Economy file parsers
│   └── utils/          # Utility functions
├── main.py             # Main GUI application entry point
├── README.md           # This file
└── requirements.txt    # Python dependencies
```

### New Modules (Ported from dayz-labs)

| Module | Description |
|--------|-------------|
| `server/process_controller.py` | Cross-platform process management for server/client |
| `server/instance_manager.py` | Multi-instance configuration with JSON storage |
| `server/pid_tracker.py` | State file based PID tracking across sessions |
| `workshop/api.py` | Robust Steam Web API + SteamCMD downloads (NEW) |
| `build/cache.py` | Content-hash based build caching for skips |
| `mcp/server.py` | JSON-RPC server with named pipes/Unix sockets |

## How to Use

1. Run the application: `python3 main.py`
2. Navigate through the tabs to access different configuration sections:
   - **Workspace**: Overview and quick links to documentation
   - **In-Game Settings**: Economy Core, Limits Definitions, Random Presets, Spawnable Types
   - **Trader Config**: Trader configurations
  - **Server Config**: Main server configuration (serverDZ.cfg), map/mod selection, workshop integration, and per-mod Mod Settings editor
   - **Advanced Settings**: Gameplay and environment settings
   - **Server Control**: Single/multi-instance server management, mod selection, bans, logs
   - **House Cleaning & Scheduling**: Memory management and restart scheduling
   - **Mod Builder**: PBO packing and mod creation tools
   - **Workshop**: Search and download Steam Workshop items

3. Edit settings using the provided controls
4. Use the **Tools** menu for validation and .cfg generation
5. Save your configurations using the **File** menu

## Workshop Integration (NEW)

### Steam Web API Integration

The new workshop module includes:

- **SteamWorkshopAPI**: Uses public endpoints for item details and search
- **SteamCMD integration**: Anonymous workshop downloads
- **WorkshopManager**: Unified interface with browser fallback for manual subscription

```python
from dayzconfigmaster.workshop.api import WorkshopManager, SteamWorkshopAPI

# Initialize manager
manager = WorkshopManager("/path/to/projects")

# Search for items
success, results = manager.search("CUP Units")
for item in results:
    print(f"{item.id}: {item.title}")

# Download an item
success, message = manager.download_item(123456789)
```

## Contributing

This project is designed to be easily extensible for developers. The codebase follows a modular structure that makes it simple to add new features or configuration sections.

### Adding New Configuration Sections

To add support for additional DayZ configuration files:
1. Create a new tab method in `main.py`
2. Add the necessary UI elements and data handling
3. Implement any required validation logic
4. Test thoroughly with existing configurations

## API Reference

### Core Classes

| Class | Description |
|-------|-------------|
| `DzlConfig` | Configuration manager with global + instance precedence |
| `ProcessController` | Cross-platform process start/stop/restart |
| `InstanceManager` | Multi-server instance configuration and management |
| `BuildCache` | Content-hash based build caching |
| `GitManager` | Git operations via GitPython |

### Workshop API Classes

| Class | Description |
|-------|-------------|
| `SteamWorkshopAPI` | Steam Web API client with GetPublishedFileDetails endpoint |
| `WorkshopManager` | Unified workshop interface with SteamCMD and browser fallback |

## License

This project is licensed under the SOVEREIGN INDIVIDUAL LICENSE Version 1.0 - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

This tool is based on extensive documentation from DZconfig.com and other DayZ server references, providing comprehensive support for all major DayZ server configuration options.

### Ported from dayz-labs
This Python port maintains full feature parity with the original .NET application while adding:
- Cross-platform compatibility (Windows, Linux, macOS)
- Enhanced workshop API integration
- Improved build caching with content hashing