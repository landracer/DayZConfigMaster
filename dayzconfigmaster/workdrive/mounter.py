# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# WorkDrive mounter for cross-platform drive mounting

"""
Work Drive Mounter - Cross-platform drive mounting for DayZ development.

Features:
- Windows: Create subst P: virtual drive
- Linux/macOS: Create symlinks to target directories
- Automatic drive detection and cleanup
"""

import os
import subprocess
from pathlib import Path
from typing import Optional, Tuple


class MountResult:
    """Result of a mount operation."""
    
    def __init__(
        self,
        success: bool,
        message: str,
        drive_letter: str = None,
        target_path: str = None
    ):
        """
        Initialize mount result.
        
        Args:
            success: Whether the operation succeeded
            message: Descriptive message
            drive_letter: The drive letter (Windows) or mount point
            target_path: Target path being mounted
        """
        self.success = success
        self.message = message
        self.drive_letter = drive_letter
        self.target_path = target_path
    
    def to_dict(self) -> dict:
        """Convert to dictionary."""
        return {
            "success": self.success,
            "message": self.message,
            "drive_letter": self.drive_letter,
            "target_path": self.target_path
        }


class WorkDrive:
    """
    Manages work drive mounting for DayZ development.
    
    Cross-platform implementation:
    - Windows: Uses subst command for virtual drives
    - Linux/macOS: Uses symlinks to target directories
    """
    
    def __init__(self, projects_root: str):
        """
        Initialize WorkDrive manager.
        
        Args:
            projects_root: Root directory of DayZ projects
        """
        self.projects_root = Path(projects_root)
        self.work_drive_letter = "P" if os.name == 'nt' else None
    
    def _is_windows(self) -> bool:
        """Check if running on Windows."""
        return os.name == 'nt'
    
    def _subst_exists(self, drive: str) -> bool:
        """Check if a subst drive exists."""
        try:
            result = subprocess.run(
                ['subst'],
                capture_output=True,
                text=True
            )
            return f"{drive}:\\ " in result.stdout or f"{drive}:\\" in result.stdout
        except (subprocess.SubprocessError, OSError):
            return False
    
    def _get_subst_target(self, drive: str) -> Optional[str]:
        """Get target path of a subst drive."""
        try:
            result = subprocess.run(
                ['subst', f"{drive}:"],
                capture_output=True,
                text=True
            )
            
            # Parse output to find the target path
            for line in result.stdout.splitlines():
                if f"{drive}: =>" in line or f"{drive}:\\ =>" in line:
                    parts = line.split("=>")
                    if len(parts) > 1:
                        return parts[1].strip().rstrip('\\')
        except (subprocess.SubprocessError, OSError):
            pass
        
        return None
    
    def _subst_mount(self, drive: str, target: str) -> MountResult:
        """
        Create a subst drive mapping.
        
        Args:
            drive: Drive letter (e.g., "P")
            target: Target directory path
            
        Returns:
            MountResult with success status
        """
        try:
            # First remove existing mount if present
            self._subst_unmount(drive)
            
            # Create the subst mapping
            result = subprocess.run(
                ['subst', f"{drive}:", str(target)],
                capture_output=True,
                text=True
            )
            
            if result.returncode == 0:
                return MountResult(
                    success=True,
                    message=f"Created subst drive {drive}: -> {target}",
                    drive_letter=drive,
                    target_path=str(target)
                )
            
            return MountResult(
                success=False,
                message=f"Failed to create subst drive: {result.stderr or result.stdout}"
            )
        except (subprocess.SubprocessError, OSError) as e:
            return MountResult(
                success=False,
                message=f"Subst mount error: {str(e)}"
            )
    
    def _subst_unmount(self, drive: str) -> bool:
        """Remove a subst drive mapping."""
        try:
            result = subprocess.run(
                ['subst', f"{drive}:", "/d"],
                capture_output=True,
                text=True
            )
            
            # Ignore error if drive doesn't exist
            return True
        except (subprocess.SubprocessError, OSError):
            return False
    
    def _symlink_mount(self, link_path: str, target: str) -> MountResult:
        """
        Create a symlink mount on Unix-like systems.
        
        Args:
            link_path: Path where symlink will be created
            target: Target directory path
            
        Returns:
            MountResult with success status
        """
        try:
            link = Path(link_path)
            
            # Remove existing symlink if present
            if link.exists() or link.is_symlink():
                link.unlink()
            
            # Create the symlink
            link.symlink_to(target)
            
            return MountResult(
                success=True,
                message=f"Created symlink {link} -> {target}",
                drive_letter=str(link),
                target_path=target
            )
        except (OSError, IOError) as e:
            return MountResult(
                success=False,
                message=f"Symlink mount error: {str(e)}"
            )
    
    def _symlink_unmount(self, link_path: str) -> bool:
        """Remove a symlink mount."""
        try:
            link = Path(link_path)
            
            if link.exists() or link.is_symlink():
                link.unlink()
            
            return True
        except (OSError, IOError):
            return False
    
    def mount_work_drive(self, target_dir: str) -> MountResult:
        """
        Mount the work drive to a target directory.
        
        Args:
            target_dir: Directory to mount as work drive
            
        Returns:
            MountResult with success status
        """
        if self._is_windows():
            # Windows: Create subst P: drive
            return self._subst_mount(self.work_drive_letter, target_dir)
        
        else:
            # Linux/macOS: Create symlink in projects root
            link_path = self.projects_root / "work"
            
            return self._symlink_mount(str(link_path), target_dir)
    
    def unmount_work_drive(self) -> bool:
        """
        Remove the work drive mount.
        
        Returns:
            True if successful (or already unmounted)
        """
        if self._is_windows():
            # Windows: Remove subst P: drive
            return self._subst_unmount(self.work_drive_letter)
        
        else:
            # Linux/macOS: Remove symlink
            link_path = self.projects_root / "work"
            return self._symlink_unmount(str(link_path))
    
    def get_mounted_path(self) -> Optional[str]:
        """
        Get the current mounted path if work drive is active.
        
        Returns:
            Path being used as work drive, or None if not mounted
        """
        if self._is_windows():
            return self._get_subst_target(self.work_drive_letter)
        
        else:
            link_path = self.projects_root / "work"
            
            if link_path.is_symlink() or (link_path.exists() and not link_path.is_dir()):
                try:
                    return str(link_path.resolve())
                except OSError:
                    pass
            
            return None
    
    def is_mounted(self) -> bool:
        """Check if work drive is currently mounted."""
        return self.get_mounted_path() is not None
    
    def ensure_mounted(self, target_dir: str) -> MountResult:
        """
        Ensure work drive is mounted to the specified directory.
        
        If already mounted to a different path, unmounts first.
        
        Args:
            target_dir: Target directory to mount
            
        Returns:
            MountResult with success status
        """
        current_path = self.get_mounted_path()
        
        if current_path == str(target_dir):
            return MountResult(
                success=True,
                message=f"Work drive already mounted at {target_dir}",
                target_path=target_dir
            )
        
        # Unmount first if different path
        if current_path:
            self.unmount_work_drive()
        
        return self.mount_work_drive(target_dir)
    
    def get_status(self) -> dict:
        """Get work drive status."""
        if self._is_windows():
            target = self._get_subst_target(self.work_drive_letter)
            is_mounted = target is not None
            
            return {
                "platform": "Windows",
                "drive_letter": self.work_drive_letter,
                "mounted": is_mounted,
                "target_path": target
            }
        
        else:
            link_path = self.projects_root / "work"
            
            if link_path.is_symlink():
                try:
                    target = str(link_path.resolve())
                except OSError:
                    target = None
                
                return {
                    "platform": "Unix",
                    "symlink_path": str(link_path),
                    "mounted": True,
                    "target_path": target
                }
            
            return {
                "platform": "Unix",
                "symlink_path": str(link_path),
                "mounted": False,
                "target_path": None
            }


def create_work_drive(projects_root: str, target_dir: str) -> Tuple[bool, str]:
    """
    Convenience function to create a work drive.
    
    Args:
        projects_root: Projects root directory
        target_dir: Directory to mount
        
    Returns:
        Tuple of (success, message)
    """
    workdrive = WorkDrive(projects_root)
    result = workdrive.mount_work_drive(target_dir)
    
    return result.success, result.message