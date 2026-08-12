# Modding Configuration

This guide covers configuring and managing mods for DayZ servers using DayZConfigMaster.

## How Mods Are Managed

DayZConfigMaster uses a **single Workshop directory** as the source of truth across all tabs. The application:

1. Auto-detects your Steam Workshop content path (e.g. `.../steamapps/workshop/content/221100`)
2. Scans all subscribed workshop items
3. **Automatically separates maps from mods** so only mods appear in the mod selection tree
4. Parses human-readable names from `mod.cpp` and `meta.cpp` files
5. Displays the full folder path for each mod

## Map vs Mod Separation

Workshop items are classified using these rules (in order):

1. **`.map` files** anywhere in the item → classified as a **map**
2. **`dayzOffline.[mapname]` folder** → classified as a **map**
3. **`config.cpp` without map references** → classified as a **mod**
4. **Name heuristic** — if `mod.cpp` / `meta.cpp` contains known map keywords (e.g. `map`, `island`, `chernarus`, `livonia`, `namalsk`, `deerisle`, `chiemsee`, `valning`, `banov`, `esseker`, `rostow`, `takistan`, `raman`, `alteria`, `sarov`, `stuart`, `xzone`, `chernobyl`) → classified as a **map**
5. **Default** → classified as a **mod**

Maps appear in the **Map Name** dropdown on the **Server Config → Mods** tab and the **Server Control** tab. Mods appear in the **Mod Selection** treeview on the **Server Config → Mods** tab.

## Mod Settings Tab

The **Server Config → Mod Settings** tab is a built-in editor for every config file that ships inside your subscribed workshop mods and your active mission folder.

### What It Scans

DayzConfigMaster automatically discovers settings files from two places:

1. **Steam Workshop mod folders** — every numeric folder under `.../steamapps/workshop/content/221100` is inspected.
   - The mod's display name is read from `meta.cpp` or `mod.cpp`.
   - Config files with these extensions are collected recursively:
     - `.json` — validated as JSON
     - `.xml` — validated as XML
     - `.cpp`, `.hpp`, `.txt` — treated as plain text
   - Binary folders (`addons/`, `Keys/`) and mod metadata (`meta.cpp`, `mod.cpp`) are ignored.
2. **Active mission folder** — files from the currently selected map's mission directory are also shown, grouped by known folders such as `Mission Core`, `Mission Database`, `DayZ Expansion`, and `TraderPlus`.

### Automatic Updates

New mods and maps appear automatically as soon as Steam finishes downloading them. You do **not** need to restart DayzConfigMaster:

- Click the **Mod Settings** tab to trigger a fresh scan.
- Click **🔄 Refresh** to rescan the workshop directory and mission folder on demand.
- Newly subscribed mods show up under their display name with their config files listed by relative path (e.g. `TraderPlus/Outdated_TraderPlusIDsConfig.json`).

> 💡 **Tip:** If a mod has no `.json`, `.xml`, `.cpp`, `.hpp`, or `.txt` files outside `addons/` and `Keys/`, it will not appear in the Mod Settings tree. That simply means the mod does not expose editable text configs.

### Editing & Saving

1. Select a file in the left tree.
2. Edit the file in the right-hand editor.
3. Click **Validate** to check JSON/XML syntax.
4. Click **Save** to write the file. A timestamped backup is created in the same folder before overwriting.

> ⚠️ **Warning:** Files inside the Steam Workshop directory are managed by Steam. DayzConfigMaster warns you before saving there because Steam may overwrite your changes on the next update. For permanent edits, copy the config into your mission folder or instance directory.

### Repair Tires

The **Repair Tires** button fixes common `cfgspawnabletypes.xml` corruption that removes vehicle wheels after mod integration:

- Removes invalid XML comments containing `--`.
- Adds missing wheel attachments for known vanilla vehicles.
- Backs up the original file before modifying it.

## Types of Mods

### Mission/Gameplay Mods
Change gameplay mechanics:
• **@CF** - Community Framework (required by many other DayZ mods)
• **@DayZ-Community-Online-Tools** - Admin and player management tools
• **@BuilderItems** - Additional base-building items

### Content Mods
Add items, vehicles, weapons:
• **@MasPuertas** - Additional doors and building options
• **@DayZ-Expansion** - Expansion mod suite (vehicles, base building, UI)
• **@SchanaModGlobalChat** - Global chat mod

### UI Mods
Modify user interface:
• Custom HUD replacements
• Menu and inventory tweaks

### Map Mods
Replace default maps with custom ones. These are **not** selected as mods — they are chosen from the **Map Name** dropdown:
• **DeerIsle** - Custom island map
• **Namalsk** - Cold climate survival map
• **Banov** - Eastern European terrain
• **Esseker**, **Rostow**, **TakistanPlus**, **Raman**, **Alteria**, **Sarov**, **Stuart Island**, **Valning**, **Chiemsee**, **XZone Chernobyl**, and seasonal variants

## Required Server Configuration

### Verify Signatures

```cpp
verifySignatures = 2;  // 0=Off, 1=Debug, 2=Verify (recommended)
```

