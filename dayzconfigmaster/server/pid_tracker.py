# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# PID tracker for DayZ server process management
# Tracks PIDs with timestamps and validates before operations

"""
PID Tracker - State file system to track processes across sessions.

Prevents recycled PID confusion by:
- Storing PIDs with timestamps in state files
- Validating PIDs are still active before operations
- Cleaning up stale entries automatically
"""

import json
import os
import time
import signal
from pathlib import Path
from typing import Dict, Any, Optional, List


class PIDEntry:
    """Represents a tracked process entry."""
    
    def __init__(
        self,
        pid: int,
        name: str,
        start_time: float = None,
        command_line: str = "",
        status: str = "running"
    ):
        """
        Initialize a PID entry.
        
        Args:
            pid: Process ID
            name: Process name/identifier
            start_time: When the process was started (default: now)
            command_line: Full command line used to start the process
            status: Process status (running, stopped, crashed)
        """
        self.pid = pid
        self.name = name
        self.start_time = start_time or time.time()
        self.command_line = command_line
        self.status = status
    
    def to_dict(self) -> Dict[str, Any]:
        """Convert entry to dictionary."""
        return {
            "pid": self.pid,
            "name": self.name,
            "start_time": self.start_time,
            "command_line": self.command_line,
            "status": self.status
        }
    
    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'PIDEntry':
        """Create entry from dictionary."""
        return cls(
            pid=data.get("pid", 0),
            name=data.get("name", ""),
            start_time=data.get("start_time", time.time()),
            command_line=data.get("command_line", ""),
            status=data.get("status", "running")
        )
    
    def is_valid(self, max_age_seconds: float = 86400) -> bool:
        """
        Check if the PID entry is still valid.
        
        Args:
            max_age_seconds: Maximum age of entry (default: 24 hours)
            
        Returns:
            True if entry is not too old
        """
        return time.time() - self.start_time < max_age_seconds


