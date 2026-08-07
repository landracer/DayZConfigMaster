# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Safe read/compare/splice operations for DayZ players.db.

DayZ stores persistent character data in a SQLite database with one table:

    CREATE TABLE Players(Id INTEGER, Alive INTEGER, UID TEXT, Data BLOB);

The ``Data`` column is an opaque binary blob controlled by the DayZ engine.
DCM does not attempt to decode it. All operations are row-level: we compare by
UID and, when restoring, copy the entire blob for a selected UID into the
current database without touching any other player's data.
"""

from __future__ import annotations

import shutil
import sqlite3
from dataclasses import dataclass
from datetime import datetime, timezone
from pathlib import Path
from typing import Dict, List, Optional, Tuple


@dataclass(frozen=True)
class PlayerRow:
    """One row from the Players table."""

    player_id: int
    alive: int
    uid: str
    data: bytes

    @property
    def data_size(self) -> int:
        """Return the byte length of the player's Data blob."""
        return len(self.data)


@dataclass
class PlayerDiff:
    """Comparison result for one UID across current and backup databases."""

    uid: str
    current: Optional[PlayerRow]
    backup: Optional[PlayerRow]

    @property
    def status(self) -> str:
        if self.current is None and self.backup is not None:
            return "only in backup"
        if self.backup is None and self.current is not None:
            return "only in current"
        if self.current is not None and self.backup is not None:
            if self.current.data == self.backup.data:
                return "identical"
            return "different"
        return "unknown"

    @property
    def is_restorable(self) -> bool:
        """A row can be spliced if we have data in the backup for this UID."""
        return self.backup is not None


class PlayersDbError(Exception):
    """Raised when a players.db operation cannot continue safely."""


class PlayersDbHandle:
    """Read-only access to a players.db file."""

    EXPECTED_TABLE = "Players"
    EXPECTED_COLUMNS = {"Id", "Alive", "UID", "Data"}

    def __init__(self, db_path: Path):
        self.db_path = Path(db_path)
        if not self.db_path.exists():
            raise PlayersDbError(f"database not found: {self.db_path}")
        self.table_name = self.EXPECTED_TABLE
        self._detect_table_name()

    def _detect_table_name(self) -> None:
        """Auto-detect the players table name in case DayZ renames it."""
        with sqlite3.connect(str(self.db_path)) as conn:
            cursor = conn.execute(
                "SELECT name FROM sqlite_master WHERE type='table'"
            )
            tables = {row[0] for row in cursor.fetchall()}
            if self.EXPECTED_TABLE in tables:
                self.table_name = self.EXPECTED_TABLE
                return
            for name in ("players", "Survivors", "survivors"):
                if name in tables:
                    self.table_name = name
                    return
            if not tables:
                raise PlayersDbError(f"no tables found in {self.db_path}")
            # Last resort: pick the only table.
            self.table_name = next(iter(tables))

    def _validate_schema(self, conn: sqlite3.Connection) -> None:
        cursor = conn.execute(
            "SELECT name FROM sqlite_master WHERE type='table' AND name=?",
            (self.table_name,),
        )
        if cursor.fetchone() is None:
            raise PlayersDbError(
                f"table {self.table_name} not found in {self.db_path}"
            )

        cursor = conn.execute(f"PRAGMA table_info({self.table_name})")
        columns = {row[1] for row in cursor.fetchall()}
        if not self.EXPECTED_COLUMNS.issubset(columns):
            missing = self.EXPECTED_COLUMNS - columns
            raise PlayersDbError(f"missing columns in {self.table_name} table: {missing}")

    def read_players(self) -> List[PlayerRow]:
        """Return all rows from the players table."""
        with sqlite3.connect(str(self.db_path)) as conn:
            self._validate_schema(conn)
            cursor = conn.execute(
                f"SELECT Id, Alive, UID, Data FROM {self.table_name} ORDER BY UID"
            )
            return [
                PlayerRow(
                    player_id=row[0],
                    alive=row[1],
                    uid=row[2],
                    data=row[3] if row[3] is not None else b"",
                )
                for row in cursor.fetchall()
            ]

    def read_players_by_uid(self) -> Dict[str, PlayerRow]:
        """Return rows keyed by UID."""
        result: Dict[str, PlayerRow] = {}
        for row in self.read_players():
            if row.uid in result:
                raise PlayersDbError(f"duplicate UID {row.uid!r} in {self.db_path}")
            result[row.uid] = row
        return result


