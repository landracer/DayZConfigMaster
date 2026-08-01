# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""VPPAdminTools integration.

DayZ on Linux does not expose a reliable vanilla/BattlEye TCP RCon port. This
module works around that by reading/writing VPPAdminTools' own files in the
server profiles directory.

- Bans are stored in ``profiles/VPPAdminTools/BanList.json``.
- Connected players are parsed from the newest VPPAdminTools session log, which
  emits ``Player "<name>" (steamId=<id>) connected to server!`` lines.
"""

import json
import re
from dataclasses import dataclass
from datetime import datetime
from pathlib import Path
from typing import Dict, List, Optional, Set, Tuple


@dataclass
class VppPlayer:
    """A player seen in VPP logs."""
    name: str
    steam_id: str
    guid: str = ""
    connected_at: Optional[datetime] = None


@dataclass
class VppBan:
    """A single VPP ban entry."""
    player_name: str
    steam_id: str
    guid: str
    reason: str
    issued_by: str
    permanent: bool
    expiration_date: Dict[str, int]


class VppAdminTools:
    """Interface to VPPAdminTools data for a single DayZ instance."""

    VPP_DIR = "VPPAdminTools"
    BAN_FILE = "BanList.json"
    PERMS_DIR = "Permissions"
    SUPER_ADMINS_FILE = "SuperAdmins/SuperAdmins.txt"
    LOG_DIR = "Logging"

    _CONNECT_RE = re.compile(
        r'Player\s+"([^"]+)"\s+\(steamId=(\d+)\)\s+connected\s+to\s+server!',
        re.IGNORECASE,
    )
    _DISCONNECT_RE = re.compile(
        r'Player\s+"([^"]+)"\s+\(steamId=(\d+)\)\s+disconnected\s+from\s+server\.',
        re.IGNORECASE,
    )

    def __init__(self, profiles_path: Path):
        self.profiles_path = Path(profiles_path)
        self.vpp_path = self.profiles_path / self.VPP_DIR
        self.ban_path = self.vpp_path / self.BAN_FILE

    @classmethod
    def from_instance(cls, dayz_server_path: Path, instance_root: Path, profile: str) -> "VppAdminTools":
        """Build a VPP interface from instance configuration."""
        profile_path = Path(profile) if profile else Path("profiles")
        if not profile_path.is_absolute():
            profile_path = instance_root / profile_path
        return cls(profile_path)

    def _load_json(self, path: Path) -> dict:
        if not path.exists():
            return {}
        try:
            return json.loads(path.read_text(encoding="utf-8"))
        except Exception:
            return {}

    def _save_json(self, path: Path, data: dict) -> None:
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(
            json.dumps(data, indent=4, ensure_ascii=False) + "\n",
            encoding="utf-8",
        )

    def is_available(self) -> bool:
        """Return True if VPPAdminTools appears configured for this instance."""
        return self.vpp_path.is_dir() and self.ban_path.exists()

    def _newest_log(self) -> Optional[Path]:
        """Return the newest VPP session log file."""
        log_dir = self.vpp_path / self.LOG_DIR
        if not log_dir.is_dir():
            return None
        candidates = [
            p for p in log_dir.iterdir()
            if p.is_file() and p.suffix.lower() == ".txt"
        ]
        if not candidates:
            return None
        return max(candidates, key=lambda p: p.stat().st_mtime)

    def get_connected_players(self) -> List[VppPlayer]:
        """Parse VPP logs and return currently connected players.

        The algorithm records every ``connected`` event and removes entries when
        a matching ``disconnected`` event is seen later in the log.
        """
        log_path = self._newest_log()
        if log_path is None:
            return []

        try:
            lines = log_path.read_text(
                encoding="utf-8", errors="ignore"
            ).splitlines()
        except Exception:
            return []

        connected: Dict[Tuple[str, str], Tuple[str, datetime]] = {}

        for line in lines:
            ts_match = re.match(r"(\d{1,2}):(\d{2}):(\d{2})\s+\|\s+", line)
            ts = None
            if ts_match:
                ts = datetime(
                    1900, 1, 1,
                    int(ts_match.group(1)),
                    int(ts_match.group(2)),
                    int(ts_match.group(3)),
                )
                line = line[ts_match.end():]

            conn = self._CONNECT_RE.search(line)
            if conn:
                name, steam_id = conn.group(1), conn.group(2)
                connected[(name.lower(), steam_id)] = (name, ts)
                continue

            disc = self._DISCONNECT_RE.search(line)
            if disc:
                name, steam_id = disc.group(1), disc.group(2)
                connected.pop((name.lower(), steam_id), None)
                continue

        return [
            VppPlayer(name=display_name, steam_id=steam_id, connected_at=ts)
            for (_key_name, steam_id), (display_name, ts) in connected.items()
        ]

    def get_bans(self) -> List[VppBan]:
        """Return the current VPP ban list."""
        data = self._load_json(self.ban_path)
        entries = data.get("m_BanList", []) if isinstance(data, dict) else []
        bans: List[VppBan] = []
        for entry in entries:
            exp = entry.get("expirationDate", {}) or {}
            bans.append(
                VppBan(
                    player_name=entry.get("playerName", ""),
                    steam_id=entry.get("Steam64Id", ""),
                    guid=entry.get("GUID", ""),
                    reason=entry.get("banReason", ""),
                    issued_by=entry.get("issuedBy", ""),
                    permanent=bool(exp.get("Permanent", 1)),
                    expiration_date=dict(exp),
                )
            )
        return bans

    def _default_expiration(self, permanent: bool = True) -> Dict[str, int]:
        return {
            "Hour": 0,
            "Minute": 0,
            "Year": 0,
            "Month": 0,
            "Day": 0,
            "Permanent": 1 if permanent else 0,
        }

    def add_ban(
        self,
        steam_id: str,
        name: str = "",
        guid: str = "",
        reason: str = "",
        issued_by: str = "DayzConfigMaster",
        permanent: bool = True,
    ) -> None:
        """Add a ban to VPPAdminTools' BanList.json."""
        data = self._load_json(self.ban_path)
        if not isinstance(data, dict):
            data = {"m_BanList": []}
        entries: List[dict] = data.setdefault("m_BanList", [])

        entries[:] = [
            e for e in entries
            if str(e.get("Steam64Id", "")).strip() != steam_id.strip()
        ]

        entries.append(
            {
                "playerName": name or "Unknown",
                "Steam64Id": steam_id,
                "GUID": guid or "0",
                "banReason": reason or "Banned via DayzConfigMaster",
                "issuedBy": issued_by,
                "expirationDate": self._default_expiration(permanent),
            }
        )
        self._save_json(self.ban_path, data)

    def remove_ban(self, steam_id: str) -> bool:
        """Remove a ban by Steam64 ID."""
        data = self._load_json(self.ban_path)
        if not isinstance(data, dict):
            return False
        entries: List[dict] = data.setdefault("m_BanList", [])
        before = len(entries)
        entries[:] = [
            e for e in entries
            if str(e.get("Steam64Id", "")).strip() != steam_id.strip()
        ]
        if len(entries) < before:
            self._save_json(self.ban_path, data)
            return True
        return False

    def ensure_super_admin(self, steam_id: str) -> bool:
        """Add a Steam64 to VPP SuperAdmins if not already present."""
        sa_path = self.vpp_path / self.PERMS_DIR / self.SUPER_ADMINS_FILE
        existing: Set[str] = set()
        if sa_path.exists():
            for line in sa_path.read_text(
                encoding="utf-8", errors="ignore"
            ).splitlines():
                line = line.strip()
                if line and not line.startswith("/"):
                    existing.add(line)
        if steam_id in existing:
            return False
        sa_path.parent.mkdir(parents=True, exist_ok=True)
        with sa_path.open("a", encoding="utf-8") as fh:
            if existing:
                fh.write("\n")
            fh.write(f"{steam_id}\n")
        return True
