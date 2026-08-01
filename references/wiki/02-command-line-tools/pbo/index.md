# pbo - Create PBO Archives

Creates PBO (Press Board Oblong) archives for DayZ mods.

## Syntax

```bash
pbo <output.pbo> <file1> [file2] [file3...] [options]
```

## Basic Usage

```bash
# Create a basic PBO
pbo mymod.pbo config.cpp scripts\3_game\something.c

# With help flag to see all options
pbo --help
```

## Description

The `pbo` command is a low-level tool for creating PBO files. It creates the archive from specified files but requires additional options for:

- Adding PBO headers (author, version, etc.)
- Signing the resulting PBO for use with BattlEye

## Options

See `pbo --help` for complete option list.

## Notes

1. The path separator in PBO file names uses forward slashes `/`
2. Headers must be added separately using other tools
3. Signatures require private keys and the signing process