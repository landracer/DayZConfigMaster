# Metrics Module

System resource monitoring for DayZ servers.

## Features
- CPU usage tracking
- Memory (RAM) monitoring
- Process-specific metrics collection
- Time-series data storage

## Usage

```python
from dayzconfigmaster.metrics import MetricsCollector, ProcessMetricsCollector

# System-wide metrics collector
collector = MetricsCollector(interval=10)
collector.start()

metrics = collector.get_current()
print(f"CPU: {metrics.cpu_percent}%")
print(f"Memory: {metrics.memory_mb} MB")

collector.stop()
```

## Data Structure

```python
@dataclass
class MetricsData:
    timestamp: datetime
    cpu_percent: float        # CPU usage percentage
    memory_percent: float     # Memory usage percentage  
    memory_mb: float          # Memory in MB
    processes_count: int      # Number of threads