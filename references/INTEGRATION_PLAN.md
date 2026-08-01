# DayZ Server Manager Integration Plan for DayzConfigMaster

## Overview
This document outlines the integration of key features from dayz-server-manager (TypeScript/Node.js) into DayzConfigMaster (Python).

## Target Features to Integrate

### 1. Discord Bot Integration
**From**: `src/services/discord.ts` + `src/interface/discord-message-handler.ts`

**Features**:
- Discord bot for remote server management
- Commands: status, players, bans, kick, ban, messages
- Role-based permissions (admin/manager/moderator/viewer)
- Automatic server status updates

**Implementation in DayzConfigMaster**:
- New module: `discord/bot.py` - Discord bot integration
- New module: `discord/handler.py` - Command handlers
- Updated GUI tab for Discord configuration
- Python-discord library integration

### 2. Event Scheduler (CRON-based)
**From**: `src/services/events.ts` + `src/config/config.ts` (Event class)

**Features**:
- CRON expression scheduling for events
- Event types: restart, message, kickAll, lock, unlock, backup
- Configurable event parameters

**Implementation in DayzConfigMaster**:
- New module: `scheduler/cron_scheduler.py` - CRON-based scheduler
- Updated server configuration UI with scheduled events tab
- Schedule server restarts, messages, backups via GUI

### 3. Hooks System
**From**: `src/config/config.ts` (Hook class) + hooks service

**Features**:
- beforeStart hook - run custom scripts before server starts
- afterStart hook - run after server is ready
- missionChanged hook - run when mission files change

**Implementation in DayzConfigMaster**:
- New module: `hooks/manager.py` - Hook execution manager
- Updated configuration UI with hooks section
- Support for Linux/bash and Windows/batch scripts

### 4. RCon Integration
**From**: `src/services/rcon.ts`

**Features**:
- RCon protocol implementation
- Player list and management (kick, ban)
- Global messages via RCon
- Server status queries

**Implementation in DayzConfigMaster**:
- New module: `rcon/client.py` - RCon client library
- Updated server control tab with RCon features
- Real-time player list display
- Player management commands

### 5. Metrics Collection
**From**: `src/services/metrics.ts` + `src/services/metrics-collector.ts`

**Features**:
- CPU usage monitoring
- Memory (RAM) usage tracking
- Server FPS logging
- Network statistics

**Implementation in DayzConfigMaster**:
- Enhanced process controller with metrics
- New GUI tab for real-time metrics display
- Historical data storage for trend analysis

### 6. Backups with Scheduling
**From**: `src/services/backups.ts`

**Features**:
- Scheduled backups via events
- Configurable backup retention (max age)
- Backup path configuration

**Implementation in DayzConfigMaster**:
- New module: `backups/manager.py` - Backup manager
- Integrated into scheduler for automated backups
- GUI configuration for backup settings

## Implementation Priority

### Phase 1: Core Server Management Features
1. **Event Scheduler** (High Priority)
   - Already has some infrastructure in House Cleaning tab
   - Extend with CRON syntax support
   - Integration with existing restart functionality

2. **Backups** (High Priority)
   - New backup manager module
   - Scheduled backups via scheduler
   - GUI configuration for backup settings

3. **RCon Integration** (Medium Priority)
   - RCon client library
   - Player management in server control tab
   - Better server monitoring

### Phase 2: Advanced Features
4. **Discord Bot** (Medium Priority)
   - Discord bot integration
   - Remote command execution
   - Status notifications

5. **Hooks System** (Low Priority)
   - Script hook execution manager
   - Configuration UI
   - Support for custom pre/post scripts

6. **Metrics Dashboard** (Low Priority)
   - Real-time resource monitoring
   - Historical data storage
   - Performance trends

## New Directory Structure

```
dayzconfigmaster/
├── discord/                    # NEW: Discord bot integration
│   ├── __init__.py
│   ├── bot.py                 # Discord bot main class
│   └── handler.py             # Command handlers
├── scheduler/                  # NEW: CRON-based scheduler
│   ├── __init__.py
│   ├── cron_scheduler.py      # Main scheduler with CRON parsing
│   └── events.py              # Event types and configurations
├── hooks/                      # NEW: Hook execution manager
│   ├── __init__.py
│   └── manager.py             # Hook execution logic
├── rcon/                       # NEW: RCon client library
│   ├── __init__.py
│   └── client.py              # RCon protocol implementation
├── metrics/                    # NEW: Metrics collection
│   ├── __init__.py
│   ├── collector.py           # System resource monitoring
│   └── storage.py             # Historical data storage
└── backups/                    # NEW: Backup manager
    ├── __init__.py
    └── manager.py             # Backup creation and management
```

## Implementation Status

### Completed (Phase 1)
- [x] Set up new directories with `__init__.py`
- [x] Created `scheduler/cron_scheduler.py` - CRON-based event scheduler with:
  - Event types: restart, message, kickAll, lock, unlock, backup
  - CRON expression parsing and validation
  - Scheduled execution loop with threading
- [x] Created `backups/manager.py` - Backup management with:
  - Create backups with configurable names
  - Cleanup old backups based on retention policy
  - List and restore backups
- [x] Created `rcon/client.py` - RCon client with:
  - Connection management
  - Authentication
  - Player management (kick, ban)
  - Global messaging
  - Server shutdown
- [x] Created `hooks/manager.py` - Hook execution system with:
  - Hook types: beforeStart, afterStart, missionChanged
  - Script configuration and execution
  - Result tracking
- [x] Created `metrics/collector.py` - Metrics collection with:
  - CPU usage monitoring
  - Memory (RAM) monitoring
  - Time-series data storage
  - Process-specific metrics collection

### To Be Completed
1. **Discord Bot** - Complete Discord integration with discord.py library
2. **GUI Integration** - Add new features to main application GUI
3. **Integration Testing** - Test all new modules together

### Step 3: Integrate with GUI (Pending)
1. Add Discord configuration tab to preferences
2. Extend House Cleaning tab with scheduler events
3. Enhance Server Control tab with RCon features

### Step 4: Advanced Features (Pending)
1. Discord bot integration
2. Hooks system
3. Metrics dashboard

## Dependencies to Install

```bash
pip install discord.py psutil python-dotenv
```

## Configuration Format (server-manager.json equivalent)

Add this section to the existing DayZ config system:

```json
{
  "discord": {
    "botToken": "",
    "channels": [
      {"channel": "server-commands", "mode": "admin"},
      {"channel": "rcon-relay", "mode": "rcon"}
    ]
  },
  "events": [
    {
      "name": "Daily Restart",
      "type": "restart",
      "cron": "0 4 * * *"
    },
    {
      "name": "Hourly Message",
      "type": "message",
      "cron": "0 * * * *",
      "params": ["Server restart in 5 minutes"]
    }
  ],
  "hooks": [
    {
      "type": "beforeStart",
      "program": "/path/to/script.sh"
    }
  ],
  "rcon": {
    "password": "rcon_password",
    "port": 2306
  },
  "metrics": {
    "collectInterval": 10,
    "storeDays": 7
  },
  "backups": {
    "path": "./backups",
    "maxAgeDays": 7
  }
}
```

## Next Steps

1. Review and approve this integration plan
2. Create branch for integration work
3. Implement Phase 1 features (Event Scheduler, Backups)
4. Test each feature before moving to next phase
5. Document new functionality in wiki