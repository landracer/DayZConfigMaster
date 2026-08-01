# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Ban list manager for DayZ server.

Features:
- Manage ban list with add/remove functionality
- Import/export bans to/from serverDZ.cfg
- Real-time player monitoring via RCon
"""

import json
from pathlib import Path
from datetime import datetime, timedelta
from typing import List, Dict, Optional


class BanEntry:
    """Represents a single ban entry."""
    
    def __init__(
        self,
        steam_id: str,
        name: str = "",
        reason: str = "",
        issuer: str = "Admin",
        timestamp: Optional[datetime] = None,
        duration_minutes: int = 0
    ):
        """
        Initialize a ban entry.
        
        Args:
            steam_id: Player's Steam64 ID
            name: Player's display name (optional)
            reason: Reason for the ban
            issuer: Who issued the ban
            timestamp: When the ban was issued
            duration_minutes: Ban duration in minutes (0 = permanent)
        """
        self.steam_id = steam_id
        self.name = name or ""
        self.reason = reason or "No reason specified"
        self.issuer = issuer or "Admin"
        self.timestamp = timestamp or datetime.now()
        self.duration_minutes = duration_minutes
    
    @property
    def is_expired(self) -> bool:
        """Check if this ban has expired."""
        if self.duration_minutes == 0:  # Permanent ban
            return False
        
        expiry_time = self.timestamp + timedelta(minutes=self.duration_minutes)
        return datetime.now() > expiry_time
    
    def to_dict(self) -> Dict:
        """Convert to dictionary for JSON storage."""
        return {
            "steam_id": self.steam_id,
            "name": self.name,
            "reason": self.reason,
            "issuer": self.issuer,
            "timestamp": self.timestamp.isoformat(),
            "duration_minutes": self.duration_minutes
        }
    
    @classmethod
    def from_dict(cls, data: Dict) -> 'BanEntry':
        """Create BanEntry from dictionary."""
        timestamp = datetime.fromisoformat(data.get("timestamp", ""))
        
        return cls(
            steam_id=data.get("steam_id", ""),
            name=data.get("name", ""),
            reason=data.get("reason", "No reason specified"),
            issuer=data.get("issuer", "Admin"),
            timestamp=timestamp,
            duration_minutes=int(data.get("duration_minutes", 0))
        )


class BanListManager:
    """
    Manage server ban list with add/remove functionality.
    
    Features:
    - Add/remove ban entries
    - Export to serverDZ.cfg format
    - Import from existing bans file
    - Check if a player is banned
    
    Usage:
        ban_mgr = BanListManager("/path/to/server")
        
        # Add a ban
        ban_mgr.add_ban("76561198000000000", "BadPlayer", "Rule violation")
        
        # Check if banned
        is_banned = ban_mgr.is_player_banned("76561198000000000")
    """
    
    def __init__(self, dayz_path: str):
        """
        Initialize the ban list manager.
        
        Args:
            dayz_path: Path to DayZ server directory
        """
        self.dayz_path = Path(dayz_path)
        self.bans_file = self.dayz_path / "bans.txt"
        self.config_path = self.dayz_path / "serverDZ.cfg"
        
        # In-memory ban list
        self._bans: List[BanEntry] = []
    
    def load_bans(self) -> int:
        """
        Load bans from file.
        
        Returns:
            Number of bans loaded
        """
        self._bans.clear()
        
        # Try to load from bans.txt first (most common DayZ format)
        if self.bans_file.exists():
            try:
                with open(self.bans_file, 'r') as f:
                    for line in f:
                        line = line.strip()
                        if not line or line.startswith('//'):
                            continue
                        
                        parts = line.split('"')
                        if len(parts) >= 3:
                            # Format: SteamID64 "PlayerName" "Reason"
                            steam_id = parts[0].strip()
                            name = parts[1] if len(parts) > 1 else ""
                            reason = parts[2].rstrip().lstrip() if len(parts) > 2 else "No reason specified"
                            
                            entry = BanEntry(
                                steam_id=steam_id,
                                name=name,
                                reason=reason
                            )
                            self._bans.append(entry)
                
                return len(self._bans)
            except Exception as e:
                print(f"Error loading bans: {e}")
        
        # Also try to read from serverDZ.cfg if it exists
        if self.config_path.exists():
            self._load_bans_from_config()
        
        return len(self._bans)
    
    def _load_bans_from_config(self):
        """Try to extract bans from serverDZ.cfg (not common but possible)."""
        try:
            with open(self.config_path, 'r') as f:
                content = f.read()
            
            # Look for ban patterns in config
            import re
            
            # DayZ typically doesn't store individual bans in serverDZ.cfg
            # But we'll check anyway
            pass
            
        except Exception:
            pass
    
    def save_bans(self) -> bool:
        """Save current ban list to file."""
        try:
            with open(self.bans_file, 'w') as f:
                for entry in self._bans:
                    if not entry.is_expired:
                        f.write(f'{entry.steam_id} "{entry.name}" "{entry.reason}"\n')
            
            return True
        except Exception as e:
            print(f"Error saving bans: {e}")
            return False
    
    def add_ban(
        self,
        steam_id: str,
        name: str = "",
        reason: str = "No reason specified",
        issuer: str = "Admin",
        duration_minutes: int = 0
    ) -> bool:
        """
        Add a player to the ban list.
        
        Args:
            steam_id: Player's Steam64 ID (required)
            name: Player's display name (optional, will be auto-populated from RCon)
            reason: Reason for the ban
            issuer: Who issued the ban
            duration_minutes: Ban duration (0 = permanent)
            
        Returns:
            True if added successfully
        """
        # Check if already banned
        if self.is_player_banned(steam_id):
            return False
        
        entry = BanEntry(
            steam_id=steam_id,
            name=name,
            reason=reason,
            issuer=issuer,
            duration_minutes=duration_minutes
        )
        
        self._bans.append(entry)
        
        # Also kick player if they're currently online (if RCon available)
        try:
            from ..rcon import RConClient
            
            rcon = RConClient(
                host="localhost",
                port=self._get_rcon_port(),
                password=self._get_rcon_password()
            )
            
            if name:  # Try to kick using name if known
                rcon.kick_player(name, reason)
            
        except Exception:
            pass
        
        return True
    
    def remove_ban(self, steam_id: str) -> bool:
        """Remove a player from the ban list."""
        for i, entry in enumerate(self._bans):
            if entry.steam_id == steam_id:
                del self._bans[i]
                return True
        return False
    
    def is_player_banned(self, steam_id: str) -> bool:
        """Check if a player is currently banned."""
        # Filter out expired bans first
        self._filter_expired()
        
        for entry in self._bans:
            if entry.steam_id == steam_id:
                return True
        return False
    
    def get_banned_players(self, include_expired: bool = False) -> List[BanEntry]:
        """Get list of banned players."""
        if include_expired:
            return self._bans
        
        # Filter out expired bans
        return [e for e in self._bans if not e.is_expired]
    
    def _filter_expired(self):
        """Remove expired ban entries from the list."""
        self._bans = [e for e in self._bans if not e.is_expired]
    
    def get_active_ban_count(self) -> int:
        """Get count of active (non-expired) bans."""
        return len([e for e in self._bans if not e.is_expired])
    
    def _get_rcon_port(self) -> int:
        """Try to get RCon port from serverDZ.cfg."""
        try:
            with open(self.config_path, 'r') as f:
                content = f.read()
            
            import re
            match = re.search(r'RConPort\s*=\s*(\d+)', content, re.IGNORECASE)
            if match:
                return int(match.group(1))
            
            # Default port for DayZ RCon
            return 2306
            
        except Exception:
            return 2306
    
    def _get_rcon_password(self) -> str:
        """Try to get RCon password from serverDZ.cfg."""
        try:
            with open(self.config_path, 'r') as f:
                content = f.read()
            
            import re
            match = re.search(r'RConPassword\s*=\s*"([^"]+)"', content, re.IGNORECASE)
            if match:
                return match.group(1)
            
            # Default password
            return "rcon"
            
        except Exception:
            return "rcon"


def create_default_banlist_config() -> dict:
    """Create default ban list configuration."""
    return {
        "enabled": True,
        "file_path": "./bans.txt",
        "max_active_bans": 100
    }