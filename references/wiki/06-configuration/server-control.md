# Server Control

This guide explains how to use the unified Server Control tab for managing DayZ servers, including single and multi-instance configurations.

## Overview

The **Server Control** tab is your central hub for DayZ server runtime management. It combines:

- 🚀 Start/stop/restart single DayZ servers
- 🚀 Start/stop/restart multiple server instances simultaneously
- 👮 RCon-based player management and bans
- 🔍 Real-time log viewing from multiple log types (script, rpt, adm, client)
- 🔍 Pattern-matched diagnostics for common server issues
- 🌐 Automatic map discovery from stock and workshop directories

> **Note:** Per-instance configuration (ports, maps, mods, instance IDs) is now managed on the **Server Config** tab. The Server Control tab displays read-only control rows that mirror the configuration rows 1:1, so you always start/stop exactly what you configured.

### Key Features

| Feature | Description |
|---------|-------------|
| **Unified Interface** | Single tab handles server control, mod selection, bans, and logs |
| **Map Auto-Discovery** | Automatically scans the `mpmissions` directory and Steam Workshop content for available maps |
| **Instance Management** | Configure up to 9 instances with unique ports per instance |
| **Mod Selection** | Per-instance mod path configuration |
| **Log Viewer** | Real-time log monitoring with multiple log type options |
| **Diagnostics** | Automated issue detection from server logs |

## Tab Structure

The Server Control tab uses an inner notebook with four main views:

```
Server Control
├── General Settings     → Single Server and Multi-Instance start/stop/restart
├── Mod Selection        → Choose mods per instance
├── Mod Integration      → Merge mod XML fragments into mission files
├── Ban List & Players   → RCon player list and ban management
└── Logs & Diagnostics   → Live log viewer and automated diagnostics
```

The **General Settings** tab contains a further sub-notebook for **Single Server** and **Multi-Instance** control. Multi-instance rows shown here are read-only copies of the rows configured on the **Server Config** tab.

## DayZ Installation Paths

Before configuring any servers, set your DayZ installation paths:

### DayZ Server Path
- **Description**: Path to your DayZ server executables
- **Required Files**: `DayZServer_x64` (production) or `DayZDiag_x64` (debug)
- **Example Linux**: `/home/user/.local/share/Steam/steamapps/common/DayZServer`
- **Example Windows**: `C:\Program Files\Steam\steamapps\common\DayZServer`

### DayZ Tools Path
- **Description**: Path to DayZ Tools for AddonBuilder (optional)
- **Used For**: PBO building and mod creation

> 💡 **Tip**: Use the "Browse" buttons next to each path field to select directories visually.

---

## Single Server Configuration

The Single Server tab is designed for managing one server instance with simple configuration options.

> **Note:** The single-server path now uses a dedicated instance directory at `~/Documents/DayZProjects/instances/default` (or your configured projects root). This isolates profiles, configs, mods, keys, and mission folders from the base DayZ server installation, just like multi-instance servers.

### Configuration Parameters

| Parameter | Description | Default | Range |
|-----------|-------------|---------|-------|
| **Mode** | Debug or normal mode | `debug` | debug, normal |
| **Port** | Game port number | `2302` | 1024-65535 |
| **Map Name** | Server map selection (auto-detected) | - | Auto-populated |
| **Map Size** | Map dimensions in meters | `2000` | 1000-100000 |
| **Max Players** | Maximum concurrent players | `60` | 1-1000 |
| **Config File** | Path to serverDZ.cfg (optional) | - | Browse to select |
| **Mod Paths** | Semicolon-separated mod list | - | Add via Browse |

### Automatic Instance Preparation

Before starting any server, DayzConfigMaster prepares the instance directory automatically:

1. **Base server files** — symlinks `dayz.gproj`, `addons/`, `dta/`, `battleye/`, server binaries, etc.
2. **Sanitized userlists** — strips comments and invalid entries from `ban.txt` and `whitelist.txt`.
3. **Mod deployment** — creates `@Mod` symlinks and copies `.bikey` signature keys into `keys/`.
4. **Mission folder** — symlinks the correct `dayzOffline.<world>` mission folder from the base game or workshop map.

See the sections below for details on each step.

### Map Auto-Discovery

The map dropdown is automatically populated by scanning:

