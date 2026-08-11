# DayZConfigMaster — Deployment Safety Guide

## What went wrong

DCM automatically modified mission XML files (`db/types.xml`, `db/events.xml`,
`cfgspawnabletypes.xml`) every time a server instance was started.  A bug in
those edits changed the `<nominal>` values of static wreck/vehicle types from
`0` to `1`.  Once those values were written into the Central Economy cache
(`storage_1/data/*.bin`) the server spawned wrecks inside buildings and in
absurd places, corrupting the persistent world.

Root cause: **XML-modifying code ran silently on every start with no user
approval, no sandbox validation, and no automatic backup of player data.**

## How it is fixed

### 1. Deployment Options (per instance)

Every instance control row now has a **Deployment Options** panel with
checkboxes that control what happens when **Start** or **Start in Terminal** is
pressed.

All XML-modifying steps default to **OFF**:

| Option | Default | What it does |
|--------|---------|--------------|
| Deploy fresh mission folder from base game | OFF | Deletes the instance mission folder and copies a clean copy from `DayZServer/mpmissions/...` |
| Backup players.db & storage_1/data before start | ON | Creates an incremental snapshot before any changes |
| Validate mission XML against sandbox factory files | ON | Compares `types.xml`, `events.xml`, `cfgspawnabletypes.xml`, `cfgeventspawns.xml` to the base game files |
| Sanitize mission economy files | OFF | Quarantines corrupt root `types.xml`/`events.xml` |
| Normalize aircraft lifetimes | OFF | Rewrites aircraft `<lifetime>` values |
| Repair nominal values from reference missions | OFF | Restores zero `<nominal>` loot values from other missions |
| Apply mod integration XML edits | OFF | Applies mod integration XML fragments |
| Apply spawn loadout XML edits | OFF | Adds configured spawnables to mission XML |
| Apply mod settings overrides | OFF | Copies mod config overrides into place |
| Require confirmation before XML-modifying steps | ON | Shows a confirmation dialog if any XML-modifying step is enabled |

**Important:** the deployment options are saved per instance in
`<instance_root>/dcm_config/deployment_options.json`.  You must click
**Save Deployment Options** before starting the server.

### 2. Confirmation dialogs

If any XML-modifying step is enabled and **Require confirmation** is on, a
dialog lists exactly which steps will run.  You must click **Yes** before DCM
touches any XML.

If sandbox validation is on and the mission XML differs from the base game
files, another warning is shown **before** the XML steps run.

### 3. Automatic pre-start backup

When **Backup players.db & storage_1/data before start** is enabled, DCM
creates a timestamped incremental backup under:

```
<instance_root>/backups/storage/prestart_instance_<id>_<timestamp>/
```

It backs up:

* `players.db` (from instance root, `storage_1/`, or `profiles/`)
* `mpmissions/<mission>/storage_1/data/` (the CE cache)

Backups use `rsync --link-dest` when available, so unchanged blocks are shared
between snapshots and disk usage stays low.  Pre-start backups keep the last
24 hours.

### 4. Scheduled storage backups

Use the standalone tool for cron-based backups:

```bash
# Every 12 hours for instance 1
0 */12 * * * cd /home/sysadmin/Documents/git/DayzConfigMaster && /usr/bin/python3 tools/backup_instance_storage.py /home/sysadmin/Documents/DayZProjects/instances/server1

# Every day at 3 AM for all instances
0 3 * * * cd /home/sysadmin/Documents/git/DayzConfigMaster && /usr/bin/python3 tools/backup_instance_storage.py /home/sysadmin/Documents/DayZProjects/instances --all
```

Default retention is 7 days (`--retention 168`).  Older backups are purged
automatically.

### 5. Sandbox validation

The sandbox validation compares the instance mission XML against:

```
DayZServer/mpmissions/dayzOffline.<world>/db/types.xml
DayZServer/mpmissions/dayzOffline.<world>/db/events.xml
DayZServer/mpmissions/dayzOffline.<world>/cfgspawnabletypes.xml
DayZServer/mpmissions/dayzOffline.<world>/cfgeventspawns.xml
```

If any of those files differ in size or SHA-256 hash, DCM logs a warning and
shows a confirmation dialog when XML modifications are also enabled.

### 6. Recommended workflow for adding spawns / loot / locations

1. **Enable only the steps you need.**  Do not turn on unrelated XML edits.
2. **Start on a test server first.**  Verify the XML output before touching a
   live public instance.
3. **Keep backups on.**  The default pre-start backup protects `players.db`
   and the CE cache.
4. **Validate the result.**  After start, check `db/types.xml` for any
   `Land_Wreck_*`, `Wreck_*`, vehicle, or aircraft entries that gained a
   non-zero `<nominal>`.
5. **If something is wrong, stop the server and restore from the pre-start
   backup** before the corrupted CE cache is written back to disk.

### 7. Restoring from a pre-start backup

1. Stop the server.
2. Copy `players.db` from the backup back to the instance root.
3. Copy `mpmissions/<mission>/storage_1/data/` from the backup back to the
   mission folder.
4. Delete or fix the offending XML file.
5. Start the server again.

## For developers

* `DeploymentOptions` lives in `dayzconfigmaster/config/per_instance_config.py`.
* The UI builder is `_build_deployment_options_frame()` in
  `dayzconfigmaster/gui/app.py`.
* `_prepare_instance_files()` reads the saved options and gates every step.
* Storage backups are implemented in `dayzconfigmaster/backups/storage_backup.py`.
* CLI tool: `tools/backup_instance_storage.py`.

## Policy

**Never modify factory XML without explicit, per-run user approval and a
backup.**  This is the default behavior from now on.
