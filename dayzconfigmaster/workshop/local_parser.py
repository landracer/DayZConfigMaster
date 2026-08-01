"""
Local Workshop Metadata Parser - Reads .acf files and enriches with Steam API.

This module implements a two-phase architecture:
Phase 1: Read appworkshop_<AppID>.acf file to get local workshop items
Phase 2: Batch-query Steam API for titles, authors, tags
"""

import json
import re
import os
from pathlib import Path
from typing import Dict, List, Optional, Any
from datetime import datetime
from dataclasses import dataclass, asdict

from .api import SteamWorkshopAPI


@dataclass
class LocalWorkshopItem:
    """Represents a workshop item found on local disk."""
    workshop_id: str
    size_on_disk_bytes: int = 0
    size_on_disk_readable: str = "0 B"
    last_local_update_epoch: int = 0
    last_local_update_date: str = ""
    path: str = ""


@dataclass  
class EnrichedWorkshopItem:
    """Complete workshop item with both local and remote metadata."""
    workshop_id: str
    mod_name: str
    author_steam_id_64: str
    visibility_status: str
    local_metadata: Dict[str, Any]
    remote_metadata: Dict[str, Any]


class LocalWorkshopMetadataParser:
    """
    Parser that reads Steam's .acf files and enriches with Steam API data.
    
    Does NOT use SteamCMD CLI (avoids session caching bugs).
    Instead reads the static appworkshop_<AppID>.acf file directly.
    """

    def __init__(self, steam_path: str):
        """
        Initialize parser with Steam installation path.
        
        Args:
            steam_path: Path to Steam or SteamCMD root directory
        """
        self.steam_path = Path(steam_path)
        self.app_id = 221100  # DayZ AppID
    
    def _find_acf_file(self) -> Optional[Path]:
        """Locate the appworkshop_<AppID>.acf file."""
        acf_name = f"appworkshop_{self.app_id}.acf"
        
        possible_paths = [
            self.steam_path / "steamapps" / "workshop" / acf_name,
            self.steam_path / "steamapps" / acf_name,
            Path.home() / ".local" / "share" / "Steam" / "steamapps" / "workshop" / acf_name,
        ]
        
        for path in possible_paths:
            if path.exists():
                return path
        
        return None
    
    def parse_acf_file(self, acf_path: Optional[Path] = None) -> List[LocalWorkshopItem]:
        """
        Parse the .acf file and extract workshop item metadata.
        
        Args:
            acf_path: Optional explicit path to ACF file
            
        Returns:
            List of LocalWorkshopItem objects with local disk info
        """
        if not acf_path:
            acf_path = self._find_acf_file()
            
        if not acf_path or not acf_path.exists():
            return []
        
        try:
            # Read the ACF file (VDF format - key-value pairs)
            content = acf_path.read_text(encoding='utf-8', errors='ignore')
        except Exception:
            return []
        
        items = []
        
        # Get the list of folders that actually exist on disk
        workshop_content_dir = self.steam_path / "steamapps" / "workshop" / "content" / str(self.app_id)
        existing_folders = set()
        if workshop_content_dir.exists():
            for folder in workshop_content_dir.iterdir():
                if folder.is_dir():
                    try:
                        _ = int(folder.name)  # Validate it's a numeric ID
                        existing_folders.add(folder.name)
                    except ValueError:
                        pass
        
        # Find WorkshopItemsInstalled block
        workshop_start = content.find('"WorkshopItemsInstalled"')
        if workshop_start == -1:
            return []
        
        # Get the lines from WorkshopItemsInstalled onward
        lines = content[workshop_start:].split('\n')
        
        in_workshop_items = False
        current_id = None
        current_size = 0
        current_time = 0
        
        for i, line in enumerate(lines):
            stripped = line.strip()
            
            # Track when we're in the WorkshopItemsInstalled block
            if '"WorkshopItemsInstalled"' in stripped:
                in_workshop_items = True
                continue
            
            if not in_workshop_items:
                continue
            
            # Look for workshop item ID lines like: "1559212036"
            id_match = re.search(r'"(\d{9,})"', stripped)
            if id_match:
                workshop_id = id_match.group(1)
                
                # Only include items that have folders on disk
                if workshop_id in existing_folders or current_size > 0:
                    # Save previous item if valid
                    if current_id is not None and current_size > 0:
                        items.append(LocalWorkshopItem(
                            workshop_id=current_id,
                            size_on_disk_bytes=int(current_size),
                            last_local_update_epoch=int(current_time),
                            path=str(workshop_content_dir / current_id)
                        ))
                    
                    # Start new item
                    current_id = workshop_id
                    current_size = 0
                    current_time = 0
                
                # Check if next line has size/timeupdated (in case it's on same line as {)
                if i + 1 < len(lines):
                    next_line = lines[i + 1].strip()
                    # Skip the { line, look for size/time in subsequent lines
                    j = i + 2  # Start after { and id lines
                    
            # Parse size from "size"    "527656"
            if current_id is not None:
                size_match = re.search(r'"size"\s+"(\d+)"', stripped)
                if size_match:
                    current_size = int(size_match.group(1))
                
                time_match = re.search(r'"timeupdated"\s+"(\d+)"', stripped)
                if time_match:
                    current_time = int(time_match.group(1))
        
        # Don't forget the last item
        if current_id is not None and current_size > 0 and current_id in existing_folders:
            items.append(LocalWorkshopItem(
                workshop_id=current_id,
                size_on_disk_bytes=int(current_size),
                last_local_update_epoch=int(current_time),
                path=str(workshop_content_dir / current_id)
            ))
        
        # Format human-readable sizes
        for item in items:
            item.size_on_disk_readable = self._format_size(item.size_on_disk_bytes)
            if item.last_local_update_epoch > 0:
                from datetime import datetime, timezone
                item.last_local_update_date = datetime.fromtimestamp(
                    item.last_local_update_epoch, tz=timezone.utc
                ).strftime('%Y-%m-%dT%H:%M:%SZ')
        
        return items
    
    def _format_size(self, bytes_val: int) -> str:
        """Convert bytes to human-readable format."""
        for unit in ['B', 'KB', 'MB', 'GB', 'TB']:
            if abs(bytes_val) < 1024.0:
                return f"{bytes_val:.2f} {unit}"
            bytes_val /= 1024.0
        return f"{bytes_val:.2f} PB"

    def find_unsubscribed_folders(self) -> List[LocalWorkshopItem]:
        """
        Find workshop content folders that exist on disk but are no longer
        listed in the WorkshopItemsInstalled block of appworkshop_<AppID>.acf.

        These are leftover downloads from unsubscribed items. Steam does not
        delete them automatically, so this lets the user clean them up.

        Returns:
            List of LocalWorkshopItem for unsubscribed folders, with actual
            disk usage calculated from the folder contents.
        """
        acf_path = self._find_acf_file()
        if not acf_path or not acf_path.exists():
            return []

        try:
            content = acf_path.read_text(encoding='utf-8', errors='ignore')
        except Exception:
            return []

        # Parse only the WorkshopItemsInstalled block.
        installed_ids: set = set()
        match = re.search(r'"WorkshopItemsInstalled"\s*\{(.*?)\n\}', content, re.DOTALL)
        if match:
            block = match.group(1)
            installed_ids = set(re.findall(r'^\s*"(\d{9,})"\s*$', block, re.MULTILINE))

        workshop_content_dir = self.steam_path / "steamapps" / "workshop" / "content" / str(self.app_id)
        if not workshop_content_dir.exists():
            return []

        unsubscribed: List[LocalWorkshopItem] = []
        for folder in sorted(workshop_content_dir.iterdir(), key=lambda p: p.name):
            if not folder.is_dir():
                continue
            try:
                _ = int(folder.name)
            except ValueError:
                continue
            if folder.name in installed_ids:
                continue

            size = sum(
                f.stat().st_size for f in folder.rglob('*') if f.is_file()
            )
            item = LocalWorkshopItem(
                workshop_id=folder.name,
                size_on_disk_bytes=size,
                path=str(folder)
            )
            item.size_on_disk_readable = self._format_size(size)
            unsubscribed.append(item)

        return unsubscribed
    
    def enrich_with_steam_api(
        self, 
        local_items: List[LocalWorkshopItem],
        api_key: Optional[str] = None
    ) -> List[EnrichedWorkshopItem]:
        """
        Enrich local workshop items with Steam API metadata.
        
        Uses POST request to GetPublishedFileDetails/v1/ endpoint (same as dayz-server-manager).
        
        Args:
            local_items: Local workshop items from ACF parsing
            api_key: Optional Steam Web API key
            
        Returns:
            List of enriched workshop items
        """
        if not local_items:
            return []
        
        import urllib.parse
        try:
            # Build form data for POST request (same format as dayz-server-manager)
            item_ids = [item.workshop_id for item in local_items]
            
            # Group into batches of 100
            max_batch_size = 100
            
            all_results = []
            
            for i in range(0, len(item_ids), max_batch_size):
                batch = item_ids[i:i + max_batch_size]
                
                # Build form data
                form_data = urllib.parse.urlencode({
                    'format': 'json',
                    'itemcount': str(len(batch)),
                })
                
                # Add each publishedfileid
                for j, mod_id in enumerate(batch):
                    form_data += f'&publishedfileids[{j}]={mod_id}'
                
                # Make POST request to Steam API
                import http.client
                conn = http.client.HTTPSConnection('api.steampowered.com')
                headers = {'Content-Type': 'application/x-www-form-urlencoded'}
                
                try:
                    conn.request('POST', '/ISteamRemoteStorage/GetPublishedFileDetails/v1/', form_data, headers)
                    response = conn.getresponse()
                    
                    if response.status == 200:
                        data = json.loads(response.read().decode())
                        
                        if 'response' in data and 'publishedfiledetails' in data['response']:
                            all_results.extend(data['response']['publishedfiledetails'])
                finally:
                    conn.close()
                    
        except Exception as e:
            print(f"Steam API error: {e}")
            all_results = []
        
        # Create mapping by workshop ID
        remote_map = {}
        for details in all_results:
            item_id = str(details.get('publishedfileid', ''))
            if item_id:
                remote_map[item_id] = details
        
        enriched_items = []
        
        for local_item in local_items:
            details = remote_map.get(local_item.workshop_id)
            
            if details:
                # Parse tags
                tags = []
                for tag_data in details.get('tags', []):
                    if isinstance(tag_data, dict) and 'tag' in tag_data:
                        tags.append(tag_data['tag'])
                
                enriched_items.append(EnrichedWorkshopItem(
                    workshop_id=local_item.workshop_id,
                    mod_name=details.get('title', f"Mod #{local_item.workshop_id}"),
                    author_steam_id_64=details.get('creator', 'Unknown'),
                    visibility_status="Public",
                    local_metadata={
                        "disk_path": local_item.path,
                        "size_on_disk_bytes": local_item.size_on_disk_bytes,
                        "size_on_disk_readable": local_item.size_on_disk_readable,
                        "last_local_update": local_item.last_local_update_date
                    },
                    remote_metadata={
                        "server_size_bytes": int(details.get('file_size', 0)),
                        "server_last_update": datetime.fromtimestamp(int(details.get('time_updated', 0))).isoformat() if details.get('time_updated') else "",
                        "thumbnail_url": details.get('preview_url', ''),
                        "tags": tags,
                        "description": details.get('file_description', '')[:500]
                    }
                ))
            else:
                # API didn't return data, use fallback
                enriched_items.append(EnrichedWorkshopItem(
                    workshop_id=local_item.workshop_id,
                    mod_name=f"Mod #{local_item.workshop_id}",
                    author_steam_id_64="Unknown",
                    visibility_status="Unknown",
                    local_metadata={
                        "disk_path": local_item.path,
                        "size_on_disk_bytes": local_item.size_on_disk_bytes,
                        "size_on_disk_readable": local_item.size_on_disk_readable,
                        "last_local_update": local_item.last_local_update_date
                    },
                    remote_metadata={
                        "server_size_bytes": 0,
                        "server_last_update": "",
                        "thumbnail_url": "",
                        "tags": [],
                        "description": ""
                    }
                ))
        
        return enriched_items
    
    def _get_cache_dir(self) -> Path:
        """Get the cache directory for workshop metadata."""
        cache_dir = Path.home() / ".cache" / "dayzconfigmaster" / "workshop"
        cache_dir.mkdir(parents=True, exist_ok=True)
        return cache_dir
    
    def _load_cached_info(self) -> Dict[str, Any]:
        """Load cached workshop info from disk."""
        cache_file = self._get_cache_dir() / "workshop_info.json"
        if cache_file.exists():
            try:
                with open(cache_file, 'r') as f:
                    data = json.load(f)
                    # Check if cache is less than 24 hours old
                    last_update = datetime.fromisoformat(data.get("last_updated", "1970-01-01"))
                    age_hours = (datetime.now() - last_update).total_seconds() / 3600
                    if age_hours < 24:
                        return data.get("items", {})
            except Exception:
                pass
        return {}
    
    def _save_cached_info(self, items: List[Dict[str, Any]]) -> None:
        """Save workshop info to cache file."""
        cache_file = self._get_cache_dir() / "workshop_info.json"
        try:
            with open(cache_file, 'w') as f:
                json.dump({
                    "last_updated": datetime.now().isoformat(),
                    "items": items
                }, f, indent=2)
        except Exception:
            pass
    
    def get_all_workshop_info(
        self, 
        api_key: Optional[str] = None,
        force_refresh: bool = False
    ) -> List[Dict[str, Any]]:
        """
        Complete workflow: parse ACF + enrich with Steam API.
        
        Args:
            api_key: Optional Steam Web API key
            force_refresh: If True, ignore cache and re-fetch from Steam
            
        Returns:
            List of complete workshop item information as dictionaries
        """
        local_items = self.parse_acf_file()
        
        if not local_items:
            return []
        
        # Try to load from cache if available and not forcing refresh
        cached_data = {}
        if not force_refresh:
            cached_data = self._load_cached_info()
        
        enriched = []
        
        for item in local_items:
            item_id = item.workshop_id
            
            # Use cached data if available
            if item_id in cached_data and not force_refresh:
                cached_item = cached_data[item_id]
                enriched.append(EnrichedWorkshopItem(
                    workshop_id=item_id,
                    mod_name=cached_item.get("mod_name", f"Mod #{item_id}"),
                    author_steam_id_64=cached_item.get("author_steam_id_64", "Unknown"),
                    visibility_status=cached_item.get("visibility_status", "Unknown"),
                    local_metadata={
                        "disk_path": item.path,
                        "size_on_disk_bytes": item.size_on_disk_bytes,
                        "size_on_disk_readable": item.size_on_disk_readable,
                        "last_local_update": item.last_local_update_date
                    },
                    remote_metadata={
                        "server_size_bytes": cached_item.get("server_size_bytes", 0),
                        "server_last_update": cached_item.get("server_last_update", ""),
                        "thumbnail_url": cached_item.get("thumbnail_url", ""),
                        "tags": cached_item.get("tags", []),
                        "description": cached_item.get("description", "")
                    }
                ))
            else:
                # Need to fetch from Steam API
                remote_info = self.steam_api.get_item_details(item_id) if api_key or hasattr(self, 'steam_api') else None
                
                if not hasattr(self, 'steam_api'):
                    # Create temp steam_api instance
                    steam_api_temp = SteamWorkshopAPI(api_key=api_key)
                    remote_info = steam_api_temp.get_item_details(item_id)
                else:
                    remote_info = self.steam_api.get_item_details(item_id) if api_key else None
                
                if remote_info and remote_info.title and remote_info.title != "Unknown":
                    mod_name = remote_info.title
                    author = remote_info.author or "Unknown"
                    
                    # Update cache with new data
                    cached_data[item_id] = {
                        "mod_name": mod_name,
                        "author_steam_id_64": author,
                        "visibility_status": "Public",
                        "server_size_bytes": remote_info.file_size,
                        "server_last_update": datetime.fromtimestamp(remote_info.updated).isoformat() if remote_info.updated else "",
                        "thumbnail_url": remote_info.preview_url or "",
                        "tags": remote_info.tags,
                        "description": remote_info.description
                    }
                else:
                    mod_name = f"Mod #{item_id}"
                    author = "Unknown"
                
                # Handle case when API fails - use local data only
                server_update = ""
                thumbnail_url = ""
                tags = []
                description = ""
                
                if remote_info:
                    server_update = datetime.fromtimestamp(remote_info.updated).isoformat() if remote_info.updated else ""
                    thumbnail_url = remote_info.preview_url or ""
                    tags = remote_info.tags or []
                    desc = remote_info.description
                    description = desc[:500] + "..." if desc and len(desc) > 500 else (desc or "")
                
                enriched.append(EnrichedWorkshopItem(
                    workshop_id=item_id,
                    mod_name=mod_name,
                    author_steam_id_64=author,
                    visibility_status="Public",
                    local_metadata={
                        "disk_path": item.path,
                        "size_on_disk_bytes": item.size_on_disk_bytes,
                        "size_on_disk_readable": item.size_on_disk_readable,
                        "last_local_update": item.last_local_update_date
                    },
                    remote_metadata={
                        "server_size_bytes": remote_info.file_size if remote_info else 0,
                        "server_last_update": server_update,
                        "thumbnail_url": thumbnail_url,
                        "tags": tags,
                        "description": description
                    }
                ))
        
        # Save cache after processing
        self._save_cached_info(cached_data)
        
        return [asdict(item) if hasattr(item, '__dataclass_fields__') else item.__dict__ 
                for item in enriched]


# Backward compatibility alias
WorkshopACFParser = LocalWorkshopMetadataParser