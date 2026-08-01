# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# SteamCMD wrapper for workshop downloads

"""
SteamCMD wrapper for downloading Workshop items.

Handles:
- Downloading Workshop items via steamcmd
- Installing and managing steamcmd
- Anonymous and authenticated downloads
- Visible console mode for user interaction
"""

import subprocess
import os
import sys
import signal
from pathlib import Path
from typing import Optional, List, Tuple


class BaseSteamCmd:
    """Base class for SteamCMD operations."""
    
    def __init__(self, install_path: str):
        """
        Initialize with SteamCMD installation path.
        
        Args:
            install_path: Path to SteamCMD directory
        """
        self.install_path = Path(install_path)
        self.steamcmd_exe = self._find_steamcmd()
    
    def _find_steamcmd(self) -> Optional[Path]:
        """Locate steamcmd executable."""
        if os.name == 'nt':
            possible_names = ["steamcmd.exe", "SteamCMD.exe"]
        else:
            possible_names = ["steamcmd.sh", "steamcmd"]
        
        for name in possible_names:
            exe_path = self.install_path / name
            if exe_path.exists():
                return exe_path
        
        return None
    
    def download_workshop_item(
        self,
        item_id: str,
        install_dir: Optional[str] = None,
        login: Optional[str] = None,
        password: Optional[str] = None
    ) -> tuple:
        """
        Download a Workshop item using steamcmd.
        
        Args:
            item_id: Workshop item ID (numeric)
            install_dir: Install directory (defaults to SteamApps/workshop/content/221100)
            login: Steam username (optional, for authenticated downloads)
            password: Steam password (optional)
        
        Returns:
            Tuple of (success, message)
        """
        if not self.steamcmd_exe:
            return False, "steamcmd.exe not found"
        
        # Build command
        cmd = [str(self.steamcmd_exe)]
        
        # Login (anonymous if no credentials provided)
        if login:
            cmd.extend(["+login", login])
        else:
            cmd.extend(["+login", "anonymous"])
        
        # Install directory
        if install_dir:
            cmd.extend(["+force_install_dir", str(Path(install_dir).resolve())])
        
        # Download item
        cmd.extend([
            "+workshop_download_item",
            "221100",  # DayZ app ID
            str(item_id),
            "+quit"
        ])
        
        try:
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=600  # 10 minute timeout for downloads
            )
            
            if "Success" in result.stdout or result.returncode == 0:
                return True, f"Downloaded item {item_id}"
            
            error_msg = result.stderr.strip() or result.stdout.strip()
            return False, f"Download failed: {error_msg}"
        
        except subprocess.TimeoutExpired:
            return False, "Download timed out"
        except Exception as e:
            return False, f"Download error: {str(e)}"
    
    def update_workshop_item(
        self,
        item_id: str,
        install_dir: Optional[str] = None
    ) -> tuple:
        """Update a Workshop item to latest version."""
        # SteamCMD automatically updates on download
        return self.download_workshop_item(item_id, install_dir)


class ConsoleVisibleSteamCmd(BaseSteamCmd):
    """
    SteamCMD wrapper with visible console output.
    
    This variant shows login prompts and Guard prompts to the user:
    - No capture of stdout/stderr
    - Real-time output to terminal
    - Interactive commands work properly
    - Windows: Uses CREATE_NEW_CONSOLE flag
    """
    
    def __init__(self, install_path: str):
        """Initialize with visible console support."""
        super().__init__(install_path)
        self.visible_output = True
    
    def download_workshop_item(
        self,
        item_id: str,
        install_dir: Optional[str] = None,
        login: Optional[str] = None,
        password: Optional[str] = None
    ) -> tuple:
        """
        Download with visible console output.
        
        The SteamCMD window stays open for user input (login, 2FA).
        
        Args:
            item_id: Workshop item ID (numeric)
            install_dir: Install directory
            login: Steam username
            password: Steam password
            
        Returns:
            Tuple of (success, message) - success is True if process starts
        """
        if not self.steamcmd_exe:
            return False, "steamcmd.exe not found"
        
        # Build command
        cmd = [str(self.steamcmd_exe)]
        
        # Login (anonymous if no credentials provided)
        if login:
            cmd.extend(["+login", login])
        else:
            cmd.extend(["+login", "anonymous"])
        
        # Install directory
        if install_dir:
            cmd.extend(["+force_install_dir", str(Path(install_dir).resolve())])
        
        # Download item
        cmd.extend([
            "+workshop_download_item",
            "221100",
            str(item_id),
            "+quit"
        ])
        
        try:
            # Windows: Create new console for visibility
            if os.name == 'nt':
                process = subprocess.Popen(
                    cmd,
                    creationflags=subprocess.CREATE_NEW_CONSOLE
                )
            else:
                # Unix-like: Use a new session with visible terminal
                process = subprocess.Popen(
                    cmd,
                    start_new_session=True,
                    stdout=None,  # No capture - show to terminal
                    stderr=None   # No capture - show to terminal
                )
            
            return True, f"SteamCMD started with PID {process.pid}"
        
        except Exception as e:
            return False, f"Failed to start SteamCMD: {str(e)}"
    
    def run_interactive(self, commands: List[str]) -> tuple:
        """
        Run SteamCMD with a list of commands visible in console.
        
        Args:
            commands: List of SteamCMD commands
            
        Returns:
            Tuple of (success, message)
        """
        if not self.steamcmd_exe:
            return False, "steamcmd.exe not found"
        
        cmd = [str(self.steamcmd_exe)]
        cmd.extend(commands)
        cmd.append("+quit")
        
        try:
            if os.name == 'nt':
                process = subprocess.Popen(
                    cmd,
                    creationflags=subprocess.CREATE_NEW_CONSOLE
                )
            else:
                process = subprocess.Popen(
                    cmd,
                    start_new_session=True,
                    stdout=None,
                    stderr=None
                )
            
            return True, f"SteamCMD started with PID {process.pid}"
        
        except Exception as e:
            return False, f"Failed to start SteamCMD: {str(e)}"


def create_steamcmd(
    install_path: str,
    visible_console: bool = False
) -> BaseSteamCmd:
    """
    Create a SteamCMD instance.
    
    Args:
        install_path: Path to SteamCMD installation
        visible_console: Use ConsoleVisibleSteamCmd for interactive use
        
    Returns:
        SteamCmd or ConsoleVisibleSteamCmd instance
    """
    if visible_console:
        return ConsoleVisibleSteamCmd(install_path)
    return BaseSteamCmd(install_path)