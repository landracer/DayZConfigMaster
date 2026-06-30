# DayZConfigMaster

A comprehensive cross-platform DayZ server configuration editor.

## Overview

DayZConfigMaster is a Python-based GUI application designed to provide a complete solution for configuring DayZ server settings. It supports all major DayZ configuration files and provides an intuitive interface for managing server parameters, spawn limits, trader configurations, and more.

## Features

- **Complete DayZ Configuration Support**:
  - Economy Core (economycore.xml)
  - Limits Definitions (limitsdefinitions.xml) with Categories/Usages/Values
  - Random Presets (cfgrandompresets.xml)
  - Spawnable Types (cfgspawnabletypes.xml)
  - Trader Configuration (traderconfig.txt)
  - Main Server Configuration (serverDZ.cfg)
  - Gameplay Settings (cfggameplay.json)
  - Environmental Settings (cfgenvironment.xml)

- **Advanced DayZ Features**:
  - Full category support (weapons, tools, clothes, containers, food, vehiclesparts, explosives)
  - Complete usage definitions (Military, Police, Hunting, Town, Village, Farm, Industrial, Medic, etc.)
  - Tier-based value system (Tier1 to Tier4 with proper zone mappings)
  - Advanced trader configuration with custom categories
  - Server performance and gameplay settings
  - Environmental control parameters

- **User-Friendly Interface**:
  - Tabbed interface for easy navigation
  - Comprehensive treeview displays for structured data
  - Text editors for complex configurations
  - Form-based input for server settings
  - Advanced settings tabs with categorized controls
  - Input validation and user guidance

## Requirements

- Python 3.6+
- Tkinter (usually included with Python)

## Installation

```bash
cd /path/to/DayzConfigMaster
python3 main.py
```

## Development Setup

1. Clone the repository:
```bash
git clone https://github.com/landracer/DayZConfigMaster.git
cd DayZConfigMaster
```

2. Run the application:
```bash
python3 main.py
```

## Project Structure

```
DayzConfigMaster/
├── __init__.py          # Package initialization
├── main.py              # Main GUI application
├── README.md           # This file
└── setup.py            # Setup script
```

## How to Use

1. Run the application: `python3 main.py`
2. Navigate through the tabs to access different configuration sections
3. Edit settings using the provided controls
4. Use the "Tools" menu for validation and .cfg generation
5. Save your configurations using the File menu

## Contributing

This project is designed to be easily extensible for developers. The codebase follows a modular structure that makes it simple to add new features or configuration sections.

### Adding New Configuration Sections

To add support for additional DayZ configuration files:
1. Create a new tab method in `main.py`
2. Add the necessary UI elements and data handling
3. Implement any required validation logic
4. Test thoroughly with existing configurations

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

This tool is based on extensive documentation from DZconfig.com and other DayZ server references, providing comprehensive support for all major DayZ server configuration options.# DayZConfigMaster
