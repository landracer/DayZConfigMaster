# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
CRON-based event scheduler for DayZ server management.

Supports scheduling of:
- Server restarts
- Global messages
- Backups
- And more custom events
"""

import re
import threading
import time
from datetime import datetime, timedelta
from enum import Enum
from pathlib import Path
from typing import Callable, Dict, List, Optional, Tuple, Any
from dataclasses import dataclass, field

# CRON expression parts: minute hour day month weekday
CRON_PATTERN = re.compile(
    r'^(\*|[0-9,-/]+)\s+(\*|[0-9,-/]+)\s+(\*|[0-9,-/]+)\s+(\*|[0-9,-/]+)\s+(\*|[0-9,-/]+)$'
)


class EventType(Enum):
    """Types of scheduled events."""
    RESTART = 'restart'           # Restart the server
    MESSAGE = 'message'           # Send global message
    KICK_ALL = 'kickAll'          # Kick all players
    LOCK = 'lock'                 # Lock server (prevent new connections)
    UNLOCK = 'unlock'             # Unlock server
    BACKUP = 'backup'             # Create a backup


@dataclass
class CronEvent:
    """A scheduled CRON event."""
    name: str                     # Event display name
    cron: str                     # CRON expression (5 parts)
    event_type: EventType         # Type of action to perform
    
    # Optional parameters for the event
    params: List[str] = field(default_factory=list)  # Message text, etc.
    
    # State tracking
    last_run: Optional[datetime] = None
    next_run: Optional[datetime] = None
    enabled: bool = True
    run_count: int = 0


class EventScheduler:
    """
    CRON-based event scheduler for DayZ server management.
    
    Features:
    - Parse and evaluate CRON expressions
    - Schedule events at specific times/intervals
    - Execute configured actions when events trigger
    
    Example CRON expressions:
        "0 4 * * *"      - Every day at 4:00 AM
        "0 */4 * * *"    - Every 4 hours
        "30 2 * * 1"     - Every Monday at 2:30 AM
        "*/15 * * * *"   - Every 15 minutes
    """
    
    def __init__(self, projects_root: str):
        """
        Initialize the event scheduler.
        
        Args:
            projects_root: Root directory for project files and backups
        """
        self.projects_root = Path(projects_root)
        self.events_dir = self.projects_root / "scheduler_events.json"
        
        # Events managed by this scheduler
        self._events: List[CronEvent] = []
        
        # Callbacks for event actions
        self._callbacks: Dict[EventType, Callable] = {}
        
        # Thread management
        self._thread: Optional[threading.Thread] = None
        self._running = False
        self._stop_event = threading.Event()
        
        # Check interval (how often to check for events)
        self.check_interval = 30  # seconds
        
    def load_events(self, config_path: Optional[str] = None) -> List[CronEvent]:
        """
        Load events from configuration file.
        
        Args:
            config_path: Path to JSON config file with event definitions
            
        Returns:
            List of configured events
        """
        self._events.clear()
        
        # Default config path if not provided
        if config_path is None:
            config_path = str(self.events_dir)
        
        config_file = Path(config_path)
        if config_file.exists():
            try:
                import json
                with open(config_file, 'r') as f:
                    data = json.load(f)
                
                events_data = data.get('events', [])
                for event_data in events_data:
                    event = self._event_from_dict(event_data)
                    if event:
                        self._events.append(event)
                        
            except Exception as e:
                print(f"Error loading events: {e}")
        
        # Calculate next run times
        self._calculate_next_runs()
        
        return self._events
    
    def _event_from_dict(self, data: Dict[str, Any]) -> Optional[CronEvent]:
        """Create a CronEvent from dictionary data."""
        try:
            event_type = EventType(data.get('type', 'restart'))
            
            # Validate CRON expression
            cron = data.get('cron', '')
            if not self._validate_cron(cron):
                print(f"Invalid CRON expression: {cron}")
                return None
            
            params = data.get('params', [])
            if isinstance(params, str):
                params = [params]
            
            return CronEvent(
                name=data.get('name', 'Unnamed Event'),
                cron=cron,
                event_type=event_type,
                params=params,
                enabled=data.get('enabled', True),
                run_count=data.get('run_count', 0)
            )
        except Exception as e:
            print(f"Error creating event: {e}")
            return None
    
    def save_events(self, config_path: Optional[str] = None) -> bool:
        """
        Save events to configuration file.
        
        Args:
            config_path: Path to JSON config file
            
        Returns:
            True if successful
        """
        try:
            # Build event list
            events_data = []
            for event in self._events:
                events_data.append({
                    'name': event.name,
                    'cron': event.cron,
                    'type': event.event_type.value,
                    'params': event.params,
                    'enabled': event.enabled,
                    'run_count': event.run_count
                })
            
            # Save to file
            if config_path is None:
                config_path = str(self.events_dir)
            
            import json
            with open(config_path, 'w') as f:
                json.dump({'events': events_data}, f, indent=2)
                
            return True
        except Exception as e:
            print(f"Error saving events: {e}")
            return False
    
    def _validate_cron(self, cron: str) -> bool:
        """Validate a CRON expression."""
        match = CRON_PATTERN.match(cron.strip())
        if not match:
            return False
        
        # Validate each part
        parts = cron.split()
        
        # Minute: 0-59
        try:
            self._parse_cron_field(parts[0], 0, 59)
        except ValueError:
            return False
        
        # Hour: 0-23
        try:
            self._parse_cron_field(parts[1], 0, 23)
        except ValueError:
            return False
        
        # Day: 1-31
        try:
            self._parse_cron_field(parts[2], 1, 31)
        except ValueError:
            return False
        
        # Month: 1-12
        try:
            self._parse_cron_field(parts[3], 1, 12)
        except ValueError:
            return False
        
        # Weekday: 0-6 (Sunday=0 or 7)
        try:
            self._parse_cron_field(parts[4], 0, 7)
        except ValueError:
            return False
        
        return True
    
    def _parse_cron_field(self, field: str, min_val: int, max_val: int) -> List[int]:
        """
        Parse a single CRON field.
        
        Supports: *, n, */n, n-m, n,m,..., n-m/s
        """
        if field == '*':
            return list(range(min_val, max_val + 1))
        
        result = []
        
        for part in field.split(','):
            # Handle step values (*/n or n-m/s)
            if '/' in part:
                range_part, step = part.split('/')
                step = int(step)
                
                if range_part == '*':
                    values = list(range(min_val, max_val + 1))
                elif '-' in range_part:
                    start, end = map(int, range_part.split('-'))
                    values = list(range(start, end + 1))
                else:
                    raise ValueError(f"Invalid step format: {part}")
                
                result.extend(values[::step])
            
            # Handle ranges (n-m)
            elif '-' in part:
                start, end = map(int, part.split('-'))
                if not (min_val <= start <= max_val and min_val <= end <= max_val):
                    raise ValueError(f"Value out of range: {part}")
                result.extend(range(start, end + 1))
            
            # Handle single values
            else:
                value = int(part)
                if not (min_val <= value <= max_val):
                    raise ValueError(f"Value out of range: {value}")
                result.append(value)
        
        return sorted(set(result))
    
    def _calculate_next_runs(self):
        """Calculate the next run time for all enabled events."""
        now = datetime.now()
        
        for event in self._events:
            if not event.enabled:
                continue
            
            # Parse CRON parts
            cron_parts = event.cron.split()
            minutes = self._parse_cron_field(cron_parts[0], 0, 59)
            hours = self._parse_cron_field(cron_parts[1], 0, 23)
            days = self._parse_cron_field(cron_parts[2], 1, 31)
            months = self._parse_cron_field(cron_parts[3], 1, 12)
            weekdays = self._parse_cron_field(cron_parts[4], 0, 7)
            
            # Find next match
            future_time = now + timedelta(days=1)  # Start looking from tomorrow
            
            for day_offset in range(365):  # Look up to a year ahead
                check_date = (now + timedelta(days=day_offset)).date()
                
                if check_date.day not in days:
                    continue
                if check_date.month not in months:
                    continue
                
                # Check weekday (only if specific day not set)
                if cron_parts[2] != '*' and cron_parts[4] != '*':
                    check_weekday = check_date.weekday() + 1  # Monday=0, Sunday=6
                    # In CRON: Sunday=0 or 7
                    check_weekday_cron = check_weekday % 7
                    if check_weekday_cron not in weekdays and 7 not in weekdays:
                        continue
                
                for hour in hours:
                    for minute in minutes:
                        try:
                            next_run = datetime(
                                year=check_date.year,
                                month=check_date.month,
                                day=check_date.day,
                                hour=hour,
                                minute=minute
                            )
                            
                            if next_run > now:
                                event.next_run = next_run
                                break
                        except ValueError:
                            continue
                    else:
                        continue
                    break
                else:
                    continue
                break
    
    def set_callback(self, event_type: EventType, callback: Callable):
        """
        Set a callback function for an event type.
        
        Args:
            event_type: The event type to register
            callback: Function to call when event triggers
                      Signature: callback(event: CronEvent) -> None
        """
        self._callbacks[event_type] = callback
    
    def add_event(self, name: str, cron: str, event_type: EventType,
                  params: Optional[List[str]] = None, enabled: bool = True) -> CronEvent:
        """
        Add a new scheduled event.
        
        Args:
            name: Display name for the event
            cron: CRON expression (5 parts)
            event_type: Type of action to perform
            params: Additional parameters for the action
            enabled: Whether the event is active
            
        Returns:
            The created CronEvent
        """
        if not self._validate_cron(cron):
            raise ValueError(f"Invalid CRON expression: {cron}")
        
        event = CronEvent(
            name=name,
            cron=cron,
            event_type=event_type,
            params=params or [],
            enabled=enabled
        )
        
        self._events.append(event)
        self._calculate_next_runs()
        
        return event
    
    def remove_event(self, name: str) -> bool:
        """Remove an event by name."""
        for i, event in enumerate(self._events):
            if event.name == name:
                del self._events[i]
                self._calculate_next_runs()
                return True
        return False
    
    def toggle_event(self, name: str) -> CronEvent:
        """Toggle an event's enabled state."""
        for event in self._events:
            if event.name == name:
                event.enabled = not event.enabled
                self._calculate_next_runs()
                return event
        raise ValueError(f"Event not found: {name}")
    
    def get_next_event(self) -> Optional[CronEvent]:
        """Get the next event to execute."""
        enabled_events = [e for e in self._events if e.enabled]
        
        if not enabled_events:
            return None
        
        # Find event with earliest next_run
        now = datetime.now()
        next_events = [
            e for e in enabled_events 
            if e.next_run is not None and e.next_run <= now
        ]
        
        if not next_events:
            return None
        
        return min(next_events, key=lambda e: e.next_run)
    
    def start(self):
        """Start the scheduler thread."""
        self._running = True
        self._stop_event.clear()
        self._thread = threading.Thread(target=self._run_loop, daemon=True)
        self._thread.start()
    
    def stop(self):
        """Stop the scheduler thread."""
        self._running = False
        self._stop_event.set()
        if self._thread:
            self._thread.join(timeout=5)
    
    def _run_loop(self):
        """Main scheduler loop."""
        while self._running and not self._stop_event.is_set():
            try:
                # Check for events to run
                event = self.get_next_event()
                
                if event and event.next_run is not None:
                    now = datetime.now()
                    
                    # Check if we should run (allow some tolerance)
                    time_to_run = (event.next_run - now).total_seconds()
                    
                    if time_to_run <= 5:  # Within 5 seconds of scheduled time
                        self._execute_event(event)
                        event.last_run = datetime.now()
                        event.run_count += 1
                        
                        # Recalculate next run
                        self._calculate_next_runs()
                
                # Wait before next check
                self._stop_event.wait(self.check_interval)
                
            except Exception as e:
                print(f"Scheduler error: {e}")
                import traceback
                traceback.print_exc()
    
    def _execute_event(self, event: CronEvent):
        """Execute a scheduled event."""
        callback = self._callbacks.get(event.event_type)
        
        if callback:
            try:
                callback(event)
                print(f"Executed event: {event.name} (type: {event.event_type.value})")
            except Exception as e:
                print(f"Error executing event {event.name}: {e}")
        else:
            print(f"No callback registered for event type: {event.event_type.value}")


