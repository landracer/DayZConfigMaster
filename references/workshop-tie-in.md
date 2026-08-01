# DayZConfigMaster - Workshop Integration & Scroll Fix Plan

> **Status:** ✅ COMPLETED — All items below have been implemented and validated.  
> **Last Updated:** 2026-07-11

## Summary of Results

| Requirement | Result |
|-------------|--------|
| Single Workshop directory shared across tabs | ✅ Implemented via `_workshop_directory_cache` in `dayzconfigmaster/gui/app.py` |
| Map dropdown shows stock + workshop maps | ✅ Implemented via `get_all_available_maps()` in `dayzconfigmaster/config/map_parser.py` |
| Mod tree filters out map folders | ✅ Implemented in `MapModClassifier.classify_workshop_item()` |
| Mouse scroll works on content area | ✅ Implemented in `ScrollableFrame` with recursive bindings |
| Multi-instance map dropdown shows workshop maps | ✅ Implemented via `_refresh_all_map_combos()` |
| Mod names parsed from `mod.cpp` / `meta.cpp` | ✅ Implemented in `MapModClassifier` |
| Mod paths populated with full folder paths | ✅ Implemented in `_update_mod_paths_from_selection()` |
| Maps and mods correctly separated | ✅ Validated: 22 maps, 31 mods |

## Critical Issues Identified by User

### Issue #1: Redundant Workshop Directory Inputs
**Problem:** The "Steam Workshop" tab automatically scans local workshop subscriptions via `.acf` files, but other tabs still ask users to manually enter a workshop directory path. This forces duplicate input.

**Current State:**
- Steam Workshop tab: Has `[Scan Local Items]` button that reads from `appworkshop_221100.acf`
- Other tabs: Have manual "Workshop Directory" input fields requiring user to re-enter the same path

**Expected State:**
- ONE workshop directory setting that serves ALL tabs
- Tabs reference this single stored path automatically

### Issue #2: Maps Not Auto-Populated from Workshop Subscriptions  
**Problem:** Workshop subscriptions are scanned but maps aren't being detected and populated into map selection dropdowns.

**Current State:**
- Workshop items classified as "map" or "mod"
- But map dropdown on Server Config still shows only stock maps
- Multi-Instance tab map dropdown also doesn't include Workshop maps

**Expected State:**
- ALL maps (stock + workshop) appear in map dropdowns
- Map detection based on folder contents:
  - `.map` files → MAP
  - `dayzOffline.[name]` folders → MAP  
  - `config.cpp` without map refs → MOD

### Issue #3: Scroll Wheel Doesn't Work Properly
**Problem:** Mouse scroll only works when hovering over scrollbar thumb, not on content area.

**Root Cause:** tkinter canvas doesn't have proper mouse wheel bindings for Linux (uses `<Button-4>` and `<Button-5>` events instead of `<MouseWheel>`).

---

## Implementation Plan

### Fix #1: Centralize Workshop Directory Configuration

#### Step 1.1: Create Settings Store for Workshop Directory

**File to Modify:** `dayzconfigmaster/gui/app.py` (init method)

Add to initialization:
```python
# Workshop directory cache - ONE source of truth
self._workshop_directory_cache = None
```

#### Step 1.2: Add Method to Get Workshop Directory

**File to Modify:** `dayzconfigmaster/gui/app.py`

```python
def _get_workshop_directory(self) -> Optional[str]:
    """
    Get the workshop directory from cached settings.
    
    First checks if we've previously determined the path.
    Falls back to auto-detection via wizard if needed.
    
    Returns:
        Workshop directory path or None if not found
    """
    # Return cached value if available
    if self._workshop_directory_cache:
        return self._workshop_directory_cache
    
    try:
        from ..setup.wizard import SetupWizard
    except ImportError:
        from dayzconfigmaster.setup.wizard import SetupWizard
    
    projects_root = Path.home() / "Documents" / "DayZProjects"
    wizard = SetupWizard(str(projects_root))
    settings = wizard.get_settings()
    
    workshop_dir = settings.get("steam_workshop_dir", "")
    
    if not workshop_dir:
        # Try auto-detection
        detected = wizard.detect_steam_workshop_path()
        if detected:
            self._workshop_directory_cache = detected
            return detected
    
    self._workshop_directory_cache = workshop_dir
    return workshop_dir

def _set_workshop_directory(self, path: str) -> None:
    """Cache the workshop directory for use across all tabs."""
    self._workshop_directory_cache = path
```

