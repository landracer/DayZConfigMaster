# DayZ Mod Conflict Debug Process

When a heavily modded DayZ server stops spawning world loot (guns, gear, etc.)
but AI/player loadouts still work, the cause is almost always a mod breaking the
Central Economy (CE) class hierarchy.  This guide documents the exact process
used to isolate the culprit.

## Symptom Checklist

- AI and scripted loadouts have weapons.
- World loot (buildings, trains, vehicles) has no guns / wrong items.
- `types.xml` parses cleanly and contains thousands of valid entries.
- `[CE][TypeSetup]` count looks plausible but many `[CE][offlineDB]` "will be
  ignored" warnings appear for **vanilla** items.

## Root Causes (in order of likelihood)

1. **A mod overrides base weapon classes** (`Pistol_Base`, `Rifle_Base`,
   `Weapon_Base`, `DefaultWeapon`, etc.).  Vehicle/clothing mods are frequent
   offenders.
2. **Stale `storage_1/data/*.bin` cache** — CE restores old binary state and
   ignores new `types.xml` entries.
3. **Missing `<ce>` references** in `cfgeconomycore.xml` so the server never
   reads the mod's custom XML.
4. **XML syntax errors** in merged mod fragments (usually caught by validation).

## Debug Steps

### 1. Confirm the active mission folder

Check `serverDZ_*.cfg`:

```text
class Missions
{
    class DayZ
    {
        template="dayzOffline.enoch";
    };
};
```

Make sure the matching folder under `mpmissions/` is the one being edited.
Multiple `dayzOffline.*` folders are a common source of silent wrong-target
edits.

### 2. Verify `types.xml` is being read and is valid

```bash
python3 - <<'PY'
import xml.etree.ElementTree as ET
p = "mpmissions/dayzOffline.enoch/db/types.xml"
root = ET.parse(p).getroot()
print("types:", len(root.findall("type")))
print("duplicates:", len([n for n in set(root) if list(root).count(n) > 1]))
PY
```

Expected: thousands of types, no parse errors, no duplicate `name` attributes.

### 3. Inspect the server RPT log

Find the newest RPT:

```bash
find profiles -name 'DayZServer_instance_1_*.RPT' -printf '%T@ %p\n' | sort -n | tail -n 1
```

Run the key searches:

```bash
RPT="profiles/DayZServer_instance_1_YYYY-MM-DD_HH-MM-SS.RPT"

# CE startup summary
grep -E "TypeSetup|RegisterConfig|Hive\] :: Init" "$RPT"

# Stale cache restore
grep "Restoring file" "$RPT" | head

# Weapon/class overrides (THE SMOKING GUN)
grep "Updating base class" "$RPT" | grep -iE "Pistol_Base|Rifle_Base|Weapon_Base|DefaultWeapon|DefaultMagazine"

# Ignored types
grep -c "will be ignored" "$RPT"
grep "will be ignored" "$RPT" | grep -iE "ak|m4|mosin|sks|mp5|izh|ump|mkii|rifle|pistol|shotgun" | head -n 20
```

### 4. Interpret the results

| Pattern | Meaning | Action |
|---|---|---|
| `Updating base class PistolCore->Weapon_Base, by SomeMod\config.cpp/CfgWeapons/Pistol_Base/` | A mod is rewriting the weapon base class. | Remove or patch the mod. |
| `Type 'AK101' will be ignored. (Type does not exist.)` | CE cannot find the class in memory, usually because a base class was broken. | Fix the mod overriding the base class. |
| `Restoring file .../storage_1/data/*.bin` | CE is loading cached state. | Stop server, delete/backup `storage_1/data/*`, restart. |
| `TypeSetup :: 542 classes` (full mods) vs `586` (single mod) | Large variance suggests a mod is suppressing classes. | Compare counts while removing mods. |

### 5. Binary-search mods if the culprit is not obvious

If multiple mods look suspicious:

1. Stop the server.
2. Delete `storage_1/data/*`.
3. Remove half the mods.
4. Start, check RPT.
5. Repeat until the bad mod is isolated.

Because CE cache must be cleared between runs, **never test mod changes without
clearing `storage_1/data/` first**, or you will chase stale cache behavior
instead of the real issue.

## Automation

Use the companion script `tools/mod_debug_analyzer.py` to run the searches
above automatically:

```bash
python3 tools/mod_debug_analyzer.py \
  /home/sysadmin/Documents/DayZProjects/instances/server1/profiles/DayZServer_instance_1_*.RPT
```

It reports:

- CE setup counts
- Number of ignored types
- Weapon types being ignored
- Mods overriding weapon base classes
- Whether stale cache was restored

## Prevention in DayzConfigMaster

- `clear_ce_storage_on_start` should be enabled whenever mod integration is
  active.
- After a successful startup, store economy XML hashes so the cache is not
  unnecessarily cleared on the next run.
- Never guess the active mission folder; always resolve it from
  `serverDZ_*.cfg` or an explicit target name.

## Lessons Learned

- The "542 classes loaded" startup message is **script classes**, not CE setup
  classes.  Look for `[CE][TypeSetup]` instead.
- A single vehicle mod can break the entire weapon economy by touching
  `CfgWeapons` base classes.
- The RPT log is the single source of truth; when in doubt, read it.
