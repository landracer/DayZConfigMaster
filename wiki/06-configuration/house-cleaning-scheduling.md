# House Cleaning & Scheduling

This guide explains how to use the House Cleaning & Scheduling tab for server maintenance, including scheduled restarts and instance storage backups.

## Overview

The **House Cleaning** tab helps you manage server health through:

- 🔄 **Memory Cleanup**: Schedule regular server restarts to flush memory leaks
- 📊 **Performance Stability**: Maintain consistent server performance throughout the day
- 🎮 **Player Experience**: Prevent unexpected crashes during gameplay
- 💾 **Instance Storage Backups**: Backup player data and world progress

## Why Schedule Server Restarts?

DayZ servers accumulate memory leaks over time as:
- Log files grow with each server session
- Player activity and item spawning creates dynamic memory usage
- Server processes hold onto cached data that's not properly released

### Memory Leak Cycle

```
Server Start → Memory: ~500 MB  
↓ (4 hours of gameplay)  
Memory: ~1.2 GB (players joining, items spawned)  
↓ (8+ hours)  
Memory: ~1.8-2.0 GB (near crash threshold)  
↓ (Restart scheduled)  
Memory: ~500 MB (fresh start)
```

### Recommended Restart Schedule

| Server Type | Restart Interval | Memory Management |
|-------------|------------------|-------------------|
| **Standard servers** (1-30 players) | Every 4-6 hours | Prevents memory accumulation |
| **High-population servers** (30+ players) | Every 3-4 hours | More activity = more leaks |
| **Multi-instance setups** ⚠️ | Staggered by 30+ min | Prevents CPU/disk spikes |

## Memory Monitoring

Enable memory logging to track trends over time:

```
Log Memory: 1 (Enabled)
```

With `logMemory = 1` enabled, check `logs/servername_memory.log` for memory usage trends and identify when your server approaches the crash threshold.

### Memory Threshold Settings

Set alerts when the server exceeds specified memory limits:
- **Warning threshold**: 1600 MB (1.5 GB)
- **Critical threshold**: 2400 MB (2.3 GB)

## Restart Scheduling

### Manual vs Automated Restarts

| Method | Pros | Cons |
|--------|------|------|
| **Manual restarts** | Full control, immediate execution | Requires admin presence |
| **Automated via cron** | No admin needed, consistent timing | Setup required |

### Restart Timing Best Practices

```
Instance 1: Every day at 04:00 (4 AM)
Instance 2: Every day at 04:30 (4:30 AM)  
Instance 3: Every day at 05:00 (5:00 AM)
```

This staggered approach:
- Prevents simultaneous Steam CMD logins
- Reduces bandwidth spikes
- Spreads resource usage across time

## Instance Storage Backup & Restore

### What Gets Backed Up?

The storage_1 folder contains critical player data:

| Folder | Contents |
|--------|----------|
| `storage_1/players.db` | Player profiles and character data |
| `storage_1/data/*.bin` | Current world state (buildings, items) |
| `storage_1/backup/*` | Automatic backup snapshots |
| `storage_1/expansion/` | Expansion mod data |

### Scanning for Storage Folders

Click **🔄 Scan Storage Folders** to detect all storage_1 directories:

```
Detected Instance Storage Directories
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Instance  Map        Storage Path
───────── ────────── ───────────────────────────────
server1   enoch      instances/server1/mpmissions/dayzOffline.enoch/storage_1
server2   chernarusplus instances/server2/mpmissions/dayzOffline.chernarusplus/storage_1
server3   namalsk    instances/server3/mpmissions/dayzOffline.namalsk/storage_1
```

### Backup Options

#### 1. Individual Instance Backups (Recommended)

Each instance/map combination gets its own timestamped backup:

```
backups/
├── storage_server1_enoch_20241215_030000/
│   ├── players.db
│   ├── data/
│   └── backup/
└── storage_server2_chernarusplus_20241215_030000/
    ├── players.db
    ├── data/
    └── backup/
```

#### 2. Bundle Server Backup

Backup everything "as-is" for complete server transfer/restore:

```
backups/server_bundle_20241215_030000/
├── instances/
│   ├── server1/
│   ├── server2/
│   └── server3/
└── backups/ (previous backups included)
```

### Perform Backup

1. Scan storage folders first
2. Select backup mode:
   - **Individual**: Each instance has its own backup
   - **Bundle**: Complete server snapshot
3. Click **🚀 Perform Backup**

### Restore from Backup

Future restores will use the backed-up storage_1 folder to restore player data and world state.

## Linux start.sh Generation

Generate a multi-instance startup script for Linux:

```
#!/bin/bash
# DayZ Server Multi-Instance Start Script (Linux)
cd "$(dirname "$0")"

echo "Starting DayZ server instances..."

# Instance 1
export HOME="$PWD/server1/.dayzhome"
...
./server1/DayZServer_instance_1 ...

# Instance 2
export HOME="$PWD/server2/.dayzhome"
...
./server2/DayZServer_instance_2 ...
```

## Best Practices

### Daily Maintenance

- Monitor memory logs for trends
- Restart servers before reaching crash threshold
- Keep backups of storage folders

### Weekly Maintenance

- Review and clean old backup files
- Verify backup integrity
- Check disk space usage

### Monthly Maintenance

- Archive old backups to external storage
- Analyze player data growth patterns
- Update restart intervals based on server performance

## Troubleshooting

### Server Keeps Crashing?

1. Enable `logMemory = 1`
2. Monitor memory log for growth pattern
3. Reduce restart interval if crashes persist
4. Check for mods causing excessive memory usage

### Storage Folder Not Found?

1. Ensure maps have been deployed (creates storage_1)
2. Verify instances folder exists
3. Check permissions on storage directories

## Related Documentation

- [Server Configuration](server-config.md) - Server settings
- [Effects & Triggers](effects-triggers.md) - World configuration
- [Modding](modding.md) - Mod management