# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Workshop browser for DayZConfigMaster

"""
Workshop browser for searching and downloading community mods.

Uses:
- Web scraping of Steam Community browse page (no API key required)
- GetPublishedFileDetails endpoint for detailed item info
- Steam Web API search with optional API key
"""

import urllib.parse
import requests
from pathlib import Path
from typing import Dict, List, Optional, Any


class WorkshopItem:
    """Information about a Workshop item."""
    
    def __init__(
        self,
        id: str,
        title: str,
        description: str = "",
        preview_url: str = "",
        subscribers: int = 0,
        created: int = 0,
        updated: int = 0,
        tags: List[str] = None,
        file_size: int = 0
    ):
        self.id = id
        self.title = title
        self.description = description
        self.preview_url = preview_url
        self.subscribers = subscribers
        self.created = created
        self.updated = updated
        self.tags = tags or []
        self.file_size = file_size
    
    def to_dict(self) -> Dict[str, Any]:
        """Convert to dictionary."""
        return {
            "id": self.id,
            "title": self.title,
            "description": self.description,
            "preview_url": self.preview_url,
            "subscribers": self.subscribers,
            "created": self.created,
            "updated": self.updated,
            "tags": self.tags,
            "file_size": self.file_size
        }


class WorkshopBrowser:
    """Browse and search DayZ Workshop items."""
    
    BASE_URL = "https://steamcommunity.com/workshop/browse"
    APP_ID = "221100"  # DayZ
    
    def __init__(self):
        self.session = requests.Session()
        self.session.headers.update({
            "User-Agent": "Mozilla/5.0 (DayZConfigMaster)"
        })
    
    def browse(
        self,
        search_text: str = "",
        sort_by: str = "trend",
        time_frame: int = -1,  # -1 = all time
        page: int = 1,
        count: int = 20,
        tags: List[str] = None
    ) -> tuple:
        """
        Browse Workshop items.
        
        Args:
            search_text: Search query
            sort_by: Sort method (trend, toprated, mostrecent)
            time_frame: Time frame in days (-1 for all time)
            page: Page number
            count: Items per page
            tags: Filter by tags
        
        Returns:
            Tuple of (success, list of items)
        """
        # Build URL with proper encoding
        url = f"{self.BASE_URL}/?appid={self.APP_ID}&section=readytouseitems"
        
        if sort_by == "trend" and time_frame != -1:
            url += f"&days={time_frame}"
        
        if search_text:
            url += f"&searchtext={urllib.parse.quote(search_text)}"
        
        url += f"&p={page}"
        
        if tags:
            for tag in tags:
                url += f"&requiredtags%5B%5D={requests.utils.quote(tag)}"
        
        try:
            response = self.session.get(url, timeout=30)
            
            if response.status_code != 200:
                return False, []
            
            items = self._parse_browse_page(response.text)
            return True, items
        
        except Exception as e:
            print(f"Browse error: {e}")
            return False, []
    
    def _parse_browse_page(self, html: str) -> List[WorkshopItem]:
        """Parse browse page HTML for item IDs and titles."""
        import re
        
        # Pattern to find Workshop items
        pattern = r'filedetails/\?id=(\d+)".*?<img\s+src="([^"]+)".*?alt="([^"]+)"'
        
        matches = re.findall(pattern, html, re.DOTALL | re.IGNORECASE)
        
        items = []
        
        for match in matches:
            item_id, preview_url, title = match
            
            # Fetch detailed info for this item
            details = self.get_details(item_id)
            
            if details:
                items.append(details)
            else:
                # Fallback to basic info
                items.append(WorkshopItem(
                    id=item_id,
                    title=title.strip(),
                    preview_url=preview_url
                ))
        
        return items
    
    def get_details(self, item_id: str) -> Optional[WorkshopItem]:
        """Get detailed information about a Workshop item."""
        # Use the keyless GetPublishedFileDetails endpoint
        url = "https://api.steamcommunity.com/ISteamRemoteStorage/GetPublishedFileDetails"
        
        data = {
            "items": [{"publishedfileid": str(item_id)}]
        }
        
        try:
            response = self.session.post(url, json=data, timeout=30)
            
            if response.status_code != 200:
                return None
            
            result = response.json()
            
            if "response" not in result or "publishedfiledetails" not in result["response"]:
                return None
            
            details = result["response"]["publishedfiledetails"][0]
            
            # Parse tags
            tags = []
            for tag_data in details.get("tags", []):
                if isinstance(tag_data, dict) and "tag" in tag_data:
                    tags.append(tag_data["tag"])
            
            return WorkshopItem(
                id=str(details.get("publishedfileid", item_id)),
                title=details.get("title", ""),
                description=details.get("file_description", "")[:500],  # Limit to 500 chars
                preview_url=details.get("preview_url", ""),
                subscribers=int(details.get("subscriptions", 0)),
                created=int(details.get("time_created", 0)),
                updated=int(details.get("time_updated", 0)),
                tags=tags,
                file_size=int(details.get("file_size", 0))
            )
        
        except Exception as e:
            print(f"Get details error: {e}")
            return None
    
    def subscribe(self, item_id: str) -> tuple:
        """
        Subscribe to a Workshop item.
        
        Note: This requires a Steam Web API access token for true API access.
        For now, opens the workshop page in browser.
        
        Args:
            item_id: Workshop item ID
        
        Returns:
            Tuple of (success, message)
        """
        # Open in browser - user can subscribe there
        import webbrowser
        webbrowser.open(f"https://steamcommunity.com/sharedfiles/filedetails/?id={item_id}")
        
        return True, "Opened workshop page"
    
    def search(
        self,
        search_text: str = "",
        app_id: int = 221100,
        page: int = 1,
        count: int = 50,
        api_key: Optional[str] = None
    ) -> tuple:
        """
        Search Workshop items using Steam Web API.
        
        Args:
            search_text: Search query
            app_id: Steam app ID (221100 for DayZ)
            page: Page number
            count: Items per page (max 100)
            api_key: Steam Web API key (optional, enables more results)
        
        Returns:
            Tuple of (success, list of items)
        """
        if api_key:
            return self._search_with_api_key(search_text, app_id, page, count, api_key)
        else:
            # Fallback to web scraping
            return self.browse(search_text, "trend", -1, page, count)
    
    def _search_with_api_key(
        self,
        search_text: str = "",
        app_id: int = 221100,
        page: int = 1,
        count: int = 50,
        api_key: str = ""
    ) -> tuple:
        """
        Search Workshop items using Steam Web API with key.
        
        Args:
            search_text: Search query
            app_id: Steam app ID (221100 for DayZ)
            page: Page number
            count: Items per page (max 100)
            api_key: Steam Web API key
        
        Returns:
            Tuple of (success, list of items)
        """
        url = "https://api.steamcommunity.com/ISteamRemoteStorage/GetPublishedFileDetails"
        
        # Get published files via search first
        query_url = f"https://api.steamcommunity.com/ISteamRemoteStorage/GetFilesForUser/{app_id}/"
        
        try:
            response = self.session.get(
                f"{query_url}?key={api_key}&appid={app_id}",
                params={
                    "count": count,
                    "start_index": (page - 1) * count
                },
                timeout=30
            )
            
            if response.status_code != 200:
                return False, []
            
            data = response.json()
            
            items = []
            for file_data in data.get("publishedfiledetails", []):
                item_id = str(file_data.get("publishedfileid", ""))
                
                # Get detailed info
                details = self.get_details(item_id)
                if details:
                    items.append(details)
            
            return True, items
        
        except Exception as e:
            print(f"Search error: {e}")
            return False, []