1. **Stock Maps**: The `mpmissions` folder inside the configured DayZ server path (e.g. `.../DayZServer/mpmissions`)
2. **Workshop Maps**: The Steam Workshop content directory for DayZ app ID `221100` (e.g. `.../steamapps/workshop/content/221100`)

Maps are detected by:
- `.map` files anywhere inside the workshop item
- `dayzOffline.[mapname]` folders
- `meta.cpp` / `mod.cpp` names containing known map keywords (e.g. `map`, `island`, `chernarus`, `livonia`, `namalsk`, `deerisle`, `chiemsee`, etc.)

Available stock maps include:
- `chernarusplus` - Standard map with cities and countryside
- `enoch` - Livonia terrain with mountainous forests
- `namalsk` - Cold climate island with abandoned facilities
- `provinggrounds_pmc` - PMC training grounds
- `tavi` - Mountainous region in northern Tanoa
- `malden` - Island with varied terrain
- `sara` - Agricultural terrain in southern Tanoa

### Port Allocation Formula

For single servers, use standard DayZ port ranges:
```
Game Port: 2302 (default)
Query Port: 2303 (Game + 1)
Steam P2P: 2304 (Game + 2)
```

> ⚠️ **Important**: If running multi-instance, ensure each instance has unique ports (see Multi-Instance section). The default multi-instance formula starts at port 2300 to leave room for the Steam P2P port of the previous instance.

### Start/Stop Controls

| Button | Description |
|--------|-------------|
| 🚀 **Start Server** | Launch the DayZ server with configured settings |
| 🛑 **Stop Server** | Gracefully stop the currently running server |
| 🔄 **Restart Server** | Stop and restart the server |

### Memory Kill Limit

The **Memory Kill Limit (GB)** field sets a hard RSS ceiling for the entire
server process tree (the main DayZ binary plus all `enfmain` children). If
total resident memory exceeds this limit, DayzConfigMaster terminates the tree
automatically.

| Default | Recommended |
|---------|-------------|
| `15` GB | Set slightly below your system's available RAM (leave headroom for the OS and GUI) |

The watchdog runs in a background thread, so it can still kill the server even
if the GUI becomes unresponsive. Memory samples are logged to:

```
<instance_root>/logs/memory_watchdog.log
```

The log format is:

```
2026-07-13 14:32:10 | server | 4.21 GB / 15.00 GB | pids=[1234, 1245]
```

If the limit is exceeded, an additional line is written:

```
2026-07-13 14:32:15 | KILLING server: memory limit exceeded (15.32 GB)
```

Use this log to identify which mod or configuration causes memory growth after
a restart.

### Mod Command-Line Quoting

DayZ's `-mod=...` argument uses semicolons to separate multiple mods. When the
GUI launches the server in a terminal emulator, that command is passed through a
shell. If the mod list is not quoted correctly, the shell interprets each
semicolon as a command separator and tries to execute the mod IDs as separate
commands.

Symptoms of unquoted mod lists:
- `server_console.log` contains lines like `@1582756848: command not found`
- The server starts but only loads the first mod (or none at all)
- Clients report missing mods or mod-list mismatches
- The server hangs after `Loading landscape took: ...` because the mission
  initializes with an incomplete mod set

DayzConfigMaster now shell-quotes every argument before joining them for
`gnome-terminal`, so `-mod=@Mod1;@Mod2;@Mod3` is passed to the DayZ binary as a
single argument. You do not need to add quotes manually in the mod path field.

### Mod Version Mismatches

Some mod families (notably **DayZ Expansion**) ship multiple workshop items that
must all be the same version. If one item is updated by Steam while another is
not, the server can fail to compile mission scripts and never open its game
port.

Symptoms of a version mismatch:
- Server process is running but port 2302 (or your configured port) never opens.
- RPT log contains `SCRIPT (E): Can't compile "Mission" script module!`.
- RPT log contains `Unknown type 'ExpansionViewController'` or similar.
- Client hangs at the loading screen because it cannot connect.

DayzConfigMaster now reads `version = "..."` from each mod's `meta.cpp`/`mod.cpp`
and warns you before starting the server if it detects mismatched versions
within a known mod family. The fix is to update all items in the family to the
same version through Steam Workshop.

### Startup Diagnostics

After a server has been running for 60 seconds, DayzConfigMaster checks whether
the configured game port is actually listening. If the port is not open, it
scans the latest server RPT log for critical errors such as:

- `Can't compile "Mission" script module!`
- `Failed to load mission scripts!`
- `Unknown type '...'`
- `Mission script has no main function`

If an error is found, a warning dialog shows the exact log line so you know why
the server is not accepting connections. Use that message to fix the underlying
mod, config, or mission issue before restarting.

### Single Server Example Configuration

```
Mode: debug
Port: 2302
Map Name: chernarusplus
Map Size: 2000
Max Players: 60
Config File: (leave blank for auto-generated)
Mod Paths: @CF,@MasPuertas
```

---

## Ban & Whitelist Sanitization

DayZ's dedicated server can get stuck in a loop reloading malformed `ban.txt` or `whitelist.txt` files. DayzConfigMaster sanitizes these files automatically before every server start.

### What Gets Sanitized

- Full-line comments (`// ...`) and inline comments are removed.
- Entries are validated:
  - `ban.txt`: only 17-digit numeric SteamID64 values.
  - `whitelist.txt`: only 44-character alphanumeric DayZ character IDs.
- Example/demo entries (e.g. `1111111111112222222222222333333333XXXXXXAAAA`) are removed.
- Duplicate entries are removed.
- Each file ends with exactly one LF newline to prevent EOF parsing loops.

### Where Sanitized Files Are Written

For every instance, sanitized copies are written to:

```
<instance_root>/ban.txt
<instance_root>/whitelist.txt
<instance_root>/profiles/ban.txt
<instance_root>/profiles/whitelist.txt
```

If the base file does not exist in the DayZ server directory, an empty sanitized file is created so the server does not fall back to a malformed copy.

---

## Mod & Key Deployment

Mods passed via **Mod Paths** are physically deployed into the instance directory before startup. This is required for the server to load mods correctly and for clients to pass signature verification.

### What Happens at Startup

1. `keys/` directory is created in the instance root.
2. Base game `.bikey` files are copied from `DayZServer/keys/`.
3. For each mod in the mod list:
   - The mod folder is resolved (workshop item, local `@Mod` folder, or absolute path).
   - An instance-local `@Name` wrapper directory is created.
   - Every item from the source mod is symlinked inside the wrapper, with well-known DayZ folder names normalized to lowercase (`Addons` → `addons`, `Keys`/`Key` → `keys`).
   - All `.bikey` files from the wrapper's lowercase `keys/` folder are copied into `instance/keys/`.
4. The server's `-mod=` argument is rewritten to use the instance-relative `@Name` paths instead of absolute workshop paths.
5. Stale `@*` wrappers left over from previous mod lists are removed.

> 💡 **Why wrappers?** Many Steam Workshop mods ship folders named `Addons` and `Keys` with capital letters. DayZ on Linux is case-sensitive and expects lowercase `addons` and `keys`. The wrapper ensures the server sees the correct case without modifying your Steam Workshop files.

### Sanity-Checking Mod Deployment

Use the included command-line tool to verify a running or stopped instance:

```bash
# Check the currently running DayZ server
python3 dayzconfigmaster/tools/mod_sanity_check.py --running

# Check a specific instance directory
python3 dayzconfigmaster/tools/mod_sanity_check.py \
  --instance ~/Documents/DayZProjects/instances/server1 \
  --mods "@3369325490;@2216645010"
```

The tool reports:
- Whether each mod wrapper exists and exposes a lowercase `addons` folder.
- How many PBOs are visible to the server.
- Whether every mod key has been copied into `instance/keys/`.
- Any missing keys or empty addons folders.

Run it after starting a server to confirm the client will see the same mod set.

### Supported Mod Path Formats

```
@CF
@CF;@MasPuertas
CF;MasPuertas
@Community Framework
/opt/dayz/mods/@CF
steam://...
```

Mods are resolved in this order:

1. **Local mod directories** — a matching `@Name` folder in any configured local mod directory.
2. **Workshop folder name** — a matching `@Name` folder directly inside the workshop content directory.
3. **Workshop item contents** — a matching `@Name` subfolder inside a numeric workshop item.
4. **Display name matching** — the `name` value from the mod's `mod.cpp` or `meta.cpp` (case-insensitive, with or without `@`, spaces allowed). Both short names (e.g. `CF`) and long display names (e.g. `Community Framework`) are matched.
5. **Workshop item folder name** — the numeric item folder itself if it matches the requested name.

> ⚠️ **Important:** Only symlinks are managed; real directories are never overwritten.

