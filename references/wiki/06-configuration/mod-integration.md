# Mod Integration

Loading a mod via the server's `-mod=` flag makes the mod's PBOs available, but
it does **not** automatically make the mod's content spawn in the world. The
server must also register the mod's classnames in its mission XML files.

DayzConfigMaster provides a **Mod Integration** tab that scans your active mods
for XML fragments and merges them into the correct mission files automatically.

## Why Mod Integration Is Needed

DayZ mission files control the in-game economy:

| File | Purpose |
|------|---------|
| `db/types.xml` | How many of each item/vehicle exist on the map |
| `cfgspawnabletypes.xml` | What attachments/parts spawn with items |
| `db/events.xml` | Event definitions (vehicle spawns, loot events) |
| `cfgeventspawns.xml` | Specific coordinates where events can spawn |
| `db/economy.xml` | Global economy settings |
| `cfglimitsdefinition.xml` / `cfglimitsdefinitionuser.xml` | Loot category limits |

Mods that add vehicles, weapons, clothing, or base-building items ship XML
snippets that must be merged into these files. Without integration, clients may
see the mod load but never encounter the content.

## How It Works

1. **Deploy mods** — Start the server (or use the Mod Selection tab) so that
   `@ModName` wrappers are created in the instance directory.
2. **Open Mod Integration** — In **Server Control → Mod Integration**, choose
   the instance you want to configure.
3. **Scan mods** — Click **🔄 Scan Mods**. DayzConfigMaster searches every
   active mod folder for XML fragments.
4. **Select mods to integrate** — Check the **Integrate** box for each mod whose
   XML you want merged.
5. **Apply** — Click **✅ Apply Integration**. The tool backs up the original
   mission files and merges the selected fragments.
6. **Restart the server** — The merged mission files are only read at server
   startup.

## Detected Fragment Types

The scanner recognizes fragments by their root XML element or filename:

| Root element / filename | Merged into |
|-------------------------|-------------|
| `<types>` / `types.xml` | `mpmissions/.../db/types.xml` |
| `<spawnabletypes>` / `cfgspawnabletypes.xml` | `mpmissions/.../cfgspawnabletypes.xml` |
| `<events>` / `events.xml` | `mpmissions/.../db/events.xml` |
| `<eventposdef>` / `cfgeventspawns.xml` | `mpmissions/.../cfgeventspawns.xml` |
| `<economy>` / `economy.xml` | `mpmissions/.../db/economy.xml` |
| `<limits>` / `cfglimitsdefinition.xml` | `mpmissions/.../cfglimitsdefinition.xml` |
| `<limitss>` / `cfglimitsdefinitionuser.xml` | `mpmissions/.../cfglimitsdefinitionuser.xml` |
| `<randompresets>` / `cfgrandompresets.xml` | `mpmissions/.../cfgrandompresets.xml` |
| `<environment>` / `cfgenvironment.xml` | `mpmissions/.../cfgenvironment.xml` |

Fragments can live anywhere inside the mod folder (`Types/`, `XML/`,
`extras/Types/`, etc.). The scanner follows directory symlinks, so it works
with DayzConfigMaster's case-normalized mod wrappers as well as with real
workshop folders.

## Duplicate Prevention

When re-applying integration, DayzConfigMaster skips entries whose `name`
attribute already exists in the target file or inside an existing integration
block. This lets you safely add new mods or re-apply after a mod update without
creating duplicates.

## Backups and Restore

Every mission file is backed up before it is modified:

- `backups/mission/<relative_path>.original` — the first pristine version
- `backups/mission/<relative_path>.latest` — the version before the most recent
  merge

To remove all integrations and return to vanilla mission files, click
**⏪ Restore Backups** and confirm. You can then re-apply only the mods you want.

## Automatic Integration on Server Start

Once you apply integration for an instance, the active selections are saved to
`mod_integration.json` in the instance root. The next time you start that
instance through DayzConfigMaster, the same integrations are re-applied
automatically (after the mission folder is deployed).

## When to Wipe Server Storage

DayZ caches the current economy state in the mission `storage_1/` directory. If
you add vehicle or loot mods and do not see new content after restarting, you
may need to wipe persistence so the server regenerates the economy from your
updated XML files.

> ⚠️ **Warning:** This resets loot and vehicle persistence. Back up first if you
> want to preserve player-built bases or stashes.

Steps:

1. Stop the server.
2. Delete `mpmissions/dayzOffline.<world>/storage_1/data/`.
3. Start the server again.

## Troubleshooting

### "No @Mod folders found"

Start the server once so DayzConfigMaster deploys the mod wrappers into the
instance directory. The integration tab reads from those wrappers.

### Mod loads but content does not spawn

- Verify the mod has a checkmark in the **Integrate** column.
- Verify the target mission file exists for your selected map.
- Try wiping `storage_1/data/` after backing it up.
- Some mods keep their XML inside PBOs; those cannot be auto-merged and must be
  extracted manually.

### Some mod XML files do not have a root element

A few mods ship files like `cfgspawnabletypes.xml` as a raw list of `<type>`
blocks without a wrapping `<spawnabletypes>` tag. The integrator detects this
and inserts the entries under the target file's root element automatically.

### Spawn Loadout Queue

The **Spawn Loadout** tab lets you build a queue of spawnables before applying
them. Select a class, set the category-appropriate counts, and click **Add to
Loadout**. When the queue is ready, click **Apply Loadout**.

| Category | Controls |
|----------|----------|
| Vehicles / Air / Water | Map limit, Event min, Event max |
| Weapons / Gear / Other | Nominal, Min, Lifetime, Restock, QuantMin, QuantMax, Usage, Value, Tier |

You can save/load queues as JSON files or persist them per-instance with the
**Save Loadout** / **Load Saved Loadout** buttons.

### Why weapons/loot still do not spawn

DayZ Central Economy reads the files declared in `cfgeconomycore.xml`. If that
file has no `<ce>` section, DayZ defaults to `db/types.xml` and `db/events.xml`.
Some maps also ship a broken root `types.xml` (at the mission root) with entries
like `category="weapon"` and `usage="Town" value="Tier12"`. If that root file
exists, it can override or conflict with the proper `db/types.xml`.

DCM edits the file DayZ actually loads. If a broken root `types.xml` is
present, remove or rename it so the server uses the clean `db/types.xml`, then
wipe `storage_1/data/` and restart.

### Duplicate entries after manual edits

Click **⏪ Restore Backups** to return to vanilla files, then re-apply
integration.
