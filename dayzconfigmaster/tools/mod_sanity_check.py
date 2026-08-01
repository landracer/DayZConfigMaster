# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Sanity-check mod deployment for a DayZ server instance.

This tool verifies that every mod configured for an instance is correctly
wrapped, exposes lowercase ``addons``/``keys`` folders, and has its signature
keys copied into the instance ``keys/`` directory. It can be run against a
running server instance to diagnose "Client has a PBO which is not part of the
server" errors.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path
from typing import Dict, List, Optional, Set, Tuple


def _read_cmdline(pid: int) -> List[str]:
    """Return the command line for a process as a list of arguments."""
    cmdline_path = Path(f"/proc/{pid}/cmdline")
    if not cmdline_path.exists():
        return []
    raw = cmdline_path.read_bytes()
    # cmdline uses null terminators; filter empty trailing entries.
    return [arg.decode("utf-8", errors="replace") for arg in raw.split(b"\x00") if arg]


def _find_dayz_server_pid() -> Optional[int]:
    """Find the PID of a running DayZServer process."""
    for proc_dir in Path("/proc").iterdir():
        if not proc_dir.name.isdigit():
            continue
        cmdline = _read_cmdline(int(proc_dir.name))
        for arg in cmdline:
            if "DayZServer" in arg and "-port=" in " ".join(cmdline):
                return int(proc_dir.name)
    return None


def _parse_mod_arg(cmdline: List[str]) -> Optional[str]:
    """Extract the -mod= value from a server command line."""
    for arg in cmdline:
        if arg.startswith("-mod="):
            return arg.split("=", 1)[1]
    return None


def _parse_port(cmdline: List[str]) -> Optional[int]:
    """Extract the -port value from a server command line."""
    for arg in cmdline:
        if arg.startswith("-port="):
            try:
                return int(arg.split("=", 1)[1])
            except ValueError:
                return None
    return None


def _get_running_instance_info() -> Optional[Tuple[Path, str, int]]:
    """Return (instance_root, mod_string, port) for a running DayZ server."""
    pid = _find_dayz_server_pid()
    if pid is None:
        return None
    cmdline = _read_cmdline(pid)
    cwd = Path(f"/proc/{pid}/cwd").resolve()
    mod_string = _parse_mod_arg(cmdline) or ""
    port = _parse_port(cmdline) or 0
    return cwd, mod_string, port


def _list_pbos(addons_dir: Path) -> List[Path]:
    """Return all .pbo files in an addons directory."""
    if not addons_dir.exists():
        return []
    return sorted(p for p in addons_dir.iterdir() if p.is_file() and p.suffix.lower() == ".pbo")


def _list_keys(keys_dir: Path) -> Set[str]:
    """Return the set of .bikey file names in a keys directory."""
    if not keys_dir.exists():
        return set()
    return {p.name for p in keys_dir.iterdir() if p.is_file() and p.suffix.lower() == ".bikey"}


def _find_source_keys(mod_source: Path) -> Set[str]:
    """Find all .bikey files in a mod source folder (any key folder name)."""
    found: Set[str] = set()
    for keys_name in ("keys", "Keys", "Key", "key"):
        keys_dir = mod_source / keys_name
        if keys_dir.exists() and keys_dir.is_dir():
            found.update(_list_keys(keys_dir))
    return found


def check_instance(instance_root: Path, mod_string: str) -> Dict[str, List[str]]:
    """Check an instance directory for mod deployment issues.

    Returns a dict with ``ok``, ``warnings``, and ``errors`` lists.
    """
    ok: List[str] = []
    warnings: List[str] = []
    errors: List[str] = []

    instance_keys_dir = instance_root / "keys"
    instance_keys = _list_keys(instance_keys_dir)

    mod_names = [m.strip() for m in mod_string.replace(",", ";").split(";") if m.strip()]
    if not mod_names:
        warnings.append("No mods configured for this instance.")
        return {"ok": ok, "warnings": warnings, "errors": errors}

    total_pbos = 0
    for mod_name in mod_names:
        wrapper = instance_root / mod_name
        if not wrapper.exists():
            errors.append(f"{mod_name}: wrapper directory does not exist")
            continue
        if not wrapper.is_dir():
            errors.append(f"{mod_name}: exists but is not a directory")
            continue

        addons = wrapper / "addons"
        if not addons.exists():
            errors.append(f"{mod_name}: missing lowercase 'addons' folder")
        elif not addons.is_symlink() and not addons.is_dir():
            errors.append(f"{mod_name}: 'addons' is not a folder or symlink")
        else:
            pbos = _list_pbos(addons)
            total_pbos += len(pbos)
            if not pbos:
                warnings.append(f"{mod_name}: 'addons' folder is empty (no .pbo files)")
            else:
                ok.append(f"{mod_name}: {len(pbos)} PBO(s) visible")

        keys = wrapper / "keys"
        if not keys.exists():
            warnings.append(f"{mod_name}: missing lowercase 'keys' folder")

        # Determine the mod source by reading the addons symlink target.
        mod_source: Optional[Path] = None
        if addons.is_symlink():
            try:
                target = addons.readlink()
                if not target.is_absolute():
                    target = (addons.parent / target).resolve()
                else:
                    target = target.resolve()
                mod_source = target.parent
            except (OSError, RuntimeError):
                pass

        if mod_source is None:
            warnings.append(f"{mod_name}: could not determine source mod folder")
            continue

        source_keys = _find_source_keys(mod_source)
        missing_keys = source_keys - instance_keys
        if missing_keys:
            errors.append(
                f"{mod_name}: keys missing from instance keys/: {', '.join(sorted(missing_keys))}"
            )
        elif source_keys:
            ok.append(f"{mod_name}: all {len(source_keys)} key(s) copied")

    ok.append(f"Instance keys/: {len(instance_keys)} key file(s) total")
    ok.append(f"Total PBOs visible to server: {total_pbos}")
    return {"ok": ok, "warnings": warnings, "errors": errors}


def main(argv: List[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="Sanity-check DayZ server mod deployment.",
    )
    parser.add_argument(
        "--instance",
        type=Path,
        help="Path to the instance directory (e.g. ~/Documents/DayZProjects/instances/server1).",
    )
    parser.add_argument(
        "--mods",
        help="Semicolon-separated mod list (e.g. '@CF;@2216645010').",
    )
    parser.add_argument(
        "--running",
        action="store_true",
        help="Inspect the currently running DayZ server process.",
    )
    args = parser.parse_args(argv)

    if args.running:
        info = _get_running_instance_info()
        if info is None:
            print("No running DayZServer process found.", file=sys.stderr)
            return 1
        instance_root, mod_string, port = info
        print(f"Found running server: pid cwd={instance_root}, port={port}")
    else:
        if not args.instance:
            parser.error("--instance is required unless --running is used")
        instance_root = args.instance.expanduser().resolve()
        mod_string = args.mods or ""

    result = check_instance(instance_root, mod_string)

    for msg in result["ok"]:
        print(f"  OK: {msg}")
    for msg in result["warnings"]:
        print(f"  WARN: {msg}")
    for msg in result["errors"]:
        print(f"  ERROR: {msg}")

    if result["errors"]:
        print("\nSanity check FAILED. Fix the errors above and restart the server.")
        return 1
    if result["warnings"]:
        print("\nSanity check passed with warnings.")
        return 0
    print("\nSanity check passed.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
