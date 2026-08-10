# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Repair corrupted ``<nominal>`` and ``<min>`` values in ``db/types.xml``.

Mod integrations and manual edits sometimes zero out loot counts for items
that should spawn naturally. This module restores ``<nominal>`` and ``<min>``
from one or more reference ``types.xml`` files while leaving intentionally
zero-nominal entries (vehicles, parts, etc.) untouched.
"""

import re
import shutil
from dataclasses import dataclass, field
from datetime import datetime
from pathlib import Path
from typing import Dict, List, Optional, Set, Tuple

from .types_xml import TypesXml, TypeEntry


# Categories that normally spawn as loot and should not have nominal=0.
DEFAULT_LOOT_CATEGORIES: Set[str] = {
    "weapons",
    "clothes",
    "tools",
    "containers",
    "food",
}

# Categories that are frequently intentional nominal=0 (spawned by events
# or attached to other items).
DEFAULT_SKIP_CATEGORIES: Set[str] = {
    "vehicle",
    "vehiclesparts",
    "explosives",
    "lootdispatch",
}

# Stock event-only vehicles and static map-placed objects that must never
# receive a dynamic spawn nominal.  Matches are case-insensitive.
_EVENT_ONLY_PATTERNS = (
    "sedan", "van", "offroad", "hatchback", "truck", "boat", "heli",
    "plane", "wreck", "aban", "staticobj_", "land_boat_", "land_wreck_",
)
_EVENT_ONLY_EXACT = {
    "civilianvan", "civilianvan_black", "civilianvan_wine",
    "civiliansedan", "civiliansedan_black", "civiliansedan_wine",
    "wreck_mi8_crashed", "wreck_uh1y",
}


def _is_event_only_or_static(name: str) -> bool:
    """Return True for stock vehicles/boats/wrecks that are event-only or static."""
    lower = name.lower()
    if lower in _EVENT_ONLY_EXACT:
        return True
    return any(pattern in lower for pattern in _EVENT_ONLY_PATTERNS)


@dataclass
class TypesRepairResult:
    """Result of repairing a ``types.xml`` file."""

    success: bool
    target_path: Path
    repaired: List[Tuple[str, str, int, int]] = field(default_factory=list)
    skipped: List[str] = field(default_factory=list)
    no_reference: List[str] = field(default_factory=list)
    backup_path: Optional[Path] = None
    error: str = ""

    @property
    def repaired_count(self) -> int:
        return len(self.repaired)

    @property
    def skipped_count(self) -> int:
        return len(self.skipped)

    @property
    def no_reference_count(self) -> int:
        return len(self.no_reference)


def _effective_categories(entry: TypeEntry) -> Set[str]:
    """Return the lowercase category names for *entry*."""
    return {cat.name.lower() for cat in entry.categories}


def _find_reference(
    name: str,
    references: List[Tuple[str, TypesXml]],
    fallback: Optional[TypesXml] = None,
) -> Optional[Tuple[str, TypeEntry]]:
    """Return the first reference entry with ``nominal > 0`` for *name*."""
    for ref_name, ref_xml in references:
        ref_entry = ref_xml.get_type(name)
        if ref_entry is not None and ref_entry.nominal > 0:
            return ref_name, ref_entry
    if fallback is not None:
        fallback_entry = fallback.get_type(name)
        if fallback_entry is not None and fallback_entry.nominal > 0:
            return "fallback", fallback_entry
    return None


def repair_nominal_values(
    target_path: Path,
    reference_paths: List[Path],
    fallback_path: Optional[Path] = None,
    output_path: Optional[Path] = None,
    backup: bool = True,
    loot_categories: Optional[Set[str]] = None,
    skip_categories: Optional[Set[str]] = None,
) -> TypesRepairResult:
    """Restore ``<nominal>`` and ``<min>`` in *target_path* from references.

    Only entries with ``nominal == 0`` are considered. Entries whose only
    categories are in *skip_categories* are left alone unless they also match
    a loot category, because some items are tagged with both ``vehicle`` and
    ``weapons``. The first reference with a non-zero nominal value wins.

    Args:
        target_path: The ``types.xml`` file to repair.
        reference_paths: Ordered list of reference ``types.xml`` files.
        fallback_path: Optional additional reference used after the ordered
            list is exhausted (e.g. the mission root ``types.xml``).
        output_path: Where to write the repaired file. Defaults to
            *target_path* (in-place repair).
        backup: If True and writing in-place, copy the original to a
            timestamped ``.pre-repair-YYYYMMDD_HHMMSS`` file.
        loot_categories: Categories that should be repaired. Defaults to
            weapons, clothes, tools, containers, food.
        skip_categories: Categories usually intentionally zero. Defaults to
            vehicle, vehiclesparts, explosives, lootdispatch.

    Returns:
        A :class:`TypesRepairResult` describing the changes.
    """
    if not target_path.exists():
        return TypesRepairResult(
            success=False,
            target_path=target_path,
            error=f"Target file not found: {target_path}",
        )

    for ref_path in reference_paths:
        if not ref_path.exists():
            return TypesRepairResult(
                success=False,
                target_path=target_path,
                error=f"Reference file not found: {ref_path}",
            )

    target_xml = TypesXml.from_file(str(target_path))
    if target_xml is None:
        return TypesRepairResult(
            success=False,
            target_path=target_path,
            error=f"Could not parse {target_path}",
        )

    references: List[Tuple[str, TypesXml]] = []
    for ref_path in reference_paths:
        ref_xml = TypesXml.from_file(str(ref_path))
        if ref_xml is None:
            return TypesRepairResult(
                success=False,
                target_path=target_path,
                error=f"Could not parse reference {ref_path}",
            )
        references.append((ref_path.name, ref_xml))

    fallback_xml: Optional[TypesXml] = None
    if fallback_path is not None:
        if not fallback_path.exists():
            return TypesRepairResult(
                success=False,
                target_path=target_path,
                error=f"Fallback file not found: {fallback_path}",
            )
        fallback_xml = TypesXml.from_file(str(fallback_path))
        if fallback_xml is None:
            return TypesRepairResult(
                success=False,
                target_path=target_path,
                error=f"Could not parse fallback {fallback_path}",
            )

    loot_cats = loot_categories if loot_categories is not None else DEFAULT_LOOT_CATEGORIES
    skip_cats = skip_categories if skip_categories is not None else DEFAULT_SKIP_CATEGORIES

    repaired: List[Tuple[str, str, int, int]] = []
    skipped: List[str] = []
    no_reference: List[str] = []

    for name, entry in target_xml.get_all_types().items():
        if entry.nominal != 0:
            continue

        # Never restore nominal for event-only vehicles or static wrecks.
        # Broken spawn generators zero these out intentionally, and references
        # may still carry the old corrupted nominal=1 values.
        if _is_event_only_or_static(name):
            skipped.append(name)
            continue

        cats = _effective_categories(entry)
        if cats & skip_cats and not (cats & loot_cats):
            skipped.append(name)
            continue

        ref = _find_reference(name, references, fallback_xml)
        if ref is None:
            no_reference.append(name)
            continue

        # Guard against reference files that still contain corrupted nominal
        # values for event-only/static classes.
        ref_source, ref_entry = ref
        if _is_event_only_or_static(name) or ref_entry.nominal == 0:
            skipped.append(name)
            continue

        entry.nominal = ref_entry.nominal
        entry.min = ref_entry.min
        target_xml.set_type(entry)
        repaired.append((name, ref_source, ref_entry.nominal, ref_entry.min))

    out_path = output_path or target_path
    backup_path: Optional[Path] = None
    if backup and out_path == target_path:
        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        backup_path = target_path.parent / f"types.xml.pre-repair-{timestamp}"
        shutil.copy2(target_path, backup_path)

    if target_xml.save(str(out_path)):
        return TypesRepairResult(
            success=True,
            target_path=out_path,
            repaired=repaired,
            skipped=skipped,
            no_reference=no_reference,
            backup_path=backup_path,
        )

    return TypesRepairResult(
        success=False,
        target_path=out_path,
        repaired=repaired,
        error=f"Could not save {out_path}",
    )