---

## Mod Integration

Loading a mod via `-mod=` is only the first step. For mod content (vehicles,
items, spawn points) to actually appear in-game, the mod's XML fragments must
be merged into the server's mission files.

DayzConfigMaster provides a **Mod Integration** tab under **Server Control**:

1. Choose the instance to configure.
2. Click **🔄 Scan Mods** to detect XML fragments in active mod folders.
3. Check **Integrate** for each mod whose fragments you want merged.
4. Click **✅ Apply Integration**.
5. Restart the server.

The tool backs up original mission files, merges fragments by root element
(`<types>`, `<spawnabletypes>`, `<events>`, `<eventposdef>`, etc.), and avoids
duplicate entries when re-applied. See [mod-integration.md](mod-integration.md)
for full details.

---

## Mission Folder Deployment

The mission template in `serverDZ.cfg` must match an available mission folder. DayzConfigMaster resolves the real internal world name (e.g. `alteria` instead of the display name `Alteria`) and ensures the mission folder is reachable.

### Resolution Order

1. If the base game has `DayZServer/mpmissions/dayzOffline.<world>`, it is **copied** into the instance.
2. Otherwise, the selected workshop map folder is searched for:
   - `dayzOffline.<world>`
   - `dayz.<world>`
   - `Offline.<world>`
   - Any folder containing a `.wrp` file
3. The first matching folder is **copied** into `instance/mpmissions/dayzOffline.<world>`.

Mission folders are copied (not symlinked) so that mod integration can safely
edit `types.xml`, `cfgspawnabletypes.xml`, and other mission files without
modifying the base game or workshop source files.

---

## Process Governance & Duplicate Start Prevention

DayzConfigMaster now maintains strict control over every server process:

- **One start per instance**: The GUI tracks which instances are already running and refuses to start the same instance twice. The **Start** button is disabled while an instance is running, and the **Stop** button is enabled.
- **Orphan cleanup**: Before starting any instance, the app scans `/proc` for existing DayZ server processes that match the instance directory, config file, and port. Any orphaned processes (for example, a lost `enfmain` child) are terminated automatically so the new start has full control.
- **Whole-tree termination**: When you stop a server, the app discovers and terminates the entire process tree — not just the top-level `DayZServer` process, but also Enfusion children such as `enfmain` and any other descendants. This prevents hidden child processes from keeping ports or files locked.

### How Orphan Detection Works

Before each start, DayzConfigMaster looks for processes that:

1. Have a command line containing `DayZ` or `dayz`.
2. Have a current working directory (`cwd`) matching the instance root.
3. Have `-config=<config_filename>` and `-port=<port>` in their command line.

Matching processes are killed gracefully, then forcefully if necessary, and their descendants are killed too.

## Process Monitoring & Crash Detection

After a server starts, the GUI polls `ProcessController.get_status()` every 2 seconds. If the server process exits or crashes, the GUI automatically:

- Updates the status bar to "Server stopped (crashed or exited)".
- Clears the instance from the running set so it can be started again.
- Re-enables the **Start** button(s).
- Disables the **Stop** button(s).
- Logs the event in the integrated log viewer.

The polling callback is cancelled when you manually stop the server or when a crash is detected, so it never runs indefinitely.

### Terminal Launch PID Tracking

When using **Start in Terminal**, the tracked process is the terminal emulator, not the DayZ server. DayzConfigMaster attempts to discover the actual DayZ child PID by reading `/proc/<terminal_pid>/task/<terminal_pid>/children` or running `pgrep -P <terminal_pid>`. The entire discovered tree is then monitored and terminated on stop.

---

## Multi-Instance Control

The Multi-Instance tab displays the instances you configured on the **Server Config** tab. Each row is read-only here and mirrors the configuration row 1:1, so the control view always matches the config view.

### Enable Multi-Instance Mode

1. Open the **Server Config** tab.
2. Check **"Enable Multi-Instance Mode"** in the Multi-Instance Configuration section.
3. Set **Instance Count** (1-9) using the spinbox.
4. Configure each instance's parameters (ports, map, mods, profile path, etc.).
5. Return to **Server Control → General Settings → Multi-Instance** to start/stop instances.

### Instance Control Per Row

Each control row shows:

