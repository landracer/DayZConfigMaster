# unpbo - Extract and List PBO Contents

Views and extracts files from DayZ PBO archives.

## Syntax

```bash
# List contents
unpbo --list <filename.pbo>

# Extract all files to current directory
unpbo <filename.pbo>

# Extract specific files
unpbo <filename.pbo> file1 file2 ...
```

## Examples

```bash
# List contents of a PBO
unpbo --list C:\path\to\mod.pbo

# Extract entire PBO
unpbo mod.pbo

# Extract specific files (using path as shown in list)
unpbo mod.pbo Prefix\scripts\3_Game\foo.c Prefix\config.cpp
```

## Options

| Flag | Description |
|------|-------------|
| `-l`, `--list` | List PBO contents without extracting |

## Notes

1. File paths shown in list output use the internal PBO format
2. Extraction preserves directory structure
3. Obfuscated script files may need additional deobfuscation processing