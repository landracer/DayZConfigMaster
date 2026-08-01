# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Deployment automation for DayZ server instances.

This module implements the proper DayZ dedicated-server workflow:

* Each instance gets its own directory containing:
    - serverDZ.cfg
    - profiles/  (isolated per instance)
    - mpmissions/ (mission storage)
    - keys/      (.bikey files from all loaded mods)
    - @Mod/      symlinks to workshop or local mod folders
    - DayZServer binary/symlink (optional, for self-contained instances)
* Mods are passed via the -mod=... command-line flag, never via serverDZ.cfg.
* .bikey files are copied (not symlinked) from each mod's keys/ folder into the
  instance keys/ folder so the server can verify client signatures.
* systemd service units can be generated for headless Linux hosts.

All operations are designed to run without elevated privileges. Directories are
created under the user's home folder and symlinks are relative where possible.
"""

from __future__ import annotations

import os
import shutil
import stat
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple


@dataclass
class DeploymentResult:
    """Result of a deployment operation."""
    success: bool
    messages: List[str] = field(default_factory=list)
    errors: List[str] = field(default_factory=list)
    instance_dir: Optional[Path] = None
    systemd_path: Optional[Path] = None
    start_script_path: Optional[Path] = None

    @property
    def ok(self) -> bool:
        return self.success and not self.errors


def _format_mod_paths(mod_paths: List[str]) -> str:
    """Return a semicolon-separated mod string suitable for -mod=."""
    normalized: List[str] = []
    for raw in mod_paths:
        if not raw:
            continue
        for part in raw.replace(",", ";").split(";"):
            part = part.strip()
            if not part:
                continue
            if Path(part).is_absolute() or part.startswith("steam://"):
                normalized.append(part)
            elif not part.startswith("@"):
                normalized.append("@" + part)
            else:
                normalized.append(part)
    return ";".join(normalized)


def _read_mod_display_name(folder: Path) -> Optional[str]:
    """Read the display name from a mod's meta.cpp or mod.cpp."""
    import re
    for filename in ("mod.cpp", "meta.cpp"):
        filepath = folder / filename
        if not filepath.exists():
            continue
        try:
            content = filepath.read_text(encoding="utf-8", errors="ignore")
            match = re.search(
                r'^\s*name\s*=\s*["\'](.+?)["\']\s*;',
                content,
                re.IGNORECASE | re.MULTILINE,
            )
            if match:
                value = match.group(1).strip()
                if value and not value.startswith("$"):
                    return value
        except (OSError, PermissionError):
            pass
    return None


def _read_mod_display_names(folder: Path) -> List[str]:
    """Return all non-empty display names found in a mod's meta.cpp/mod.cpp."""
    import re
    names: List[str] = []
    seen = set()
    for filename in ("mod.cpp", "meta.cpp"):
        filepath = folder / filename
        if not filepath.exists():
            continue
        try:
            content = filepath.read_text(encoding="utf-8", errors="ignore")
            for match in re.finditer(
                r'^\s*name\s*=\s*["\'](.+?)["\']\s*;',
                content,
                re.IGNORECASE | re.MULTILINE,
            ):
                value = match.group(1).strip()
                if value and not value.startswith("$"):
                    normalized = value.lower().lstrip("@")
                    if normalized not in seen:
                        seen.add(normalized)
                        names.append(value)
        except (OSError, PermissionError):
            pass
    return names