#### Step 1.3: Remove Manual Workshop Directory Inputs from Other Tabs

**Tabs to Update:**
- Server Config tab - replace manual input with auto-populated display
- Mod Builder tab - replace with cached value

**Replace this code in Server Config tab:**
```python
# REMOVE: Manual workshop directory input field
```

**With this code:**
```python
# Display current workshop directory (auto-detected)
workshop_dir = self._get_workshop_directory()
if workshop_dir:
    ttk.Label(mod_frame, text=f"Workshop Directory: {workshop_dir}").grid(
        row=row_mod, column=0, columnspan=3, sticky=tk.W, padx=5, pady=2
    )
else:
    # Show only if needed for specific features
    pass  # Only show input when actually required
```

---

### Fix #2: Auto-Populate Maps from Workshop Subscriptions

#### Step 2.1: Enhance Map Discovery Method

**File to Modify:** `dayzconfigmaster/gui/app.py` (`_get_available_maps()` method)

```python
def _get_available_maps(self) -> List[str]:
    """Scan stock and workshop directories for available maps.
    
    Returns combined list of:
    - Stock maps from mpmissions directory
    - Workshop maps (folders containing .map files or dayzOffline.[name])
    """
    maps: Set[str] = set()
    
    # 1. Scan Stock Maps from mpmissions
    stock_map_dirs = [
        Path.home() / ".local" / "share" / "Steam" / "steamapps" / "common" / "DayZServer" / "mpmissions",
        Path.home() / ".steam" / "steam" / "steamapps" / "common" / "DayZServer" / "mpmissions",
    ]
    
    dayz_path = self.dayz_path_var.get().strip() if hasattr(self, "dayz_path_var") else ""
    if dayz_path:
        stock_map_dirs.insert(0, Path(dayz_path) / "mpmissions")
    
    for stock_dir in stock_map_dirs:
        if stock_dir.exists():
            for item in stock_dir.iterdir():
                if item.is_dir() and not item.name.startswith('.'):
                    # Extract map name: dayzOffline.[name] -> [name]
                    if item.name.startswith("dayzOffline."):
                        maps.add(item.name[len("dayzOffline."):])
                    else:
                        maps.add(item.name)
    
    # 2. Scan Workshop Maps
    workshop_dir = self._get_workshop_directory()
    if workshop_dir:
        content_path = Path(workshop_dir) / "content" / "223350"
        if content_path.exists():
            for folder in content_path.iterdir():
                if not folder.is_dir() or folder.name.startswith('.'):
                    continue
                
                # Check if this is a map (has .map files)
                has_map = any(f.suffix.lower() == '.map' for f in folder.rglob("*.map"))
                
                # Or check for dayzOffline.[name] pattern
                has_dayz_offline = any(
                    item.is_dir() and item.name.startswith("dayzOffline.")
                    for item in folder.iterdir()
                )
                
                if has_map or has_dayz_offline:
                    # Extract display name
                    map_name = self._extract_display_map_name(folder)
                    maps.add(map_name)
    
    return sorted(maps, key=str.lower)

def _extract_display_map_name(self, folder: Path) -> str:
    """Extract human-readable map name from workshop folder."""
    # Look for dayzOffline.[name] pattern first
    for item in folder.iterdir():
        if item.is_dir() and item.name.startswith("dayzOffline."):
            return item.name[len("dayzOffline."):]
    
    # Otherwise use folder name (workshop ID is usually descriptive)
    return folder.name
```

#### Step 2.2: Update Map Population Logic

**File to Modify:** `dayzconfigmaster/gui/app.py` (`_populate_maps()` method)

