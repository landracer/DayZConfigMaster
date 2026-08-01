# Scheduler Module

CRON-based event scheduler for DayZ server management.

## Features
- CRON expression scheduling for events
- Event types: restart, message, kickAll, lock, unlock, backup
- Configurable event parameters
- Background thread execution

## Usage

```python
from dayzconfigmaster.scheduler import EventScheduler, EventType

# Create scheduler
scheduler = EventScheduler("/path/to/projects")
scheduler.load_events()

# Add a scheduled event (daily restart at 4 AM)
event = scheduler.add_event(
    name="Daily Restart",
    cron="0 4 * * *",
    event_type=EventType.RESTART,
    enabled=True
)

# Set callback for restart events
def on_restart(event):
    print(f"Restarting server: {event.name}")

scheduler.set_callback(EventType.RESTART, on_restart)

# Start the scheduler
scheduler.start()

# ... later ...
scheduler.stop()
```

## CRON Expression Format

`minute hour day month weekday`

Examples:
- `0 4 * * *` - Every day at 4:00 AM
- `0 */4 * * *` - Every 4 hours
- `30 2 * * 1` - Every Monday at 2:30 AM
- `*/15 * * * *` - Every 15 minutes

## BackupManager

```python
from dayzconfigmaster.scheduler import EventScheduler, EventType
from dayzconfigmaster.backups import BackupManager

scheduler = EventScheduler("/path/to/projects")
backup_manager = BackupManager("/path/to/projects")

# Set callback for backup events
def on_backup(event):
    success, path = backup_manager.create_backup()
    print(f"Backup created: {path}")

scheduler.set_callback(EventType.BACKUP, on_backup)