def _resolve_mod_source(
    mod_name: str,
    workshop_dir: Optional[Path] = None,
    local_mod_dirs: Optional[List[Path]] = None,
) -> Optional[Path]:
    """
    Find the actual mod folder on disk for a given mod token.

    Args:
        mod_name: Mod token such as '@CF' or 'CF' or an absolute path.
        workshop_dir: Root workshop content directory (e.g. .../workshop/content/221100).
        local_mod_dirs: Additional directories to search for @Mod folders.

    Returns:
        Path to the mod folder if found, otherwise None.
    """
    if not mod_name:
        return None

    token = mod_name.strip()
    if Path(token).is_absolute():
        path = Path(token)
        return path if path.exists() else None

    # Keep the original requested name for display-name matching.
    requested_name = token
    # Normalize to @Name for filesystem searches.
    if not token.startswith("@"):
        token = "@" + token
    name_only = token.lstrip("@")

    # Search local mod directories first (user-provided paths take precedence)
    for directory in local_mod_dirs or []:
        candidate = directory / token
        if candidate.exists() and candidate.is_dir():
            return candidate

    # Search workshop by folder name
    if workshop_dir and workshop_dir.exists():
        candidate = workshop_dir / token
        if candidate.exists() and candidate.is_dir():
            return candidate

        # Workshop items are stored by numeric ID. Try matching by @Name inside
        # the item folder, then by display name from meta.cpp/mod.cpp, then by
        # the item folder name itself.
        for item_dir in workshop_dir.iterdir():
            if not item_dir.is_dir():
                continue
            candidate = item_dir / token
            if candidate.exists() and candidate.is_dir():
                return candidate

            for display in _read_mod_display_names(item_dir):
                display_name_only = display.lstrip("@")
                if display_name_only.lower() == name_only.lower():
                    return item_dir
                if requested_name.lower() == display.lower():
                    return item_dir
                if requested_name.lower() == display_name_only.lower():
                    return item_dir

            # Some mods unpack directly into the workshop item folder
            if item_dir.name.lower() == name_only.lower():
                return item_dir

    return None


def _find_bikey_files(mod_folder: Path) -> List[Path]:
    """Return all .bikey files found inside a mod folder."""
    bikeys: List[Path] = []
    if not mod_folder.exists():
        return bikeys

    # DayZ mods ship keys in folders named keys/Keys/Key/key/etc.
    for child in mod_folder.iterdir():
        if child.is_dir() and child.name.lower() in {"keys", "key"}:
            for key_file in child.iterdir():
                if key_file.is_file() and key_file.suffix.lower() == ".bikey":
                    bikeys.append(key_file)

    # Some older mods place keys at the mod root
    for key_file in mod_folder.iterdir():
        if key_file.is_file() and key_file.suffix.lower() == ".bikey":
            bikeys.append(key_file)

    return bikeys


def _relative_symlink(target: Path, link_path: Path) -> None:
    """Create a relative symlink from link_path to target."""
    link_path.parent.mkdir(parents=True, exist_ok=True)
    if link_path.exists() or link_path.is_symlink():
        link_path.unlink()

    try:
        rel_target = os.path.relpath(target, link_path.parent)
    except ValueError:
        rel_target = target  # fallback to absolute on different drives (Windows)

    link_path.symlink_to(rel_target, target_is_directory=True)


def _create_mod_wrapper(instance_root: Path, link_name: str, mod_source: Path) -> Path:
    """Create an instance-local @Mod wrapper with case-normalized entries.

    DayZ on Linux is case-sensitive, but many workshop mods ship folders with
    capital letters (``Addons``, ``Keys``, ``Key``). The Windows client and
    server expect lowercase ``addons`` and ``keys``. This wrapper creates a
    directory in the instance root and symlinks every item from the source mod
    folder, normalizing the names of well-known DayZ folders to lowercase.

    Returns:
        Path to the wrapper directory (e.g. ``instance_root/@123456789``).
    """
    wrapper = instance_root / link_name
    if wrapper.is_symlink():
        wrapper.unlink()
    wrapper.mkdir(parents=True, exist_ok=True)

    # Map from any known source folder name (case-insensitive) to the
    # lowercase canonical name DayZ expects.
    canonical_names = {
        "addons": "addons",
        "keys": "keys",
        "key": "keys",
        "dta": "dta",
        "mpmissions": "mpmissions",
    }

    for item in mod_source.iterdir():
        target_name = canonical_names.get(item.name.lower(), item.name)

        link_path = wrapper / target_name
        if link_path.exists() or link_path.is_symlink():
            link_path.unlink()

        try:
            rel_target = os.path.relpath(item, wrapper)
        except ValueError:
            rel_target = item

        link_path.symlink_to(
            rel_target,
            target_is_directory=item.is_dir(),
        )

    return wrapper


def _copy_bikeys(
    mod_folder: Path,
    instance_keys_dir: Path,
    copied: Optional[set] = None,
) -> List[str]:
    """Copy .bikey files from mod_folder into instance_keys_dir."""
    messages: List[str] = []
    copied = copied or set()
    instance_keys_dir.mkdir(parents=True, exist_ok=True)

    for key_file in _find_bikey_files(mod_folder):
        dest = instance_keys_dir / key_file.name
        if dest.name in copied:
            continue
        shutil.copy2(key_file, dest)
        copied.add(dest.name)
        messages.append(f"Copied key: {key_file.name}")

    return messages


