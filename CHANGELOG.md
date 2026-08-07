# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased]

### Added

- **Instance preflight checks** (`dayzconfigmaster/server/instance_preflight.py`)
  - Validates instance root, profile directory, mission `storage_1`,
    `players.db`, world `.bin` files, mod storage, journal files, and
    filesystem attributes before the server starts.
  - SQLite write probe (`BEGIN IMMEDIATE`) catches the "attempt to write a
    readonly database" class of failures at deploy time.
  - Auto-repair removes immutable/append-only attributes and fixes permissions.

- **Per-instance mod presets** (`dayzconfigmaster/config/mod_presets.py`)
  - Save, load, apply, and delete named ordered mod lists per instance.
  - Presets persist in `<projects_root>/mod_presets.json`.
  - UI added to the Server Config tab with dropdown, Apply, Save As, and
    Delete controls.

- **Aircraft/helicopter lifetime normalization**
  - (`dayzconfigmaster/economy/aircraft_lifetime.py`)
  - Automatically sets `types.xml` `<lifetime>` to `3888000` seconds
    (45 days) for aircraft and helicopter classes whose lifetime is lower.
  - Runs automatically after mission deployment and on server start, with a
    backup created before any mutation.
  - Manual "Fix Aircraft Lifetimes" button added under **House Cleaning &
    Scheduling > Mission XML Maintenance**.

- **Deployment manifest & quick-skip logic**
  - (`dayzconfigmaster/config/deployment_manifest.py`)
  - Tracks deployed state so redundant copy/symlink/integration work can be
    skipped when nothing has changed.

- **Per-instance configuration & backup tooling**
  - (`dayzconfigmaster/config/per_instance_config.py`)
  - (`dayzconfigmaster/backups/instance_storage.py`)
  - (`dayzconfigmaster/backups/players_db.py`)
  - (`dayzconfigmaster/backups/map_storage_state.py`)
  - (`dayzconfigmaster/backups/cross_instance_storage.py`)
  - Supports instance-specific overrides, storage backup/restore, players.db
    comparison/splicing, and cross-instance map storage tracking.

- **Mod settings editor** (`dayzconfigmaster/gui/mod_settings_editor/editor.py`)
  - GUI editor for per-mod configuration files.

- **Tests** for preflight checks, mod presets, aircraft lifetime, deployment
  manifest, instance storage, players database splicing, map storage state,
  cross-instance storage, and app backup hooks.

### Changed

- `dayzconfigmaster/gui/app.py`
  - Integrated preflight checks into all server start paths (normal, terminal,
    batch).
  - Fixed `UnboundLocalError: ModIntegrationManager` by moving the import to
    the top of `_prepare_instance_files`.
  - Fixed "Text file busy" binary copy by reusing the existing instance binary
    when a running process blocks replacement.
  - Added mod preset UI controls to the Server Config tab.
  - Wired aircraft lifetime normalization into deploy and start flows.

- `dayzconfigmaster/config/mod_integration.py`
  - Added process-level TTL cache and 8-second timeout for workshop class-name
    scans to prevent UI freezes during vehicle wheel lookups.

- `dayzconfigmaster/economy/__init__.py`
  - Exported `ensure_aircraft_lifetime`, `AircraftLifetimeResult`, and
    `MAX_VEHICLE_LIFETIME`.

- `dayzconfigmaster/gui/mod_integration_tab.py`
  - Updated to use cached/timed-out vehicle discovery.

### Removed

- Stopped tracking compiled Python bytecode (`__pycache__`, `*.pyc`) and
  external reference repositories under `references/git/`.

### Repository Hygiene

- Added `.gitignore` covering Python caches, virtual environments, test
  caches, build artifacts, runtime state (`jim.json`, `instances/`, `logs/`,
  `storage_1/`), and heavy external bundles (`steamcmd/`, `references/git/`).
