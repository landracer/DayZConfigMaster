# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Tests for players.db row-level compare/splice."""

import sqlite3
from pathlib import Path

import pytest

from dayzconfigmaster.backups.players_db import (
    PlayersDbError,
    PlayersDbHandle,
    PlayersDbSplicer,
    compare_players_dbs,
    find_players_db,
)


def _make_db(path: Path, rows: list) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with sqlite3.connect(str(path)) as conn:
        conn.execute(
            "CREATE TABLE Players (Id INTEGER, Alive INTEGER, UID TEXT, Data BLOB)"
        )
        conn.executemany(
            "INSERT INTO Players (Id, Alive, UID, Data) VALUES (?, ?, ?, ?)",
            rows,
        )
        conn.commit()


def test_read_players(tmp_path: Path) -> None:
    db = tmp_path / "players.db"
    _make_db(db, [(1, 1, "uid_a", b"blob_a"), (2, 1, "uid_b", b"blob_b")])

    handle = PlayersDbHandle(db)
    players = handle.read_players()
    assert len(players) == 2
    assert {p.uid for p in players} == {"uid_a", "uid_b"}


def test_compare_detects_different_blob(tmp_path: Path) -> None:
    current = tmp_path / "current.db"
    backup = tmp_path / "backup.db"
    _make_db(current, [(1, 1, "uid_a", b"current_a")])
    _make_db(backup, [(1, 1, "uid_a", b"backup_a")])

    ok, diffs, msg = compare_players_dbs(current, backup)
    assert ok is True
    assert len(diffs) == 1
    assert diffs[0].status == "different"


def test_compare_detects_missing_in_backup(tmp_path: Path) -> None:
    current = tmp_path / "current.db"
    backup = tmp_path / "backup.db"
    _make_db(current, [(1, 1, "uid_a", b"blob")])
    _make_db(backup, [])

    ok, diffs, msg = compare_players_dbs(current, backup)
    assert diffs[0].status == "only in current"


def test_splice_replaces_blob(tmp_path: Path) -> None:
    current = tmp_path / "current.db"
    backup = tmp_path / "backup.db"
    _make_db(current, [(1, 1, "uid_a", b"current_a"), (2, 1, "uid_b", b"uid_b_current")])
    _make_db(backup, [(1, 1, "uid_a", b"backup_a"), (2, 1, "uid_b", b"uid_b_backup")])

    splicer = PlayersDbSplicer(current)
    ok, msg = splicer.splice_player(backup, "uid_a")
    assert ok is True

    handle = PlayersDbHandle(current)
    players = handle.read_players_by_uid()
    assert players["uid_a"].data == b"backup_a"
    assert players["uid_b"].data == b"uid_b_current"

    # Safety backup should have been created.
    assert len(list(current.parent.glob("players.db.dcm_safety_*.bak"))) == 1


def test_splice_creates_row_if_uid_missing(tmp_path: Path) -> None:
    current = tmp_path / "current.db"
    backup = tmp_path / "backup.db"
    _make_db(current, [(1, 1, "uid_a", b"current_a")])
    _make_db(backup, [(2, 1, "uid_b", b"backup_b")])

    splicer = PlayersDbSplicer(current)
    ok, msg = splicer.splice_player(backup, "uid_b")
    assert ok is True

    players = PlayersDbHandle(current).read_players_by_uid()
    assert "uid_b" in players
    assert players["uid_b"].data == b"backup_b"


def test_splice_fails_for_missing_uid_in_backup(tmp_path: Path) -> None:
    current = tmp_path / "current.db"
    backup = tmp_path / "backup.db"
    _make_db(current, [(1, 1, "uid_a", b"blob")])
    _make_db(backup, [])

    splicer = PlayersDbSplicer(current)
    ok, msg = splicer.splice_player(backup, "uid_a")
    assert ok is False
    assert "not found in backup" in msg


def test_find_players_db(tmp_path: Path) -> None:
    db = tmp_path / "players.db"
    _make_db(db, [(1, 1, "uid", b"blob")])
    assert find_players_db(tmp_path) == db


def test_handle_rejects_bad_schema(tmp_path: Path) -> None:
    db = tmp_path / "players.db"
    db.parent.mkdir(parents=True, exist_ok=True)
    with sqlite3.connect(str(db)) as conn:
        conn.execute("CREATE TABLE Other (id INTEGER)")
    with pytest.raises(PlayersDbError):
        PlayersDbHandle(db).read_players()