def _make_executable(path: Path) -> None:
    """Add executable permission to a file on POSIX systems."""
    if os.name != "nt":
        current = path.stat().st_mode
        path.chmod(current | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)


def deploy_instance(
    instance_name: str,
    base_server_dir: Path,
    instance_dir: Path,
    mod_paths: List[str],
    workshop_dir: Optional[Path] = None,
    local_mod_dirs: Optional[List[Path]] = None,
    cfg_content: Optional[str] = None,
    cfg_filename: str = "serverDZ.cfg",
    copy_base_server: bool = False,
    base_server_excludes: Optional[List[str]] = None,
    instance_id: int = 1,
) -> DeploymentResult:
    """
    Deploy a single DayZ server instance directory.

    Args:
        instance_name: Human-readable instance name.
        base_server_dir: Path to the base DayZ server installation.
        instance_dir: Target directory for this instance.
        mod_paths: List of mod tokens/paths to link into the instance.
        workshop_dir: Optional Steam workshop content directory.
        local_mod_dirs: Optional list of directories containing local @Mod folders.
        cfg_content: Optional serverDZ.cfg content to write.
        cfg_filename: Name of the config file to write.
        copy_base_server: If True, copy base server files instead of symlinking the binary.
        base_server_excludes: Patterns to exclude when copying base server files.
        instance_id: Numeric instance ID, used to create a unique binary copy.

    Returns:
        DeploymentResult with details of the operation.
    """
    result = DeploymentResult(success=True)
    result.instance_dir = instance_dir

    try:
        instance_dir.mkdir(parents=True, exist_ok=True)
        result.messages.append(f"Instance directory: {instance_dir}")

        # Standard subdirectories
        profiles_dir = instance_dir / "profiles"
        mpmissions_dir = instance_dir / "mpmissions"
        keys_dir = instance_dir / "keys"

        profiles_dir.mkdir(parents=True, exist_ok=True)
        mpmissions_dir.mkdir(parents=True, exist_ok=True)
        keys_dir.mkdir(parents=True, exist_ok=True)

        result.messages.append("Created profiles/, mpmissions/, keys/")

        # Write serverDZ.cfg
        if cfg_content is not None:
            cfg_path = instance_dir / cfg_filename
            cfg_path.write_text(cfg_content, encoding="utf-8")
            result.messages.append(f"Wrote {cfg_filename}")

        # Base server binary / directory
        if base_server_dir and base_server_dir.exists():
            if copy_base_server:
                _copy_base_server(base_server_dir, instance_dir, base_server_excludes)
                result.messages.append("Copied base server files into instance")
            else:
                # Copy the server executable with an instance-unique name.
                # Steam/BattlEye identify a server by its executable path; running
                # the same physical binary for multiple instances causes only the
                # first started instance to accept connections.
                # The first available normal binary is copied to
                # DayZServer_instance_<id> and the first debug binary to
                # DayZDiag_instance_<id> for a stable launch name.
                normal_copied = False
                debug_copied = False
                for exe_name in ("DayZServer", "DayZServer_x64", "DayZDiag", "DayZDiag_x64"):
                    src = base_server_dir / exe_name
                    if not src.exists() or not src.is_file():
                        continue
                    is_debug = "Diag" in exe_name
                    if is_debug and debug_copied:
                        continue
                    if not is_debug and normal_copied:
                        continue
                    dst_name = f"{'DayZDiag' if is_debug else 'DayZServer'}_instance_{instance_id}"
                    dst = instance_dir / dst_name
                    try:
                        shutil.copy2(src, dst)
                        result.messages.append(f"Copied {exe_name} -> {dst_name}")
                        if is_debug:
                            debug_copied = True
                        else:
                            normal_copied = True
                    except Exception as exc:
                        result.errors.append(f"Could not copy {exe_name}: {exc}")

                # Symlink required shared libraries / battleye / game data.
                # The executable itself is copied above with an instance-unique name.
                required_names = {
                    "dayz.gproj",
                    "dayzsetting.xml",
                    "steam_appid.txt",
                    "addons",
                    "dta",
                    "battleye",
                    "libsteam_api.so",
                    "steamclient.so",
                }
                for item in base_server_dir.iterdir():
                    if item.name in required_names:
                        dst = instance_dir / item.name
                        _relative_symlink(item, dst)
                        continue
                    if item.name.startswith("@"):
                        continue
                    if item.is_dir() and item.name in {"enoch", "chernarusplus", "sakhal"}:
                        dst = instance_dir / item.name
                        _relative_symlink(item, dst)
        else:
            result.errors.append(f"Base server directory not found: {base_server_dir}")

        # Mod wrappers and bikey copying
        linked_mods: List[str] = []
        copied_keys: set = set()
        for mod in mod_paths:
            mod_source = _resolve_mod_source(mod, workshop_dir, local_mod_dirs)
            if mod_source is None:
                result.errors.append(f"Mod not found: {mod}")
                continue

            link_name = "@" + mod_source.name.lstrip("@")
            mod_link = _create_mod_wrapper(instance_dir, link_name, mod_source)
            linked_mods.append(link_name)
            result.messages.append(f"Linked mod: {link_name} -> {mod_source}")

            key_messages = _copy_bikeys(mod_link, keys_dir, copied_keys)
            result.messages.extend(key_messages)

        if linked_mods:
            result.messages.append(f"Linked {len(linked_mods)} mod(s)")

    except Exception as exc:  # pragma: no cover - defensive
        result.success = False
        result.errors.append(f"Deployment failed: {exc}")

    return result