| Parameter | Description | Formula/Example |
|-----------|-------------|-----------------|
| **Instance ID** | Unique identifier (1-9) | 1, 2, 3... |
| **Config File** | Server config filename | `serverDZ_1.cfg` |
| **Root Folder** | Instance root path | `/dayz/server1` |
| **Game Port** | Game connection port | `2300 + (instance * 100)` |
| **Query Port** | Query port | `Game Port + 1` |
| **Steam P2P** | Steam peer-to-peer port | `Game Port + 2` |
| **Map Name** | Server map (auto-detected) | Auto-populated from mpmissions |
| **Profile Path** | Player data storage | `/dayz/server1/profile` |
| **Mod Paths** | Semicolon-separated mods | @CF;@MasPuertas |

> 💡 **Tip**: To change any value, edit it on the **Server Config** tab. The control row updates automatically.

### Port Allocation Formula

For multi-instance servers, the default formula used by the application is:
```
Game Port = 2300 + (instance_num × 100)
Query Port = Game Port + 1
Steam P2P = Game Port + 2
```

| Instance | Game | Query | Steam |
|----------|------|-------|-------|
| 1 | 2300 | 2301 | 2302 |
| 2 | 2400 | 2401 | 2402 |
| 3 | 2500 | 2501 | 2502 |

> ⚠️ **Warning**: Port conflicts will prevent servers from starting. Verify no other services use these ports.

### Configuration Example (3 Instances)

Configured on **Server Config**:

**Instance 1:**
```
Instance ID: 1
Config File: serverDZ_1.cfg
Game Port: 2300
Query Port: 2301
Steam P2P: 2302
Map Name: chernarusplus
Profile Path: /dayz/server1/profile
```

**Instance 2:**
```
Instance ID: 2
Config File: serverDZ_2.cfg
Game Port: 2400
Query Port: 2401
Steam P2P: 2402
Map Name: enoch
Profile Path: /dayz/server2/profile
```

**Instance 3:**
```
Instance ID: 3
Config File: serverDZ_3.cfg
Game Port: 2500
Query Port: 2501
Steam P2P: 2502
Map Name: namalsk
Profile Path: /dayz/server3/profile
```

### Start/Stop Controls

| Button | Description |
|--------|-------------|
| 🚀 **Start** | Launch the selected instance |
| 🛑 **Stop** | Stop the selected instance |
| 🚀 **Start All Instances** | Launch all configured instances |
| 🛑 **Stop All Instances** | Stop all running instances |

> ⚠️ **Storage Isolation**: Each instance uses its own `instanceId` which creates isolated player data and building changes. This allows the same player to have separate characters on different maps.

---

## Log Viewer

Monitor server logs in real-time with the integrated log viewer.

### Log Type Options

| Log Type | Description |
|----------|-------------|
| `script` | Script execution logs (most useful for debugging) |
| `rpt` | General server reports and events |
| `adm` | Admin command logs |
| `client` | Client connection/disconnection logs |

### Controls

| Button | Description |
|--------|-------------|
| 🔄 **Refresh** | Update log viewer with latest entries (last 100 lines) |
| **Clear Log** | Clear the current log display |

> 💡 **Tip**: Use `script` logs for most troubleshooting as they show configuration errors and script issues.

---

## Diagnostics Analysis

The diagnostics engine analyzes server logs to identify common issues.

### Issue Categories Detected

| Category | Description |
|----------|-------------|
| **Configuration Errors** | Invalid settings in config files |
| **Path Issues** | Missing or incorrect paths |
| **Port Conflicts** | Multiple services on same port |
| **Mod Loading Errors** | Missing or incompatible mods |

### How to Use

1. Ensure logs are being generated (server must be running)
2. Click **🔍 Analyze Logs for Issues**
3. Review detected issues with causes and fixes

### Example Output

```
❌ Port Conflict
   Cause: Multiple instances attempting to use same port 2302
   Fix: Change instance ports to unique values (2300, 2400, 2500, etc.)

⚠️ Map Not Found
   Cause: Config references non-existent map "custom_map"
   Fix: Select valid map from dropdown or add custom map to mpmissions
```

---

## Best Practices

### Single Instance Servers

1. **Memory Management**: Allocate 2-3 GB RAM per instance
2. **Map Selection**: Use `chernarusplus` for testing, production maps for live servers
3. **Log Monitoring**: Regularly check logs during first server run
4. **Mod Testing**: Test mods on a single instance before deploying to production

### Multi-Instance Servers

