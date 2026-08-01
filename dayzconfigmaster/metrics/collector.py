# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Metrics collector for DayZ server system resources.

Monitors:
- CPU usage
- Memory (RAM)
- Process statistics
"""

import threading
import time
from dataclasses import dataclass, field
from datetime import datetime
from pathlib import Path
from typing import Dict, List, Optional, Callable


@dataclass
class MetricsData:
    """Metrics snapshot at a point in time."""
    timestamp: datetime
    cpu_percent: float = 0.0
    memory_percent: float = 0.0
    memory_mb: float = 0.0
    processes_count: int = 0
    
    # DayZ-specific metrics
    fps: Optional[int] = None
    players_connected: int = 0
    tps: Optional[float] = None


class MetricsCollector:
    """
    Collect and store system resource metrics for the server.
    
    Features:
    - Background collection thread
    - Time-series data storage
    - Configurable sampling interval
    
    Usage:
        collector = MetricsCollector(interval=10)  # Every 10 seconds
        collector.start()
        
        # Get current metrics
        metrics = collector.get_current()
        
        # Get history (last N samples)
        history = collector.get_history(60)  # Last 60 samples
        
        collector.stop()
    """
    
    def __init__(self, interval: int = 10):
        """
        Initialize the metrics collector.
        
        Args:
            interval: Sampling interval in seconds
        """
        self.interval = interval
        
        # Current/latest metrics
        self._current: Optional[MetricsData] = None
        
        # History of samples (oldest first)
        self._history: List[MetricsData] = []
        self._max_history = 1000  # Maximum samples to keep
        
        # Thread control
        self._thread: Optional[threading.Thread] = None
        self._running = False
        self._lock = threading.Lock()
        
        # Callbacks when new metrics arrive
        self._callbacks: List[Callable[[MetricsData], None]] = []
    
    def start(self):
        """Start the background collection thread."""
        if self._running:
            return
        
        self._running = True
        self._thread = threading.Thread(target=self._collect_loop, daemon=True)
        self._thread.start()
        
        # Collect initial metrics
        self._collect_once()
    
    def stop(self):
        """Stop the background collection thread."""
        self._running = False
        if self._thread:
            self._thread.join(timeout=5)
            self._thread = None
    
    def _collect_loop(self):
        """Main collection loop."""
        while self._running:
            try:
                self._collect_once()
                time.sleep(self.interval)
            except Exception as e:
                print(f"Metrics collection error: {e}")
    
    def _collect_once(self) -> MetricsData:
        """Collect a single metrics snapshot."""
        # Use psutil if available
        try:
            import psutil
            
            # System-wide CPU usage
            cpu_percent = psutil.cpu_percent(interval=0.1)
            
            # Memory info
            mem_info = psutil.virtual_memory()
            memory_percent = mem_info.percent
            memory_mb = mem_info.used / (1024 * 1024)
            
            # Process count
            processes_count = len(psutil.pids())
            
            metrics = MetricsData(
                timestamp=datetime.now(),
                cpu_percent=cpu_percent,
                memory_percent=memory_percent,
                memory_mb=round(memory_mb, 2),
                processes_count=processes_count
            )
            
        except ImportError:
            # Fallback: basic metrics without psutil
            import os
            
            metrics = MetricsData(
                timestamp=datetime.now(),
                cpu_percent=0.0,
                memory_percent=0.0,
                memory_mb=0.0,
                processes_count=1
            )
        
        # Update current and history
        with self._lock:
            self._current = metrics
            self._history.append(metrics)
            
            # Trim history if too large
            while len(self._history) > self._max_history:
                self._history.pop(0)
            
            # Notify callbacks
            for callback in self._callbacks:
                try:
                    callback(metrics)
                except Exception as e:
                    print(f"Callback error: {e}")
        
        return metrics
    
    def get_current(self) -> Optional[MetricsData]:
        """Get the most recent metrics."""
        with self._lock:
            return self._current
    
    def get_history(self, count: int = 100) -> List[MetricsData]:
        """
        Get historical metrics.
        
        Args:
            count: Number of samples to retrieve (oldest first)
            
        Returns:
            List of MetricsData objects
        """
        with self._lock:
            return list(self._history[-count:])
    
    def get_stats(self) -> Dict[str, float]:
        """Get statistics about collected metrics."""
        with self._lock:
            if not self._history:
                return {
                    'sample_count': 0,
                    'avg_cpu': 0.0,
                    'avg_memory': 0.0
                }
            
            cpu_values = [m.cpu_percent for m in self._history]
            mem_values = [m.memory_percent for m in self._history]
            
            return {
                'sample_count': len(self._history),
                'avg_cpu': sum(cpu_values) / len(cpu_values),
                'max_cpu': max(cpu_values),
                'avg_memory': sum(mem_values) / len(mem_values),
                'max_memory': max(mem_values)
            }
    
    def add_callback(self, callback: Callable[[MetricsData], None]):
        """
        Add a callback that's called when new metrics arrive.
        
        Args:
            callback: Function taking MetricsData as argument
        """
        self._callbacks.append(callback)
    
    def clear_history(self):
        """Clear the history of collected samples."""
        with self._lock:
            self._history.clear()


class ProcessMetricsCollector:
    """
    Collect metrics for a specific process (e.g., DayZ server).
    """
    
    def __init__(self, process_id: Optional[int] = None):
        """
        Initialize process metrics collector.
        
        Args:
            process_id: PID of the process to monitor
                      (None means auto-detect by name)
        """
        self.process_id = process_id
        self._process = None
        
        try:
            import psutil
            if process_id:
                try:
                    self._process = psutil.Process(process_id)
                except psutil.NoSuchProcess:
                    pass
        except ImportError:
            pass
    
    def set_process_id(self, pid: int):
        """Set the process ID to monitor."""
        self.process_id = pid
        try:
            import psutil
            self._process = psutil.Process(pid)
        except (ImportError, psutil.NoSuchProcess):
            self._process = None
    
    def get_metrics(self) -> Optional[MetricsData]:
        """
        Get current metrics for the monitored process.
        
        Returns:
            MetricsData or None if process not found
        """
        try:
            import psutil
            
            if self._process is None:
                # Try to find by PID
                if self.process_id:
                    try:
                        self._process = psutil.Process(self.process_id)
                    except psutil.NoSuchProcess:
                        return None
                else:
                    return None
            
            cpu_percent = self._process.cpu_percent(interval=0.1)
            
            mem_info = self._process.memory_info()
            memory_mb = mem_info.rss / (1024 * 1024)
            
            # Get number of threads
            try:
                threads = self._process.num_threads()
            except Exception:
                threads = 0
            
            return MetricsData(
                timestamp=datetime.now(),
                cpu_percent=cpu_percent,
                memory_percent=mem_info.percent,
                memory_mb=round(memory_mb, 2),
                processes_count=threads,
                fps=None,
                players_connected=0,
                tps=None
            )
            
        except (ImportError, psutil.NoSuchProcess):
            return None
        except Exception as e:
            print(f"Process metrics error: {e}")
            return None
    
    def find_by_name(self, name: str) -> Optional[int]:
        """
        Find a process by name and set it for monitoring.
        
        Args:
            name: Process name to find
            
        Returns:
            PID if found, None otherwise
        """
        try:
            import psutil
            
            for proc in psutil.process_iter(['pid', 'name']):
                try:
                    if proc.info['name'] and name.lower() in proc.info['name'].lower():
                        self.set_process_id(proc.info['pid'])
                        return proc.info['pid']
                except (psutil.NoSuchProcess, psutil.AccessDenied):
                    continue
            
            return None
            
        except ImportError:
            return None