# DayZ Dev Tools - Python API Modules

This section documents each public module in dayz-dev-tools.

## Public Modules

All other functions and classes not listed here are considered private and may change without notice.

### 1. dayz_dev_tools.extract_pbo

Extract files from PBO archives.

```python
from dayz_dev_tools import extract_pbo, pbo_reader

# Extract specific files
with open('mod.pbo', 'rb') as f:
    reader = pbo_reader.PBOReader(f)
    extract_pbo.extract_pbo(reader, ['Prefix/scripts/3_Game/foo.c'], verbose=True)
```

**Function:** `extract_pbo(reader, files_to_extract, *, verbose, deobfuscate, cfgconvert, pattern)`

| Parameter | Type | Description |
|-----------|------|-------------|
| reader | PBOReader | Instance representing the PBO archive |
| files_to_extract | list[str] | List of file paths to extract |
| verbose | bool | Print extraction progress to stdout |
| deobfuscate | bool | Attempt to deobfuscate script files |
| cfgconvert | str\|None | Path to CfgConvert.exe for binarized configs |
| pattern | str\|None | Glob pattern to filter extracted files |

---

### 2. dayz_dev_tools.guid

Convert SteamID64 to DayZ GUID.

**Function:** `guid_for_steamid64(steamid64) -> str`

```python
from dayz_dev_tools import guid

dayz_guid = guid.guid_for_steamid64(76561197970002375)
print(f"DayZ GUID: {dayz_guid}")
```

---

### 3. dayz_dev_tools.keys

Manage public key files (*.bikey).

**Function:** `copy_keys(source, destination) -> None`

```python
from dayz_dev_tools import keys

# Copy all .bikey files from one directory to another
keys.copy_keys('/path/to/source', '/path/to/destination')
```

---

### 4. dayz_dev_tools.launch_settings

Configure DayZ Server launch settings.

**Class:** `LaunchSettings(config: ServerConfig)`

```python
from dayz_dev_tools import launch_settings, server_config

# Create from config
config = server_config.load('server.toml')
settings = launch_settings.LaunchSettings(config)
```

**Methods:**

| Method | Description |
|--------|-------------|
| `add_mod(name)` | Add a client mod to load |
| `add_server_mod(name)` | Add a server-only mod |
| `add_parameter(param)` | Add command line parameter |
| `set_mission_directory(path)` | Set mission directory path |
| `config()` | Get config filename (usually serverDZ.cfg) |
| `directory()` | Get working directory |
| `executable()` | Get executable path |
| `mission_directory()` | Get mission directory |
| `mods()` | List of mods to load |
| `parameters()` | Extra command line parameters |
| `profile_directory()` | Get profile directory |

---

### 5. dayz_dev_tools.list_pbo

List PBO archive contents.

**Function:** `list_pbo(reader, *, verbose) -> None`

```python
from dayz_dev_tools import list_pbo, pbo_reader

with open('mod.pbo', 'rb') as f:
    reader = pbo_reader.PBOReader(f)
    list_pbo.list_pbo(reader, verbose=True)
```

---

### 6. dayz_dev_tools.pbo_file

Interface for PBO archive files.

**Class:** `PBOFile(prefix, filename, mime_type, original_size, reserved, time_stamp, data_size, content_reader)`

| Attribute | Type | Description |
|-----------|------|-------------|
| data_size | int | Size of file in PBO archive |
| time_stamp | int | Unix timestamp of file creation/modification |

**Methods:**

| Method | Description |
|--------|-------------|
| `deobfuscated_filename(index)` | Get deobfuscated filename |
| `filename()` | Get raw filename |
| `invalid()` | Check if filename is invalid Windows format |
| `normalized_filename()` | Get normalized filename |
| `obfuscated()` | Check if file appears to be obfuscated script |
| `split_filename()` | Split filename into path components |
| `type()` | Get 4-character file type |
| `unpack(output_file)` | Write file contents |
| `unpacked_size()` | Get original file size |

---

### 7. dayz_dev_tools.pbo_reader

Interface for reading PBO archives.

**Class:** `PBOReader(file: BinaryIO)`

**Methods:**

| Method | Description |
|--------|-------------|
| `file(filename)` | Get specific file from archive |
| `files()` | List all files in archive |
| `headers()` | Get archive headers as list of tuples |
| `prefix()` | Get archive prefix (if any) |

---

### 8. dayz_dev_tools.pbo_writer

Interface for writing PBO archives.

**Class:** `PBOWriter(cfgconvert: str\|None)`

**Methods:**

| Method | Description |
|--------|-------------|
| `add_file(path)` | Add file to archive |
| `add_header(name, value)` | Add custom header |
| `write(output)` | Write PBO archive |

---

### 9. dayz_dev_tools.script_logs

Utilities for script log files.

**Functions:**

- `newest(directory) -> str\|None` - Find newest script_*.log file
- `stream(outfile, infile, keep_streaming) -> None` - Stream log contents
- `wait_for_new(directory, previous_log_name, *, timeout=10) -> str\|None` - Wait for new log

---

### 10. dayz_dev_tools.server_config

Parse DayZ server configuration files.

**Class:** `ServerConfig`

| Attribute | Type | Description |
|-----------|------|-------------|
| executable | str | Server executable path |
| config | str | Config filename (serverDZ.cfg) |
| directory | str\|None | Working directory |
| profile_directory | str\|None | Profile folder |
| mission_directory | str\|None | Mission directory |
| workshop_directory | str | Workshop mods location |
| bundles | dict[str, BundleConfig] | Defined bundles |

**Function:** `load(filename) -> ServerConfig`

```python
from dayz_dev_tools import server_config

config = server_config.load('server.toml')
print(f"Mission: {config.mission_directory}")