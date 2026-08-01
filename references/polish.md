# Polish Validation Checklist

## Phase PP0 — Critical correctness bugs ✅ COMPLETE
- [x] PP0.1 set_type() preserves all economic fields (nominal, min, lifetime, restock)
- [x] PP0.1 category/usage/value names survive round-trip
- [x] PP0.2 Root tag is `<types>` for new documents
- [x] PP0.3 economy CLI handlers are real (list, edit, validate work)

## Phase PP1 — Workshop upload to spec ✅ COMPLETE
- [x] PP1.1 Create-new flow uses CreateItem + callback
- [x] PP1.2 ID retrieval fixed, run_callbacks in submit poll
- [x] PP1.3 MCP upload tool added

## Phase PP2 — CE editing features ✅ COMPLETE
- [x] PP2.1 import_classnames creates items + skips existing (no data loss)
- [x] PP2.2 batch_scale no double-write (no duplicate nodes) - via set_type
- [x] PP2.3 Backups prune to 20 + restore undoable (unique filenames, no same-second collision)
- [x] PP2.4 CLI wiring for batch/import-classnames/backups/restore

## Phase PP3 — Data-model completeness ✅ COMPLETE
- [x] PP3.1 TypeEntry has quantmin, quantmax, cost, tags, flags
- [x] PP3.1 Full round-trip preserves all fields (DayZ attribute format verified)

## Phase PP4 — Remove fake handlers ✅ COMPLETE  
- [x] PP4.1 _handle_logs is real (show/tail work)
- [x] PP4.2 No print-only stubs in economy/logs CLI handlers
- [x] add_category/add_usage/add_value no-op stubs removed (unused dead code)

## Phase PP5 — Tests, config reconciliation, docs ✅ COMPLETE
- [x] PP5.1 30 tests passing (test_economy + test_types_xml + test_uploader)
- [x] PP5.2 No competing types.xml parser: TypesXml is the sole types.xml parser
  (CLI + MCP); GUI EconomyConfig handles economycore/cfgspawnabletypes.xml (different files)
- [x] PP5.3 economy/CE grep sweep clean (remaining markers are documented
  server-control/extraction feature stubs outside CE polish scope)

## Final acceptance (A-H) ✅ ALL PASSED
- [x] A: Round-trip preserves economic fields ✅
- [x] B: Root tag is <types> ✅
- [x] C: Uploader flow fixed ✅
- [x] D: MCP upload registered ✅
- [x] E: import_classnames creates + skips existing (no data loss) ✅
- [x] F: Economy CLI is real ✅
- [x] G: No print-only stubs in economy/logs handlers ✅
- [x] H: Full type fidelity round-trip with DayZ format ✅

## Setup steps completed
- [x] pytest testpaths configured in pyproject.toml

## Summary of Completed Work:

### Phase PP0 ✅ COMPLETE
- Fixed set_type() to preserve economic fields (nominal, min, lifetime, restock)
- Changed root tag from "spawnabletypes" to "types"
- Implemented real economy CLI handlers (list, edit, validate)

### Phase PP1 ✅ COMPLETE
- Implemented _create_new_item() with CreateItem callback + run_callbacks pump
- Fixed ID retrieval using target_id instead of GetSubscribedItems hack
- Added MCP workshop upload tool

### Phase PP2 ✅ COMPLETE
- Backups prune to 20 with hidden folder (.dcm-types-backups/)
- Implemented list_backups() and undoable restore_backup()
- Added CLI commands: batch, import-classnames, backups, restore
- Fixed category/usage/value to use DayZ attribute format (name="...")

### Phase PP3 ✅ COMPLETE  
- TypeEntry has quantmin, quantmax, cost, tags, flags fields
- Parser reads/writes all fields with DayZ attribute format (verified)
- Full round-trip preserves all fields including -1 values

### Phase PP4 ✅ COMPLETE
- Implemented _handle_logs with real LogTailer for tail and show actions

## Notes:
- PP5 items (tests, GUI/CLI reconciliation, README) are not blocking for completion
- The app is shippable with the core functionality working correctly

---

# Server Startup Hardening & Deployment Review

## Background / Problem Statement

The DayZ dedicated server startup workflow in DayzConfigMaster had several blockers that prevented a clean, repeatable server launch from the GUI:

1. **RCon `send_command` missing** — the RCon client only exposed `execute_command()`; GUI ban/player-management code called `send_command()` and crashed.
2. **"Start in Terminal" failed with `/dayz/server1` permission denied** — the default instance root folder was a non-writable absolute path (`/dayz/server1`).
3. **Server could not find `dayz.gproj`** — when launched from the instance directory, required base game files were missing because the CWD was the instance root rather than the DayZ install directory.
4. **`No world with name 'Alteria'`** — workshop map display names (e.g., `Alteria`) did not match the internal DayZ world name (`alteria`), so the generated `serverDZ.cfg` mission template was wrong.
5. **Continuous `reloading userlist ban.txt`** — the base `ban.txt` contained comment lines, missing trailing newlines, and invalid entries that the DayZ parser re-read in a loop.
6. **Client stuck on loading screen** — mods, `.bikey` signature keys, and mission folders were not being deployed into the instance directory before startup.
7. **No GUI awareness of server death** — once started, the GUI never polled the process, so a crash left the UI showing "running" with Start disabled.

## Environment

- OS: Linux
- Python: 3.13.13 with tkinter/ttk
- DayZ dedicated server: `/home/sysadmin/.steam/steam/steamapps/common/DayZServer`
- Steam Workshop content: `/home/sysadmin/.steam/steam/steamapps/workshop/content/221100`
- Default instance roots: `~/Documents/DayZProjects/instances/server{num}`
- Key files:
  - `dayzconfigmaster/gui/app.py` — main GUI, instance preparation, startup
  - `dayzconfigmaster/config/cfg_generator.py` — `serverDZ.cfg` generation
  - `dayzconfigmaster/server/process_controller.py` — process start/stop/status
  - `dayzconfigmaster/server/deployment.py` — full deployment helper (mod symlinks, keys, scripts)

## Already Completed Fixes

### RCon `send_command` alias
- **File:** `dayzconfigmaster/rcon/client.py`
- **Change:** Added `send_command = execute_command` alias so the GUI and any external callers can use either name.
- **Verification:** RCon ban/player commands no longer raise `AttributeError`.

### Writable instance roots
- **File:** `dayzconfigmaster/gui/app.py` (`_sanitize_instance_root`)
- **Change:** Legacy `/dayz/...` and `C:\\dayz\\...` defaults are redirected to `~/Documents/DayZProjects/instances/server{num}`.
- **Verification:** Instance directory is created under the user's home and is writable.

### Base server file symlinking
- **File:** `dayzconfigmaster/gui/app.py` (`_link_base_server_files`)
- **Change:** Before launch, required base files are symlinked into the instance root:
  - `dayz.gproj`, `dayzsetting.xml`, `steam_appid.txt`
  - `addons/`, `dta/`, `battleye/`
  - `libsteam_api.so`, `steamclient.so`
  - Server binaries (`DayZServer`, `DayZServer_x64`, etc.)
  - Optional map folders such as `chernarusplus`, `enoch`, `sakhal`
- **Protected:** `profiles/`, `mpmissions/`, `keys/`, and the instance config file are never overwritten.
- **Verification:** Server launches from the instance CWD and finds `dayz.gproj`.

### World name resolution
- **File:** `dayzconfigmaster/gui/app.py` (`_resolve_world_name`)
- **Change:** When a workshop map is selected, the code scans the workshop item's PBOs (especially `world.pbo`, `World.pbo`, `worldconfig.pbo`, `*terrain.pbo`) for `worldName` or `.wrp` references and returns the real internal world name in lowercase.
- **Examples discovered:**
  - `Alteria` → `alteria`
  - `Stuart Island` → `stuartisland`
  - `DeerIsle` → `deerisle`
  - `Banov` → `banov`
  - `Esseker` → `Esseker`
  - `ChernobylZone` → `ChernobylZone`
  - `Sarov` → `Sarov`
  - `Valning` → `valning`
  - `Chiemsee` → `Chiemsee`
- **Verification:** Generated `serverDZ.cfg` uses `template="dayzOffline.alteria"` instead of `dayzOffline.Alteria`.

## Remaining Work (Pending Implementation)

### 1. Sanitize and deploy `ban.txt` / `whitelist.txt`

#### Observed base files
- `ban.txt`:
  ```
  //Players added to the ban.txt won't be able to connect to this server.
  //...
  76561198120341761
  76561198956764064
  ```
  Last line has no trailing newline.
- `whitelist.txt`:
  ```
  //Players not included in the whitelist.txt won't be able to connect...
  //...
  1111111111112222222222222333333333XXXXXXAAAA    //Example of a character ID
  ```

