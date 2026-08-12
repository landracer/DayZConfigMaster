#!/usr/bin/env python3
"""Analyze a DayZ server RPT log for common mod/economy conflicts.

Usage:
    python3 tools/mod_debug_analyzer.py <path-to.RPT>

Reports the key signals that indicate why world loot is not spawning:
- CE setup counts
- Stale cache restore
- Weapon base-class overrides (the usual culprit)
- Ignored types, especially weapons
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path
from typing import Dict, List, Tuple


def read_rpt(path: Path) -> str:
    return path.read_text(encoding="utf-8", errors="ignore")


def find_ce_setup(rpt: str) -> Dict[str, str]:
    """Return CE setup summary lines."""
    results: Dict[str, str] = {}
    for line in rpt.splitlines():
        m = re.search(r"\[CE\]\[TypeSetup\] :: (\d+) classes setuped", line)
        if m:
            results["type_setup"] = m.group(1)
        m = re.search(r"\[CE\]\[RegisterConfig\] :: (\d+) config classes registered", line)
        if m:
            results["register_config"] = m.group(1)
        if "[CE][Hive] :: Init sequence finished" in line:
            results["init_finished"] = "yes"
    return results


def find_base_class_overrides(rpt: str) -> List[str]:
    """Return weapon base-class override warnings from mods (skip vanilla DayZ)."""
    overrides: List[str] = []
    pattern = re.compile(
        r"Updating base class .*?(Pistol_Base|Rifle_Base|Weapon_Base|DefaultWeapon|DefaultMagazine)"
    )
    for line in rpt.splitlines():
        if pattern.search(line) and "DZ\\" not in line:
            overrides.append(line.strip())
    return overrides


def find_ignored_types(rpt: str) -> Tuple[int, List[str]]:
    """Return total ignored types and weapon-related ones."""
    weapon_keywords = re.compile(
        r"\b(AK|AKM|AK101|AK74|M4A1|Mosin|SKS|MP5|SG5K|Izh|Izh18|Izh43|Cr527|Cz527|M70|M91|Vaiga|UMP|MKII|Rifle|Pistol|Shotgun|Firearm)\b",
        re.IGNORECASE,
    )
    ignored: List[str] = []
    weapon_ignored: List[str] = []
    pattern = re.compile(r"\[CE\]\[offlineDB\] :: Type '([^']+)' will be ignored")
    for line in rpt.splitlines():
        m = pattern.search(line)
        if m:
            ignored.append(m.group(1))
            if weapon_keywords.search(m.group(1)):
                weapon_ignored.append(m.group(1))
    return len(ignored), weapon_ignored


def find_cache_restores(rpt: str) -> List[str]:
    """Return stale cache restore messages."""
    restores: List[str] = []
    for line in rpt.splitlines():
        if "Restoring file" in line and "storage_1/data" in line:
            restores.append(line.strip())
    return restores


def analyze(rpt_path: Path) -> None:
    rpt = read_rpt(rpt_path)

    print(f"Analysis of {rpt_path.name}")
    print("=" * 60)

    ce = find_ce_setup(rpt)
    print("CE Startup:")
    print(f"  TypeSetup classes:     {ce.get('type_setup', 'NOT FOUND')}")
    print(f"  RegisterConfig classes: {ce.get('register_config', 'NOT FOUND')}")
    print(f"  Init finished:          {ce.get('init_finished', 'NO')}")
    print()

    restores = find_cache_restores(rpt)
    print(f"Stale cache restores: {len(restores)}")
    if restores:
        for r in restores[:5]:
            print(f"  {r}")
        if len(restores) > 5:
            print(f"  ... and {len(restores) - 5} more")
    print()

    overrides = find_base_class_overrides(rpt)
    print(f"Weapon base-class overrides: {len(overrides)}")
    for o in overrides:
        print(f"  {o}")
    print()

    total_ignored, weapon_ignored = find_ignored_types(rpt)
    print(f"Ignored types (total): {total_ignored}")
    if weapon_ignored:
        print(f"Ignored WEAPON types: {len(weapon_ignored)}")
        for name in sorted(set(weapon_ignored))[:20]:
            print(f"  - {name}")
        if len(set(weapon_ignored)) > 20:
            print(f"  ... and {len(set(weapon_ignored)) - 20} more")
    else:
        print("No weapon types were ignored.")
    print()

    if overrides or weapon_ignored:
        print("VERDICT: A mod is likely breaking the weapon class hierarchy.")
        print("Look at the 'Updating base class' lines above and remove that mod.")
    elif restores:
        print("VERDICT: Stale CE cache was restored. Clear storage_1/data/* and restart.")
    elif total_ignored:
        print("VERDICT: Some types are ignored but weapons look OK. Check vehicle/mod parts.")
    else:
        print("VERDICT: No obvious economy conflict found in this RPT.")


def main(argv: List[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Analyze a DayZ RPT log for mod conflicts.")
    parser.add_argument("rpt", type=Path, help="Path to the .RPT file")
    args = parser.parse_args(argv)

    if not args.rpt.exists():
        print(f"Error: file not found: {args.rpt}", file=sys.stderr)
        return 1

    analyze(args.rpt)
    return 0


if __name__ == "__main__":
    sys.exit(main())
