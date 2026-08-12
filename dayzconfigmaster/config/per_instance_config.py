# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Per-instance configuration storage for DayZConfigMaster.

Each server instance can have its own:
- Spawn loadout (enabled mod spawnables with counts)
- Mod settings overrides (per-instance copies of workshop/mission config files)

This module is headless and can be used by both the GUI and the deployment
pipeline.  All paths are absolute and resolved before storage.
"""

from __future__ import annotations

import json
import shutil
from dataclasses import dataclass, field
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple


DEFAULT_SPAWN_COUNT = 10


@dataclass
class DeploymentOptions:
    """Runtime-toggleable deployment steps for one instance.

    All XML-modifying steps default to False so a fresh checkout cannot
    silently alter factory mission files.  Backup and validation default to
    True because they are read-only safety measures.
    """

    deploy_mission_folder: bool = False
    sanitize_mission_economy: bool = False
    normalize_aircraft_lifetimes: bool = False
    repair_nominal_values: bool = False
    apply_mod_integration: bool = False
    apply_spawn_loadout: bool = False
    apply_mod_settings_overrides: bool = False
    backup_storage_before_start: bool = True
    clear_ce_storage_on_start: bool = True
    validate_against_sandbox: bool = True
    require_confirmation_for_xml_changes: bool = True

    def to_dict(self) -> Dict[str, Any]:
        return {
            "deploy_mission_folder": self.deploy_mission_folder,
            "sanitize_mission_economy": self.sanitize_mission_economy,
            "normalize_aircraft_lifetimes": self.normalize_aircraft_lifetimes,
            "repair_nominal_values": self.repair_nominal_values,
            "apply_mod_integration": self.apply_mod_integration,
            "apply_spawn_loadout": self.apply_spawn_loadout,
            "apply_mod_settings_overrides": self.apply_mod_settings_overrides,
            "backup_storage_before_start": self.backup_storage_before_start,
            "clear_ce_storage_on_start": self.clear_ce_storage_on_start,
            "validate_against_sandbox": self.validate_against_sandbox,
            "require_confirmation_for_xml_changes": self.require_confirmation_for_xml_changes,
        }

    @classmethod
    def from_dict(cls, data: Optional[Dict[str, Any]]) -> "DeploymentOptions":
        if data is None:
            return cls()
        return cls(
            deploy_mission_folder=data.get("deploy_mission_folder", False),
            sanitize_mission_economy=data.get("sanitize_mission_economy", False),
            normalize_aircraft_lifetimes=data.get("normalize_aircraft_lifetimes", False),
            repair_nominal_values=data.get("repair_nominal_values", False),
            apply_mod_integration=data.get("apply_mod_integration", False),
            apply_spawn_loadout=data.get("apply_spawn_loadout", False),
            apply_mod_settings_overrides=data.get("apply_mod_settings_overrides", False),
            backup_storage_before_start=data.get("backup_storage_before_start", True),
            clear_ce_storage_on_start=data.get("clear_ce_storage_on_start", True),
            validate_against_sandbox=data.get("validate_against_sandbox", True),
            require_confirmation_for_xml_changes=data.get(
                "require_confirmation_for_xml_changes", True
            ),
        )

    def any_xml_modification_enabled(self) -> bool:
        """Return True if any step that can alter XML is enabled."""
        return any(
            (
                self.sanitize_mission_economy,
                self.normalize_aircraft_lifetimes,
                self.repair_nominal_values,
                self.apply_mod_integration,
                self.apply_spawn_loadout,
                self.apply_mod_settings_overrides,
            )
        )


@dataclass
class SpawnableEntry:
    """A single spawnable selected for an instance."""
    name: str
    category: str
    source: str
    spawn_count: int = DEFAULT_SPAWN_COUNT
    min_count: int = 0
    lifetime: int = 7200
    restock: int = 0
    quantmin: int = 30
    quantmax: int = 80
    usage: str = "Town"
    value: str = "Tier12"
    tier: int = 1
    locations: List[Dict[str, float]] = field(default_factory=list)
    event_min: int = 1
    event_max: int = 1

    def to_dict(self) -> Dict[str, Any]:
        return {
            "name": self.name,
            "category": self.category,
            "source": self.source,
            "spawn_count": self.spawn_count,
            "min_count": self.min_count,
            "lifetime": self.lifetime,
            "restock": self.restock,
            "quantmin": self.quantmin,
            "quantmax": self.quantmax,
            "usage": self.usage,
            "value": self.value,
            "tier": self.tier,
            "locations": self.locations,
            "event_min": self.event_min,
            "event_max": self.event_max,
        }

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "SpawnableEntry":
        return cls(
            name=data.get("name", ""),
            category=data.get("category", "generic"),
            source=data.get("source", ""),
            spawn_count=data.get("spawn_count", DEFAULT_SPAWN_COUNT),
            min_count=data.get("min_count", 0),
            lifetime=data.get("lifetime", 7200),
            restock=data.get("restock", 0),
            quantmin=data.get("quantmin", 30),
            quantmax=data.get("quantmax", 80),
            usage=data.get("usage", "Town"),
            value=data.get("value", "Tier12"),
            tier=data.get("tier", 1),
            locations=list(data.get("locations", [])),
            event_min=data.get("event_min", 1),
            event_max=data.get("event_max", 1),
        )


@dataclass
class InstanceSpawnLoadout:
    """Persisted spawn loadout state for one instance."""
    enabled: List[SpawnableEntry] = field(default_factory=list)
    random_remix_percentage: int = 25
    random_remix_count: int = DEFAULT_SPAWN_COUNT
    last_updated: Optional[str] = None

    def to_dict(self) -> Dict[str, Any]:
        return {
            "enabled": [e.to_dict() for e in self.enabled],
            "random_remix_percentage": self.random_remix_percentage,
            "random_remix_count": self.random_remix_count,
            "last_updated": self.last_updated,
        }

    @classmethod
    def from_dict(cls, data: Optional[Dict[str, Any]]) -> "InstanceSpawnLoadout":
        if data is None:
            return cls()
        return cls(
            enabled=[SpawnableEntry.from_dict(e) for e in data.get("enabled", [])],
            random_remix_percentage=data.get("random_remix_percentage", 25),
            random_remix_count=data.get("random_remix_count", DEFAULT_SPAWN_COUNT),
            last_updated=data.get("last_updated"),
        )


@dataclass
class ModSettingsOverride:
    """A per-instance override of a mod settings file."""
    source_path: Path
    relative_target: str
    content: str
    last_updated: Optional[str] = None

    def to_dict(self) -> Dict[str, Any]:
        return {
            "source_path": str(self.source_path),
            "relative_target": self.relative_target,
            "content": self.content,
            "last_updated": self.last_updated,
        }

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "ModSettingsOverride":
        return cls(
            source_path=Path(data.get("source_path", "")),
            relative_target=data.get("relative_target", ""),
            content=data.get("content", ""),
            last_updated=data.get("last_updated"),
        )


class PerInstanceConfigManager:
    """Read and write per-instance configuration.

    Layout under projects_root/instances/<instance_name>/:
        dcm_config/
            spawn_loadout.json
            mod_settings_overrides.json
    """

    def __init__(self, instance_root: Path):
        self.instance_root = Path(instance_root)
        self.config_dir = self.instance_root / "dcm_config"
        self.spawn_loadout_path = self.config_dir / "spawn_loadout.json"
        self.mod_settings_path = self.config_dir / "mod_settings_overrides.json"
        self.deployment_options_path = self.config_dir / "deployment_options.json"

    def _ensure_dir(self) -> None:
        self.config_dir.mkdir(parents=True, exist_ok=True)

    def load_spawn_loadout(self) -> InstanceSpawnLoadout:
        """Load the saved spawn loadout, or an empty one if none exists."""
        if not self.spawn_loadout_path.exists():
            return InstanceSpawnLoadout()
        try:
            data = json.loads(self.spawn_loadout_path.read_text(encoding="utf-8"))
            return InstanceSpawnLoadout.from_dict(data)
        except (json.JSONDecodeError, OSError):
            return InstanceSpawnLoadout()

    def save_spawn_loadout(self, loadout: InstanceSpawnLoadout) -> None:
        """Persist *loadout* to the instance's spawn_loadout.json file."""
        self._ensure_dir()
        loadout.last_updated = datetime.now(timezone.utc).isoformat()
        self.spawn_loadout_path.write_text(
            json.dumps(loadout.to_dict(), indent=2, ensure_ascii=False),
            encoding="utf-8",
        )

    def load_deployment_options(self) -> DeploymentOptions:
        """Load saved deployment options, or conservative defaults."""
        if not self.deployment_options_path.exists():
            return DeploymentOptions()
        try:
            data = json.loads(self.deployment_options_path.read_text(encoding="utf-8"))
            return DeploymentOptions.from_dict(data)
        except (json.JSONDecodeError, OSError):
            return DeploymentOptions()

    def save_deployment_options(self, options: DeploymentOptions) -> None:
        """Persist *options* to the instance's deployment_options.json file."""
        self._ensure_dir()
        self.deployment_options_path.write_text(
            json.dumps(options.to_dict(), indent=2, ensure_ascii=False),
            encoding="utf-8",
        )

    def load_mod_settings_overrides(self) -> Dict[str, ModSettingsOverride]:
        """Load saved mod-settings overrides keyed by source path."""
        if not self.mod_settings_path.exists():
            return {}
        try:
            data = json.loads(self.mod_settings_path.read_text(encoding="utf-8"))
            return {
                key: ModSettingsOverride.from_dict(value)
                for key, value in data.items()
            }
        except (json.JSONDecodeError, OSError):
            return {}

    def save_mod_settings_overrides(
        self, overrides: Dict[str, ModSettingsOverride]
    ) -> None:
        """Persist *overrides* to the instance's mod_settings_overrides.json file."""
        self._ensure_dir()
        now = datetime.now(timezone.utc).isoformat()
        for override in overrides.values():
            override.last_updated = now
        self.mod_settings_path.write_text(
            json.dumps(
                {key: value.to_dict() for key, value in overrides.items()},
                indent=2,
                ensure_ascii=False,
            ),
            encoding="utf-8",
        )

    def apply_spawn_loadout(
        self,
        mission_root: Path,
        workshop_dir: Optional[Path] = None,
    ) -> Tuple[bool, List[str]]:
        """Apply the saved spawn loadout to a mission folder.

        Returns (success, messages).
        """
        from dayzconfigmaster.config.mod_integration import ModIntegrationWorkflow

        loadout = self.load_spawn_loadout()
        if not loadout.enabled:
            return True, ["No spawn loadout configured for this instance."]

        workflow = ModIntegrationWorkflow(mission_root, workshop_dir)
        messages: List[str] = []
        ok = True

        for entry in loadout.enabled:
            is_vehicle = entry.category in ("vehicle", "air", "water")
            # Vehicles/air/water are spawned by events.xml and must never be
            # tagged with loot usage/value.  Pass None so the integration
            # workflow uses safe empty defaults for those categories.
            result = workflow.integrate_spawnable_mod(
                entry.name,
                spawn_count=entry.spawn_count,
                category=entry.category,
                usage=None if is_vehicle else entry.usage,
                value=None if is_vehicle else entry.value,
                locations=entry.locations,
                event_min=entry.event_min if is_vehicle else 1,
                event_max=entry.event_max if is_vehicle else 1,
                min_count=0 if is_vehicle else entry.min_count,
                lifetime=3888000 if is_vehicle else entry.lifetime,
                restock=1800 if is_vehicle else entry.restock,
                quantmin=-1 if is_vehicle else entry.quantmin,
                quantmax=-1 if is_vehicle else entry.quantmax,
            )
            if result.ok:
                messages.append(f"Enabled {entry.name} ({entry.category}) x{entry.spawn_count}")
            else:
                ok = False
                messages.append(f"Failed to enable {entry.name}: {result.actions[-1].error if result.actions else 'unknown'}")

        return ok, messages

    def apply_mod_settings_overrides(
        self,
        instance_root: Path,
        mission_root: Optional[Path] = None,
    ) -> Tuple[bool, List[str]]:
        """Copy saved mod-settings overrides into the instance.

        Expansion/VPP/ZomBerry and similar mods read settings from the server
        profile directory.  We write overrides there so each instance gets its
        own configuration.  Mission-scoped overrides (paths starting with
        ``mpmissions/``) are written into the instance's mission folder.
        """
        overrides = self.load_mod_settings_overrides()
        if not overrides:
            return True, ["No mod settings overrides for this instance."]

        messages: List[str] = []
        ok = True

        profile_dir = instance_root / "profiles"
        for key, override in overrides.items():
            rel = override.relative_target
            if rel.lower().startswith("mpmissions/"):
                if mission_root is None:
                    messages.append(
                        f"Skipped mission override {rel}: mission root not available"
                    )
                    continue
                # rel is mpmissions/<mission>/<rest>; mission_root is already
                # the specific mission folder, so drop both leading parts.
                rel_after_mission = Path(*Path(rel).parts[2:])
                target = mission_root / rel_after_mission
            else:
                target = profile_dir / rel

            try:
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_text(override.content, encoding="utf-8")
                messages.append(f"Applied override: {rel}")
            except OSError as exc:
                ok = False
                messages.append(f"Failed to write {rel}: {exc}")

        return ok, messages

    def export_mod_settings_override(
        self,
        source_path: Path,
        relative_target: str,
        content: str,
    ) -> None:
        """Store an edited mod-settings file as a per-instance override."""
        overrides = self.load_mod_settings_overrides()
        key = str(source_path)
        overrides[key] = ModSettingsOverride(
            source_path=source_path,
            relative_target=relative_target,
            content=content,
        )
        self.save_mod_settings_overrides(overrides)
