# DayZ Dev Tools - Command Line Utilities

DayZ Dev Tools provides several CLI utilities for managing DayZ mods and servers.

## Available Commands

| Command | Description |
|---------|-------------|
| `guid` | Convert Steam ID64 to DayZ GUID |
| `pbo` | Create PBO (Press Board Oblong) archives |
| `unpbo` | Extract/list contents of PBO files |
| `run-server` | Launch DayZ Server with bundle configuration |

## Usage

All commands support `--help` or `-h` flags:

```bash
command --help
```

---

## Commands

- [guid](./guid/) - Steam ID to GUID conversion
- [pbo](./pbo/) - PBO file creation
- [unpbo](./unpbo/) - PBO extraction and listing
- [run-server](./run-server/) - Server launch with bundles