"""Tests for dayzconfigmaster.server.instance_preflight."""

import os
import sqlite3
from pathlib import Path

import pytest

from dayzconfigmaster.server.instance_preflight import (
    InstancePreflightChecker,
    InstancePreflightRepair,
    run_instance_preflight,
)


@pytest.fixture
def instance(tmp_path: Path) -> Path:
    """Create a minimal valid instance layout."""
    root = tmp_path / "instance"
    mpmissions = root / "mpmissions" / "dayzOffline.enoch"
    storage = mpmissions / "storage_1"
    storage.mkdir(parents=True)

    # Create a valid players.db with a Players table.
    db_path = storage / "players.db"
    conn = sqlite3.connect(str(db_path))
    conn.execute(
        "CREATE TABLE Players ("
        "Id INTEGER PRIMARY KEY, Alive INTEGER, UID CHAR(64), Data BLOB)"
    )
    conn.execute(
        "INSERT INTO Players (Alive, UID, Data) VALUES (1, 'uid1', x'00')"
    )
    conn.commit()
    conn.close()

    # World data files.
    (storage / "data").mkdir()
    (storage / "data" / "dynamic_000.bin").write_bytes(b"world")
    (storage / "spawnpoints.bin").write_bytes(b"spawns")

    # Profiles dir.
    (root / "profiles").mkdir()

    # Config file.
    (root / "serverDZ_1.cfg").write_text("// cfg")

    return root


class TestInstancePreflightHappyPath:
    def test_all_checks_pass(self, instance: Path):
        checker = InstancePreflightChecker(
            instance, instance_id=1, map_name="enoch"
        )
        result = checker.run()
        assert result.ok
        assert not result.has_errors
        # No backup exists in the fixture, so a warning is expected.
        assert any(i.rule == "players-db-no-backup" for i in result.warnings)

    def test_convenience_function(self, instance: Path):
        result = run_instance_preflight(
            instance, instance_id=1, map_name="enoch"
        )
        assert result.ok


class TestWritableChecks:
    def test_instance_root_not_writable(self, instance: Path):
        instance.chmod(0o555)
        try:
            checker = InstancePreflightChecker(
                instance, instance_id=1, map_name="enoch"
            )
            result = checker.run()
            assert not result.ok
            assert any(
                i.rule == "instance-root-readonly" for i in result.errors
            )
        finally:
            instance.chmod(0o755)

    def test_profile_dir_not_writable(self, instance: Path):
        profiles = instance / "profiles"
        profiles.chmod(0o555)
        try:
            checker = InstancePreflightChecker(
                instance, instance_id=1, map_name="enoch"
            )
            result = checker.run()
            assert not result.ok
            assert any(i.rule == "profile-dir-readonly" for i in result.errors)
        finally:
            profiles.chmod(0o755)

    def test_storage_readonly(self, instance: Path):
        storage = instance / "mpmissions" / "dayzOffline.enoch" / "storage_1"
        storage.chmod(0o555)
        try:
            checker = InstancePreflightChecker(
                instance, instance_id=1, map_name="enoch"
            )
            result = checker.run()
            assert not result.ok
            assert any(i.rule == "storage-readonly" for i in result.errors)
        finally:
            storage.chmod(0o755)

    def test_players_db_readonly(self, instance: Path):
        db_path = (
            instance
            / "mpmissions"
            / "dayzOffline.enoch"
            / "storage_1"
            / "players.db"
        )
        db_path.chmod(0o444)
        try:
            checker = InstancePreflightChecker(
                instance, instance_id=1, map_name="enoch"
            )
            result = checker.run()
            assert not result.ok
            assert any(
                i.rule in ("players-db-readonly", "players-db-sqlite-readonly")
                for i in result.errors
            )
        finally:
            db_path.chmod(0o644)

    def test_data_file_readonly(self, instance: Path):
        data_file = (
            instance
            / "mpmissions"
            / "dayzOffline.enoch"
            / "storage_1"
            / "data"
            / "dynamic_000.bin"
        )
        data_file.chmod(0o444)
        try:
            checker = InstancePreflightChecker(
                instance, instance_id=1, map_name="enoch"
            )
            result = checker.run()
            assert not result.ok
            assert any(i.rule == "data-file-readonly" for i in result.errors)
        finally:
            data_file.chmod(0o644)