1. **Port Planning**: Plan ports in advance (2300, 2400, 2500...)
2. **Storage Isolation**: Each instance MUST have unique root/profile paths
3. **Resource Management**:
   - Minimum: 16 GB RAM for 2 instances
   - Recommended: 32+ GB RAM for 3+ instances
4. **Staggered Restarts**: Offset restart times by 30+ minutes between instances

### General Recommendations

1. **Enable logMemory = 1** in serverDZ.cfg to track memory trends
2. **Use Debug Mode** during initial setup, switch to Normal for production
3. **Test Configurations**: Always test on a single instance first
4. **Backup Configuration**: Save working configs before major changes

---

## Common Issues and Solutions

### Issue: Server won't start
- **Check**: DayZ server path is correct (DayZServer_x64 exists)
- **Verify**: Port is not in use by another process
- **Review**: Logs for specific error messages

### Issue: Map not found
- **Verify**: Map folder exists in `mpmissions` directory
- **Format**: Folder should be named like `dayzOffline.chernarusplus`
- **Refresh**: Restart application to refresh map list

### Issue: Port already in use
```
# Linux/Mac - Find process using port
lsof -i :2302
# or
netstat -tulpn | grep :2302

# Windows
netstat -ano | findstr :2302
```

### Issue: Mod not loading
- **Check**: Mod folder exists in server root directory
- **Format**: Folder name must match mod line exactly (case-sensitive on Linux)
- **Signatures**: Set `verifySignatures = 1` for development testing

---

## Mod Selection

Mod paths are configured per instance on the **Server Config** tab:

- Each instance gets its own **Mod Paths** field.
- Use the workshop mod tree to select Steam Workshop mods; the tree automatically separates maps from mods.
- Use the **Browse** button next to a selected mod to open its folder.
- Mod paths are stored per instance and used when starting the server.

The **Server Control** tab displays the resulting mod paths in read-only control rows.

## Ban List & Players Inner Tab

The **Ban List & Players** tab provides RCon-based player management:

- View currently connected players
- Ban players by Steam ID
- Configure ban duration and reason
- Requires the server to be running and RCon to be enabled

> 💡 **Tip**: RCon settings are configured in `serverDZ.cfg` (`rconPassword`, `rconPort`).

## Server Config Layout

The **Server Config** tab is organized into four category tabs to keep configuration manageable:

```
Server Config
├── Core            → Basic, Security, Voice & View, Time & Weather,
│                     Network & Instance, Logging & Advanced settings
├── Mods            → Map selection, mod tree, mod paths, workshop integration
├── Multi-Instance  → Enable toggle, instance count, per-instance config rows
└── Files           → In-Game Settings and per-instance Effects & Triggers
```

### Core Tab

All `serverDZ.cfg` fields are grouped into compact `LabelFrame` sections with a two-column layout:

- **Basic** — Server Name, Port, Query Port, Max Players, Game Mode
- **Security** — Server Password, Admin Password, Whitelist, Verify Signatures, Force Same Build
- **Voice & View** — VoN, 3rd Person, Crosshair, Personal Light, Lighting Config
- **Time & Weather** — Server Time, Time Acceleration, Night Acceleration, Persistent Time
- **Network & Instance** — Guaranteed Updates, Login Queue, Instance ID, Storage Auto Fix, Shard ID
- **Logging & Advanced** — Log Memory, Admin/Network Logs, Update Settings, Memory Limit, Thread Count, Description

### Mods Tab

- **Map Selection** — dropdown populated from stock and workshop maps
- **Mod Selection** — checkbox treeview showing mod name, workshop ID, and folder path
- **Mod Paths** — semicolon-separated mod list with a Browse button
- **Workshop Integration** — workshop directory and Scan Workshop button
- **Mod Settings** — built-in editor that auto-discovers JSON/XML/CPP/HPP/TXT config files inside every workshop mod folder and the active mission folder; click **Refresh** to rescan after subscribing to new mods

### Multi-Instance Tab

- **Enable Multi-Instance Mode** toggle
- **Instance Count** spinbox (1–9)
- Per-instance configuration rows (port, map, mod paths, config file)

### Files Tab

The **Files** tab contains an inner notebook with two tabs:

#### In-Game Settings

Moved from the old standalone top-level tab, this tab uses a compact sidebar layout:

