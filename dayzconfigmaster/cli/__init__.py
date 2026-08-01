# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# CLI package for DayZConfigMaster

"""
Command Line Interface package.

Provides the `dzl` command with 25+ subcommands:
- server: start, stop, restart, status, logs (5)
- build: prebuild, build, sign, cache (4)
- config: get, set, list, validate (4)
- workshop: download, update, search, auth (4)
- instance: create, delete, enable, disable, list (5)
- preset: save, load, list, delete (4)
- workdrive: mount, umount, status (3)
- utils: hash, validate, clean, info (4)

Total: 26+ subcommands
"""

from .handler import CLIHandler, main

__all__ = ['CLIHandler', 'main']