class TestDatabaseIntegrity:
    def test_corrupt_players_db(self, instance: Path):
        db_path = (
            instance
            / "mpmissions"
            / "dayzOffline.enoch"
            / "storage_1"
            / "players.db"
        )
        db_path.write_bytes(b"this is not sqlite")
        checker = InstancePreflightChecker(
            instance, instance_id=1, map_name="enoch"
        )
        result = checker.run()
        assert not result.ok
        assert any(
            i.rule in ("players-db-open-failed", "players-db-integrity-fail")
            for i in result.errors
        )

    def test_players_db_missing_table(self, instance: Path):
        db_path = (
            instance
            / "mpmissions"
            / "dayzOffline.enoch"
            / "storage_1"
            / "players.db"
        )
        conn = sqlite3.connect(str(db_path))
        conn.execute("DROP TABLE Players")
        conn.commit()
        conn.close()
        checker = InstancePreflightChecker(
            instance, instance_id=1, map_name="enoch"
        )
        result = checker.run()
        assert result.ok
        assert any(
            i.rule == "players-db-no-players-table" for i in result.warnings
        )


class TestJournalFiles:
    def test_stale_journal_file(self, instance: Path):
        storage = instance / "mpmissions" / "dayzOffline.enoch" / "storage_1"
        (storage / "players.db-journal").write_text("stale")
        checker = InstancePreflightChecker(
            instance, instance_id=1, map_name="enoch"
        )
        result = checker.run()
        assert result.ok
        assert any(i.rule == "stale-journal-file" for i in result.warnings)

    def test_stale_wal_file(self, instance: Path):
        storage = instance / "mpmissions" / "dayzOffline.enoch" / "storage_1"
        (storage / "players.db-wal").write_text("stale")
        checker = InstancePreflightChecker(
            instance, instance_id=1, map_name="enoch"
        )
        result = checker.run()
        assert result.ok
        assert any(i.rule == "stale-journal-file" for i in result.warnings)


class TestBackupConsistency:
    def test_no_backup_warning(self, instance: Path):
        checker = InstancePreflightChecker(
            instance, instance_id=1, map_name="enoch"
        )
        result = checker.run()
        assert any(i.rule == "players-db-no-backup" for i in result.warnings)

    def test_current_matches_backup(self, instance: Path):
        from dayzconfigmaster.backups.instance_storage import (
            InstanceStorageBackupManager,
        )

        storage = instance / "mpmissions" / "dayzOffline.enoch" / "storage_1"
        mgr = InstanceStorageBackupManager(instance, 1, "enoch")
        ok, _ = mgr.create_backup(storage)
        assert ok

        checker = InstancePreflightChecker(
            instance, instance_id=1, map_name="enoch"
        )
        result = checker.run()
        assert any(
            i.rule == "players-db-matches-backup" for i in result.issues
        )

    def test_fewer_rows_than_backup(self, instance: Path):
        from dayzconfigmaster.backups.instance_storage import (
            InstanceStorageBackupManager,
        )

        storage = instance / "mpmissions" / "dayzOffline.enoch" / "storage_1"
        mgr = InstanceStorageBackupManager(instance, 1, "enoch")
        ok, _ = mgr.create_backup(storage)
        assert ok

        # Remove a row from the live DB.
        db_path = storage / "players.db"
        conn = sqlite3.connect(str(db_path))
        conn.execute("DELETE FROM Players WHERE UID = 'uid1'")
        conn.commit()
        conn.close()

        checker = InstancePreflightChecker(
            instance, instance_id=1, map_name="enoch"
        )
        result = checker.run()
        assert any(
            i.rule == "players-db-fewer-rows-than-backup"
            for i in result.warnings
        )


class TestRepair:
    def test_repair_fixes_readonly_permissions(self, instance: Path):
        db_path = (
            instance
            / "mpmissions"
            / "dayzOffline.enoch"
            / "storage_1"
            / "players.db"
        )
        db_path.chmod(0o444)
        data_file = (
            instance
            / "mpmissions"
            / "dayzOffline.enoch"
            / "storage_1"
            / "data"
            / "dynamic_000.bin"
        )
        data_file.chmod(0o444)
        try:
            checker = InstancePreflightChecker(
                instance, instance_id=1, map_name="enoch"
            )
            repair = InstancePreflightRepair(checker)
            result = repair.repair()
            assert result.ok
            assert os.access(db_path, os.W_OK)
            assert os.access(data_file, os.W_OK)
        finally:
            db_path.chmod(0o644)
            data_file.chmod(0o644)


class TestMissingMission:
    def test_missing_mission_dir(self, tmp_path: Path):
        root = tmp_path / "instance"
        root.mkdir()
        (root / "profiles").mkdir()
        checker = InstancePreflightChecker(
            root, instance_id=1, map_name="enoch"
        )
        result = checker.run()
        assert not result.ok
        assert any(i.rule == "mission-dir-missing" for i in result.errors)
