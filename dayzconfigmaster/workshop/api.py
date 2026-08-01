# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Steam Workshop API Module - Comprehensive workshop management.

This module provides robust integration with Steam Workshop using:
- Steam Web API (public endpoints)
- steamcmd CLI tool for downloads
- Browser fallback for manual subscription
- All code governed by the SOVEREIGN INDIVIDUAL LICENSE.
"""

import os
import json
import subprocess
import requests
from pathlib import Path
from typing import Dict, List, Optional, Tuple, Any
from dataclasses import dataclass, field


@dataclass
class WorkshopItemInfo:
    """Detailed information about a workshop item."""
    id: str
    title: str
    description: str = ""
    preview_url: str = ""
    subscribers: int = 0
    created: int = 0
    updated: int = 0
    tags: List[str] = field(default_factory=list)
    file_size: int = 0
    author: str = ""
    rating: float = 0.0


class SteamWorkshopAPI:
    """
    Robust Steam Workshop API client.
    
    Features:
    - Public endpoints (no API key required for basic operations)
    - Search with optional API key for more results
    - GetPublishedFileDetails for full item info
    - Fallback to steamcmd for downloads when needed
    """

    BASE_URL = "https://api.steamcommunity.com"
    
    def __init__(self, api_key: Optional[str] = None):
        """
        Initialize API client.
        
        Args:
            api_key: Steam Web API key (optional, enables more results)
        """
        self.api_key = api_key or os.environ.get("STEAM_API_KEY")
        self.session = requests.Session()
        self.session.headers.update({
            "User-Agent": "DayZConfigMaster/1.0"
        })
    
    def get_item_details(self, item_id: str) -> Optional[WorkshopItemInfo]:
        """
        Get detailed information about a workshop item.
        
        Uses the public GetPublishedFileDetails endpoint (no API key required).
        
        Args:
            item_id: Workshop item ID
            
        Returns:
            WorkshopItemInfo or None if not found
        """
        details = self.get_multiple_item_details([str(item_id)])
        return details[0] if details else None
    
    def get_multiple_item_details(self, item_ids: List[str]) -> List[WorkshopItemInfo]:
        """
        Get detailed information about multiple workshop items in a single request.
        
        Uses the public GetPublishedFileDetails endpoint (no API key required).
        Supports up to 100 items per request.
        
        Args:
            item_ids: List of workshop item IDs
            
        Returns:
            List of WorkshopItemInfo objects (may be shorter than input if some not found)
        """
        url = f"{self.BASE_URL}/ISteamRemoteStorage/GetPublishedFileDetails"
        
        # Steam API accepts up to 100 items per request
        max_batch_size = 100
        all_results = []
        
        for i in range(0, len(item_ids), max_batch_size):
            batch = item_ids[i:i + max_batch_size]
            
            data = {
                "items": [{"publishedfileid": str(item_id)} for item_id in batch]
            }
            
            try:
                response = self.session.post(url, json=data, timeout=30)
                
                if response.status_code != 200:
                    continue
                
                result = response.json()
                
                if "response" not in result or "publishedfiledetails" not in result["response"]:
                    continue
                
                for details in result["response"]["publishedfiledetails"]:
                    # Parse tags
                    tags = []
                    for tag_data in details.get("tags", []):
                        if isinstance(tag_data, dict) and "tag" in tag_data:
                            tags.append(tag_data["tag"])
                    
                    all_results.append(WorkshopItemInfo(
                        id=str(details.get("publishedfileid", "")),
                        title=details.get("title", ""),
                        description=details.get("file_description", ""),
                        preview_url=details.get("preview_url", ""),
                        subscribers=int(details.get("subscriptions", 0)),
                        created=int(details.get("time_created", 0)),
                        updated=int(details.get("time_updated", 0)),
                        tags=tags,
                        file_size=int(details.get("file_size", 0)),
                        author=details.get("creator", ""),
                        rating=float(details.get("rating", 0)) / 5.0
                    ))
            
            except Exception:
                # Silently handle network errors - user will see status message instead
                continue
        
        return all_results
    
    def search_items(
        self,
        search_text: str = "",
        app_id: int = 221100,
        page: int = 1,
        count: int = 50
    ) -> Tuple[bool, List[WorkshopItemInfo]]:
        """
        Search workshop items using public endpoints.
        
        Args:
            search_text: Search query
            app_id: Steam app ID (221100 for DayZ)
            page: Page number
            count: Items per page
            
        Returns:
            Tuple of (success, list of item info)
        """
        items = []
        
        # Use GetFilesForUser endpoint which doesn't require API key for browsing
        # We'll browse the community workshop directly
        
        url = f"{self.BASE_URL}/ISteamRemoteStorage/GetFilesForUser/{app_id}/"
        
        params = {
            "count": count,
            "start_index": (page - 1) * count
        }
        
        if self.api_key:
            params["key"] = self.api_key
        
        try:
            response = self.session.get(url, params=params, timeout=30)
            
            if response.status_code != 200:
                return False, []
            
            data = response.json()
            
            # Get detailed info for each file
            for file_data in data.get("publishedfiledetails", []):
                item_id = str(file_data.get("publishedfileid", ""))
                info = self.get_item_details(item_id)
                if info:
                    items.append(info)
            
            return True, items
        
        except Exception as e:
            print(f"Search error: {e}")
            # Fallback to browser-based search
            from .browser import WorkshopBrowser
            browser = WorkshopBrowser()
            return browser.browse(search_text, "trend", -1, page, count)
    
    def download_item(
        self,
        item_id: str,
        install_dir: Optional[str] = None,
        use_steamcmd: bool = True,
        visible_console: bool = False
    ) -> Tuple[bool, str]:
        """
        Download a workshop item.
        
        Args:
            item_id: Workshop item ID
            install_dir: Installation directory (defaults to Steam workshop dir)
            use_steamcmd: Use steamcmd for download (recommended)
            visible_console: Show console output
            
        Returns:
            Tuple of (success, message)
        """
        if use_steamcmd:
            return self._download_with_steamcmd(item_id, install_dir, visible_console)
        else:
            # Use browser API method
            return self._download_via_api(item_id, install_dir)
    
    def _download_with_steamcmd(
        self,
        item_id: str,
        install_dir: Optional[str],
        visible_console: bool
    ) -> Tuple[bool, str]:
        """Download using steamcmd."""
        try:
            from .steamcmd import ConsoleVisibleSteamCmd, BaseSteamCmd
            
            # Find steamcmd
            local_steamcmd = Path("/home/sysadmin/Documents/git/DayzConfigMaster/steamcmd")
            if (local_steamcmd / "steamcmd.sh").exists():
                steamcmd_path = local_steamcmd
            else:
                steamcmd_path = None
                for possible_path in [
                    Path.home() / "Steam" / "steamcmd",
                    Path("/usr/share/steam/steamcmd"),
                    Path("/opt/steam/steamcmd")
                ]:
                    if (possible_path / "steamcmd.sh").exists():
                        steamcmd_path = possible_path
                        break
            
            if not steamcmd_path:
                return False, "SteamCMD installation not found. Please install SteamCMD."
            
            # Create steamcmd instance - visible_console controls whether console is shown
            steamcmd = ConsoleVisibleSteamCmd(str(steamcmd_path)) if visible_console else BaseSteamCmd(str(steamcmd_path))
            
            # Download with anonymous login and specify the workshop content directory
            cmd_args = [
                "+login", "anonymous",
                f"+force_install_dir", str(Path(install_dir).parent),
                f"+workshop_download_item", "221100", str(item_id),
                "+quit"
            ]
            
            result = steamcmd.run_interactive(cmd_args) if visible_console else steamcmd.download_workshop_item(str(item_id), install_dir=str(install_dir))
            
            return result
        
        except Exception as e:
            return False, f"Download failed: {str(e)}"
    
    def _download_via_api(self, item_id: str, install_dir: Optional[str]) -> Tuple[bool, str]:
        """Fallback download via browser API."""
        # Open workshop page - user can subscribe/download manually
        import webbrowser
        
        workshop_url = f"https://steamcommunity.com/sharedfiles/filedetails/?id={item_id}"
        webbrowser.open(workshop_url)
        
        return True, f"Opened workshop page: {workshop_url}"


class WorkshopManager:
    """
    Unified workshop management combining Steam Web API and steamcmd.
    
    Priority order for downloads:
    1. steamcmd (fastest, reliable for anonymous mods)
    2. Browser fallback (manual subscription)
    """

    def __init__(self, projects_root: str):
        """
        Initialize workshop manager.
        
        Args:
            projects_root: Projects root directory
        """
        self.projects_root = Path(projects_root)
        self.steam_api = SteamWorkshopAPI()
    
    def download_item(self, item_id: str) -> Tuple[bool, str]:
        """
        Download a workshop item with intelligent method selection.
        
        Args:
            item_id: Workshop item ID
            
        Returns:
            Tuple of (success, message)
        """
        # Use the workshop directory from projects_root if available
        workshop_content_dir = Path(self.projects_root) / "steamapps" / "workshop" / "content" / "221100"
        
        # Default to projects_root if workshop content dir doesn't exist
        if not workshop_content_dir.exists():
            workshop_content_dir = self.projects_root
        
        install_path = str(workshop_content_dir)
        
        return self.steam_api.download_item(str(item_id), install_dir=install_path, use_steamcmd=True, visible_console=False)
    
    def search(self, query: str) -> Tuple[bool, List[WorkshopItemInfo]]:
        """
        Search workshop items.
        
        Args:
            query: Search query
            
        Returns:
            Tuple of (success, list of item info)
        """
        return self.steam_api.search_items(query)
    
    def get_item_info(self, item_id: str) -> Optional[WorkshopItemInfo]:
        """Get detailed info about a workshop item."""
        return self.steam_api.get_item_details(str(item_id))