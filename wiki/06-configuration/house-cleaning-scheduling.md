# House-Cleaning & Scheduling

This guide covers server restart scheduling and memory leak management for DayZ servers.

## Why Schedule Server Restarts?

DayZ servers accumulate memory leaks over time, causing RAM usage to gradually increase until the server crashes. Scheduled restarts are essential for:

### Benefits of Regular Restarts

• **🔄 Memory Cleanup**: Flush accumulated memory leaks each restart
• **📊 Performance Stability**: Maintain consistent performance throughout the day  
• **🎮 Player Experience**: Prevent unexpected crashes during gameplay
• **💾 Database Integrity**: Regular restarts help maintain clean database state

## Recommended Restart Intervals

| Server Type | Restart Interval | Reason |
|-------------|-----------------|--------|
| Standard (10-30 players) | Every 4-6 hours | Balanced memory management |
| High-population (30-50 players) | Every 3-4 hours | More activity = faster RAM growth |
| Multi-instance setups | Staggered by 30+ min | Prevent simultaneous resource spikes |

## Memory Monitoring

### Enable Memory Logging

Add to your `serverDZ.cfg`:

```cpp
logMemory = 1;
```

This creates a memory log file at: `logs/servername_memory.log`

### Memory Kill Limit (Watchdog)

DayzConfigMaster can automatically kill a runaway server before it freezes the
host. In **Server Control → General Settings → Single Server**, set the
**Memory Kill Limit (GB)** field to a value slightly below your system's
available RAM (default: `15` GB).

When the server process tree exceeds this limit, DayzConfigMaster:

1. Logs the violation to `<instance_root>/logs/memory_watchdog.log`.
2. Terminates the entire process tree (main binary + `enfmain` children).
3. Updates the GUI status bar and log viewer.

The watchdog runs in a background thread, so it works even if the GUI becomes
unresponsive.

### Monitor for Memory Issues

When memory usage consistently hits limits before the scheduled restart:
- Reduce player count per instance
- Remove resource-heavy mods
- Consider fewer maps running simultaneously
- Increase server RAM allocation
- Review `<instance_root>/logs/memory_watchdog.log` to find when growth starts

## Multi-Instance Staggered Restarts

For multiple instances on same hardware, offset restart times to prevent:

• **CPU Spikes**: Multiple servers restarting simultaneously
• **Disk I/O Bottlenecks**: All instances writing logs at once
• **Steam CMD Conflicts**: Simultaneous Steam login attempts

### Recommended Schedule

| Instance | Restart Time |
|----------|--------------|
| Server 1 | Every day at 4:00 AM |
| Server 2 | Every day at 4:30 AM |
| Server 3 | Every day at 5:00 AM |

### Using cron for Linux

```bash
# Add to crontab -e
# Instance 1: Restart daily at 4:00 AM
0 4 * * * /path/to/restart.sh 1 >> /var/log/dayz-restarts.log 2>&1

# Instance 2: Restart daily at 4:30 AM  
30 4 * * * /path/to/restart.sh 2 >> /var/log/dayz-restarts.log 2>&1

# Instance 3: Restart daily at 5:00 AM
0 5 * * * /path/to/restart.sh 3 >> /var/log/dayz-restarts.log 2>&1
```

## Memory Usage Guidelines

| RAM Usage | Action Required |
|-----------|-----------------|
| < 70% before restart | Normal operation |
| 70-90% before restart | Monitor closely, consider shorter interval |
| > 90% before restart | Reduce player count or mods immediately |

### Linux vs Windows Memory Overhead

• **Linux (headless)**: ~500 MB OS overhead
• **Windows Server**: 2-4 GB OS overhead

For multi-instance setups, Linux is preferred due to lower memory overhead.

## Best Practices Summary

1. **Enable logMemory = 1** to track daily memory trends
2. **Set restart interval** based on observed RAM growth patterns
3. **Stagger multiple instances** by at least 30 minutes between restarts
4. **Monitor logs** after each restart for issues
5. **Adjust mod load** if instances consistently hit RAM limits
6. **Test changes gradually** before applying to production servers

## Linux start.sh Template with Restart Logic

```bash
#!/bin/bash
# DayZ Server with Auto-Restart (Linux)

while true; do
    echo "$(date): Starting server instance..."
    
    ./dayzserver -port=2302 -config=serverDZ.cfg \
        -profiles=./profile -instanceId=1 \
        -mod="@ModPack"
    
    echo "$(date): Server stopped, restarting in 5 seconds..."
    sleep 5
done
```

Use with cron to restart at specific times instead of infinite loop.