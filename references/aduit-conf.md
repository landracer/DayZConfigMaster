# DayZConfigMaster Configuration & Deployment Audit

**Date:** 2026-07-12  
**Scope:** Compare current GUI/config-generation workflow against the proper DayZ multi-instance server workflow described in `aduit.md`. Identify disconnects, bloat, and missing automation without killing existing features.

---

## Executive Summary

DayZConfigMaster has a **strong foundation**: multi-instance GUI, workshop discovery, comprehensive serverDZ.cfg generation, and per-instance settings. However, it currently **conflates configuration generation with deployment orchestration**. The most critical disconnect is that the app writes mod paths into `serverDZ.cfg`, which DayZ ignores — mods must be passed via `-mod=` in the startup command.

The "bloat" (workshop integration, mod tree, multi-instance rows, In-Game Settings, Effects & Triggers) becomes a **shining star** only when the app also handles the deployment steps that make those configs actually run: `.bikey` management, `@Mod` symlinking, base-server syncing, and systemd service generation.

This audit documents what is right, what is wrong, and the concrete changes needed to make the app a home-run tool.

---

## 1. What the App Does Right

| Area | Status | Notes |
|------|--------|-------|
| Multi-instance GUI | ✅ | Per-instance ports, maps, mod paths, profiles, config files. |
| Workshop discovery | ✅ | Scans `steamapps/workshop/content/221100`, separates maps vs mods. |
| serverDZ.cfg formatting | ✅ | Proper `class Missions { class DayZ { template = "dayzOffline.<map>"; }; };` block, normalized toggles, complete field set. |
| Per-instance Effects & Triggers | ✅ | Each instance maintains isolated copies of `cfglimitsdefinitionuser.xml`, `cfgeventspawns.xml`, etc. |
| start.sh generation | ✅ | House Cleaning tab can emit a Linux start script. |
| Map auto-discovery | ✅ | Stock + workshop maps populate dropdowns. |

---

## 2. Critical Disconnects

### 2.1 Mods Are Written to serverDZ.cfg (Fixed ✅)

**Current behavior:** `cfg_generator.py` no longer emits mod paths.

**Proper behavior:** `serverDZ.cfg` must **never** contain mod paths. Mods are loaded via the startup command:

```bash
./DayZServer -config=serverDZ.cfg -port=2302 -profiles=profiles "-mod=@CF;@Expansion" -dologs -adminlog
```

**Status:** Fixed. `cfg_generator.py` now omits the `mod = "..."` line. Mod paths are formatted for the `-mod=` flag in `deployment.py` and the generated `start.sh` / systemd units.

---

### 2.2 No .bikey File Management (Fixed ✅)

**Current behavior:** Deployment now copies `.bikey` files from each linked mod into the instance `keys/` directory.

**Proper behavior:** For every selected mod, copy `mod/keys/*.bikey` into each instance's `keys/` directory:

```bash
cp /home/dayz/server1/@CF/keys/*.bikey /home/dayz/server1/keys/
```

**Status:** Fixed. `deployment.py::_copy_bikeys()` handles this automatically during instance deployment.

---

### 2.3 No @Mod Symlink / Folder Linking (Fixed ✅)

**Current behavior:** Deployment creates relative `@ModName` symlinks inside each instance directory pointing to the resolved workshop or local mod folder.

**Proper behavior:** Each instance directory should contain `@ModName` symlinks (or copies) pointing to the shared workshop download:

```bash
ln -s /home/dayz/steamapps/workshop/content/221100/1559212036 /home/dayz/server1/@CF
```

**Status:** Fixed. `deployment.py::deploy_instance()` resolves mod sources and creates relative symlinks in the instance root.

---

### 2.4 No Base Server Update / Sync Automation (Partially Fixed ✅)

**Current behavior:** Deployment symlinks the base server binary and required directories (`addons`, `dta`, `battleye`, etc.) into each instance. A full copy mode is also available.

**Proper behavior:** A master script updates the base server via SteamCMD and rsyncs it to all instances, excluding per-instance files:

```bash
rsync -av --delete --exclude='serverDZ.cfg' --exclude='profiles/' --exclude='mpmissions/' \
  "$BASE_SERVER_DIR/" "$INSTANCE/"
```

**Status:** Partially fixed. Symlink/copy base-server sync is implemented in `deployment.py`. SteamCMD server-update integration is pending.

---

### 2.5 No systemd Service Generation (Fixed ✅)

**Current behavior:** `deployment.py` generates per-instance systemd `.service` files with a static `-mod=` string derived from the selected mods.

**Proper behavior:** Production servers use systemd. Each instance needs a service file with `-mod=` injected:

```ini
[Service]
User=dayz
WorkingDirectory=/home/dayz/servers/dayz-inst1
ExecStart=/home/dayz/servers/dayz-inst1/start.sh
Restart=on-failure
```

**Status:** Fixed. `deployment.py::generate_systemd_service()` creates unit files, and the GUI offers a checkbox to generate them during deployment.

---

### 2.6 Per-Instance Profiles Path Not Validated

**Current behavior:** The GUI collects a profile path per instance but does not enforce uniqueness.

