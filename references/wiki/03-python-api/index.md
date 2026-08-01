# DayZ Dev Tools - Python API Reference

This section documents the public Python APIs provided by dayz-dev-tools for programmatic access to PBO management and server configuration.

## Public Modules

The following modules make up the public API (all other functions are considered private):

| Module | Description |
|--------|-------------|
| `dayz_dev_tools.extract_pbo` | Extract files from PBO archives |
| `dayz_dev_tools.guid` | Steam ID to GUID conversion |
| `dayz_dev_tools.keys` | Public key file management |
| `dayz_dev_tools.launch_settings` | Server launch configuration |
| `dayz_dev_tools.list_pbo` | List PBO contents |
| `dayz_dev_tools.pbo_file` | PBO file interface |
| `dayz_dev_tools.pbo_reader` | Read PBO archives |
| `dayz_dev_tools.pbo_writer` | Write PBO archives |
| `dayz_dev_tools.script_logs` | Script log utilities |
| `dayz_dev_tools.server_config` | Server configuration handling |

## Usage

```python
from dayz_dev_tools import extract_pbo, pbo_reader

# Read a PBO file
with open('mod.pbo', 'rb') as f:
    reader = pbo_reader.PBOReader(f)
    
# List files
for pbo_file in reader.files():
    print(pbo_file.filename())
```

## See Also

- [Command Line Tools](../02-command-line-tools/) - CLI equivalents of these APIs