class PIDTracker:
    """
    Tracks PIDs across sessions using state files.
    
    State file format:
    {
        "server_1": {
            "pid": 12345,
            "name": "DayZServer_x64",
            "start_time": 1700000000.0,
            "command_line": "...",
            "status": "running"
        },
        "client_1": {
            ...
        }
    }
    """
    
    def __init__(self, state_dir: str = None):
        """
        Initialize PID tracker.
        
        Args:
            state_dir: Directory to store state files (default: .pids/)
        """
        self.state_dir = Path(state_dir or ".pids")
        self.state_dir.mkdir(parents=True, exist_ok=True)
        self._cache: Dict[str, PIDEntry] = {}
    
    def _get_state_file(self, name: str) -> Path:
        """Get path to state file for a process."""
        return self.state_dir / f"{name}.json"
    
    def track(
        self,
        name: str,
        pid: int,
        command_line: str = "",
        status: str = "running"
    ) -> bool:
        """
        Track a new process.
        
        Args:
            name: Process identifier (e.g., "server_1", "client_a")
            pid: Process ID
            command_line: Full command line used to start the process
            status: Initial status
            
        Returns:
            True if tracking started successfully
        """
        entry = PIDEntry(
            pid=pid,
            name=name,
            command_line=command_line,
            status=status
        )
        
        self._cache[name] = entry
        return self._save_entry(name, entry)
    
    def _save_entry(self, name: str, entry: PIDEntry) -> bool:
        """Save an entry to disk."""
        try:
            state_file = self._get_state_file(name)
            
            with open(state_file, 'w') as f:
                json.dump(entry.to_dict(), f, indent=2)
            
            return True
        except IOError as e:
            print(f"Error saving PID entry: {e}")
            return False
    
    def get(self, name: str) -> Optional[PIDEntry]:
        """
        Get tracked process by name.
        
        Args:
            name: Process identifier
            
        Returns:
            PIDEntry if found and valid, None otherwise
        """
        # Check cache first
        if name in self._cache:
            entry = self._cache[name]
            if entry.is_valid():
                return entry
        
        # Load from disk
        state_file = self._get_state_file(name)
        
        if not state_file.exists():
            return None
        
        try:
            with open(state_file, 'r') as f:
                data = json.load(f)
            
            entry = PIDEntry.from_dict(data)
            
            # Validate PID is still active (optional - can be disabled)
            if self._is_pid_active(entry.pid):
                self._cache[name] = entry
                return entry
            
            # PID not active, but keep entry for reference
            entry.status = "stale"
            self._save_entry(name, entry)
            
            return entry
            
        except (json.JSONDecodeError, IOError) as e:
            print(f"Error loading PID entry: {e}")
            return None
    
    def _is_pid_active(self, pid: int) -> bool:
        """
        Check if a PID is currently active.
        
        Args:
            pid: Process ID to check
            
        Returns:
            True if process exists
        """
        if pid <= 0:
            return False
        
        try:
            # Try sending signal 0 (doesn't affect process, just checks existence)
            os.kill(pid, 0)
            return True
        except OSError:
            return False
    
    def update_status(self, name: str, status: str) -> bool:
        """
        Update the status of a tracked process.
        
        Args:
            name: Process identifier
            status: New status
            
        Returns:
            True if successful
        """
        entry = self.get(name)
        if not entry:
            return False
        
        entry.status = status
        self._cache[name] = entry
        return self._save_entry(name, entry)
    
    def stop(self, name: str) -> bool:
        """
        Mark a process as stopped.
        
        Args:
            name: Process identifier
            
        Returns:
            True if successful
        """
        return self.update_status(name, "stopped")
    
    def is_running(self, name: str) -> bool:
        """
        Check if a tracked process is still running.
        
        Args:
            name: Process identifier
            
        Returns:
            True if process appears to be running
        """
        entry = self.get(name)
        if not entry:
            return False
        
        # Status-based check first
        if entry.status == "stopped":
            return False
        
        # PID-based check (only for recent entries)
        if time.time() - entry.start_time < 3600:  # Within last hour
            return self._is_pid_active(entry.pid)
        
        # For older entries, trust the status
        return True
    
    def get_all_tracked(self) -> Dict[str, PIDEntry]:
        """Get all tracked processes."""
        result = {}
        
        for state_file in self.state_dir.glob("*.json"):
            name = state_file.stem
            
            try:
                with open(state_file, 'r') as f:
                    data = json.load(f)
                
                entry = PIDEntry.from_dict(data)
                
                # Only include active entries
                if self._is_pid_active(entry.pid):
                    result[name] = entry
            except (json.JSONDecodeError, IOError):
                continue
        
        return result
    
    def clear(self, name: str) -> bool:
        """
        Clear a tracked process from state.
        
        Args:
            name: Process identifier
            
        Returns:
            True if successful
        """
        try:
            state_file = self._get_state_file(name)
            
            if state_file.exists():
                state_file.unlink()
            
            if name in self._cache:
                del self._cache[name]
            
            return True
        except IOError as e:
            print(f"Error clearing PID entry: {e}")
            return False
    
    def clear_all(self) -> int:
        """
        Clear all tracked processes.
        
        Returns:
            Number of entries cleared
        """
        count = 0
        
        for state_file in self.state_dir.glob("*.json"):
            try:
                state_file.unlink()
                count += 1
            except IOError:
                continue
        
        self._cache.clear()
        return count
    
    def get_by_pid(self, pid: int) -> Optional[str]:
        """
        Find process name by PID.
        
        Args:
            pid: Process ID
            
        Returns:
            Process name if found, None otherwise
        """
        for state_file in self.state_dir.glob("*.json"):
            try:
                with open(state_file, 'r') as f:
                    data = json.load(f)
                
                if data.get("pid") == pid:
                    return state_file.stem
            except (json.JSONDecodeError, IOError):
                continue
        
        return None


def cleanup_stale_pids(tracker: PIDTracker, max_age_days: float = 7) -> int:
    """
    Remove stale PID entries older than specified age.
    
    Args:
        tracker: PIDTracker instance
        max_age_days: Maximum age in days (default: 7)
        
    Returns:
        Number of entries cleaned up
    """
    count = 0
    now = time.time()
    max_age_seconds = max_age_days * 86400
    
    for state_file in tracker.state_dir.glob("*.json"):
        try:
            with open(state_file, 'r') as f:
                data = json.load(f)
            
            start_time = data.get("start_time", now)
            
            if now - start_time > max_age_seconds:
                state_file.unlink()
                count += 1
                
        except (json.JSONDecodeError, IOError):
            continue
    
    return count