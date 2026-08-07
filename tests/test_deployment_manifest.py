# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Tests for deployment manifest caching."""

from pathlib import Path

from dayzconfigmaster.config.deployment_manifest import (
    DeploymentManifest,
    DeploymentManifestManager,
    compute_quick_skip_status,
)


def test_manifest_save_and_load(tmp_path: Path) -> None:
    mgr = DeploymentManifestManager(tmp_path)
    manifest = DeploymentManifest(
        instance_id=1,
        map_name="ChernarusPlus",
        dayz_server_path="/dayz",
        instance_root=str(tmp_path),
        mods_str="@CF;@Expansion",
        cfg_content_hash="abc123",
    )
    mgr.save(manifest)
    loaded = mgr.load()
    assert loaded is not None
    assert loaded.equals(manifest)


def test_manifest_detects_change(tmp_path: Path) -> None:
    mgr = DeploymentManifestManager(tmp_path)
    first = DeploymentManifest(
        instance_id=1,
        map_name="ChernarusPlus",
        dayz_server_path="/dayz",
        instance_root=str(tmp_path),
        mods_str="@CF;@Expansion",
        cfg_content_hash="abc123",
    )
    mgr.save(first)

    second = DeploymentManifest(
        instance_id=1,
        map_name="ChernarusPlus",
        dayz_server_path="/dayz",
        instance_root=str(tmp_path),
        mods_str="@CF;@Expansion;@Trader",
        cfg_content_hash="abc123",
    )
    assert not mgr.is_up_to_date(second)


def test_quick_skip_false_without_manifest(tmp_path: Path) -> None:
    current = DeploymentManifest(
        instance_id=1,
        map_name="ChernarusPlus",
        dayz_server_path="/dayz",
        instance_root=str(tmp_path),
        mods_str="@CF",
        cfg_content_hash="abc123",
    )
    can_skip, reason = compute_quick_skip_status(tmp_path, current)
    assert can_skip is False
    assert "no previous" in reason


def test_quick_skip_false_without_mission(tmp_path: Path) -> None:
    mgr = DeploymentManifestManager(tmp_path)
    current = DeploymentManifest(
        instance_id=1,
        map_name="ChernarusPlus",
        dayz_server_path="/dayz",
        instance_root=str(tmp_path),
        mods_str="@CF",
        cfg_content_hash="abc123",
    )
    mgr.save(current)
    can_skip, reason = compute_quick_skip_status(tmp_path, current)
    assert can_skip is False
    assert "mission folder missing" in reason


def test_quick_skip_true_when_manifest_and_mission_present(tmp_path: Path) -> None:
    mgr = DeploymentManifestManager(tmp_path)
    (tmp_path / "mpmissions" / "dayzOffline.chernarusplus").mkdir(parents=True)
    current = DeploymentManifest(
        instance_id=1,
        map_name="ChernarusPlus",
        dayz_server_path="/dayz",
        instance_root=str(tmp_path),
        mods_str="@CF",
        cfg_content_hash="abc123",
    )
    mgr.save(current)
    can_skip, reason = compute_quick_skip_status(tmp_path, current)
    assert can_skip is True


def test_manifest_tracks_mod_integration_state(tmp_path: Path) -> None:
    mgr = DeploymentManifestManager(tmp_path)
    (tmp_path / "mpmissions" / "dayzOffline.chernarusplus").mkdir(parents=True)

    def resolve_mod(_token: str) -> Path:
        return tmp_path / "workshop" / "@CF"

    (tmp_path / "workshop" / "@CF").mkdir(parents=True)
    (tmp_path / "workshop" / "@CF" / "meta.cpp").write_text("name = CF;")

    class _Var:
        def __init__(self, value: str):
            self._value = value
        def get(self) -> str:
            return self._value

    instance = {"id": _Var("1"), "map": _Var("ChernarusPlus")}
    manifest = mgr.compute(
        instance=instance,
        dayz_server_path=tmp_path / "dayz",
        instance_root=tmp_path,
        mods_str="@CF",
        mission_source_path=None,
        spawn_loadout={},
        mod_settings_overrides={},
        mod_integration_state={"active_mods": ["@CF"]},
        cfg_content="cfg",
        resolve_mod_source=resolve_mod,
    )

    assert manifest.mod_integration_state_json == '{"active_mods": ["@CF"]}'
    assert "@CF" in manifest.mod_fingerprints
    assert manifest.mod_fingerprints["@CF"] != ""

    mgr.save(manifest)
    assert mgr.is_up_to_date(manifest)

    changed = mgr.compute(
        instance=instance,
        dayz_server_path=tmp_path / "dayz",
        instance_root=tmp_path,
        mods_str="@CF",
        mission_source_path=None,
        spawn_loadout={},
        mod_settings_overrides={},
        mod_integration_state={"active_mods": ["@CF", "@Trader"]},
        cfg_content="cfg",
        resolve_mod_source=resolve_mod,
    )
    assert not mgr.is_up_to_date(changed)