- A narrow **category list** on the left with:
  - **Classes** — item/weapon/vehicle/clothing/food/tool class definitions
  - **Defaults** — default server values
  - **Categories (Limits)** — loot category limits
  - **Usages (Limits)** — loot usage definitions
  - **Values/Tiers (Limits)** — loot tier/zone definitions
  - **Random Presets** — preset cargo/attachment configurations
  - **Spawnable Types** — item spawn definitions
- The selected category's full-height treeview on the right

Click a category in the list to switch the tree. This avoids nested tabs and keeps the header compact.

These settings are global to the server configuration.

#### Effects & Triggers

This tab is **bound per instance**. Use the **Instance** dropdown at the top to select which instance you are editing. Each instance maintains its own copies of:

- `cfglimitsdefinitionuser.xml`
- `cfgeventspawns.xml`
- `cfgeffectarea.json`
- `cfgundergroundtriggers.json`
- `mapclusterproto.xml`
- `mapgroupdirt.xml`
- `events.xml`
- `globals.xml`
- `messages.xml`

When you generate configs, the output includes both `serverDZ_<id>.cfg` and the per-instance Effects & Triggers files (e.g. `cfglimitsdefinitionuser_1.xml`, `cfgeffectarea_2.json`).

> 💡 **Tip**: Always check the **Instance** dropdown before editing Effects & Triggers. Changes are saved to the selected instance when you switch instances or generate configs.

## Integration with Other Tabs

The Server Control tab works seamlessly with:

| Tab | Integration |
|-----|-------------|
| **Server Config** | Configure maps, workshop mods, verify signatures, instance count, per-instance settings, In-Game Settings, per-instance Effects & Triggers, and generate all config files |
| **House Cleaning & Scheduling** | Use multi-instance staggered restarts and `start.sh` generation |
| **Mod Builder** | Build and pack mods before adding them to instances |
| **Workshop** | Search and download workshop items that appear in the Server Config mod tree |

Note: The old "Multi-Instance Manager", "Ban List & Players", and standalone log tabs have been consolidated into this unified **Server Control** tab. All runtime functionality is now accessible from one central location, while configuration lives on the **Server Config** tab.
---

## Quick Reference

### File Locations (Linux)

```
Steam Common:     /home/user/.steam/steam/steamapps/common/DayZServer/
MP Missions:      /home/user/.steam/steam/steamapps/common/DayZServer/mpmissions/
Client Install:   /home/user/.local/share/Steam/steamapps/common/DayZ/
Workshop Content: /home/user/.steam/steam/steamapps/workshop/content/221100/
Projects Root:    /home/user/Documents/DayZProjects/
Instance Root:    /home/user/Documents/DayZProjects/instances/server1/
Profile Data:     /home/user/Documents/DayZProjects/instances/server1/profiles/
Server RPT Logs:  /home/user/.local/share/DayZ/DayZServer_YYYY-MM-DD_HH-MM-SS.RPT
Instance Logs:    /home/user/Documents/DayZProjects/instances/server1/logs/
```

### File Locations (Windows)

```
Steam Common:     C:\Program Files\Steam\steamapps\common\DayZServer\
MP Missions:      ...\steamapps\common\DayZServer\mpmissions\
Client Install:   C:\Program Files\Steam\steamapps\common\DayZ\
Workshop Content: ...\steamapps\workshop\content\221100\
Projects Root:    %USERPROFILE%\Documents\DayZProjects\
Instance Root:    %USERPROFILE%\Documents\DayZProjects\instances\server1\
Profile Data:     %USERPROFILE%\Documents\DayZProjects\instances\server1\profiles\
Server RPT Logs:  %LOCALAPPDATA%\DayZ\DayZServer_YYYY-MM-DD_HH-MM-SS.RPT
Instance Logs:    %USERPROFILE%\Documents\DayZProjects\instances\server1\logs\
```

### Essential serverDZ.cfg Parameters

```cpp
// Basic Settings
name = "My DayZ Server";
port = 2302;
queryPort = 2303;
maxPlayers = 60;

// Instance (multi-instance only)
instanceId = 1;  // Must be unique per instance

// Mod Support
verifySignatures = 2;     // 0=Off, 1=Debug, 2=Verify
forceSameBuild = 1;       // 0=No, 1=Yes
mod = "@CF,@MasPuertas";

// Paths
profiles = "./profile";   // Base profile directory
cfg = "serverDZ.cfg";     // Config file path

// Logging
logMemory = 1;
```