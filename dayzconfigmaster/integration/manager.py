# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Server Manager - Main integration point for all DayZ server management features.

This module connects all the subsystems:
- EventScheduler -> ProcessController restarts
- Backups with scheduled events  
- RCon client with player management
- Discord bot with commands
- Metrics collection
"""

import json
from pathlib import Path
from typing import Optional, Dict, List

try:
    from ..scheduler.cron_scheduler import EventScheduler, EventType, CronEvent, BackupManager
except ImportError:
    from scheduler.cron_scheduler import EventScheduler, EventType, CronEvent, BackupManager

try:
    from ..rcon.client import RConClient
except ImportError:
    from rcon.client import RConClient

try:
    from ..hooks.manager import HookManager, HookType
except ImportError:
    from hooks.manager import HookManager, HookType

try:
    from ..metrics.collector import MetricsCollector, ProcessMetricsCollector
except ImportError:
    from metrics.collector import MetricsCollector, ProcessMetricsCollector


class ServerManager:
    """
    Central manager for DayZ server management features.
    
    Integrates:
    - EventScheduler for scheduled tasks (restarts, backups, messages)
    - ProcessController for server control
    - RConClient for player management
    - HookManager for lifecycle hooks
    - MetricsCollector for system monitoring
    
    Usage:
        manager = ServerManager("/path/to/projects")
        manager.load_config()
        
        # Register callbacks to the scheduler
        manager.setup_callbacks()
        
        # Start everything
        manager.start()
        
        # ... later ...
        manager.stop()
    """
    
    def __init__(self, projects_root: str):
        """
        Initialize the server manager.
        
        Args:
            projects_root: Root directory for project files
        """
        self.projects_root = Path(projects_root)
        self.config_path = self.projects_root / "server_config.json"
        
        # Modules
        self.scheduler: Optional[EventScheduler] = None
        self.backup_manager: Optional[BackupManager] = None
        self.hook_manager: Optional[HookManager] = None
        self.metrics_collector: Optional[MetricsCollector] = None
        
        # Process controller (set externally)
        self.process_controller = None
        
        # RCon client (optional)
        self.rcon_client: Optional[RConClient] = None
        
        # Discord bot (optional)
        self.discord_bot = None
        
    def load_config(self, config_path: Optional[str] = None):
        """
        Load configuration and initialize modules.
        
        Args:
            config_path: Path to JSON config file
        """
        if config_path is None:
            config_path = str(self.config_path)
        
        # Initialize scheduler
        self.scheduler = EventScheduler(str(self.projects_root))
        
        # Try to load events from file
        self.scheduler.load_events()
        
        # Initialize backup manager
        self.backup_manager = BackupManager(str(self.projects_root))
        
        # Initialize hook manager
        self.hook_manager = HookManager(str(self.projects_root))
        self.hook_manager.load_hooks()
        
        # Initialize metrics collector (optional)
        try:
            import psutil  # Check if available
            self.metrics_collector = MetricsCollector(interval=10)
        except ImportError:
            self.metrics_collector = None
        
    def setup_callbacks(self):
        """Set up callbacks to connect modules."""
        if not self.scheduler or not self.process_controller:
            return
        
        # Restart callback
        def on_restart(event: CronEvent):
            if self.hook_manager:
                results = self.hook_manager.execute_before_start()
                for result in results:
                    if not result.success:
                        print(f"Hook failed: {result.error}")
            
            success, msg = self.process_controller.restart_server()
            print(f"Server restart: {success} - {msg}")
            
            if self.hook_manager:
                self.hook_manager.execute_after_start()
        
        # Message callback
        def on_message(event: CronEvent):
            msg = " ".join(event.params) if event.params else "Scheduled message"
            print(f"Sending message: {msg}")
        
        # Backup callback  
        def on_backup(event: CronEvent):
            if self.backup_manager:
                success, path = self.backup_manager.create_backup()
                print(f"Backup: {success} - {path}")
        
        # Register callbacks
        self.scheduler.set_callback(EventType.RESTART, on_restart)
        self.scheduler.set_callback(EventType.MESSAGE, on_message)
        self.scheduler.set_callback(EventType.BACKUP, on_backup)
    
    def start(self):
        """Start all managed services."""
        if self.metrics_collector:
            self.metrics_collector.start()
        
        if self.scheduler:
            self.scheduler.start()
        
        print("ServerManager started")
    
    def stop(self):
        """Stop all managed services."""
        if self.scheduler:
            self.scheduler.stop()
        
        if self.metrics_collector:
            self.metrics_collector.stop()
        
        print("ServerManager stopped")
    
    def get_status(self) -> Dict[str, any]:
        """Get current server status."""
        status = {
            "running": False,
            "cpu_percent": 0.0,
            "memory_mb": 0.0
        }
        
        if self.process_controller:
            process_status = self.process_controller.get_status()
            status["running"] = process_status.get("server_running", False)
        
        if self.metrics_collector:
            metrics = self.metrics_collector.get_current()
            if metrics:
                status["cpu_percent"] = metrics.cpu_percent
                status["memory_mb"] = metrics.memory_mb
        
        return status
    
    def add_event(self, name: str, cron: str, event_type: EventType,
                  params: Optional[List[str]] = None):
        """Add a scheduled event."""
        if self.scheduler:
            return self.scheduler.add_event(name, cron, event_type, params)
        return None
    
    def create_backup(self) -> tuple:
        """Create a backup."""
        if self.backup_manager:
            return self.backup_manager.create_backup()
        return False, "Backup manager not initialized"
    
    def get_backups(self) -> List[Path]:
        """List available backups."""
        if self.backup_manager:
            return self.backup_manager.list_backups()
        return []
    
    def execute_hook(self, hook_type: HookType):
        """Execute hooks of a specific type."""
        if self.hook_manager:
            return self.hook_manager.execute_hook(hook_type)
        return []


def create_default_servers(projects_root: str) -> List[Dict]:
    """
    Create default server configurations.
    
    Args:
        projects_root: Root directory for projects
        
    Returns:
        List of default server config dicts
    """
    servers = []
    
    # Default server configuration
    default_server = {
        "name": "default",
        "enabled": True,
        "port": 2302,
        "query_port": 2303,
        "max_players": 60,
        "map_size": 2000,
        "dayz_path": str(Path(projects_root) / "DayZServer"),
        "profiles_path": str(Path(projects_root) / "profiles")
    }
    
    servers.append(default_server)
    return servers