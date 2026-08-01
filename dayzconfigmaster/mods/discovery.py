# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Mod discovery module for DayZ mod management

"""
Mod discovery module for scanning and identifying DayZ mods.

Scans configured roots to find available mods and their metadata.
"""

import os
from pathlib import Path
from typing import Dict, List, Optional, Any


class ModInfo:
    """Information about a discovered mod."""
    
    def __init__(
        self,
        name: str,
        path: str,
        enabled: bool = False,
        side: str = "both",  # both|server|client
        missing: bool = False,
        version: Optional[str] = None,
        author: Optional[str] = None,
        description: Optional[str] = None
    ):
        self.name = name
        self.path = path
        self.enabled = enabled
        self.side = side
        self.missing = missing
        self.version = version
        self.author = author
        self.description = description
    
    def to_dict(self) -> Dict[str, Any]:
        """Convert to dictionary for JSON serialization."""
        return {
            "name": self.name,
            "path": self.path,
            "enabled": self.enabled,
            "side": self.side,
            "missing": self.missing,
            "version": self.version,
            "author": self.author,
            "description": self.description
        }


class ModDiscovery:
    """Discovers DayZ mods in configured paths."""
    
    # Common mod metadata files
    METADATA_FILES = ["mod.cpp", "meta.cpp", "config.cpp"]
    
    def __init__(self, scan_roots: List[str]):
        """
        Initialize with paths to scan for mods.
        
        Args:
            scan_roots: List of directory paths to scan
        """
        self.scan_roots = [Path(root) for root in scan_roots]
    
    def discover(self) -> List[ModInfo]:
        """Scan configured roots and return list of discovered mods."""
        found_mods = []
        
        for root in self.scan_roots:
            if not root.exists():
                continue
            
            # Scan immediate subdirectories
            try:
                for subdir in root.iterdir():
                    if subdir.is_dir() and self._is_mod(subdir):
                        mod_info = self._read_mod_metadata(subdir)
                        found_mods.append(mod_info)
            except (OSError, PermissionError) as e:
                print(f"Error scanning {root}: {e}")
        
        return found_mods
    
    def _is_mod(self, path: Path) -> bool:
        """Check if a directory looks like a DayZ mod."""
        # Check for common mod structure
        addons_dir = path / "addons"
        
        if addons_dir.exists() and addons_dir.is_dir():
            return True
        
        # Or check for direct config.cpp in root
        config_cpp = path / "config.cpp"
        if config_cpp.exists():
            return True
        
        return False
    
    def _read_mod_metadata(self, path: Path) -> ModInfo:
        """Read mod metadata from files."""
        name = path.name  # Default to folder name
        version = None
        author = None
        description = None
        
        # Try to read mod.cpp or meta.cpp for metadata
        for metadata_file in self.METADATA_FILES:
            filepath = path / metadata_file
            if filepath.exists():
                try:
                    content = filepath.read_text()
                    
                    # Parse name from file (simple pattern matching)
                    import re
                    
                    # Look for name field
                    name_match = re.search(r'name\s*=\s*"([^"]+)"', content, re.IGNORECASE)
                    if name_match:
                        name = name_match.group(1).strip()
                    
                    # Look for version
                    version_match = re.search(r'version\s*=\s*"([^"]+)"', content, re.IGNORECASE)
                    if version_match:
                        version = version_match.group(1).strip()
                    
                    # Look for author
                    author_match = re.search(r'author\s*=\s*"([^"]+)"', content, re.IGNORECASE)
                    if author_match:
                        author = author_match.group(1).strip()
                    
                    # Look for description
                    desc_match = re.search(r'description\s*=\s*"([^"]+)"', content, re.IGNORECASE)
                    if desc_match:
                        description = desc_match.group(1).strip()
                    
                    break  # Found a metadata file, stop searching
                    
                except Exception as e:
                    print(f"Error reading {filepath}: {e}")
        
        return ModInfo(
            name=name,
            path=str(path),
            version=version,
            author=author,
            description=description
        )
    
    def merge_with_config(
        self,
        discovered: List[ModInfo],
        configured: Dict[str, Dict[str, Any]]
    ) -> List[ModInfo]:
        """
        Merge discovered mods with configuration.
        
        Args:
            discovered: List of currently discovered mods
            configured: Configured mod state (enabled/side from previous session)
        
        Returns:
            Merged list with configured state preserved
        """
        # Build config lookup
        config_lookup = {}
        for path, state in configured.items():
            config_lookup[path] = state
        
        result = []
        
        # Process discovered mods
        for mod in discovered:
            if mod.path in config_lookup:
                # Preserve configured state
                state = config_lookup[mod.path]
                mod.enabled = state.get("enabled", False)
                mod.side = state.get("side", "both")
            
            result.append(mod)
        
        return result


