# Mod Debug & Diagnosis

When a heavily modded DayZ server stops spawning world loot (guns, gear,
vehicles, etc.) but AI or scripted loadouts still work, the problem is usually
a mod conflict in the Central Economy (CE) class hierarchy. This page documents
the diagnostic process and the tools in this repository that automate it.

## Quick Check: Run the Analyzer

The fastest way to diagnose a mod conflict is to run the RPT analyzer:

```bash
python3 tools/mod_debug_analyzer.py \
  /home/sysadmin/Documents/DayZProjects/instances/server1/profiles/DayZServer_instance_1_*.RPT
```

It reports:

- CE startup counts (`[CE][TypeSetup]`, `[CE][RegisterConfig]`)
- Whether stale `storage_1/data/*.bin` cache was restored
- Any mod overriding weapon base classes (`Pistol_Base`, `Rifle_Base`, etc.)
- Total ignored types and whether weapons are among them
- A one-line verdict

## Symptom Checklist

- AI and scripted loadouts have weapons.
- World loot (buildings, trains, vehicles) has no guns or wrong items.
- `types.xml` parses cleanly and contains thousands of valid entries.
- `[CE][TypeSetup]` count looks plausible but many `[CE][offlineDB]` "will be
  ignored" warnings appear for **vanilla** items.

## Common Root Causes

1. **Mod overrides base weapon classes.** Vehicle/clothing mods sometimes touch
   `CfgWeapons/Pistol_Base`, `CfgWeapons/Rifle_Base`, or `CfgWeapons/Weapon_Base`.
   This breaks inheritance for every vanilla weapon.
2. **Stale `storage_1/data/*.bin` cache.** CE restores old binary state and
   ignores new `types.xml` entries.
3. **Missing `<ce>` references** in `cfgeconomycore.xml` so the server never
   reads a mod's custom XML.
4. **XML syntax errors** in merged mod fragments.

## Manual Debug Steps

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
edits. DayzConfigMaster now resolves the active folder strictly from the config
file or an explicit target name.

### 2. Verify `types.xml` is valid

```bash
python3 - <<'PY'
import xml.etree.ElementTree as ET
p = "mpmissions/dayzOffline.enoch/db/types.xml"
root = ET.parse(p).getroot()
print("types:", len(root.findall("type")))
PY
```

Expected: thousands of types, no parse errors.

### 3. Search the RPT log

```bash
RPT="profiles/DayZServer_instance_1_YYYY-MM-DD_HH-MM-SS.RPT"

# CE startup summary
grep -E "TypeSetup|RegisterConfig|Hive\] :: Init" "$RPT"

# Stale cache restore
grep "Restoring file" "$RPT" | head

# Weapon base-class overrides
grep "Updating base class" "$RPT" | grep -iE "Pistol_Base|Rifle_Base|Weapon_Base"

# Ignored types
grep -c "will be ignored" "$RPT"
grep "will be ignored" "$RPT" | grep -iE "ak|m4|mosin|sks|mp5|izh|ump|mkii" | head
```

### 4. Interpret the results

| Pattern | Meaning | Action |
|---|---|---|
| `Updating base class PistolCore->Weapon_Base, by SomeMod\config.cpp/CfgWeapons/Pistol_Base/` | A mod is rewriting the weapon base class. | Remove or patch the mod. |
| `Type 'AK101' will be ignored. (Type does not exist.)` | CE cannot find the class in memory, usually because a base class was broken. | Fix the mod overriding the base class. |
| `Restoring file .../storage_1/data/*.bin` | CE is loading cached state. | Stop server, delete/backup `storage_1/data/*`, restart. |
| Big variance in `TypeSetup` count between mod sets | A mod is suppressing classes. | Binary-search mods, clearing cache each time. |

## Binary-Searching Mods

If the bad mod is not obvious:

1. Stop the server.
2. Delete `storage_1/data/*`.
3. Remove half the mods.
4. Start, check RPT.
5. Repeat until isolated.

**Always clear `storage_1/data/` between tests.** CE cache will hide or mimic
real mod behavior if you do not.

## What to Look For in the RPT

- The "542 classes loaded" startup message is **script classes**, not CE setup
  classes. Look for `[CE][TypeSetup]` instead.
- A single vehicle mod can break the entire weapon economy by touching
  `CfgWeapons` base classes.
- The RPT log is the single source of truth.

## Prevention in DayzConfigMaster

- Enable `clear_ce_storage_on_start` whenever mod integration is active.
- Store economy XML hashes after a successful startup so the cache is not
  unnecessarily cleared on the next run.
- Use explicit mission folder resolution; never guess between multiple
  `dayzOffline.*` folders.

## Case Study: Ironhorde Humvee

A real example from this project:

```text
Updating base class PistolCore->Weapon_Base, by IronhordeHumvee\config.cpp/CfgWeapons/Pistol_Base/
Updating base class RifleCore->Weapon_Base, by IronhordeHumvee\config.cpp/CfgWeapons/Rifle_Base/
```

This vehicle mod overrode base weapon classes. CE then ignored all vanilla
weapons (`AK101`, `AK74`, `AKM`, `M4A1`, etc.) with "Type does not exist."
Removing the mod restored world weapon spawns.

## Related Files

- [tools/mod_debug_analyzer.py](../../tools/mod_debug_analyzer.py)
- [mod-corruption.md](mod-corruption.md)
- [server-config-mods.md](server-config-mods.md)
