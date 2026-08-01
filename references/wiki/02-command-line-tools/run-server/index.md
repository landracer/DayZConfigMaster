# run-server - Launch DayZ Server with Bundles

Runs DayZ Server locally using bundle configurations.

## Syntax

```bash
run-server <bundle-name> [options]
```

## Examples

```bash
# Run a bundle defined in server.toml or bundles.py
run-server mybundle

# Run server with additional parameters
run-server mybundle --config custom.cfg
```

## Configuration Files

Bundles are defined in two ways:

### 1. TOML Configuration (server.toml)

```toml
[bundle.mybundle]
mods = '@Mod1;@Mod2'
mission_directory = 'mpmissions\dayzoffline.chernarusplus'
parameters = ['-adminLog']
```

### 2. Python Bundles (bundles.py)

```python
def mybundle(settings):
    settings.add_mod('@Mod1')
    settings.set_mission_directory('mpmissions\dayzoffline.chernarusplus')
```

## Bundle Configuration Options

| Option | Description |
|--------|-------------|
| `executable` | Path to DayZServer executable |
| `config` | Path to server config file |
| `directory` | Working directory for server |
| `profile_directory` | Profile folder for logs/saves |
| `mission_directory` | MP mission folder path |
| `workshop_directory` | Workshop mods location |
| `mods` | List of client mods to load |
| `server_mods` | List of server-only mods |
| `parameters` | Extra command line parameters |

## Parameters

| Flag | Description |
|------|-------------|
| `-h`, `--help` | Show help message |

## Notes

1. Bundles can be defined in either TOML or Python (not both simultaneously for same name)
2. Mods prefixed with `@` are loaded from workshop directory
3. Python bundles offer more flexibility than TOML definitions