class ModLoader:
    """
    Manages enabled mod list for server launch.
    
    Handles:
    - Loading/saving mod configuration
    - Side assignment (server/client/both)
    - Load order preservation
    """
    
    def __init__(self, projects_root: str):
        self.projects_root = Path(projects_root)
        self.mods_dir = self.projects_root / "mods"
        self.config_path = self.projects_root / ".dzl" / "mod_config.json"
        
        # Ensure config directory exists
        self.config_path.parent.mkdir(parents=True, exist_ok=True)
        
        self.enabled_mods: List[Dict[str, Any]] = []
        self._load()
    
    def _load(self):
        """Load saved mod configuration."""
        if not self.config_path.exists():
            return
        
        try:
            with open(self.config_path, 'r') as f:
                data = json.load(f)
            
            self.enabled_mods = data.get("enabled_mods", [])
        except Exception as e:
            print(f"Error loading mod config: {e}")
    
    def save(self):
        """Save current configuration."""
        try:
            import datetime
            import json
            
            data = {
                "enabled_mods": self.enabled_mods,
                "last_save": str(datetime.now())
            }
            
            with open(self.config_path, 'w') as f:
                json.dump(data, f, indent=2)
        
        except Exception as e:
            print(f"Error saving mod config: {e}")
    
    def enable_mod(self, path: str, side: str = "both") -> bool:
        """Enable a mod with specified side."""
        # Check if already in list
        for mod in self.enabled_mods:
            if mod["path"] == path:
                mod["enabled"] = True
                mod["side"] = side
                return True
        
        # Add new entry
        self.enabled_mods.append({
            "path": path,
            "enabled": True,
            "side": side
        })
        
        return True
    
    def disable_mod(self, path: str) -> bool:
        """Disable a mod."""
        for mod in self.enabled_mods:
            if mod["path"] == path:
                mod["enabled"] = False
                return True
        
        return False
    
    def get_enabled_list(self) -> List[Dict[str, Any]]:
        """Get list of enabled mods (sorted by side and path)."""
        return [m for m in self.enabled_mods if m.get("enabled", False)]
    
    def get_server_mods(self) -> List[str]:
        """Get server-side mod paths."""
        return [
            m["path"] for m in self.enabled_mods
            if m.get("side") in ("both", "server")
        ]
    
    def get_client_mods(self) -> List[str]:
        """Get client-side mod paths."""
        return [
            m["path"] for m in self.enabled_mods
            if m.get("side") in ("both", "client")
        ]


class ModDeployer:
    """
    Deploys built PBOs to work drive or server profile.
    
    Handles:
    - Deploying PBOs to DayZ work drive (P:)
    - Copying to server profiles directory
    - Setting up required folder structure
    """
    
    def __init__(self, dayz_path: str, projects_root: str):
        self.dayz_path = Path(dayz_path)
        self.projects_root = Path(projects_root)
        
        # Work drive paths (will be mounted on Windows)
        self.work_drive_source = self.projects_root / "workdrive"
    
    def deploy_pbo(
        self,
        pbo_file: str,
        server_name: Optional[str] = None
    ) -> tuple:
        """
        Deploy a PBO to the appropriate location.
        
        Args:
            pbo_file: Path to built PBO file
            server_name: Server instance name (if not provided, deploys to work drive)
        
        Returns:
            Tuple of (success, message)
        """
        try:
            pbo_path = Path(pbo_file)
            
            if not pbo_path.exists():
                return False, f"PBO file not found: {pbo_file}"
            
            # Determine deployment location
            if server_name:
                # Deploy to server profiles
                profile_dir = self.dayz_path / "profiles" / server_name
                addons_dir = profile_dir / "Addons"
                
                # Create directory structure
                addons_dir.mkdir(parents=True, exist_ok=True)
                
                # Copy PBO to server profile
                dest_pbo = addons_dir / pbo_path.name
                import shutil
                shutil.copy2(str(pbo_path), str(dest_pbo))
                
                return True, f"PBO deployed to server profile"
            
            else:
                # Deploy to work drive (P:)
                # On Windows: subst P: <work_drive_source>
                # On Linux/macOS: symlink
                
                mods_dir = self.work_drive_source / "Mods"
                mods_dir.mkdir(parents=True, exist_ok=True)
                
                dest_pbo = mods_dir / pbo_path.name
                import shutil
                shutil.copy2(str(pbo_path), str(dest_pbo))
                
                return True, f"PBO deployed to work drive"
        
        except Exception as e:
            return False, f"Deployment error: {str(e)}"
    
    def setup_work_drive(self) -> tuple:
        """
        Set up the work drive mount/symlink.
        
        Returns:
            Tuple of (success, message)
        """
        try:
            self.work_drive_source.mkdir(parents=True, exist_ok=True)
            
            if os.name == 'nt':
                # Windows: Use subst command
                import subprocess
                cmd = ["subst", "P:", str(self.work_drive_source)]
                result = subprocess.run(cmd, capture_output=True)
                
                if result.returncode == 0:
                    return True, "Work drive mounted (P:)"
                else:
                    return False, f"Failed to mount work drive: {result.stderr.decode()}"
            
            else:
                # Linux/macOS: Use symlink
                work_drive = Path("/p")  # Standard Unix mount point
                
                if not work_drive.exists():
                    work_drive.mkdir(parents=True)
                
                link_path = work_drive / "mods"
                if link_path.exists():
                    link_path.unlink()
                
                link_path.symlink_to(self.work_drive_source / "Mods")
                
                return True, "Work drive symlink created (/p/mods)"
        
        except Exception as e:
            return False, f"Work drive setup error: {str(e)}"