# Workshop API Reference

The Workshop API module provides robust integration with Steam Workshop for downloading and managing DayZ mods.

## Overview

This module provides:
- **Steam Web API** client using public endpoints (no API key required)
- **SteamCMD integration** for anonymous workshop downloads
- **Unified manager** with browser fallback for manual subscription

## Classes

### SteamWorkshopAPI

Robust Steam Workshop API client using public endpoints.

```python
from dayzconfigmaster.workshop.api import SteamWorkshopAPI

api = SteamWorkshopAPI()
```

#### Methods

##### `get_item_details(item_id) -> Optional[WorkshopItemInfo]`

Get detailed information about a workshop item using the public GetPublishedFileDetails endpoint (no API key required).

```python
info = api.get_item_details("123456789")
if info:
    print(f"Title: {info.title}")
    print(f"Subscribers: {info.subscribers:,}")
```

##### `search_items(query, app_id=221100, page=1, count=50) -> Tuple[bool, List[WorkshopItemInfo]]`

Search workshop items using public endpoints.

```python
success, results = api.search_items("CUP Units", count=20)
for item in results:
    print(f"{item.id}: {item.title}")
```

##### `download_item(item_id, install_dir=None, use_steamcmd=True, visible_console=False) -> Tuple[bool, str]`

Download a workshop item using steamcmd by default, or open the workshop page for manual subscription.

```python
success, message = api.download_item("123456789")
print(message)
```

### WorkshopManager

Unified workshop management combining Steam Web API and steamcmd.

```python
from dayzconfigmaster.workshop.api import WorkshopManager

manager = WorkshopManager("/path/to/projects")
```

#### Methods

##### `download_item(item_id) -> Tuple[bool, str]`

Download a workshop item using steamcmd. Falls back to opening the workshop page for manual subscription if steamcmd fails.

```python
success, message = manager.download_item("123456789")
```

##### `search(query) -> Tuple[bool, List[WorkshopItemInfo]]`

Search workshop items via the Steam Web API.

```python
success, results = manager.search("CUP Units")
if success:
    for item in results:
        print(f"{item.id}: {item.title}")
```

##### `get_item_info(item_id) -> Optional[WorkshopItemInfo]`

Get detailed info about a workshop item.

```python
info = manager.get_item_info("123456789")
if info:
    print(f"Title: {info.title}")
    print(f"Rating: {info.rating}/5")
```

## WorkshopItemInfo Dataclass

Contains detailed information about a workshop item:

```python
@dataclass
class WorkshopItemInfo:
    id: str
    title: str
    description: str
    preview_url: str
    subscribers: int
    created: int
    updated: int
    tags: List[str]
    file_size: int
    author: str
    rating: float
```

## Usage Examples

### Basic Workshop Download

```python
from dayzconfigmaster.workshop.api import WorkshopManager

# Initialize manager with your projects directory
manager = WorkshopManager("/path/to/projects")

# Download a workshop item
success, message = manager.download_item("123456789")
if success:
    print(f"Downloaded: {message}")
else:
    print(f"Failed: {message}")
```

### Search and Display Results

```python
from dayzconfigmaster.workshop.api import WorkshopManager

manager = WorkshopManager("/path/to/projects")

# Search for items
success, results = manager.search("CUP Units")

if success and results:
    print(f"Found {len(results)} items:\n")
    
    for item in results[:10]:  # Show first 10
        print(f"{item.id}: {item.title}")
        print(f"  Subscribers: {item.subscribers:,}")
        print(f"  Rating: {'*' * int(item.rating)} ({item.rating:.1f}/5)")
        if item.tags:
            print(f"  Tags: {', '.join(item.tags[:3])}")
        print()
else:
    print("No items found")
```

### Get Item Details

```python
from dayzconfigmaster.workshop.api import WorkshopManager

manager = WorkshopManager("/path/to/projects")

# Get detailed info about an item
info = manager.get_item_info("123456789")

if info:
    print(f"Title: {info.title}")
    print(f"Description: {info.description[:200]}...")
    print(f"Author: {info.author}")
    print(f"Size: {info.file_size / 1024 / 1024:.2f} MB")
    print(f"Created: {info.created}")
    print(f"Updated: {info.updated}")
```

### CLI Integration

The WorkshopManager can also be used via the CLI:

```bash
# Search for workshop items
dzl workshop search "mod name"

# Download an item
dzl workshop download --id 123456789

# Check authentication status
dzl workshop auth

# Upload or update a workshop item
dzl workshop upload --folder ./MyMod --title "My Mod" --id 123456789
```

## Error Handling

The API provides proper error handling:

```python
success, message = manager.download_item("123456789")

if not success:
    # Handle different failure modes
    if "not found" in message.lower():
        print("Item not found on Workshop")
    elif "timeout" in message.lower():
        print("Download timed out - try again")
    else:
        print(f"Error: {message}")
```

## Configuration

### Setting Steam API Key (Optional)

To enable more search results, set your Steam Web API key:

```python
import os
from dayzconfigmaster.workshop.api import SteamWorkshopAPI

os.environ["STEAM_API_KEY"] = "your_api_key_here"

api = SteamWorkshopAPI()
```

Get an API key at: https://steamcommunity.com/dev/apikey

### SteamCMD Configuration

For anonymous workshop downloads, ensure `steamcmd` is installed and available:

```bash
# Linux example
mkdir -p ~/steamcmd
cd ~/steamcmd
wget https://steamcdn-a.akamaihd.net/client/installer/steamcmd_linux.tar.gz
tar -xvzf steamcmd_linux.tar.gz
```

The `WorkshopManager` will use the configured projects root to locate steamcmd, or you can pass an explicit `install_dir` to `download_item`.