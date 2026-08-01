# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Backup manager for DayZ server backups.

Features:
- Create and manage server backups
- Configurable retention policies
- Automatic cleanup of old backups
"""

import os
import shutil
from datetime import datetime, timedelta
from pathlib import Path
from typing import List, Optional, Tuple, Dict
from pathlib import Path


class InstanceStorageInfo:
    """Represents storage_1 folder information for an instance."""
    
    def __init__(self, 
                 instance_id: str,
                 instance_path: Path,
                 map_name: str,
                 mpmissions_path: Path,
                 storage_path: Path):
        self.instance_id = instance_id
        self.instance_path = instance_path
        self.map_name = map_name
        self.mpmissions_path = mpmissions_path
        self.storage_path = storage_path
    
    def to_dict(self) -> Dict[str, str]:
        """Convert to dictionary for JSON serialization."""
        return {
            "instance_id": self.instance_id,
            "instance_path": str(self.instance_path),
            "map_name": self.map_name,
            "mpmissions_path": str(self.mpmissions_path),
            "storage_path": str(self.storage_path)
        }


class BackupManager:
    """
    Manage server backups with scheduled creation and retention policies.
    
    Integrates with EventScheduler to create automated backup events.
    """
    
    def __init__(self, projects_root: str):
        """
        Initialize the backup manager.
        
        Args:
            projects_root: Root directory containing the DayZ installation
        """
        self.projects_root = Path(projects_root)
        self.backup_dir = self.projects_root / "backups"
        self.backup_retention_days = 7
        
        # Create backup directory if it doesn't exist
        self.backup_dir.mkdir(parents=True, exist_ok=True)
    
    def create_backup(self, name: Optional[str] = None) -> Tuple[bool, str]:
        """
        Create a server backup.
        
        Args:
            name: Optional backup name (defaults to timestamp)
            
        Returns:
            Tuple of (success, message/path)
        """
        if not self.backup_dir.exists():
            self.backup_dir.mkdir(parents=True)
        
        # Generate backup name
        if name is None:
            timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
            name = f"backup_{timestamp}"
        
        backup_path = self.backup_dir / name
        
        try:
            # Find DayZ server directory (usually parent of profiles)
            dayz_server = self.projects_root / "DayZServer"
            
            if not dayz_server.exists():
                return False, "DayZServer directory not found"
            
            # Backup the entire server directory
            shutil.copytree(
                str(dayz_server),
                str(backup_path),
                symlinks=True,
                ignore=shutil.ignore_patterns('*.log', '*.rpt')
            )
            
            return True, str(backup_path)
            
        except Exception as e:
            return False, f"Backup failed: {e}"
    
    def cleanup_old_backups(self, max_age_days: Optional[int] = None):
        """
        Remove backups older than specified days.
        
        Args:
            max_age_days: Maximum age in days (default: use instance setting)
        """
        if max_age_days is None:
            max_age_days = self.backup_retention_days
        
        cutoff_date = datetime.now() - timedelta(days=max_age_days)
        
        for backup_path in self.backup_dir.iterdir():
            if backup_path.is_dir():
                try:
                    # Check modification time
                    mtime = datetime.fromtimestamp(backup_path.stat().st_mtime)
                    
                    if mtime < cutoff_date:
                        shutil.rmtree(backup_path)
                        print(f"Removed old backup: {backup_path.name}")
                        
                except Exception as e:
                    print(f"Error checking backup {backup_path}: {e}")
    
    def list_backups(self) -> List[Path]:
        """List all available backups."""
        if not self.backup_dir.exists():
            return []
        
        return [p for p in self.backup_dir.iterdir() if p.is_dir()]
    
    def get_backup_size(self, backup_name: str) -> int:
        """Get the size of a specific backup in bytes."""
        backup_path = self.backup_dir / backup_name
        
        if not backup_path.exists():
            return 0
        
        total = 0
        
        for path in backup_path.rglob('*'):
            if path.is_file():
                try:
                    total += path.stat().st_size
                except OSError:
                    continue
        
        return total
    
    def restore_backup(self, backup_name: str) -> Tuple[bool, str]:
        """
        Restore a server from backup.
        
        Args:
            backup_name: Name of the backup to restore
            
        Returns:
            Tuple of (success, message)
        """
        backup_path = self.backup_dir / backup_name
        
        if not backup_path.exists():
            return False, f"Backup not found: {backup_name}"
        
        dayz_server = self.projects_root / "DayZServer"
        
        try:
            # Remove current installation
            if dayz_server.exists():
                shutil.rmtree(dayz_server)
            
            # Restore from backup
            shutil.copytree(str(backup_path), str(dayz_server))
            
            return True, f"Restored from backup: {backup_name}"
            
        except Exception as e:
            return False, f"Restore failed: {e}"


def find_instance_storage_dirs(projects_root: str) -> List[InstanceStorageInfo]:
    """
    Scan the instances directory to find all storage_1 folders for each instance/map combination.
    
    Args:
        projects_root: Root directory containing DayZProjects/instances
        
    Returns:
        List of InstanceStorageInfo objects
    """
    instances_dir = Path(projects_root) / "instances"
    results: List[InstanceStorageInfo] = []
    
    if not instances_dir.exists():
        return results
    
    # Scan each instance folder
    for instance_path in sorted(instances_dir.iterdir()):
        if not instance_path.is_dir():
            continue
        
        instance_id = instance_path.name
        if not instance_id.startswith("server"):
            continue
        
        mpmissions_path = instance_path / "mpmissions"
        if not mpmissions_path.exists():
            continue
        
        # Scan mpmissions for map folders with storage_1
        for mp_folder in sorted(mpmissions_path.iterdir()):
            if not mp_folder.is_dir():
                continue
            
            storage_path = mp_folder / "storage_1"
            if storage_path.exists() and storage_path.is_dir():
                # Extract map name from folder (e.g., dayzOffline.enoch -> enoch)
                map_name = ""
                if mp_folder.name.startswith("dayzOffline."):
                    map_name = mp_folder.name.replace("dayzOffline.", "")
                elif mp_folder.name.startswith("dayz."):
                    map_name = mp_folder.name.replace("dayz.", "")
                
                results.append(InstanceStorageInfo(
                    instance_id=instance_id,
                    instance_path=instance_path,
                    map_name=map_name,
                    mpmissions_path=mp_folder,
                    storage_path=storage_path
                ))
    
    return results


def backup_instance_storage(
    projects_root: str, 
    instance_info: InstanceStorageInfo,
    backup_dir: Optional[Path] = None,
    name: Optional[str] = None
) -> Tuple[bool, str]:
    """
    Backup the storage_1 folder for a specific instance/map combination.
    
    Args:
        projects_root: Root directory containing DayZProjects/instances
        instance_info: InstanceStorageInfo object with path information
        backup_dir: Optional custom backup directory (defaults to projects_root/backups)
        name: Optional backup name
        
    Returns:
        Tuple of (success, message/path)
    """
    if backup_dir is None:
        backup_dir = Path(projects_root) / "backups"
    
    backup_dir.mkdir(parents=True, exist_ok=True)
    
    # Generate backup name
    if name is None:
        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        map_part = instance_info.map_name.replace(".", "_") if instance_info.map_name else "unknown"
        name = f"storage_{instance_info.instance_id}_{map_part}_{timestamp}"
    
    backup_path = backup_dir / name
    
    try:
        # Backup the storage_1 folder
        shutil.copytree(
            str(instance_info.storage_path),
            str(backup_path),
            symlinks=True,
            ignore=shutil.ignore_patterns('*.log', '*.rpt', '*.tmp')
        )
        
        return True, str(backup_path)
        
    except Exception as e:
        return False, f"Storage backup failed: {e}"


def restore_instance_storage(
    projects_root: str,
    instance_info: InstanceStorageInfo,
    backup_name: str,
    backup_dir: Optional[Path] = None
) -> Tuple[bool, str]:
    """
    Restore storage_1 folder from backup.
    
    Args:
        projects_root: Root directory containing DayZProjects/instances
        instance_info: InstanceStorageInfo object with path information
        backup_name: Name of the backup to restore
        backup_dir: Optional custom backup directory
        
    Returns:
        Tuple of (success, message)
    """
    if backup_dir is None:
        backup_dir = Path(projects_root) / "backups"
    
    backup_path = backup_dir / backup_name
    
    if not backup_path.exists():
        return False, f"Backup not found: {backup_name}"
    
    try:
        # Remove current storage and restore from backup
        if instance_info.storage_path.exists():
            shutil.rmtree(instance_info.storage_path)
        
        shutil.copytree(str(backup_path), str(instance_info.storage_path))
        
        return True, f"Restored storage from backup: {backup_name}"
        
    except Exception as e:
        return False, f"Storage restore failed: {e}"


def create_default_backup_config() -> dict:
    """Create default backup configuration."""
    return {
        "enabled": True,
        "retention_days": 7,
        "schedule_cron": "0 3 * * *",  # Every day at 3 AM
        "max_backups": 10
    }
