# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Headless repair of common cfgspawnabletypes.xml problems.

Fixes invalid XML comments that contain double dashes and ensures common
vehicles have wheel attachments. The GUI layer handles user-facing messages.
"""

import re
from dataclasses import dataclass, field
from datetime import datetime
from pathlib import Path
from typing import List, Optional


VEHICLE_WHEELS = {
    "OffroadHatchback": ("HatchbackWheel", 4),
    "OffroadHatchback_Blue": ("HatchbackWheel", 4),
    "OffroadHatchback_White": ("HatchbackWheel", 4),
    "Hatchback_02": ("Hatchback_02_Wheel", 4),
    "Hatchback_02_Blue": ("Hatchback_02_Wheel", 4),
    "Hatchback_02_Black": ("Hatchback_02_Wheel", 4),
    "Sedan_02": ("Sedan_02_Wheel", 4),
    "Sedan_02_Red": ("Sedan_02_Wheel", 4),
    "CivilianVan": ("Van_01_Wheel", 4),
    "CivilianVan_Black": ("Van_01_Wheel", 4),
    "CivilianVan_Wine": ("Van_01_Wheel", 4),
    "Truck_01_Covered": ("Truck_01_Wheel", 6),
    "Truck_01_Covered_Blue": ("Truck_01_Wheel", 6),
    "Truck_01_Covered_Orange": ("Truck_01_Wheel", 6),
    "Truck_01_Transport": ("Truck_01_Wheel", 6),
    "Offroad_02": ("Offroad_02_Wheel", 4),
    "Offroad_02_Blue": ("Offroad_02_Wheel", 4),
    "Offroad_02_Green": ("Offroad_02_Wheel", 4),
    "Offroad_02_Red": ("Offroad_02_Wheel", 4),
    "Offroad_02_White": ("Offroad_02_Wheel", 4),
}

TYPE_PATTERN = re.compile(r'<type name="([^"]+)">(.*?)</type>', re.DOTALL)


@dataclass
class RepairResult:
    """Outcome of a cfgspawnabletypes.xml repair pass."""
    success: bool
    target_path: Path
    fixes: List[str] = field(default_factory=list)
    backup_path: Optional[Path] = None
    changed: bool = False
    error: str = ""


def _sanitize_comment(match: "re.Match[str]") -> str:
    body = match.group(1)
    if "--" in body:
        clean = re.sub(r"-{2,}", "-", body)
        return "<!-- " + clean + " -->"
    return match.group(0)


def _ensure_wheels(match: "re.Match[str]") -> str:
    name = match.group(1)
    block = match.group(2)
    if name not in VEHICLE_WHEELS:
        return match.group(0)
    wheel, count = VEHICLE_WHEELS[name]
    if wheel in block:
        return match.group(0)
    attachments = "\n".join(
        '\t\t<attachments chance="1.00">\n'
        f'\t\t\t<item name="{wheel}" chance="1.00" />\n'
        '\t\t</attachments>'
        for _ in range(count)
    )
    new_block = block.rstrip() + "\n" + attachments + "\n\t"
    return f'<type name="{name}">\n{new_block}</type>'


def repair_cfg_spawnable_types(
    target_path: Path,
    backup_root: Optional[Path] = None,
) -> RepairResult:
    """Repair cfgspawnabletypes.xml at *target_path*.

    Args:
        target_path: Path to the file to repair.
        backup_root: Directory in which to write the backup. Defaults to
            ``target_path.parent / "backups" / "mission"``.

    Returns:
        A :class:`RepairResult` describing what changed and where the backup
        was saved.
    """
    if not target_path.exists():
        return RepairResult(
            success=False,
            target_path=target_path,
            error=f"cfgspawnabletypes.xml not found at {target_path}",
        )

    try:
        text = target_path.read_text(encoding="utf-8")
    except Exception as exc:
        return RepairResult(
            success=False,
            target_path=target_path,
            error=f"Could not read {target_path}: {exc}",
        )

    original_text = text
    fixes: List[str] = []

    new_text = re.sub(
        r"<!--(.*?)-->(?=\s*<|\s*$)",
        _sanitize_comment,
        text,
        flags=re.DOTALL,
    )
    if new_text != text:
        fixes.append("Removed invalid XML comments containing '--'")
        text = new_text

    new_text = TYPE_PATTERN.sub(_ensure_wheels, text)
    if new_text != text:
        added = []
        for name, (wheel, _count) in VEHICLE_WHEELS.items():
            if name not in original_text or wheel not in original_text:
                if name in new_text and wheel in new_text:
                    added.append(name)
        if added:
            fixes.append(f"Added wheel attachments for {len(added)} vehicle(s)")
        text = new_text

    if text == original_text:
        return RepairResult(
            success=True,
            target_path=target_path,
            changed=False,
            fixes=[],
        )

    if backup_root is None:
        backup_root = target_path.parent / "backups" / "mission"
    backup_root.mkdir(parents=True, exist_ok=True)
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    backup_path = backup_root / f"cfgspawnabletypes.xml.repair.{timestamp}"

    try:
        backup_path.write_text(original_text, encoding="utf-8")
        target_path.write_text(text, encoding="utf-8")
    except Exception as exc:
        return RepairResult(
            success=False,
            target_path=target_path,
            error=f"Could not write repaired file: {exc}",
        )

    return RepairResult(
        success=True,
        target_path=target_path,
        changed=True,
        fixes=fixes,
        backup_path=backup_path,
    )
