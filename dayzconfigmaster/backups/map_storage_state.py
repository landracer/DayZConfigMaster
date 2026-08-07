# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Per-instance map storage state tracking.

Records which map was last run on an instance so that map swaps can be
handled cleanly: when an instance is configured for a different map, DCM can
offer to restore the most recent backup for that map (from any instance) or
start fresh.
"""

from __future__ import annotations

import json
from dataclasses import dataclass, field
from datetime import datetime, timezone
from pathlib import Path
from typing import Optional


@dataclass
class InstanceMapStorageState:
    """Persisted state about the last map run on an instance."""

    instance_id: int
    last_run_map: str = ""
    last_run_at: Optional[str] = None
    last_backup_path: Optional[str] = None

    def to_dict(self) -> dict:
        return {
            "instance_id": self.instance_id,
            "last_run_map": self.last_run_map,
            "last_run_at": self.last_run_at,
            "last_backup_path": self.last_backup_path,
        }

    @classmethod
    def from_dict(cls, data: dict) -> "InstanceMapStorageState":
        return cls(
            instance_id=data.get("instance_id", 0),
            last_run_map=data.get("last_run_map", ""),
            last_run_at=data.get("last_run_at"),
            last_backup_path=data.get("last_backup_path"),
        )


class InstanceMapStorageTracker:
    """Read and write per-instance map storage state."""

    STATE_FILENAME = "dcm_storage_state.json"

    def __init__(self, instance_root: Path):
        self.instance_root = Path(instance_root)
        self.state_path = self.instance_root / self.STATE_FILENAME

    def load(self) -> InstanceMapStorageState:
        if not self.state_path.exists():
            return InstanceMapStorageState(
                instance_id=self._guess_instance_id()
            )
        try:
            data = json.loads(self.state_path.read_text(encoding="utf-8"))
            state = InstanceMapStorageState.from_dict(data)
            state.instance_id = self._guess_instance_id()
            return state
        except (json.JSONDecodeError, OSError):
            return InstanceMapStorageState(
                instance_id=self._guess_instance_id()
            )

    def save(self, state: InstanceMapStorageState) -> None:
        self.instance_root.mkdir(parents=True, exist_ok=True)
        self.state_path.write_text(
            json.dumps(state.to_dict(), indent=2, ensure_ascii=False),
            encoding="utf-8",
        )

    def record_run(self, map_name: str, backup_path: Optional[Path] = None) -> None:
        """Update the state after starting an instance."""
        state = self.load()
        state.last_run_map = map_name
        state.last_run_at = datetime.now(timezone.utc).isoformat()
        state.last_backup_path = str(backup_path) if backup_path else None
        self.save(state)

    def _guess_instance_id(self) -> int:
        """Try to derive the instance id from the folder name."""
        name = self.instance_root.name.lower()
        if name.startswith("server"):
            try:
                return int(name[len("server"):])
            except ValueError:
                pass
        return 0