class PlayersDbSplicer:
    """Copy a single player's Data blob from a backup into the current DB."""

    def __init__(self, current_db_path: Path):
        self.current_db_path = Path(current_db_path)

    def _safety_backup_path(self) -> Path:
        timestamp = datetime.now(timezone.utc).strftime("%Y%m%d_%H%M%S")
        return self.current_db_path.parent / f"players.db.dcm_safety_{timestamp}.bak"

    def _copy_with_splice(
        self,
        backup_db_path: Path,
        uid: str,
        target_path: Path,
    ) -> Tuple[bool, str]:
        """Copy current DB to target, replacing the selected UID's row."""
        if not self.current_db_path.exists():
            return False, f"current database not found: {self.current_db_path}"
        if not backup_db_path.exists():
            return False, f"backup database not found: {backup_db_path}"

        current_handle = PlayersDbHandle(self.current_db_path)
        backup_handle = PlayersDbHandle(backup_db_path)

        current_players = current_handle.read_players_by_uid()
        backup_players = backup_handle.read_players_by_uid()

        if uid not in backup_players:
            return False, f"UID {uid!r} not found in backup database"

        backup_row = backup_players[uid]

        # Build the new player set.
        new_players = list(current_players.values())
        replaced = False
        for i, row in enumerate(new_players):
            if row.uid == uid:
                new_players[i] = backup_row
                replaced = True
                break
        if not replaced:
            new_players.append(backup_row)

        # Write to a temporary file first, then swap.
        temp_path = target_path.with_suffix(".tmp")
        try:
            shutil.copy2(str(self.current_db_path), str(temp_path))
            with sqlite3.connect(str(temp_path)) as conn:
                # Wipe and rewrite only the players table rows.
                conn.execute(f"DELETE FROM {backup_handle.table_name}")
                conn.executemany(
                    f"INSERT INTO {backup_handle.table_name} (Id, Alive, UID, Data) VALUES (?, ?, ?, ?)",
                    [(r.player_id, r.alive, r.uid, r.data) for r in new_players],
                )
                conn.commit()

            # Verify the spliced database opens and has the expected rows.
            with sqlite3.connect(str(temp_path)) as verify_conn:
                cursor = verify_conn.execute(
                    f"SELECT COUNT(*) FROM {backup_handle.table_name}"
                )
                row_count = cursor.fetchone()[0]
                if row_count != len(new_players):
                    raise PlayersDbError(
                        f"verification failed: expected {len(new_players)} rows, got {row_count}"
                    )

            temp_path.replace(target_path)
            return True, f"spliced UID {uid!r} from {backup_db_path.name}"
        except Exception as exc:
            if temp_path.exists():
                try:
                    temp_path.unlink()
                except OSError:
                    pass
            return False, f"splice failed: {exc}"

    def splice_player(
        self,
        backup_db_path: Path,
        uid: str,
        dry_run: bool = False,
    ) -> Tuple[bool, str]:
        """Restore one player's Data blob from a backup.

        Steps:
        1. Validate both databases have the expected Players table.
        2. Create a dated safety backup of the current DB.
        3. (unless dry_run) Write the spliced database in place.
        """
        # Schema validation first.
        PlayersDbHandle(self.current_db_path)
        PlayersDbHandle(backup_db_path)

        if dry_run:
            return True, f"would splice UID {uid!r} from {backup_db_path.name}"

        safety_path = self._safety_backup_path()
        try:
            shutil.copy2(str(self.current_db_path), str(safety_path))
        except Exception as exc:
            return False, f"failed to create safety backup: {exc}"

        ok, msg = self._copy_with_splice(backup_db_path, uid, self.current_db_path)
        if not ok:
            # Try to roll back from safety backup.
            try:
                shutil.copy2(str(safety_path), str(self.current_db_path))
            except Exception as rollback_exc:
                return False, (
                    f"{msg}; rollback from safety backup also failed: {rollback_exc}"
                )
        return ok, msg


def compare_players_dbs(
    current_db_path: Path,
    backup_db_path: Path,
) -> Tuple[bool, List[PlayerDiff], str]:
    """Compare Players tables across two databases.

    Returns (ok, diffs, message). diffs is sorted by UID.
    """
    try:
        current = PlayersDbHandle(current_db_path).read_players_by_uid()
        backup = PlayersDbHandle(backup_db_path).read_players_by_uid()
    except PlayersDbError as exc:
        return False, [], str(exc)

    diffs: List[PlayerDiff] = []
    all_uids = sorted(set(current.keys()) | set(backup.keys()))
    for uid in all_uids:
        diffs.append(
            PlayerDiff(
                uid=uid,
                current=current.get(uid),
                backup=backup.get(uid),
            )
        )
    return True, diffs, f"compared {len(all_uids)} player(s)"


def find_players_db(storage_path: Path) -> Optional[Path]:
    """Return the players.db path inside a storage_1 folder if it exists."""
    candidates = [storage_path / "players.db", storage_path / "Players.db"]
    for candidate in candidates:
        if candidate.exists():
            return candidate
    return None