• **2 (Verify)** - Ensures mod signatures are valid (most secure)
• **1 (Debug)** - Allows unsigned mods for development/testing
• **0 (Off)** - No signature checks (not recommended for public servers)

### Force Same Build

```cpp
forceSameBuild = 1;  // 0=No, 1=Yes
```

Ensures all players use the same game version as the server.

### Mod Paths

DayZConfigMaster generates the `mod` line automatically from your selections. When using full folder paths (the default):

```cpp
mod = "/path/to/steamapps/workshop/content/221100/1559212036;/path/to/steamapps/workshop/content/221100/123456789";
```

Using full paths avoids case-sensitivity issues on Linux. You can also use traditional `@ModName` notation for locally installed mods.

## Selecting Mods in the GUI

### Server Config → Mods Tab

The **Server Config → Mods** tab contains the main mod selection interface:

1. **Map Selection** — choose the server map from stock and workshop maps
2. **Workshop Directory** — auto-detected; use the **Browse** button only if you need to override it
3. **🔄 Scan Workshop** — refreshes the mod tree from your local workshop content
4. **Mod Selection treeview** — shows:
   - **Status** — checkbox (`☑` selected / `☐` not selected)
   - **Mod Name** — parsed from `mod.cpp` / `meta.cpp`
   - **Workshop ID** — the Steam Workshop item ID
   - **Folder Path** — full path to the workshop item folder

Click any row to toggle the checkbox. Selected mods are automatically added to the **Mod Paths** field using their full folder paths.

### Adding Local Mods

Use the **+ Add Mod** button to browse for a local mod directory, or the **Browse** button next to the **Mod Paths** field. The selected folder path is appended to the mod list.

### Removing Mods

Select one or more rows and click **- Remove Selected** to remove them from the tree and the **Mod Paths** field.

## Steam Workshop Integration

### Download Mods via SteamCMD

```bash
# SteamCMD command to download a workshop item
app_download <APP_ID> <WORKSHOP_ITEM_ID>

# DayZ app ID is 221100
```

You can also use the built-in **Workshop** tab or the CLI:

```bash
# Search for a mod
dzl workshop search "Community Framework"

# Download by ID
dzl workshop download --id 1559212036
```

## Popular Community Mods

| Mod | Description | Requirements |
|-----|-------------|--------------|
| @CF | Community Framework | Required by many DayZ mods |
| @DayZ-Community-Online-Tools | Admin and player management | Usually requires @CF |
| @MasPuertas | Additional doors/buildings | None |
| @DayZ-Expansion | Expansion mod suite | @CF, @DayZ-Expansion-Licensed |
| @SchanaModGlobalChat | Global chat mod | None |

## Installation Steps

1. **Subscribe** to the mod on Steam Workshop (or download it externally)
2. **Wait for Steam to download** the item to `.../steamapps/workshop/content/221100/<workshop_id>`
3. **Open DayZConfigMaster** and go to the **Server Config** tab
4. **Click 🔄 Scan Workshop** to populate the mod tree
5. **Check the mods** you want to use
6. **Open the Mod Settings tab** to edit per-mod JSON/XML/CPP/HPP/TXT configs (optional)
7. **Generate or save** your `serverDZ.cfg`
8. **Ensure all players have the same mod version**
9. **Test on a local machine** before deploying to production

## Common Issues and Solutions

### Issue: "Signature verification failed" / "Client has a PBO which is not part of the server"
• Solution: Ensure `.bikey` files from every loaded mod's `keys/` folder have been copied into the instance `keys/` directory. DayzConfigMaster does this automatically at startup when mods are listed in **Mod Paths** and resolved successfully. Some mods ship their key in a singular `Key/` folder; this is handled automatically.
• Solution: Verify every mod in the client's `-mod=` list is also loaded by the server, with matching case and version.
• Solution: Run `python3 dayzconfigmaster/tools/mod_sanity_check.py --running` to verify wrappers, PBO visibility, and copied keys.
• Solution: Set `verifySignatures = 1;` for development or ensure proper signatures.

### Issue: Players can't join
• Solution: Verify all players have the same mod versions installed.

### Issue: Mod not loading
• Solution: Check the folder path in the **Mod Paths** field is correct and case-sensitive on Linux.
• Solution: Verify the mod is classified as a mod (not a map) in the mod tree.
• Solution: If you entered a display name such as `@Community Framework`, confirm it matches the `name` value in the mod's `mod.cpp` or `meta.cpp`.
• Solution: Make sure required dependency mods (e.g. @CF) are also selected

## Performance Considerations

Some mods significantly increase memory usage:
• **Audio mods** (@JSRS): +200-500 MB RAM
• **Vehicle mods**: +300-600 MB RAM  
• **Map mods**: +100-300 MB RAM

Monitor with `logMemory = 1;` and adjust player counts accordingly.

## Security Best Practices

1. Use `verifySignatures = 2;` for production servers
2. Download mods only from trusted sources
3. Keep mod versions consistent across all instances
4. Regularly update mods to patch security vulnerabilities
5. Review mod permissions and capabilities before installation