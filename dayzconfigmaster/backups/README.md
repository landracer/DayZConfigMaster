# Backups Module

Backup manager for DayZ server.

## Features
- Create backups of server installation
- Configurable retention policies
- Automatic cleanup of old backups
- List and restore backups

## Usage

```python
from dayzconfigmaster.backups import BackupManager

# Create backup manager
backup_mgr = BackupManager("/path/to/projects")

# Create a backup
success, path = backup_mgr.create_backup()
print(f"Backup created: {path}")

# Create a backup with custom name
success, path = backup_mgr.create_backup("pre_update_backup")
print(f"Backup created: {path}")

# List all backups
backups = backup_mgr.list_backups()
for backup in backups:
    size = backup_mgr.get_backup_size(backup.name)
    print(f"{backup.name}: {size} bytes")

# Cleanup old backups (7 days default)
backup_mgr.cleanup_old_backups()

# Restore a backup
success, msg = backup_mgr.restore_backup("backup_20240101")
print(msg)
```

## Configuration

Backups are stored in the `backups/` subdirectory of the projects root.

### Scheduled Backups

To schedule backups using the EventScheduler:

```python
from dayzconfigmaster.scheduler import EventScheduler, EventType
from dayzconfigmaster.backups import BackupManager

scheduler = EventScheduler("/path/to/projects")
backup_mgr = BackupManager("/path/to/projects")

def on_backup(event):
    success, path = backup_mgr.create_backup()
    print(f"Backup: {success} - {path}")

scheduler.set_callback(EventType.BACKUP, on_backup)

# Add a daily backup at 3 AM
scheduler.add_event(
    name="Daily Backup",
    cron="0 3 * * *",
    event_type=EventType.BACKUP,
    enabled=True
)
```

## CRON Examples for Scheduled Backups

- `0 3 * * *` - Every day at 3:00 AM
- `0 3 * * 0` - Every Sunday at 3:00 AM (weekly)
- `0 3 1 * *` - First of each month at 3:00 AM (monthly)