```python
def _populate_maps(self, combo_box) -> None:
    """Populate a map dropdown with available maps from stock and workshop directories."""
    current_value = combo_box.get()
    
    # Get fresh map list
    unique_maps = self._get_available_maps()
    combo_box['values'] = unique_maps
    
    # Preserve selection if still valid
    if current_value in unique_maps:
        combo_box.set(current_value)
    elif unique_maps:
        combo_box.set(unique_maps[0])
```

#### Step 2.3: Add Map Classification Info

```python
def _classify_workshop_item(self, folder: Path) -> str:
    """Determine if workshop folder is a map or mod."""
    # Check for .map files (definitive map indicator)
    if any(f.suffix.lower() == '.map' for f in folder.rglob("*.map")):
        return "map"
    
    # Check for dayzOffline.[name] pattern
    if any(item.is_dir() and item.name.startswith("dayzOffline.") 
           for item in folder.iterdir()):
        return "map"
    
    # Default to mod (has config.cpp without map references)
    return "mod"
```

---

### Fix #3: Scroll Wheel Fixes

#### Step 3.1: Update ScrollableFrame with Mousewheel Bindings

**File to Modify:** `dayzconfigmaster/gui/app.py` (`ScrollableFrame` class)

Add these bindings to the `__init__` method after canvas configuration:

```python
def __init__(self, parent, *args, **kwargs):
    super().__init__(parent, *args, **kwargs)
    
    self.canvas = tk.Canvas(self, borderwidth=0, highlightthickness=0)
    # ... existing scrollbar code ...
    
    # ADD THESE: Mouse wheel bindings for cross-platform scrolling
    # Linux uses Button-4 (scroll up) and Button-5 (scroll down)
    self.canvas.bind("<Button-4>", lambda e: self.canvas.yview_scroll(-1, "units"))
    self.canvas.bind("<Button-5>", lambda e: self.canvas.yview_scroll(1, "units"))
    
    # Windows/Mac uses MouseWheel event
    try:
        # Standard format
        self.canvas.bind("<MouseWheel>", 
                        lambda e: self.canvas.yview_scroll(int(-e.delta/120), "units"))
    except tk.TclError:
        pass
    
    # Add bindings to frame as well (some systems require this)
    self.frame.bind("<Button-4>", lambda e: self.canvas.yview_scroll(-1, "units"))
    self.frame.bind("<Button-5>", lambda e: self.canvas.yview_scroll(1, "units"))
```

---

## Files to Modify

| File | Changes |
|------|---------|
| `dayzconfigmaster/gui/app.py` | - Add `_workshop_directory_cache` variable<br>- Add `_get_workshop_directory()` method<br>- Update `__init__` with mousewheel bindings in ScrollableFrame<br>- Fix `_populate_maps()` to use auto-detected maps |
| `dayzconfigmaster/workshop-tie-in.md` | This documentation file |

---

## Testing Checklist

All items have been verified:

- [x] Workshop directory is auto-filled and shared across tabs
- [x] Map dropdown shows both stock AND workshop maps
- [x] Mod selection correctly filters out map folders
- [x] Mouse scroll wheel works on ANY content area (not just scrollbar)
- [x] Multi-Instance tab map dropdown also shows workshop maps
- [x] Mod names are parsed from `mod.cpp` / `meta.cpp`
- [x] Mod paths use full folder paths
- [x] Maps and mods are correctly separated

### Validation Output

```
40 passed in 0.29s
Available maps: ['Alteria', 'Banov', 'chernarusplus', 'Chiemsee', ...]
Workshop mods: 31
Workshop maps: 22
First 5 tree items are mods only.
```

---

## Implementation Order

1. **First:** Fix ScrollableFrame mousewheel bindings (quick win, no side effects)
2. **Second:** Add `_get_workshop_directory()` method
3. **Third:** Update `_populate_maps()` to use auto-detected Workshop maps
4. **Fourth:** Remove redundant workshop directory inputs from other tabs

---

## Notes for Jr. Dev

- The mousewheel fix uses both `<Button-4>`/`<Button-5>` (Linux) and `<MouseWheel>` (Windows/Mac)
- Workshop directory should only be shown in Server Config if user needs to modify it
- Map classification is based on content: `.map` files = MAP, otherwise MOD