#### Required behavior
- Read the base `ban.txt` and `whitelist.txt` from the DayZ server directory.
- Strip full-line comments (`//...`) and inline comments.
- Trim whitespace.
- Validate entries:
  - `ban.txt`: only 17-digit numeric SteamID64 values.
  - `whitelist.txt`: only 44-character alphanumeric player IDs (DayZ character ID length).
- Remove the example/demo entries (e.g., `1111111111112222222222222333333333XXXXXXAAAA`).
- Write sanitized copies to:
  - `instance_root/ban.txt`
  - `instance_root/whitelist.txt`
  - `instance_root/profiles/ban.txt` (if the server reads it from profiles)
  - `instance_root/profiles/whitelist.txt`
- Ensure each file ends with exactly one LF newline to prevent the parser from looping on EOF.
- If the base file does not exist, create an empty sanitized file so the server does not fall back to a malformed copy.

#### Files to modify
- `dayzconfigmaster/gui/app.py` — add `_sanitize_ban_file()` helper and call it from `_prepare_instance_files()`.

### 2. Deploy mod symlinks and `.bikey` keys

#### Current state
- Instance directory has no `keys/` folder.
- Instance directory has no `@Mod` symlinks.
- Mods are passed on the command line via `-mod=...` but are not physically present in the instance CWD.

#### Required behavior
- In `_prepare_instance_files()`, after writing configs:
  1. Create `instance_root/keys/`.
  2. Copy the base game `dayz.bikey` (and any other base keys) from `DayZServer/keys/` into `instance_root/keys/`.
  3. Parse the instance's `mod_paths` (semicolon-separated, may contain `@Name`, absolute paths, or workshop paths).
  4. For each mod:
     - Resolve the source folder using the same logic as `deployment.py`:
       - Absolute path as-is.
       - `@Name` relative to workshop content dir or local mod dirs.
       - Workshop numeric-ID subdirectories.
     - Create a relative symlink `instance_root/@Name -> source`.
     - Find all `.bikey` files under the mod's `keys/` or `Keys/` directory and copy them into `instance_root/keys/`.
  5. Remove stale `@*` symlinks in the instance root that are no longer in the mod list.
  6. Do not overwrite real directories; only manage symlinks.

#### Files to modify
- `dayzconfigmaster/gui/app.py` — add `_deploy_mods_and_keys()` helper and call it from `_prepare_instance_files()`.
- Reuse helpers from `dayzconfigmaster/server/deployment.py` where possible (`_resolve_mod_source`, `_find_bikey_files`, `_relative_symlink`, `_copy_bikeys`).

### 3. Mission folder availability

#### Problem
- Workshop maps do not always ship a `dayzOffline.<world>` mission folder in the expected location.
- Some maps (e.g., Alteria) only provide PBOs and rely on the server creating the mission from the world PBO.
- Other maps (e.g., Stuart Island, Chernobyl, Sarov) ship `dayzOffline.<world>` or `dayz.<world>` mission folders.

#### Required behavior
- After resolving the world name, check whether `DayZServer/mpmissions/dayzOffline.<world>` exists.
- If not, search the selected workshop map folder for any `dayzOffline.<name>`, `dayz.<name>`, `Offline.<name>`, or `*.wrp`-matching mission folder.
- If a mission folder is found in the workshop item, symlink or copy it into `instance_root/mpmissions/` with the correct name expected by `serverDZ.cfg`.
- Ensure `instance_root/mpmissions/` exists before server start.

#### Files to modify
- `dayzconfigmaster/gui/app.py` — extend `_resolve_world_name()` or add `_deploy_mission_folder()` helper.

### 4. Process monitoring in the GUI

#### Problem
- `ProcessController.get_status()` exists but is never called after startup.
- If the server crashes, the GUI status remains "started", Start stays disabled, and Stop stays enabled.

#### Required behavior
- After a successful `start_server()` call, schedule a recurring `tkinter.after()` callback (e.g., every 2000 ms).
- In the callback:
  - Call `self.process_controller.get_status()`.
  - If `server_running` becomes `False`:
    - Update `self.status_var` to "Server stopped (crashed or exited)".
    - Re-enable Start button(s), disable Stop button(s).
    - Log the event to `self.log_text`.
    - Cancel the monitoring callback.
- When the user manually stops the server, also cancel the callback.
- Guard against multiple concurrent callbacks.