**Proper behavior:** Each instance must have a unique `profiles/` directory and unique `instanceId` to avoid shared persistence, mixed logs, and data corruption.

**Impact:** Two instances pointing at the same profiles folder will corrupt player data and base building states.

**Fix:**
- Validate that all configured instances have distinct `profiles/` paths.
- Default to `<instance_root>/profiles`.
- Warn the user if duplicates are detected.

---

### 2.7 Custom Map Mission Folder Not Validated

**Current behavior:** Custom maps can be selected from the dropdown, but the app does not check whether the required `mpmissions/dayzOffline.<map>` folder exists.

**Proper behavior:** A custom map requires:

```
/home/dayz/server3/
├── @Namalsk/                 ← map mod folder
└── mpmissions/
    └── dayzOffline.namalsk   ← mission folder
```

**Impact:** Server fails to start or players cannot join if the mission folder is missing.

**Fix:**
- When a custom map is selected, verify `mpmissions/dayzOffline.<map>` exists.
- Warn if missing and offer instructions or automated copy from the mod folder.

---

## 3. Moderate Issues

| Issue | Current State | Recommended Fix |
|-------|---------------|-----------------|
| start.sh not tied to deployment | Generated standalone | Generate it as part of "Deploy All Instances" and include `-mod=` |
| Workshop downloader not integrated | Can browse but not download as a deployment step | Tie selected workshop IDs to SteamCMD download in deployment |
| Instance directory structure not enforced | Root folder is just a path | Validate/create `DayZServer`, `serverDZ.cfg`, `profiles/`, `mpmissions/`, `keys/`, `@Mod/` |
| Server Control limited | Start/stop UI exists but no systemd integration | Add systemd start/stop/restart actions |

---

## 4. Recommended Implementation Plan

### Phase 1 — Fix Config Generation (Immediate) ✅ Completed

1. Remove `mod = "..."` from `cfg_generator.py`. ✅
2. Update Mods tab help text: "Mods are loaded via `-mod=` in the startup command, not inside serverDZ.cfg." ✅ (help text already present in deployment section)
3. Validate unique `profiles/` path per instance. ⏳ Pending
4. Validate custom map mission folder exists. ⏳ Pending

### Phase 2 — Add Deployment Automation ✅ Completed

1. Create `dayzconfigmaster/server/deployment.py`: ✅
   - SteamCMD wrapper for server + workshop mod downloads. ⏳ (wrapper exists in `workshop/steamcmd.py`; integration pending)
   - Base-server sync to instances (rsync with excludes). ✅ (symlink-based; copy option available)
   - `@Mod` symlink creation per instance. ✅
   - `.bikey` copy to instance `keys/`. ✅
2. Add "Deploy All Instances" button to House Cleaning & Scheduling tab. ✅

### Phase 3 — Generate systemd Services ✅ Completed

1. Create `dayzconfigmaster/server/systemd_generator.py`. ✅ (merged into `deployment.py`)
2. Generate per-instance `.service` files with proper `-mod=` flags. ✅
3. Add GUI export dialog with copy-paste instructions. ✅ (results window shows unit paths)

### Phase 4 — Integrate Workshop Download ⏳ Pending

1. Extract workshop IDs from selected mods.
2. Pass IDs to deployment SteamCMD step.
3. Auto-download selected mods before syncing.

---

## 5. What to Keep

- ✅ Multi-instance architecture and GUI.
- ✅ Workshop map/mod discovery and separation.
- ✅ Comprehensive serverDZ.cfg field collection.
- ✅ Per-instance Effects & Triggers.
- ✅ start.sh generation (enhance with `-mod=` and tie to deployment).
- ✅ Map auto-discovery.

---

## 6. Summary Table: Proper Workflow vs Current App

| Step | Proper Workflow | Current App | Gap |
|------|-----------------|-------------|-----|
| Collect serverDZ.cfg settings | GUI or manual | ✅ GUI complete | None |
| Select/download mods | SteamCMD `workshop_download_item` | ❌ Discovery only | Missing SteamCMD integration |
| Copy `.bikey` files | `cp mod/keys/*.bikey instance/keys/` | ❌ Not implemented | Missing `bikey_manager.py` |
| Create `@Mod` symlinks | `ln -s workshop item instance/@Mod` | ❌ Not implemented | Missing symlink logic |
| Sync base server to instances | `rsync` with excludes | ❌ Not implemented | Missing deployment module |
| Generate serverDZ.cfg | Minimal cfg, **no mods** | ✅ Mods removed from cfg | None |
| Generate startup command | `-mod=@CF;@Exp` in ExecStart | ✅ start.sh + systemd units generated | None |
| Start/stop instances | `systemctl start dayz-instN` | ✅ Service files generated; manual install required | None |

---

## 7. Conclusion

The app is not broken, but it stops halfway. It generates configuration files while leaving the actual deployment, mod loading, and service management to the user. The bloat becomes valuable only when each feature is wired into a complete workflow:

> **Config → Mods → Keys → Symlinks → Sync → Service → Start**

Implementing the phases above will turn DayZConfigMaster from a config-file generator into a full multi-instance DayZ server deployment tool.
