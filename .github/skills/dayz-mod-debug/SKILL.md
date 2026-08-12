---
name: dayz-mod-debug
description: "Use when diagnosing DayZ server mod conflicts, missing world loot, broken weapon spawns, Central Economy (CE) issues, stale storage_1/data cache, or interpreting a DayZ RPT log. Automates detection of base-class overrides, ignored weapon types, and stale CE cache restores."
---

# DayZ Mod Debug Skill

## When to Use

Invoke this skill whenever the user reports:

- No weapons, gear, or vehicles spawning in the world
- AI/loadouts have items but world loot does not
- Suspected mod conflict after adding/updating mods
- Need to interpret a DayZ `.RPT` log
- Stale `storage_1/data/*.bin` cache concerns
- Wrong mission folder being edited by mod integration

## Diagnostic Process

1. **Identify the active mission folder.**
   - Read `serverDZ_*.cfg` for `template="..."`.
   - Verify `mpmissions/<template>/db/types.xml` exists and is the file being
     modified.
   - If multiple `dayzOffline.*` folders exist, do not guess — use the config
     template or an explicit target name.

2. **Validate the merged economy XML.**
   - Confirm `types.xml` parses and has thousands of entries.
   - Check for duplicate `type/@name` values.
   - Ensure `cfgeconomycore.xml` references the correct `db/types.xml` and
     `db/events.xml` if the mission uses `<ce>` sections.

3. **Inspect the newest RPT log.**
   - Find the newest `DayZServer_instance_*.RPT` in the instance `profiles/`
     directory.
   - Run `tools/mod_debug_analyzer.py` on it.
   - If the analyzer is unavailable, manually grep for:
     - `[CE][TypeSetup]` and `[CE][RegisterConfig]` counts
     - `Restoring file` (stale cache)
     - `Updating base class` + `Pistol_Base|Rifle_Base|Weapon_Base`
     - `will be ignored` + weapon names

4. **Interpret and act.**
   - **Base-class override from a mod:** remove or patch that mod. Vehicle mods
     are frequent offenders.
   - **Stale cache restore:** stop server, backup/delete `storage_1/data/*`,
     restart.
   - **XML syntax errors:** repair the offending mod fragment using the XML
     repair helpers.
   - **Many ignored vehicle parts:** the mod's `types.xml` entries are missing;
     verify mod XML integration ran and the fragment was not rejected.

## Key Commands

```bash
# Run the analyzer (preferred)
python3 tools/mod_debug_analyzer.py <path-to.RPT>

# Manual fallback searches
grep -E "TypeSetup|RegisterConfig|Hive\] :: Init" <RPT>
grep "Restoring file" <RPT>
grep "Updating base class" <RPT> | grep -iE "Pistol_Base|Rifle_Base|Weapon_Base"
grep -c "will be ignored" <RPT>
grep "will be ignored" <RPT> | grep -iE "ak|m4|mosin|sks|mp5|izh|ump|mkii"
```

## Key Files

- [wiki/06-configuration/mod-debug-diagnosis.md](../../../wiki/06-configuration/mod-debug-diagnosis.md)
- [tools/mod_debug_analyzer.py](../../../tools/mod_debug_analyzer.py)
- [dayzconfigmaster/mods/integration.py](../../../dayzconfigmaster/mods/integration.py)
- [dayzconfigmaster/economy/ce_storage.py](../../../dayzconfigmaster/economy/ce_storage.py)

## Common False Trails to Avoid

- The startup message "542 classes loaded" refers to script classes, not CE
  setup classes. Look for `[CE][TypeSetup]` instead.
- A valid `types.xml` does not guarantee loot spawns; the CE cache or a broken
  base class can still suppress everything.
- Never test mod changes without clearing `storage_1/data/*` between runs.

## Example Verdicts

| Analyzer Output | Meaning |
|---|---|
| Weapon base-class overrides detected | Remove the mod shown in the override line. |
| Weapon types ignored | Usually caused by a base-class override. |
| Stale cache restores detected | Clear `storage_1/data/` and restart. |
| Some types ignored, weapons OK | Vehicle mod parts missing from `types.xml`; usually harmless to loot. |
