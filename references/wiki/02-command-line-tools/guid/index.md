# guid - Steam ID to DayZ GUID Converter

Converts 64-bit Steam IDs to DayZ GUID format.

## Syntax

```bash
guid <steamid64>
```

## Examples

```bash
# Convert a Steam ID to GUID
guid 76561197970002375
```

## Purpose

DayZ uses a different GUID format than Steam's 64-bit IDs. The `guid` tool converts between these formats, which is useful for:

- Player tracking and logging
- Admin list management
- Ban system integration
- Custom mod development

## Output Format

The output is the DayZ GUID string corresponding to the input Steam ID.