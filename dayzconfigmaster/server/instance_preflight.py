# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Instance preflight checks for DayZ server startup.

Validates that all files DayZ will touch are present, writable, and
consistent before the server process starts. The goal is to catch the
"attempt to write a readonly database" class of failures at deploy time
rather than after players have logged in.
"""

from __future__ import annotations

import enum
import hashlib
import os
import sqlite3
import subprocess
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple


class Severity(enum.Enum):
    """Issue severity levels."""

    INFO = "info"
    WARNING = "warning"
    ERROR = "error"


@dataclass(frozen=True)
class PreflightIssue:
    """A single preflight finding."""

    severity: Severity
    rule: str
    message: str
    path: Optional[Path] = None
    remediation: Optional[str] = None

    def to_dict(self) -> Dict[str, Any]:
        return {
            "severity": self.severity.value,
            "rule": self.rule,
            "message": self.message,
            "path": str(self.path) if self.path else None,
            "remediation": self.remediation,
        }


@dataclass
class InstancePreflightResult:
    """Collected result from running instance preflight checks."""

    issues: List[PreflightIssue] = field(default_factory=list)

    @property
    def ok(self) -> bool:
        return not any(i.severity == Severity.ERROR for i in self.issues)

    @property
    def has_errors(self) -> bool:
        return any(i.severity == Severity.ERROR for i in self.issues)

    @property
    def has_warnings(self) -> bool:
        return any(i.severity == Severity.WARNING for i in self.issues)

    @property
    def errors(self) -> List[PreflightIssue]:
        return [i for i in self.issues if i.severity == Severity.ERROR]

    @property
    def warnings(self) -> List[PreflightIssue]:
        return [i for i in self.issues if i.severity == Severity.WARNING]

    def add(
        self,
        severity: Severity,
        rule: str,
        message: str,
        path: Optional[Path] = None,
        remediation: Optional[str] = None,
    ) -> None:
        self.issues.append(
            PreflightIssue(
                severity=severity,
                rule=rule,
                message=message,
                path=path,
                remediation=remediation,
            )
        )

    def to_dict(self) -> Dict[str, Any]:
        return {
            "ok": self.ok,
            "issues": [i.to_dict() for i in self.issues],
        }


class InstancePreflightChecker:
    """Validate a DayZ server instance before startup."""

    # Files that must be writable for player persistence to work.
    CRITICAL_STORAGE_FILES: Tuple[str, ...] = ("players.db", "spawnpoints.bin")

    def __init__(
        self,
        instance_root: Path,
        instance_id: int = 1,
        map_name: str = "",
        config_filename: Optional[str] = None,
        profile_dir: Optional[Path] = None,
    ):
        self.instance_root = Path(instance_root)
        self.instance_id = instance_id
        self.map_name = map_name or "unknown"
        self.config_filename = config_filename or f"serverDZ_{instance_id}.cfg"
        self.profile_dir = (
            Path(profile_dir)
            if profile_dir
            else self.instance_root / "profiles"
        )
        self._mission_dir: Optional[Path] = None
        self._storage_path: Optional[Path] = None

    def run(self) -> InstancePreflightResult:
        """Run all preflight checks and return the result."""
        result = InstancePreflightResult()

        self._resolve_mission_dir(result)
        self._check_instance_root_writable(result)
        self._check_profile_dir_writable(result)
        self._check_config_readable(result)
        self._check_storage_1_writable(result)
        self._check_data_files_writable(result)
        # Check for stale journal files and attributes before opening SQLite;
        # SQLite may clean up journal files once it opens the database.
        self._check_stale_journal_files(result)
        self._check_file_attributes(result)
        # Integrity and backup comparison must happen before the write probe,
        # which mutates the database file.
        self._check_players_db_integrity(result)
        self._check_players_db_backup_consistency(result)
        self._check_players_db_writable(result)

        return result

    def _resolve_mission_dir(self, result: InstancePreflightResult) -> None:
        """Locate the mission directory for the configured map."""
        if not self.instance_root.exists():
            result.add(
                Severity.ERROR,
                "instance-root-missing",
                f"Instance root does not exist: {self.instance_root}",
                self.instance_root,
                "Create the instance directory or correct the root folder setting.",
            )
            return

        # Prefer the ModIntegrationManager heuristic when available.
        try:
            from dayzconfigmaster.mods.integration import ModIntegrationManager

            mission_dir = ModIntegrationManager(
                self.instance_root
            )._find_mission_dir()
        except Exception as exc:
            result.add(
                Severity.WARNING,
                "mission-dir-lookup-failed",
                f"Could not use ModIntegrationManager to find mission dir: {exc}",
                self.instance_root,
            )
            mission_dir = None

        if mission_dir is None:
            # Fallback: search mpmissions for a dayzOffline.<world> folder.
            mpmissions = self.instance_root / "mpmissions"
            if mpmissions.exists():
                candidates = [
                    d
                    for d in mpmissions.iterdir()
                    if d.is_dir() and d.name.lower().startswith("dayzoffline.")
                ]
                if len(candidates) == 1:
                    mission_dir = candidates[0]
                elif len(candidates) > 1:
                    result.add(
                        Severity.WARNING,
                        "multiple-mission-dirs",
                        f"Found multiple mission directories: {[d.name for d in candidates]}. "
                        "Using the first one; verify the configured map is correct.",
                        mpmissions,
                    )
                    mission_dir = candidates[0]

        if mission_dir is None or not mission_dir.exists():
            result.add(
                Severity.ERROR,
                "mission-dir-missing",
                f"No mission directory found under {self.instance_root / 'mpmissions'}. "
                "Deploy the mission folder before starting the server.",
                self.instance_root / "mpmissions",
                "Run a full deployment or copy the correct dayzOffline.<map> mission folder into mpmissions/.",
            )
            return

        self._mission_dir = mission_dir
        self._storage_path = mission_dir / "storage_1"

    def _check_instance_root_writable(
        self, result: InstancePreflightResult
    ) -> None:
        if not self.instance_root.exists():
            return
        if not os.access(self.instance_root, os.W_OK):
            result.add(
                Severity.ERROR,
                "instance-root-readonly",
                f"Instance root directory is not writable: {self.instance_root}",
                self.instance_root,
                "Fix ownership/permissions (e.g. chown -R user:group) before starting.",
            )
            return

        test_file = self.instance_root / ".dcm_write_test"
        try:
            test_file.write_text("ok")
            test_file.unlink()
        except OSError as exc:
            result.add(
                Severity.ERROR,
                "instance-root-write-failed",
                f"Instance root write test failed: {exc}",
                self.instance_root,
                "The directory may be read-only, on a full filesystem, or have an immutable/append-only attribute.",
            )

    def _check_profile_dir_writable(
        self, result: InstancePreflightResult
    ) -> None:
        if not self.profile_dir.exists():
            try:
                self.profile_dir.mkdir(parents=True, exist_ok=True)
            except OSError as exc:
                result.add(
                    Severity.ERROR,
                    "profile-dir-create-failed",
                    f"Cannot create profile directory {self.profile_dir}: {exc}",
                    self.profile_dir,
                    "Create the directory manually and ensure it is writable.",
                )
                return

        if not os.access(self.profile_dir, os.W_OK):
            result.add(
                Severity.ERROR,
                "profile-dir-readonly",
                f"Profile directory is not writable: {self.profile_dir}",
                self.profile_dir,
                "DayZ writes logs, mod settings, and BattlEye files here. Fix permissions before starting.",
            )
            return

        test_file = self.profile_dir / ".dcm_write_test"
        try:
            test_file.write_text("ok")
            test_file.unlink()
        except OSError as exc:
            result.add(
                Severity.ERROR,
                "profile-dir-write-failed",
                f"Profile directory write test failed: {exc}",
                self.profile_dir,
                "Mods will fail to save state if this directory cannot be written to.",
            )

    def _check_config_readable(self, result: InstancePreflightResult) -> None:
        cfg_path = self.instance_root / self.config_filename
        if not cfg_path.exists():
            result.add(
                Severity.WARNING,
                "config-missing",
                f"Config file not found yet: {cfg_path}. It will be generated during deployment.",
                cfg_path,
            )
            return
        if not os.access(cfg_path, os.R_OK):
            result.add(
                Severity.ERROR,
                "config-unreadable",
                f"Config file exists but is not readable: {cfg_path}",
                cfg_path,
                "Fix file permissions so the DayZ server process can read its config.",
            )

    def _check_storage_1_writable(
        self, result: InstancePreflightResult
    ) -> None:
        if self._storage_path is None:
            return
        if not self._storage_path.exists():
            try:
                self._storage_path.mkdir(parents=True, exist_ok=True)
                result.add(
                    Severity.INFO,
                    "storage-created",
                    f"Created missing storage_1 directory: {self._storage_path}",
                    self._storage_path,
                )
            except OSError as exc:
                result.add(
                    Severity.ERROR,
                    "storage-create-failed",
                    f"Cannot create storage_1 directory {self._storage_path}: {exc}",
                    self._storage_path,
                    "DayZ cannot persist player or world data without this directory.",
                )
                return

        if not os.access(self._storage_path, os.W_OK):
            result.add(
                Severity.ERROR,
                "storage-readonly",
                f"storage_1 directory is not writable: {self._storage_path}",
                self._storage_path,
                "This is the primary cause of the 'attempt to write a readonly database' error.",
            )
            return

        test_file = self._storage_path / ".dcm_write_test"
        try:
            test_file.write_text("ok")
            test_file.unlink()
        except OSError as exc:
            result.add(
                Severity.ERROR,
                "storage-write-failed",
                f"storage_1 write test failed: {exc}",
                self._storage_path,
                "Even with correct ownership, extended attributes (immutable/append-only) can block writes.",
            )

    def _check_data_files_writable(
        self, result: InstancePreflightResult
    ) -> None:
        if self._storage_path is None or not self._storage_path.exists():
            return

        data_dir = self._storage_path / "data"
        if data_dir.exists():
            for item in data_dir.iterdir():
                if item.is_file() and not os.access(item, os.W_OK):
                    result.add(
                        Severity.ERROR,
                        "data-file-readonly",
                        f"World-state file is not writable: {item}",
                        item,
                        "World persistence will fail or revert if this file cannot be updated.",
                    )

        # Also check known mod storage folders.
        for mod_dir_name in ("communityframework", "expansion"):
            mod_dir = self._storage_path / mod_dir_name
            if mod_dir.exists():
                for item in mod_dir.rglob("*"):
                    if item.is_file() and not os.access(item, os.W_OK):
                        result.add(
                            Severity.ERROR,
                            "mod-storage-file-readonly",
                            f"Mod storage file is not writable: {item}",
                            item,
                            "Mods such as Community Framework or Expansion store player state here.",
                        )

    def _check_players_db_writable(
        self, result: InstancePreflightResult
    ) -> None:
        if self._storage_path is None:
            return
        db_path = self._storage_path / "players.db"
        if not db_path.exists():
            result.add(
                Severity.INFO,
                "players-db-missing",
                "players.db does not exist yet. DayZ will create it on first player login.",
                db_path,
            )
            return

        if not os.access(db_path, os.W_OK):
            result.add(
                Severity.ERROR,
                "players-db-readonly",
                f"players.db is not writable: {db_path}",
                db_path,
                "This directly causes player time-warp on logout/login.",
            )
            return

        # The definitive test: open SQLite and attempt a write transaction.
        conn: Optional[sqlite3.Connection] = None
        try:
            conn = sqlite3.connect(str(db_path))
            conn.execute("BEGIN IMMEDIATE")
            conn.execute(
                "CREATE TABLE IF NOT EXISTS _dcm_preflight_probe (id INTEGER)"
            )
            conn.execute("INSERT INTO _dcm_preflight_probe (id) VALUES (1)")
            conn.execute("DELETE FROM _dcm_preflight_probe WHERE id = 1")
            conn.execute("DROP TABLE IF EXISTS _dcm_preflight_probe")
            conn.commit()
        except sqlite3.OperationalError as exc:
            msg = str(exc).lower()
            if "readonly" in msg or "read-only" in msg:
                result.add(
                    Severity.ERROR,
                    "players-db-sqlite-readonly",
                    f"SQLite reports players.db is read-only: {exc}",
                    db_path,
                    "Check for immutable attributes, stale journal files, or another process locking the database.",
                )
            elif "database is locked" in msg:
                result.add(
                    Severity.ERROR,
                    "players-db-locked",
                    f"players.db is locked by another process: {exc}",
                    db_path,
                    "Stop any other DayZ instance, backup tool, or file sync tool using this database.",
                )
            else:
                result.add(
                    Severity.ERROR,
                    "players-db-write-failed",
                    f"SQLite write test failed on players.db: {exc}",
                    db_path,
                    "The database may be corrupt or on a read-only filesystem.",
                )
        except Exception as exc:
            result.add(
                Severity.ERROR,
                "players-db-unexpected-error",
                f"Unexpected error testing players.db: {exc}",
                db_path,
            )
        finally:
            if conn is not None:
                try:
                    conn.close()
                except Exception:
                    pass

    def _check_players_db_integrity(
        self, result: InstancePreflightResult
    ) -> None:
        if self._storage_path is None:
            return
        db_path = self._storage_path / "players.db"
        if not db_path.exists():
            return

        conn: Optional[sqlite3.Connection] = None
        try:
            conn = sqlite3.connect(str(db_path))
            integrity = conn.execute("PRAGMA integrity_check").fetchone()
            if integrity is None or integrity[0] != "ok":
                result.add(
                    Severity.ERROR,
                    "players-db-integrity-fail",
                    f"players.db integrity check failed: {integrity}",
                    db_path,
                    "Restore from the most recent backup before starting the server.",
                )
            quick = conn.execute("PRAGMA quick_check").fetchone()
            if quick is None or quick[0] != "ok":
                result.add(
                    Severity.ERROR,
                    "players-db-quick-check-fail",
                    f"players.db quick_check failed: {quick}",
                    db_path,
                    "Restore from the most recent backup before starting the server.",
                )

            # Verify the Players table exists and has the expected columns.
            tables = {
                row[0]
                for row in conn.execute(
                    "SELECT name FROM sqlite_master WHERE type='table'"
                )
            }
            if "Players" not in tables:
                result.add(
                    Severity.WARNING,
                    "players-db-no-players-table",
                    "players.db has no 'Players' table. This is normal for a fresh server, "
                    "but unusual for an existing one.",
                    db_path,
                )
        except sqlite3.DatabaseError as exc:
            result.add(
                Severity.ERROR,
                "players-db-open-failed",
                f"Cannot open players.db as SQLite: {exc}",
                db_path,
                "The file may be corrupt or zero-length. Restore from backup.",
            )
        finally:
            if conn is not None:
                try:
                    conn.close()
                except Exception:
                    pass

    def _check_stale_journal_files(
        self, result: InstancePreflightResult
    ) -> None:
        if self._storage_path is None or not self._storage_path.exists():
            return

        for pattern in ("*-journal", "*-wal", "*-shm"):
            for stale in self._storage_path.glob(pattern):
                if stale.is_file():
                    result.add(
                        Severity.WARNING,
                        "stale-journal-file",
                        f"Stale SQLite journal/WAL file found: {stale}. "
                        "DayZ may treat the database as read-only until it is removed.",
                        stale,
                        "Stop the server and delete this file, or restore players.db from a backup.",
                    )

    def _check_file_attributes(self, result: InstancePreflightResult) -> None:
        """Detect immutable/append-only attributes that block writes."""
        if self._storage_path is None or not self._storage_path.exists():
            return

        paths_to_check = [self._storage_path]
        db_path = self._storage_path / "players.db"
        if db_path.exists():
            paths_to_check.append(db_path)
        data_dir = self._storage_path / "data"
        if data_dir.exists():
            paths_to_check.extend(f for f in data_dir.iterdir() if f.is_file())

        for path in paths_to_check:
            attrs = self._lsattr(path)
            if attrs is None:
                continue
            if "i" in attrs:
                result.add(
                    Severity.ERROR,
                    "file-immutable",
                    f"File/directory has the immutable attribute set ({attrs}): {path}",
                    path,
                    "Remove it with: chattr -i <path>. The server cannot write to immutable files.",
                )
            if "a" in attrs:
                result.add(
                    Severity.WARNING,
                    "file-append-only",
                    f"File/directory has the append-only attribute set ({attrs}): {path}",
                    path,
                    "Append-only can confuse SQLite. Consider chattr -a <path> unless you require it.",
                )

    def _check_players_db_backup_consistency(
        self, result: InstancePreflightResult
    ) -> None:
        """Compare current players.db to the latest backup for corruption hints."""
        if self._storage_path is None or not self._storage_path.exists():
            return
        db_path = self._storage_path / "players.db"
        if not db_path.exists():
            return

        try:
            from dayzconfigmaster.backups.instance_storage import (
                InstanceStorageBackupManager,
            )

            mgr = InstanceStorageBackupManager(
                self.instance_root, self.instance_id, self.map_name
            )
            backups = mgr.list_backups()
        except Exception:
            # Backup manager may not be available or configured; skip this check.
            return

        if not backups:
            result.add(
                Severity.WARNING,
                "players-db-no-backup",
                "No storage_1 backup exists for this instance/map. A backup is strongly recommended before start.",
                self._storage_path,
                "Use the backup button or start flow to create one.",
            )
            return

        latest = backups[0]
        backup_db = latest.path / "players.db"
        if not backup_db.exists():
            result.add(
                Severity.WARNING,
                "players-db-backup-missing-db",
                f"Most recent storage backup ({latest.path.name}) does not contain a players.db.",
                latest.path,
            )
            return

        # Compare row counts. A current DB with far fewer rows than the backup
        # is a strong signal that storageAutoFix rebuilt it from an older state.
        try:
            current_rows = self._count_players_rows(db_path)
            backup_rows = self._count_players_rows(backup_db)
            if current_rows is not None and backup_rows is not None:
                if current_rows < backup_rows:
                    result.add(
                        Severity.WARNING,
                        "players-db-fewer-rows-than-backup",
                        f"Current players.db has {current_rows} player row(s), but the latest backup "
                        f"({latest.path.name}) has {backup_rows}. This can happen when "
                        "storageAutoFix restores from an older checkpoint.",
                        db_path,
                        "Verify the backup contains the expected player state before continuing.",
                    )
        except Exception:
            pass

        # Compare file hashes to detect unexpected divergence.
        try:
            current_hash = self._hash_file(db_path)
            backup_hash = self._hash_file(backup_db)
            if current_hash and backup_hash and current_hash == backup_hash:
                result.add(
                    Severity.INFO,
                    "players-db-matches-backup",
                    f"Current players.db matches the latest backup ({latest.path.name}).",
                    db_path,
                )
        except Exception:
            pass

    @staticmethod
    def _count_players_rows(db_path: Path) -> Optional[int]:
        conn: Optional[sqlite3.Connection] = None
        try:
            conn = sqlite3.connect(str(db_path))
            tables = {
                row[0]
                for row in conn.execute(
                    "SELECT name FROM sqlite_master WHERE type='table'"
                )
            }
            if "Players" not in tables:
                return 0
            return conn.execute("SELECT COUNT(*) FROM Players").fetchone()[0]
        except Exception:
            return None
        finally:
            if conn is not None:
                try:
                    conn.close()
                except Exception:
                    pass

    @staticmethod
    def _hash_file(path: Path) -> Optional[str]:
        try:
            h = hashlib.sha256()
            with open(path, "rb") as f:
                for chunk in iter(lambda: f.read(65536), b""):
                    h.update(chunk)
            return h.hexdigest()
        except Exception:
            return None

    @staticmethod
    def _lsattr(path: Path) -> Optional[str]:
        """Return the lsattr flags for a path, or None if unavailable."""
        try:
            result = subprocess.run(
                ["lsattr", "-d", str(path)],
                capture_output=True,
                text=True,
                check=False,
                timeout=5,
            )
            if result.returncode != 0:
                return None
            # lsattr output: "--------------e------- /path"
            parts = result.stdout.strip().split()
            if parts:
                return parts[0]
            return None
        except Exception:
            return None


class InstancePreflightRepair:
    """Optional repair actions for common preflight failures."""

    def __init__(self, checker: InstancePreflightChecker):
        self.checker = checker

    def repair(self) -> InstancePreflightResult:
        """Attempt to fix common filesystem-level issues and re-run checks."""
        instance_root = self.checker.instance_root
        profile_dir = self.checker.profile_dir
        storage_path = self.checker._storage_path

        if instance_root.exists():
            self._chmod_tree(instance_root, dir_mode=0o755, file_mode=0o644)
        if profile_dir.exists():
            self._chmod_tree(profile_dir, dir_mode=0o755, file_mode=0o644)
        if storage_path is not None and storage_path.exists():
            self._chmod_tree(storage_path, dir_mode=0o755, file_mode=0o644)
            self._remove_immutable(storage_path)

        return self.checker.run()

    @staticmethod
    def _chmod_tree(
        path: Path, dir_mode: int = 0o755, file_mode: int = 0o644
    ) -> None:
        for item in path.rglob("*"):
            try:
                if item.is_dir():
                    item.chmod(dir_mode)
                elif item.is_file():
                    item.chmod(file_mode)
            except OSError:
                pass
        try:
            path.chmod(dir_mode)
        except OSError:
            pass

    @staticmethod
    def _remove_immutable(path: Path) -> None:
        try:
            subprocess.run(
                ["chattr", "-R", "-i", "-a", str(path)],
                capture_output=True,
                check=False,
                timeout=30,
            )
        except Exception:
            pass


def run_instance_preflight(
    instance_root: Path,
    instance_id: int = 1,
    map_name: str = "",
    config_filename: Optional[str] = None,
    profile_dir: Optional[Path] = None,
    attempt_repair: bool = False,
) -> InstancePreflightResult:
    """Convenience function to run instance preflight checks."""
    checker = InstancePreflightChecker(
        instance_root=instance_root,
        instance_id=instance_id,
        map_name=map_name,
        config_filename=config_filename,
        profile_dir=profile_dir,
    )
    result = checker.run()
    if attempt_repair and result.has_errors:
        repair = InstancePreflightRepair(checker)
        result = repair.repair()
    return result
