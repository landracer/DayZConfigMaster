# Multi-Instance Server Manager

This guide explains how to run multiple DayZ server instances on the same hardware using different map configurations.

> **GUI Workflow:** In DayzConfigMaster, configure instances on the **Server Config → Multi-Instance** tab (enable multi-instance, set count, fill per-instance ports/maps/mods), then start/stop them on the **Server Control** tab. The control rows mirror the config rows 1:1.
>
> The **Multi-Instance** tab is scrollable, so all instance rows remain reachable on low-resolution screens.

## Overview

DayZ servers can run multiple instances simultaneously using the same executable files. Each instance uses:
- Unique configuration file (serverDZ_1.cfg, serverDZ_2.cfg, etc.)
- Different port set (Game, Query, Steam)
- Separate instanceId values
- Isolated profile/database paths
- Its own `keys/`, `@Mod` symlinks, and sanitized `ban.txt`/`whitelist.txt`
- Strict one-start-at-a-time governance (the GUI prevents starting an already-running instance)

DayzConfigMaster automatically prepares each instance directory before startup: it symlinks base server files, sanitizes userlists, deploys mods and `.bikey` files, and ensures the correct mission folder is available. It also kills any orphaned DayZ processes for that instance before starting. See [Server Control](server-control.md) for details on each preparation step.

## Port Configuration

Each instance requires unique ports to prevent conflicts:

| Instance | Game Port | Query Port | Steam P2P Port |
|----------|-----------|------------|----------------|
| 1 | 2302 | 2303 | 2304 |
| 2 | 2402 | 2403 | 2404 |
| 3 | 2502 | 2503 | 2504 |

Port formula: `Base + (instance_num * 100)`

## Linux startup.sh Example

```bash
#!/bin/bash
# DayZ Server Multi-Instance Start Script (Linux)

cd "$(dirname "$0")"

echo "Starting DayZ server instances..."

# Instance 1
nohup ./dayzserver -port=2302 -config=serverDZ_1.cfg \
    -profiles=./profile1 -instanceId=1 \
    -mod="@ModPack" > logs/server1.log 2>&1 &
echo "Instance 1 started on port 2302"

# Instance 2
nohup ./dayzserver -port=2402 -config=serverDZ_2.cfg \
    -profiles=./profile2 -instanceId=2 \
    -mod="@ModPack" > logs/server2.log 2>&1 &
echo "Instance 2 started on port 2402"

# Instance 3
nohup ./dayzserver -port=2502 -config=serverDZ_3.cfg \
    -profiles=./profile3 -instanceId=3 \
    -mod="@ModPack" > logs/server3.log 2>&1 &
echo "Instance 3 started on port 2502"

echo "All instances started successfully!"
wait
```

## Windows Batch File Example

```batch
@echo off
REM Instance 1
start "" "DayZServer_x64.exe" -config=serverDZ_1.cfg -port=2302 ^
    -profiles=C:\DayZ_Server1\Profile -instanceId=1 -mod=@ModPack

REM Instance 2  
start "" "DayZServer_x64.exe" -config=serverDZ_2.cfg -port=2402 ^
    -profiles=C:\DayZ_Server2\Profile -instanceId=2 -mod=@ModPack
```

## Critical: Storage Isolation

Each instance MUST use a separate storage/database folder to prevent data corruption:

- Instance 1: `/profile/serverName/1/data` (Player A on Map 1)
- Instance 2: `/profile/serverName/2/data` (Player A on Map 2)

When using `instanceId` in serverDZ.cfg, the game automatically creates:
- Separate player data per instance
- Independent building changes per instance

## Recommended Maps for Multi-Instance

| Map Name | Description |
|----------|-------------|
| chernarusplus | Standard map with cities and countryside |
| enoch | Livonia - mountainous terrain with forests |
| namalsk | Cold climate island with abandoned facilities |
| provinggrounds_pmc | PMC training grounds with urban layout |
| tavi | Mountainous region in northern Tanoa |
| malden | Island with varied terrain |
| sara | Agricultural terrain in southern Tanoa |

## Memory Requirements

Running multiple instances requires careful RAM management:
- Each instance typically uses 1.5-3 GB RAM
- Minimum: 16 GB RAM for 2 instances
- Recommended: 32+ GB RAM for 3+ instances
- Use `logMemory = 1` in serverDZ.cfg to monitor usage

## Per-Instance Effects & Triggers

In DayzConfigMaster, each instance can have its own **Effects & Triggers** configuration files. On the **Server Config → Files** tab, open the **Effects & Triggers** inner tab and use the **Instance** dropdown to select the instance you want to edit.

Files maintained per instance:

- `cfglimitsdefinitionuser.xml`
- `cfgeventspawns.xml`
- `cfgeffectarea.json`
- `cfgundergroundtriggers.json`
- `mapclusterproto.xml`
- `mapgroupdirt.xml`
- `events.xml`
- `globals.xml`
- `messages.xml`

When you generate configs, the output includes files like:

```
serverDZ_1.cfg
cfglimitsdefinitionuser_1.xml
cfgeffectarea_1.json
serverDZ_2.cfg
cfglimitsdefinitionuser_2.xml
cfgeffectarea_2.json
```

This lets each map/instance have custom spawn limits, event areas, underground triggers, and messages without sharing global settings.

## Key Configuration Parameters

```cpp
// In serverDZ.cfg, each instance needs unique values:
instanceId = 1;              // Unique per instance (1-9)
port = 2302;                 // Game port (unique per instance)
queryPort = 2303;            // Query port (port + 1)
 Steam port = 2304;          // Steam P2P port (port + 2)
profiles = "./profile1";     // Unique profile path