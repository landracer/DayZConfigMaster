# Mod Corruption Detection and Recovery

## Problem: Corrupted or Broken Mod Folders

DayZ on Linux can encounter issues when mod folders become corrupted. Common symptoms include:

### Symptoms of Corrupted Mods
1. **Server fails to start** - No useful error message in logs
2. **"Is a directory" errors** - The server tries to execute a folder path
3. **Symlink loops** - Infinite recursion when following symlinks
4. **"Untangle" required** - Mod paths with broken links

### Root Causes
1. **Partial downloads** - SteamCMD stopped before completion
2. **Disk full** - Write interrupted mid-download
3. **Broken symlinks** - Original workshop item removed/renamed
4. **Filesystem corruption** - Rare, but possible on failing drives

## Solution: Mod Sanity Check System

### The Untangle Mods Button

The GUI includes an "Untangle Mods" button on the Server Config tab that:

1. **Scans all mods** in your workshop directory
2. **Detects corrupted folders** (missing files, broken symlinks)
3. **Validates folder structure** (addons/, keys/ directories present)
4. **Computes checksums** of critical files (meta.cpp, mod.cpp)

### How to Use

1. Open **Server Config > Mods** tab
2. Click **🔧 Untangle Mods (Check & Fix Folders)**
3. Wait for the scan to complete
4. Review any red-flagged mods
5. For problematic mods:
   - Click "Re-download Selected Mod" on the Mods tab to refresh from Steam
   - Or use "Cleanup Unsubscribed" if you no longer need the mod (see [Server Config — Mods Tab](./server-config-mods.md#cleanup-unsubscribed---how-it-works))

### MD5 Checksum Verification

The system computes MD5 checksums of critical mod files on first run:

```python
# Example: Check a single mod
from dayzconfigmaster.mods.sanity_check import ModChecker
from pathlib import Path

checker = ModChecker(Path("/home/sysadmin/.steam/steam/steamapps/workshop/content/221100"))
results = checker.check_all_mods()

for workshop_id, result in results.items():
    if not result.get("success"):
        print(f"Mod {workshop_id} FAILED: {result['issues']}")
```

### Manual Recovery Steps

If the "Untangle Mods" button doesn't fix an issue:

1. **Delete the corrupted folder**:
   ```bash
   rm -rf /path/to/workshop/content/221100/CORRUPTED_ID
   ```

2. **Resubscribe in Steam Workshop** or re-download via "Re-download Selected"

3. **Refresh mod list** in DayzConfigMaster

4. **Start server again**

## Prevention: Regular Health Checks

Run sanity checks periodically:

```bash
# Check all mods every Sunday at 2 AM
0 2 * * 0 python -m dayzconfigmaster.mods.sanity_check > /var/log/dayz_mod_checks.log
```

Or use the GUI button once a week before your server restart window.

## Documentation Links

- [Server Configuration](./server-config.md)
- [Modding](../modding.md)
- [Server Control](./server-control.md)