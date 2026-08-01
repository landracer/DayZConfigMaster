# Server Config — Mods Tab

The **Mods** tab on the **Server Config** screen manages the workshop and local mods that are available to your server instances.

## Mod List

The left pane shows every mod that DayZ Config Master has discovered. Mods can come from:

- The Steam Workshop directory (`steamapps/workshop/content/221100/<workshop_id>`)
- Local directories you browse to manually

Each row displays the mod's display name (read from `meta.cpp` or `mod.cpp`) and its workshop ID when known.

## Selecting Mods for an Instance

1. Choose the instance you want to configure from the instance selector.
2. Check the box next to each mod you want loaded for that instance.
3. Selected mods are applied when the instance is prepared and started.

## Button Reference

| Button | Purpose |
|--------|---------|
| **+ Add Mod** | Browse the filesystem and add a local mod directory to the list. |
| **- Remove Selected** | Remove the selected mod entries from the list. |
| **Refresh List** | Rescan the workshop and local mod directories. |
| **Update Mods** | Re-download every selected workshop mod via `steamcmd` to ensure it is on the latest version. Maps are skipped because they are managed by the **Map** dropdown. |
| **Cleanup Unsubscribed** | Delete workshop folders for mods you have deselected or unsubscribed from Steam. |

## Cleanup Unsubscribed — How It Works

The **Cleanup Unsubscribed** button scans your workshop directory for leftover mod folders that are no longer needed and offers them for deletion. It uses a two-pass detection strategy to handle the fact that Steam's subscription record (the `.acf` file) may be out of date:

### Pass 1 — Steam Subscription File (.acf)

Steam stores its subscription state in `appworkshop_221100.acf`, located inside your Steam installation. This file lists every workshop item that is currently subscribed and the disk folder where it lives.

**Important:** The `.acf` file **does not update instantly** when you unsubscribe a mod through the Steam client or web interface. There can be a delay of minutes to hours (or even days) before Steam writes the new state. During this window:

- The item still appears as "subscribed" in the `.acf` file
- DayZConfigMaster cannot see it as unsubscribed via Pass 1
- Your mod will still show on disk and is protected from deletion

If Pass 1 finds orphan folders (items in the `.acf` marked unsubscribed), they are immediately offered for deletion. This is the fastest and most reliable detection path.

### Pass 2 — GUI Active-Mod Comparison (Fallback)

When Pass 1 finds **zero** orphans (the ACF still lists everything as subscribed), DayZConfigMaster performs a fallback scan:

1. It reads every folder on disk under `steamapps/workshop/content/221100/<workshop_id>/`
2. It compares those IDs against the **currently active mod list** in your GUI (mods you have checked on for loading)
3. Any folder whose ID is **not present** in your active mod list becomes an orphan candidate

This means:
- If you deselected a mod in the GUI but haven't unsubscribed it from Steam yet, it will still show up and can be cleaned up
- If you unsubscribed mods through Steam but also deselected them in the GUI, they appear immediately (no need to wait for ACF sync)
- **Active/selected mods are always protected** — they will never be offered for deletion

### When Should You Use Cleanup Unsubscribed?

| Scenario | When to run cleanup | What you'll see |
|----------|-------------------|-----------------|
| Deselected mods in DCM GUI (still subscribed on Steam) | Immediately after deselecting | Those mods appear as orphans via Pass 2 |
| Unsubscribed via Steam client/web | After Steam syncs the `.acf` file (may take hours) | Orphans detected via Pass 1 |
| Unsubscribed via DCM's local unsubscribe button | Immediately | Detected immediately by both passes |
| Reset/reinstalled Steam | Run after re-syncing subscriptions | Depends on ACF state after Steam restart |

### Best Practices

1. **Deselect first, then clean up:** Remove mods from your active list in the GUI, then click "Cleanup Unsubscribed" to remove old folders immediately without waiting for Steam.
2. **Restart Steam to sync faster:** If you unsubscribed via the Steam client, fully closing and reopening Steam helps it update the `.acf` file sooner.
3. **Re-download before deleting:** If you're unsure whether a folder should be removed, try "Re-download Selected" first — it will refresh the mod to its latest version.
4. **Review before confirming:** The cleanup dialog lets you uncheck any items you want to keep. Always review the list and total disk space before confirming deletion.
5. **Run after bulk changes:** After adding or removing many mods at once, run Cleanup Unsubscribed to remove stale folders and reclaim disk space.

## Updating Mods

Because the DayZ server does not run the Steam client, workshop mods can fall behind the versions used by players. Use the **Update Mods** button to refresh them:

1. Select the mods you want to update in the mod list.
2. Click **Update Mods**.
3. A progress window shows the status of each download.
4. When finished, the mod list and map dropdowns are refreshed automatically.

> **Note:** The update process uses `steamcmd` with an anonymous login. Mods that require authentication or are no longer available will be reported in the completion summary.
