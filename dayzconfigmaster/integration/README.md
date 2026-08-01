# Integration Module

Central integration point for all DayZ server management features.

## ServerManager

The main class that connects all subsystems:
- **EventScheduler** - Scheduled tasks (restarts, backups, messages)
- **ProcessController** - Server start/stop/restart
- **RConClient** - Player management and commands
- **HookManager** - Lifecycle hooks (beforeStart, afterStart, missionChanged)
- **MetricsCollector** - System resource monitoring

## Usage

```python
from dayzconfigmaster.integration import ServerManager
from dayzconfigmaster.server.process_controller import ProcessController

# Initialize manager
manager = ServerManager("/path/to/projects")
manager.load_config()

# Set up process controller (your existing controller)
process_controller = ProcessController("/path/to/DayZServer")
manager.process_controller = process_controller

# Connect all callbacks
manager.setup_callbacks()

# Start the manager
manager.start()

# ... run your server ...

# Stop when done
manager.stop()
```

## Features

| Feature | Description |
|---------|-------------|
| Scheduled Restarts | CRON-based server restarts |
| Automated Backups | Periodic backups with retention |
| Lifecycle Hooks | Run scripts at key moments |
| Metrics Collection | CPU, memory, process monitoring |

## Dependencies

Install optional features:

```bash
pip install discord.py psutil python-dotenv
```

- `psutil` - Required for metrics collection
- `discord.py` - For Discord bot integration