def _copy_base_server(
    base_dir: Path,
    instance_dir: Path,
    excludes: Optional[List[str]] = None,
) -> None:
    """Copy base server files into an instance directory."""
    excludes = set(excludes or [])
    for item in base_dir.iterdir():
        if item.name in excludes:
            continue
        dest = instance_dir / item.name
        if dest.exists() or dest.is_symlink():
            if dest.is_dir() and not dest.is_symlink():
                shutil.rmtree(dest)
            else:
                dest.unlink()
        if item.is_dir():
            shutil.copytree(item, dest, symlinks=True, ignore_dangling_symlinks=True)
        else:
            shutil.copy2(item, dest)


def generate_start_script(
    instance_dir: Path,
    exe_name: Optional[str] = None,
    config_file: str = "serverDZ.cfg",
    port: int = 2302,
    max_players: int = 60,
    mod_paths: Optional[List[str]] = None,
    extra_params: Optional[List[str]] = None,
    profiles_dir: str = "profiles",
    output_name: str = "start.sh",
    instance_id: int = 1,
) -> Path:
    """
    Generate a POSIX start script for an instance.

    Args:
        instance_dir: Instance directory.
        exe_name: Server executable name. If omitted, an instance-unique
            binary name is used (e.g. DayZServer_instance_2).
        config_file: Config file name relative to instance_dir.
        port: Server game port.
        max_players: Maximum players.
        mod_paths: List of mod tokens/paths.
        extra_params: Additional command-line parameters.
        profiles_dir: Profiles directory relative to instance_dir.
        output_name: Name of the generated script.
        instance_id: Numeric instance ID used to build the default exe_name.

    Returns:
        Path to the generated script.
    """
    script_path = instance_dir / output_name
    mod_string = _format_mod_paths(mod_paths or [])

    if exe_name is None:
        exe_name = f"DayZServer_instance_{instance_id}"

    params: List[str] = [
        f"-config={config_file}",
        f"-port={port}",
        f"-maxPlayers={max_players}",
        f"-profiles={profiles_dir}",
        "-dologs",
        "-adminLog",
        "-freezecheck",
    ]
    if mod_string:
        params.append(f'-mod="{mod_string}"')
    if extra_params:
        params.extend(extra_params)

    home_dir = instance_dir / ".dayzhome"
    config_dir = home_dir / ".config"
    data_dir = home_dir / ".local" / "share"
    runtime_dir = home_dir / ".runtime"
    tmp_dir = home_dir / ".tmp"

    lines = [
        "#!/usr/bin/env bash",
        "# DayZ server start script generated by DayzConfigMaster",
        f"# Instance: {instance_dir.name}",
        "set -euo pipefail",
        f'cd "$(dirname "$(readlink -f "$0")")"',
        "",
        "# Isolate Steam/IPC state so multiple instances on the same Linux",
        "# user do not collide. DayZ's steamclient.so uses singleton state in",
        "# $HOME; without isolation only the first-started server is connectable.",
        f'mkdir -p "{home_dir}" "{config_dir}" "{data_dir}" "{runtime_dir}" "{tmp_dir}"',
        f'export HOME="{home_dir}"',
        f'export XDG_CONFIG_HOME="{config_dir}"',
        f'export XDG_DATA_HOME="{data_dir}"',
        f'export XDG_RUNTIME_DIR="{runtime_dir}"',
        f'export TMPDIR="{tmp_dir}"',
        f'export STEAM_RUNTIME_DIR="{runtime_dir}/steam"',
        "",
        f'./{exe_name} {" ".join(params)} "$@"',
    ]

    script_path.write_text("\n".join(lines) + "\n", encoding="utf-8")
    _make_executable(script_path)
    return script_path


