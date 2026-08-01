# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Headless server startup diagnostics.

These helpers were extracted from :class:`DayzConfigMasterApp` so they can be
unit-tested and reused without a Tkinter display.
"""

import glob
import os
import re
import socket
from pathlib import Path
from typing import Dict, List, Optional


SCRIPT_ERROR_PATTERNS = [
    r"SCRIPT\s+\(E\): Can't compile .*? script module!",
    r"SCRIPT\s+\(E\): Failed to load mission scripts!",
    r"Unknown type '([^']+)'",
    r"Mission script has no main function",
]


def is_server_port_bound(port: int) -> bool:
    """Return True if *port* is already listening on any interface."""
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            sock.settimeout(0.5)
            sock.bind(("0.0.0.0", port))
            # If bind succeeds, the port is NOT in use.
            return False
    except OSError:
        return True
    except Exception:
        return False


def scan_server_log_for_errors(instance_root: Path) -> List[str]:
    """Scan the latest server RPT/log for critical startup errors."""
    errors: List[str] = []
    log_dir = instance_root / "profiles"
    if not log_dir.exists():
        log_dir = instance_root
    try:
        log_files = sorted(
            glob.glob(str(log_dir / "DayZServer_*.RPT")) +
            glob.glob(str(log_dir / "*.RPT")),
            key=os.path.getmtime,
            reverse=True,
        )
    except Exception:
        log_files = []

    if not log_files:
        for fallback in ["nohup.out", "server_console.log"]:
            fb_path = instance_root / fallback
            if fb_path.exists():
                log_files.append(str(fb_path))
                break
        if not log_files:
            return errors

    try:
        text = Path(log_files[0]).read_text(errors="ignore")
    except Exception:
        return errors

    for pattern in SCRIPT_ERROR_PATTERNS:
        for match in re.finditer(pattern, text):
            line = match.group(0).strip()
            if line not in errors:
                errors.append(line)
            if len(errors) >= 5:
                break
        if errors:
            break
    return errors


def detect_mod_version_mismatches(instance_root: Path) -> List[str]:
    """Warn when known mod families have mismatched versions."""
    warnings: List[str] = []
    versions: Dict[str, Dict[str, str]] = {}
    version_pattern = re.compile(r'^version\s*=\s*"([^"]+)"', re.IGNORECASE)
    name_pattern = re.compile(r'^name\s*=\s*"([^"]+)"', re.IGNORECASE)

    for wrapper in sorted(instance_root.glob("@*")):
        if not wrapper.is_dir():
            continue
        mod_id = wrapper.name.lstrip("@")
        meta_files = list(wrapper.glob("meta.cpp")) + list(wrapper.glob("mod.cpp"))
        version = ""
        for meta in meta_files:
            try:
                for line in meta.read_text(errors="ignore").splitlines():
                    m = version_pattern.match(line.strip())
                    if m:
                        version = m.group(1)
                        break
            except Exception:
                continue
            if version:
                break

        if not version:
            continue

        name = ""
        for meta in meta_files:
            try:
                for line in meta.read_text(errors="ignore").splitlines():
                    m = name_pattern.match(line.strip())
                    if m:
                        name = m.group(1)
                        break
            except Exception:
                continue
            if name:
                break

        lower_name = name.lower()
        family: Optional[str] = None
        if "dayz-expansion" in lower_name:
            family = "DayZ Expansion"
        elif "expansion" in lower_name:
            family = "DayZ Expansion"

        if family:
            versions.setdefault(family, {})[mod_id] = version

    for family, mod_versions in versions.items():
        if len(mod_versions) < 2:
            continue
        unique = set(mod_versions.values())
        if len(unique) > 1:
            details = ", ".join(
                f"@{mid}={v}" for mid, v in sorted(mod_versions.items())
            )
            warnings.append(
                f"{family} mods have mismatched versions: {details}. "
                "Update all of them to the same version in Steam Workshop."
            )
    return warnings
