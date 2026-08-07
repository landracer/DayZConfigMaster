# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Tests for app-level backup and deployment-skip hooks."""

from pathlib import Path
from unittest.mock import MagicMock

from dayzconfigmaster.backups.instance_storage import InstanceStorageBackupManager


def test_instance_storage_backup_path_layout(tmp_path: Path) -> None:
    """The per-instance backup manager uses the expected directory layout."""
    instance_root = tmp_path / "server1"
    mgr = InstanceStorageBackupManager(instance_root, 1, "ChernarusPlus")
    storage = instance_root / "mpmissions" / "dayzOffline.chernarusplus" / "storage_1"
    storage.mkdir(parents=True)
    (storage / "players.db").write_text("current")

    ok, msg = mgr.create_backup(storage)
    assert ok is True

    backups = list((instance_root / "backups" / "storage_1" / "ChernarusPlus").iterdir())
    assert len(backups) == 1
    assert (backups[0] / "players.db").read_text() == "current"


def test_manifest_skip_when_outputs_present(tmp_path: Path) -> None:
    """compute_quick_skip_status returns True when manifest and mission exist."""
    from dayzconfigmaster.config.deployment_manifest import (
        DeploymentManifest,
        DeploymentManifestManager,
        compute_quick_skip_status,
    )

    mgr = DeploymentManifestManager(tmp_path)
    (tmp_path / "mpmissions" / "dayzOffline.chernarusplus").mkdir(parents=True)
    manifest = DeploymentManifest(
        instance_id=1,
        map_name="ChernarusPlus",
        dayz_server_path="/dayz",
        instance_root=str(tmp_path),
        mods_str="@CF",
        cfg_content_hash="abc",
    )
    mgr.save(manifest)

    can_skip, reason = compute_quick_skip_status(tmp_path, manifest)
    assert can_skip is True
    assert "manifest matches" in reason
