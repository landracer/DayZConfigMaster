# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Mod preset persistence for per-instance mod loadouts.

A mod preset is a named, ordered list of mod tokens such as:

    "PvE": "@CF;@DayZ-Expansion;@VPPAdminTools"

Presets are stored in ``<projects_root>/mod_presets.json`` so they survive
application restarts and can be reused across instances.
"""

from __future__ import annotations

import json
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple


@dataclass
class ModPreset:
    """A named mod loadout."""

    name: str
    mods: List[str] = field(default_factory=list)

    @property
    def mod_string(self) -> str:
        """Return the ordered mod list as a DayZ ``-mod=`` string."""
        return ";".join(self.mods)

    @classmethod
    def from_mod_string(cls, name: str, mod_string: str) -> "ModPreset":
        """Build a preset from a raw mod string."""
        mods = [m.strip() for m in mod_string.split(";") if m.strip()]
        return cls(name=name, mods=mods)


class ModPresetManager:
    """Load, save, and manage named mod presets under a projects root."""

    FILENAME = "mod_presets.json"

    def __init__(self, projects_root: Path):
        self.projects_root = Path(projects_root)
        self._path = self.projects_root / self.FILENAME
        self._presets: Dict[str, ModPreset] = {}
        self._load()

    def _load(self) -> None:
        """Read presets from disk."""
        if not self._path.exists():
            self._presets = {}
            return
        try:
            data = json.loads(self._path.read_text(encoding="utf-8"))
            self._presets = {}
            for name, value in data.items():
                if isinstance(value, list):
                    self._presets[name] = ModPreset(
                        name=name, mods=[str(m) for m in value]
                    )
                elif isinstance(value, str):
                    self._presets[name] = ModPreset.from_mod_string(
                        name, value
                    )
        except (OSError, json.JSONDecodeError):
            self._presets = {}

    def _save(self) -> bool:
        """Write presets to disk."""
        try:
            self.projects_root.mkdir(parents=True, exist_ok=True)
            payload: Dict[str, List[str]] = {
                name: preset.mods for name, preset in self._presets.items()
            }
            self._path.write_text(
                json.dumps(payload, indent=2, ensure_ascii=False),
                encoding="utf-8",
            )
            return True
        except OSError:
            return False

    def list_presets(self) -> List[str]:
        """Return preset names in alphabetical order."""
        return sorted(self._presets.keys())

    def get_preset(self, name: str) -> Optional[ModPreset]:
        """Return a preset by name, or None if it does not exist."""
        return self._presets.get(name)

    def save_preset(self, name: str, mods: List[str]) -> Tuple[bool, str]:
        """Save or update a preset."""
        name = name.strip()
        if not name:
            return False, "Preset name cannot be empty."
        cleaned = [m.strip() for m in mods if m.strip()]
        self._presets[name] = ModPreset(name=name, mods=cleaned)
        if self._save():
            return True, f"Saved preset '{name}'."
        return False, f"Could not write presets to {self._path}."

    def save_preset_from_string(
        self, name: str, mod_string: str
    ) -> Tuple[bool, str]:
        """Convenience helper that parses a semicolon-separated mod string."""
        mods = [m.strip() for m in mod_string.split(";") if m.strip()]
        return self.save_preset(name, mods)

    def delete_preset(self, name: str) -> Tuple[bool, str]:
        """Remove a preset."""
        if name not in self._presets:
            return False, f"Preset '{name}' not found."
        del self._presets[name]
        if self._save():
            return True, f"Deleted preset '{name}'."
        return False, f"Could not write presets to {self._path}."

    def apply_preset_to_string(self, name: str) -> Optional[str]:
        """Return the mod string for a preset, or None if not found."""
        preset = self._presets.get(name)
        return preset.mod_string if preset else None

    def to_dict(self) -> Dict[str, Any]:
        """Return a JSON-serializable representation."""
        return {name: preset.mods for name, preset in self._presets.items()}

    def export_presets(self, export_path: Path) -> Tuple[bool, str]:
        """Write all presets to a JSON file for backup/sharing."""
        target = Path(export_path)
        try:
            target.write_text(
                json.dumps(self.to_dict(), indent=2, ensure_ascii=False),
                encoding="utf-8",
            )
            return True, f"Exported {len(self._presets)} presets to {target}."
        except OSError as exc:
            return False, f"Failed to export presets: {exc}"

    def import_presets(
        self,
        import_path: Path,
        overwrite: bool = False,
    ) -> Tuple[bool, str]:
        """Load presets from a JSON file.

        Presets are merged into the existing collection. Existing presets are
        only overwritten when ``overwrite`` is True.
        """
        source = Path(import_path)
        if not source.exists():
            return False, f"Import file not found: {source}"
        try:
            data = json.loads(source.read_text(encoding="utf-8"))
        except (OSError, json.JSONDecodeError) as exc:
            return False, f"Failed to read import file: {exc}"

        if not isinstance(data, dict):
            return False, "Invalid preset file format: expected a JSON object."

        added, skipped = 0, 0
        for name, value in data.items():
            if not isinstance(name, str) or not name.strip():
                continue
            name = name.strip()
            if name in self._presets and not overwrite:
                skipped += 1
                continue
            if isinstance(value, list):
                self._presets[name] = ModPreset(
                    name=name, mods=[str(m).strip() for m in value if str(m).strip()]
                )
                added += 1
            elif isinstance(value, str):
                self._presets[name] = ModPreset.from_mod_string(name, value)
                added += 1

        if self._save():
            total = len(self._presets)
            return True, f"Imported {added} presets, skipped {skipped}. Total: {total}."
        return False, "Could not write presets after import."