class BackupManager:
    """
    Manage server backups with scheduled creation and retention policies.
    
    Integrates with EventScheduler to create automated backup events.
    """
    
    def __init__(self, projects_root: str):
        """
        Initialize the backup manager.
        
        Args:
            projects_root: Root directory containing the DayZ installation
        """
        self.projects_root = Path(projects_root)
        self.backup_dir = self.projects_root / "backups"
        self.backup_retention_days = 7
        
        # Create backup directory if it doesn't exist
        self.backup_dir.mkdir(parents=True, exist_ok=True)
    
    def create_backup(self, name: Optional[str] = None) -> Tuple[bool, str]:
        """
        Create a server backup.
        
        Args:
            name: Optional backup name (defaults to timestamp)
            
        Returns:
            Tuple of (success, message/path)
        """
        if not self.backup_dir.exists():
            self.backup_dir.mkdir(parents=True)
        
        # Generate backup name
        if name is None:
            timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
            name = f"backup_{timestamp}"
        
        backup_path = self.backup_dir / name
        
        try:
            import shutil
            
            # Find DayZ server directory (usually parent of profiles)
            dayz_server = self.projects_root / "DayZServer"
            
            if not dayz_server.exists():
                return False, "DayZServer directory not found"
            
            # Backup the entire server directory
            shutil.copytree(
                str(dayz_server),
                str(backup_path),
                symlinks=True,
                ignore=shutil.ignore_patterns('*.log', '*.rpt')
            )
            
            return True, str(backup_path)
            
        except Exception as e:
            return False, f"Backup failed: {e}"
    
    def cleanup_old_backups(self, max_age_days: Optional[int] = None):
        """
        Remove backups older than specified days.
        
        Args:
            max_age_days: Maximum age in days (default: use instance setting)
        """
        if max_age_days is None:
            max_age_days = self.backup_retention_days
        
        cutoff_date = datetime.now() - timedelta(days=max_age_days)
        
        for backup_path in self.backup_dir.iterdir():
            if backup_path.is_dir():
                try:
                    # Check modification time
                    mtime = datetime.fromtimestamp(backup_path.stat().st_mtime)
                    
                    if mtime < cutoff_date:
                        import shutil
                        shutil.rmtree(backup_path)
                        print(f"Removed old backup: {backup_path.name}")
                        
                except Exception as e:
                    print(f"Error checking backup {backup_path}: {e}")
    
    def list_backups(self) -> List[Path]:
        """List all available backups."""
        if not self.backup_dir.exists():
            return []
        
        return [p for p in self.backup_dir.iterdir() if p.is_dir()]
    
    def get_backup_size(self, backup_name: str) -> int:
        """Get the size of a specific backup in bytes."""
        backup_path = self.backup_dir / backup_name
        
        if not backup_path.exists():
            return 0
        
        import shutil
        total = 0
        
        for path in backup_path.rglob('*'):
            if path.is_file():
                try:
                    total += path.stat().st_size
                except OSError:
                    continue
        
        return total
    
    def restore_backup(self, backup_name: str) -> Tuple[bool, str]:
        """
        Restore a server from backup.
        
        Args:
            backup_name: Name of the backup to restore
            
        Returns:
            Tuple of (success, message)
        """
        backup_path = self.backup_dir / backup_name
        
        if not backup_path.exists():
            return False, f"Backup not found: {backup_name}"
        
        dayz_server = self.projects_root / "DayZServer"
        
        try:
            import shutil
            
            # Remove current installation (backup first?)
            if dayz_server.exists():
                shutil.rmtree(dayz_server)
            
            # Restore from backup
            shutil.copytree(str(backup_path), str(dayz_server))
            
            return True, f"Restored from backup: {backup_name}"
            
        except Exception as e:
            return False, f"Restore failed: {e}"


def create_default_events() -> List[Dict[str, Any]]:
    """Create default event configurations."""
    return [
        {
            'name': 'Daily Restart at 4 AM',
            'cron': '0 4 * * *',
            'type': 'restart'
        },
        {
            'name': 'Message Every Hour',
            'cron': '0 * * * *',
            'type': 'message',
            'params': ['Server maintenance in 5 minutes']
        }
    ]