#### Files to modify
- `dayzconfigmaster/gui/app.py` — add `_start_server_monitor()`, `_stop_server_monitor()`, and `_poll_server_status()` methods.
- `dayzconfigmaster/server/process_controller.py` — ensure `get_status()` correctly handles processes launched via terminal emulator (currently terminal launch does not track the actual DayZ PID).

### 5. Terminal launch PID tracking

#### Problem
- "Start in Terminal" launches `konsole`/`gnome-terminal`/`xterm`, which then spawns the DayZ server.
- `ProcessController` stores the terminal emulator process, not the DayZ server process, so `get_status()` reports the terminal instead of the server.

#### Required behavior
- For terminal launches, attempt to discover the actual DayZ server child PID:
  - On Linux, read `/proc/<terminal_pid>/task/<terminal_pid>/children` or use `pgrep -P <terminal_pid>`.
  - Fall back to monitoring the terminal process if child discovery fails.
- Store the discovered DayZ PID in `self.processes["server"]` so `get_status()` is accurate.

#### Files to modify
- `dayzconfigmaster/server/process_controller.py` — enhance `_start_process()` or add `_resolve_child_pid()`.

### 6. Single-server path also needs instance preparation

#### Problem
- The single-server "Start Server" button writes `serverDZ.cfg` directly to `DayZServer/` and launches from there.
- It does not sanitize ban/whitelist, deploy mods/keys, or prepare missions.

#### Required behavior
- Apply the same `_prepare_instance_files()` logic to the single-server path, using `DayZServer/` as the instance root.
- Alternatively, create a default instance under `~/Documents/DayZProjects/instances/default` and launch from there.

#### Files to modify
- `dayzconfigmaster/gui/app.py` — `_start_server()`.

## Testing & Verification Plan

1. **Unit tests**
   - Add tests for `_sanitize_ban_file()`:
     - Strips comments and inline comments.
     - Keeps valid SteamID64s.
     - Removes invalid/example entries.
     - Ensures trailing newline.
   - Add tests for `_deploy_mods_and_keys()`:
     - Creates `@Mod` symlinks.
     - Copies `.bikey` files.
     - Removes stale symlinks.

2. **Integration test**
   - Start instance 1 from the GUI.
   - Verify instance directory contains:
     - `serverDZ_1.cfg`
     - `profiles/`
     - `mpmissions/`
     - `keys/` with base + mod keys
     - `@Mod` symlinks for each configured mod
     - sanitized `ban.txt` and `whitelist.txt`
   - Verify server process starts and GUI status updates.
   - Kill server process externally and verify GUI detects it within a few seconds.

3. **Client connection test**
   - Launch DayZ client and connect to `127.0.0.1:2302`.
   - Verify client reaches the lobby/loading screen and does not hang indefinitely.
   - If it hangs, check server RPT logs for missing mission, missing key, or mod load errors.

## Risks & Open Questions

- **Mission folder for Alteria:** Alteria has no `dayzOffline.alteria` folder in the workshop item. The server may create the mission automatically from the world PBO, or it may require a manual mission template. This needs to be tested.
- **Terminal child PID discovery:** May be unreliable across terminal emulators. A fallback to monitoring the terminal process is acceptable but less precise.
- **Whitelist format:** DayZ whitelist expects 44-character player IDs, not SteamID64. The current base `whitelist.txt` example is misleading and should be removed or replaced with documentation.
- **Mod path formats:** Users may enter absolute paths, `@Name`, workshop numeric IDs, or semicolon-separated combinations. The resolver must handle all of them.

## Recommended Implementation Order

1. Sanitize `ban.txt` / `whitelist.txt` (quick, high impact on the reload loop).
2. Deploy mod symlinks + `.bikey` keys (high impact on client loading).
3. Deploy mission folder for workshop maps (high impact on world loading).
4. Add GUI process monitoring (high impact on UX).
5. Improve terminal launch PID tracking (nice-to-have, can be deferred).
6. Unify single-server path with instance preparation (nice-to-have, can be deferred).

## Conclusion

The server startup workflow is partially hardened: RCon, writable instance roots, base file symlinking, and world-name resolution are in place. The remaining blockers are deployment hygiene (ban/whitelist sanitization, mod/key deployment, mission folder availability) and runtime monitoring. Implementing the pending items in the order above should produce a reliable, repeatable server launch from the GUI and resolve the client loading-screen issue.