def generate_systemd_service(
    instance_name: str,
    instance_dir: Path,
    start_script: str = "start.sh",
    user: Optional[str] = None,
    description: Optional[str] = None,
    output_dir: Optional[Path] = None,
) -> Path:
    """
    Generate a systemd service unit file for an instance.

    Args:
        instance_name: Service/instance name (used in the filename).
        instance_dir: Instance directory.
        start_script: Start script name relative to instance_dir.
        user: User to run the service as (defaults to current user).
        description: Optional service description.
        output_dir: Directory to write the unit file to.

    Returns:
        Path to the generated unit file.
    """
    user = user or os.environ.get("USER", "dayz")
    description = description or f"DayZ server instance {instance_name}"
    output_dir = output_dir or instance_dir
    output_dir.mkdir(parents=True, exist_ok=True)

    service_name = f"dayz-{instance_name.lower().replace(' ', '-')}"
    unit_path = output_dir / f"{service_name}.service"

    content = f"""[Unit]
Description={description}
After=network.target

[Service]
Type=simple
User={user}
WorkingDirectory={instance_dir}
ExecStart={instance_dir / start_script}
Restart=on-failure
RestartSec=10

[Install]
WantedBy=multi-user.target
"""

    unit_path.write_text(content, encoding="utf-8")
    return unit_path


def deploy_all_instances(
    base_server_dir: Path,
    instances: List[Dict[str, Any]],
    projects_root: Path,
    workshop_dir: Optional[Path] = None,
    local_mod_dirs: Optional[List[Path]] = None,
    generate_scripts: bool = True,
    generate_systemd: bool = False,
    systemd_output_dir: Optional[Path] = None,
) -> Dict[str, DeploymentResult]:
    """
    Deploy multiple DayZ server instances.

    Args:
        base_server_dir: Path to the base DayZ server installation.
        instances: List of instance dicts. Each dict should contain:
            name, id, cfg_content, mod_paths, port, max_players, profile.
        projects_root: Root directory for all projects/instances.
        workshop_dir: Optional Steam workshop content directory.
        local_mod_dirs: Optional list of local mod directories.
        generate_scripts: If True, generate start.sh for each instance.
        generate_systemd: If True, generate systemd unit files.
        systemd_output_dir: Directory for systemd unit files.

    Returns:
        Mapping of instance name to DeploymentResult.
    """
    results: Dict[str, DeploymentResult] = {}
    instances_root = projects_root / "instances"
    instances_root.mkdir(parents=True, exist_ok=True)

    for instance in instances:
        name = str(instance.get("name", "instance"))
        instance_id = int(instance.get("id", 1) or 1)
        instance_dir = instances_root / f"{name}_{instance_id}"

        result = deploy_instance(
            instance_name=name,
            base_server_dir=base_server_dir,
            instance_dir=instance_dir,
            mod_paths=instance.get("mod_paths", []),
            workshop_dir=workshop_dir,
            local_mod_dirs=local_mod_dirs,
            cfg_content=instance.get("cfg_content"),
            instance_id=instance_id,
        )

        if generate_scripts and result.instance_dir:
            script_path = generate_start_script(
                instance_dir=result.instance_dir,
                port=int(instance.get("port", 2302) or 2302),
                max_players=int(instance.get("max_players", 60) or 60),
                mod_paths=instance.get("mod_paths", []),
                profiles_dir=str(instance.get("profile", "profiles")),
                instance_id=instance_id,
            )
            result.start_script_path = script_path
            result.messages.append(f"Generated start script: {script_path}")

        if generate_systemd and result.instance_dir:
            unit_path = generate_systemd_service(
                instance_name=name,
                instance_dir=result.instance_dir,
                output_dir=systemd_output_dir,
            )
            result.systemd_path = unit_path
            result.messages.append(f"Generated systemd unit: {unit_path}")

        results[name] = result

    return results
