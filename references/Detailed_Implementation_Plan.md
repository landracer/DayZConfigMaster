# DayZConfigMaster - Comprehensive UI/Configuration Refactoring Plan

## Executive Summary

This document outlines a detailed implementation plan to address critical architectural issues in the DayZConfigMaster application. The plan addresses five major areas: **Server Config tab**, **Server Control tab**, **Tab consolidation/reorganization**, **Map/Mod parsing improvements**, and **UI scrolling fixes**.

---

## Current Architecture Analysis

### Existing Tab Structure (Current State)

1. **Workspace** - Overview page
2. **In-Game Settings** - Economy Core, Limits Definitions, Random Presets, Spawnable Types
3. **Trader Config** - Trader configurations
4. **Server Config** - serverDZ.cfg configuration
5. **Advanced Settings** - Gameplay & Environment settings
6. **Server Control** - Server management (single/multi-instance)
7. **House Cleaning & Scheduling** - Restart scheduling & memory management
8. **Modding Configuration** - Mod setup and Workshop integration
9. **Ban List & Players** - RCon-based player management
10. **Effects & Triggers** - Various config file types (Limits, Events, Effect Areas, etc.)
11. **Steam Workshop** - Workshop item search and download

### Identified Issues

| Issue | Impact | Priority |
|-------|--------|----------|
| Maps not properly connected to mods/Workshop | Incorrect mod paths generated | HIGH |
| Mod Support Configuration section disorganized | Confusing user experience | HIGH |
| Multi-instance requires separate config page | Incomplete implementation | HIGH |
| Ban List & Players on separate tab | Fragmented workflow | MEDIUM |
| Effects & Triggers on separate tab | Duplicated configuration options | LOW |
| Scrolling not working properly | UI usability issues | CRITICAL |

---

## Detailed Implementation Plan

### Phase 1: Server Config Tab Refactoring

#### 1.1 Map Discovery and Parsing Improvements

**Current Problem:** Maps are scanned from local directories but Workshop maps aren't properly parsed or distinguished from mods.

**Solution Architecture:**

```
Map Discovery Flow:
1. Scan Stock Directories (mpmissions folder)
   - dayzOffline.[mapname] → extract [mapname]
2. Scan Workshop Content Directory
   - For each workshop folder, check for .map files in subdirectories
   - If found, treat as a MAP (not mod)
   - If no .map files but contains config.cpp, treat as MOD

Parsing Logic:
- Maps: Folder structure with dayzOffline.[mapname] or [mapname].pbo/.map
- Mods: Folders with @ModName containing config.cpp but NO map files
```

**Implementation Steps:**

1. **Enhance `_get_available_maps()` method:**
   - Parse stock maps from `mpmissions` directory
   - Parse Workshop maps by checking for `.map` file presence
   - Return separate lists: `stock_maps`, `workshop_maps`

2. **Create new parsing module:** `dayzconfigmaster/config/map_parser.py`
   ```python
   class MapParser:
       def parse_stock_maps(dayz_server_path) -> List[str]
       def parse_workshop_maps(workshop_content_path) -> Dict[str, str]
           # Returns {workshop_id: map_name}
   ```

3. **Update UI dropdown population:**
   - Combine stock + workshop maps
   - Sort alphabetically
   - Auto-detect from DayZ path if configured

#### 1.2 Mod Parsing and Separation

**Current Problem:** Workshop items show mods and maps together, making it impossible to distinguish between them for configuration.

**Solution Architecture:**

```
Workshop Item Classification:
- If folder contains .map files OR dayzOffline.[name] → MAP
- If folder contains config.cpp with no map references → MOD
- Store both in separate UI sections
```

**Implementation Steps:**

1. **Add classification to `LocalWorkshopItem`:**
   ```python
   @dataclass
   class WorkshopClassification:
       workshop_id: str
       item_name: str
       item_type: str  # 'map' or 'mod'
       path: str
       tags: List[str]
   ```

2. **Update `_local_workshop_items` to store classified data:**
   ```python
   self._maps_list = []      # Only map items
   self._mods_list = []      # Only mod items
   self._all_workshop = []   # All items (for search)
   ```

3. **Create UI components for each:**
   - Map dropdown above "Map Name" field
   - Mod checkboxes section in Server Config tab

#### 1.3 Mod Support Configuration Section Cleanup

**Current State:** Messy grid layout with mod paths mixed with workshop settings.

**Target State:** Well-organized sections:

```
Mod Support Configuration (Collapsible GroupBox):
├── Map Selection
│   ├── [Dropdown] Select Map: [chernarusplus ▼]
│   └── [Info] Source: Workshop item #123456789
│
├── Mod Selection (Multi-select)
│   ├── [Treeview with checkboxes]
│   │   - ☑ @CBA_A3 (Community Base Addons)
│   │   - ☐ @MasPuertas (Additional Doors)
│   │   - ☐ @JSRS (Joint Sounds Realism System)
│   └── [Browse...] [Remove Selected] buttons
│
├── Workshop Integration
│   ├── [Path input] Workshop Directory: /home/user/.steam/...
│   └── [Scan] button to refresh local items
│
└── Verification Settings
    ├── Verify Signatures: [Dropdown: 0 (Off) | 1 (Debug) | 2 (Verify)]
    └── Force Same Build: [Checkbox]
```

**Implementation Steps:**

1. **Add `@` prefix detection for mod paths**
2. **Parse Workshop items into categorized lists**
3. **Create checkbox-based mod selection component**
4. **Generate mod path string from selected mods**

---

### Phase 2: Multi-Instance Configuration

#### 2.1 Instance Count Settings

**Current State:** Spinbox to set instance count exists but no per-instance config page.

**Solution Architecture:**

```
Multi-Instance Page (embedded in Server Control):
├── Enable Multi-Instance Mode [Checkbox]
├── Instance Count: [Spinbox: 1-9]
│
└── Per-Instance Configuration (repeats for each instance):
    ┌─ Instance 1 Config ─────────────────────┐
    │ Instance ID:     [1]                    │
    │ Config File:     [/dayz/server1.cfg]    │
    │ Root Folder:     [/dayz/server1]        │
    │ Game Port:       [2302]                 │
    │ Query Port:      [2303]                 │
    │ Steam Port:      [2304]                 │
    │ Map Name:        [chernarusplus ▼]     │
    │ Profile Path:    [/dayz/server1/profile]│
    │ Mod Paths:       [@Mod1;@Mod2]          │
    └─────────────────────────────────────────┘

    ┌─ Instance 2 Config ─────────────────────┐
    ... (same structure) ...
    └─────────────────────────────────────────┘
```

**Implementation Steps:**

1. **Create `_create_instance_config_page()` method**
   - Generates per-instance configuration rows dynamically
   - Each row is a LabelFrame containing all instance settings

2. **Add validation for unique ports**
   ```python
   def validate_instance_ports(self) -> bool:
       # Check all game/query/steam ports are unique
   ```

3. **Generate separate .cfg files per instance**
   - Use instance ID to create unique config paths
   - Auto-generate default paths

#### 2.2 Config File Generation

**Implementation Strategy:**

```python
def generate_instance_cfgs(self) -> Dict[int, str]:
    """Generate serverDZ.cfg content for each instance"""
    configs = {}
    
    for instance in self._instance_vars:
        instance_id = int(instance["id"].get())
        
        cfg_content = f"""// DayZ Server Configuration (serverDZ_{instance_id}.cfg)
// Generated by DayzConfigMaster
//
// Instance ID: {instance_id}
// Generated: {datetime.now().isoformat()}

hostname = "{self.config.name}_Instance{instance_id}";
port = {instance["game_port"].get()};
queryPort = {instance["query_port"].get()};
maxPlayers = {self.config.max_players};
password = "{self.config.password}";

mapSize = 2000;
timeSettings = "2020/4/1/8/00";
weather = 0.5;

verifySignatures = {self.config.verify_signatures};
forceSameBuild = {self.config.force_same_build};

mod = "{';'.join(self.selected_mod_paths)}";

// Profile settings
profiles = "./profile{instance_id}";

// Log settings
adminLog = true;
logMemory = true;
"""
        
        configs[instance_id] = cfg_content
    
    return configs
```

---

### Phase 3: Tab Consolidation

#### 3.1 Ban List & Players → Server Control Tabs

**Current State:** "Ban List & Players" tab exists separately from Server Control.

**Proposed Structure:**

```
Server Control Tab (Consolidated):
├── Single Server Sub-tab
│   └── [No changes - keep current]
│
├── Multi-Instance Sub-tab
│   └── [No changes - keep current]
│
└── Inner Tabs:
    ├── General Settings     (current single/multi controls)
    ├── Mod Selection        (new - mod checkboxes per instance)
    ├── Ban List & Players   (new - RCon integration moved here)
    │   ├── RCon Configuration
    │   ├── Add/Edit Bans
    │   └── Connected Players list
    └── Logs & Diagnostics   (moved from main Server Control section)
```

**Implementation Steps:**

1. **Create inner notebook in Server Control tab**
2. **Move Ban List UI components to new "Ban List" sub-tab**
3. **Keep RCon connection state across tabs**

#### 3.2 Effects & Triggers → Server Config Tabs

**Current State:** Separate "Effects & Triggers" tab with multiple sub-tabs.

**Analysis:** These ARE part of the server configuration and should be consolidated.

**Proposed Structure:**

```
Server Config Tab (Enhanced):
├── Basic Settings
├── Map Selection
├── Mod Selection
└── [NEW Inner Notebook]
    ├── Performance & Limits      (was "Effects & Triggers > Limits")
    ├── Event Spawns              (was "Effect & Triggers > Event Spawns")
    ├── Effect Areas              (was "Effect & Triggers > Effect Areas")
    ├── Underground Triggers      (was "Effect & Triggers > Underground Triggers")
    └── Map Config                (Map clusters, dirt types)
```

**Justification:**
- Effects and triggers directly affect server behavior
- Configuration is generated in serverDZ.cfg or related config files
- Consolidation reduces navigation complexity

#### 3.3 Modding Configuration Tab → Server Config

**Current State:** "Modding Configuration" tab with duplicate mod settings.

**Decision:** **Phase out this tab entirely** in favor of proper integration.

**Rationale:**
1. Mod selection is now part of Server Config (map/mod separation)
2. Workshop integration is also in Server Config
3. PBO builder functionality can remain but should be moved to a sub-tab

**Migration Path:**

```
Old Tab → New Structure:
Modding Configuration → Server Config > Mod Selection Sub-Tab
    ├── Verify Signatures       → Already in Server Config
    ├── Force Same Build        → Already in Server Config  
    ├── Workshop Integration    → Already in Server Config
    └── Popular Mods List       → Move to "Info" section
```

---

### Phase 4: Scrolling Implementation

#### 4.1 Current Scrolling Issues

**Analysis:** The `ScrollableFrame` class exists but may not be properly applied.

#### 4.2 Solution Strategy

Apply scrolling consistently across all tabs:

```python
# Pattern to use throughout:
frame = ScrollableFrame(self.notebook)
self.notebook.add(frame, text="Tab Name")

# ALL content goes in frame.frame (not directly in frame)
content_frame = ttk.Frame(frame.frame)
content_frame.pack(fill=tk.BOTH, expand=True)
```

#### 4.3 Tabs Requiring Fix

| Tab | Current State | Required Change |
|-----|---------------|-----------------|
| Server Config | ✅ Has ScrollableFrame | Verify proper usage |
| Server Control | ✅ Has ScrollableFrame | Verify proper usage |
| Ban List & Players | ❓ Check | Add if missing |
| Effects & Triggers | ❓ Check | Add if missing |

---

## Technical Implementation Details

### New Files to Create

1. **`dayzconfigmaster/config/map_parser.py`**
   - Parse stock maps from mpmissions directory
   - Parse Workshop maps by checking for .map files
   - Classify workshop items as map/mod

2. **`dayzconfigmaster/gui/instance_config.py`**
   - Per-instance configuration UI components
   - Port validation logic
   - CFG generation per instance

3. **`dayzconfigmaster/config/cfg_generator.py`**
   - Generate serverDZ.cfg content
   - Support for multi-instance configs
   - Mod path formatting

### Modified Files

1. **`dayzconfigmaster/gui/app.py`** (Major refactoring)
2. **`dayzconfigmaster/workshop/local_parser.py`** (Enhance classification)
3. **`dayzconfigmaster/server/process_controller.py`** (Minor updates)

---

## User Workflow Changes

### Before (Current State)

1. Open "Server Config" tab
2. See disorganized mod section with Workshop items mixed
3. Map selection may not work properly
4. Multi-instance requires separate configuration page
5. Ban List is on separate tab → need to jump between tabs
6. Effects/Triggers are separate from config

### After (New State)

1. Open "Server Config" tab
2. Map dropdown shows all available maps (stock + Workshop)
3. Mod checkboxes list mods only (Workshop items properly classified)
4. Multi-instance is in Server Control with proper per-instance configs
5. Ban List & Players integrated into Server Control inner tabs
6. Effects & Triggers embedded in Server Config as sub-tabs

---

## Testing Plan

### Unit Tests to Add

1. **Map Parser Tests:**
   - Parse stock maps from mpmissions directory
   - Classify Workshop items correctly
   
2. **Mod Classification Tests:**
   - Folder with .map files → classified as "map"
   - Folder with config.cpp only → classified as "mod"

3. **Multi-Instance Tests:**
   - Generate unique ports per instance
   - Validate unique constraints

### Integration Tests

1. Load workshop item, verify map/mod classification
2. Select mods, verify mod path string generation
3. Configure multi-instance, generate configs
4. Switch between tabs, verify scrolling works

---

## Risk Assessment

| Risk | Impact | Mitigation |
|------|--------|------------|
| Breaking existing functionality | HIGH | Thorough testing before deployment |
| Workshop API changes | MEDIUM | Use fallback parsing (folder scan) |
| Scrolling issues persist | LOW | Comprehensive review of ScrollableFrame usage |

---

## Implementation Timeline

**Phase 1: Server Config Tab Refactoring**
- Week 1: Map discovery + Workshop parsing improvements
- Week 2: Mod separation + UI cleanup

**Phase 2: Multi-Instance Configuration**
- Week 3: Instance page generation
- Week 4: Config file generation per instance

**Phase 3: Tab Consolidation**
- Week 5: Ban List integration
- Week 6: Effects & Triggers embedding

**Phase 4: Scrolling Fixes**
- Week 7: Apply ScrollableFrame consistently
- Week 8: Final testing and documentation

---

## Phase 5: Detailed Code Implementation Examples

### 5.1 Map/Mod Classification Logic

**New File:** `dayzconfigmaster/config/map_parser.py`

```python
"""
Map and Mod Parser for DayZ Workshop Items

This module distinguishes between:
- MAPS: Files/folders containing .map files or dayzOffline.[name]
- MODS: Folders with config.cpp but no map references
"""

from pathlib import Path
from typing import Dict, List, Tuple

class MapModClassifier:
    """Classify workshop items as maps or mods."""
    
    def classify_workshop_item(self, item_path: Path) -> str:
        """
        Determine if a workshop folder is a MAP or MOD.
        
        Args:
            item_path: Path to the workshop item folder
            
        Returns:
            'map' or 'mod'
        """
        # Check for .map files (definitive map indicator)
        map_files = list(item_path.rglob("*.map"))
        if map_files:
            return "map"
        
        # Check for dayzOffline.[name] pattern
        for item in item_path.iterdir():
            if item.is_dir() and item.name.startswith("dayzOffline."):
                return "map"
        
        # Check for config.cpp (mod indicator)
        config_file = item_path / "config.cpp"
        if config_file.exists():
            content = config_file.read_text(encoding='utf-8', errors='ignore')
            # If config doesn't reference a map, it's a mod
            if 'map' not in content.lower() and 'mpmissions' not in content.lower():
                return "mod"
        
        # Default to mod if unsure
        return "mod"


class WorkshopMapParser:
    """Parse workshop directories for map information."""
    
    def __init__(self, workshop_content_path: str):
        self.path = Path(workshop_content_path)
    
    def get_workshop_maps(self) -> Dict[str, Tuple[str, str]]:
        """
        Get all maps from workshop content.
        
        Returns:
            {workshop_id: (folder_name, item_name)}
        """
        maps = {}
        
        if not self.path.exists():
            return maps
        
        for folder in self.path.iterdir():
            if not folder.is_dir():
                continue
            
            # Check if this is a map
            classifier = MapModClassifier()
            classification = classifier.classify_workshop_item(folder)
            
            if classification == "map":
                # Extract display name from folder or config
                maps[folder.name] = (folder.name, self._extract_map_name(folder))
        
        return maps
    
    def _extract_map_name(self, folder: Path) -> str:
        """Extract human-readable map name."""
        # Check for dayzOffline.[name]
        for item in folder.iterdir():
            if item.is_dir() and item.name.startswith("dayzOffline."):
                return item.name[len("dayzOffline."):]
        
        return folder.name
```

### 5.2 Enhanced Mod Support UI

**Modified File:** `dayzconfigmaster/gui/app.py` (Server Config Tab)

```python
def _create_mod_support_section(self, parent_frame: ttk.Frame) -> None:
    """Create the enhanced mod support configuration section."""
    
    # Create collapsible frame for mod support
    mod_frame = ttk.LabelFrame(parent_frame, text="Mod Support Configuration", padding=10)
    mod_frame.pack(fill=tk.X, padx=5, pady=10)
    
    row = 0
    
    # === Map Selection Section ===
    map_section = ttk.LabelFrame(mod_frame, text="Map Selection", padding=8)
    map_section.grid(row=row, column=0, columnspan=3, sticky=tk.W+tk.E, pady=(0, 10))
    
    ttk.Label(map_section, text="Select Map:").pack(side=tk.LEFT, padx=(5, 10))
    
    self._map_combo = ttk.Combobox(
        map_section, 
        width=40,
        values=self._get_available_maps()
    )
    self._map_combo.pack(side=tk.LEFT)
    
    # Display workshop info
    self._workshop_map_info = ttk.Label(map_section, text="", foreground="gray")
    self._workshop_map_info.pack(side=tk.LEFT, padx=(10, 5))
    
    row += 1
    
    # === Mod Selection Section (Treeview with Checkboxes) ===
    mod_selection_frame = ttk.LabelFrame(mod_frame, text="Mod Selection", padding=8)
    mod_selection_frame.grid(row=row, column=0, columnspan=3, sticky=tk.W+tk.E, pady=(0, 10))
    
    # Frame for treeview with scrollbar
    tree_container = ttk.Frame(mod_selection_frame)
    tree_container.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
    
    # Treeview with checkbox column
    mod_tree_scroll_y = ttk.Scrollbar(tree_container, orient=tk.VERTICAL)
    mod_tree_scroll_x = ttk.Scrollbar(tree_container, orient=tk.HORIZONTAL)
    
    self._mod_tree = ttk.Treeview(
        tree_container,
        columns=("Status", "ModName"),
        show="headings",
        yscrollcommand=mod_tree_scroll_y.set,
        xscrollcommand=mod_tree_scroll_x.set,
        height=10
    )
    
    mod_tree_scroll_y.config(command=self._mod_tree.yview)
    mod_tree_scroll_x.config(command=self._mod_tree.xview)
    
    self._mod_tree.column("Status", width=50, anchor=tk.CENTER)
    self._mod_tree.column("ModName", width=350)
    
    self._mod_tree.heading("Status", text=" ☑/☐ ")
    self._mod_tree.heading("ModName", text="Mod Name")
    
    mod_tree_scroll_y.pack(side=tk.RIGHT, fill=tk.Y)
    mod_tree_scroll_x.pack(side=tk.BOTTOM, fill=tk.X)
    self._mod_tree.pack(fill=tk.BOTH, expand=True)
    
    # Bind click to toggle checkbox
    self._mod_tree.bind("<Button-1>", self._on_mod_tree_click)
    
    row += 1
    
    # === Mod Buttons Row ===
    mod_button_frame = ttk.Frame(mod_selection_frame)
    mod_button_frame.pack(fill=tk.X, pady=(5, 0))
    
    ttk.Button(
        mod_button_frame,
        text="+ Add Mod",
        command=self._browse_mod_directory
    ).pack(side=tk.LEFT, padx=2)
    
    ttk.Button(
        mod_button_frame,
        text="- Remove Selected",
        command=self._remove_selected_mods
    ).pack(side=tk.LEFT, padx=2)
    
    ttk.Button(
        mod_button_frame,
        text="Refresh List",
        command=self._refresh_mod_list
    ).pack(side=tk.LEFT, padx=2)
    
    row += 1
    
    # === Workshop Integration Section ===
    workshop_section = ttk.LabelFrame(mod_frame, text="Workshop Integration", padding=8)
    workshop_section.grid(row=row, column=0, columnspan=3, sticky=tk.W+tk.E)
    
    ttk.Label(workshop_section, text="Workshop Directory:").grid(
        row=0, column=0, sticky=tk.W, padx=(5, 10), pady=2
    )
    
    self._workshop_dir_var = tk.StringVar()
    workshop_entry = ttk.Entry(
        workshop_section,
        width=60,
        textvariable=self._workshop_dir_var
    )
    workshop_entry.grid(row=0, column=1, sticky=tk.W+tk.E, pady=2)
    
    ttk.Button(
        workshop_section,
        text="Browse",
        command=self._browse_workshop_directory
    ).grid(row=0, column=2, padx=(5, 5), pady=2)
    
    ttk.Button(
        workshop_section,
        text="🔄 Scan Workshop",
        command=self._scan_local_workshop_items
    ).grid(row=1, column=0, sticky=tk.W, pady=(5, 5))
    
    self._workshop_status_label = ttk.Label(workshop_section, text="", foreground="gray")
    self._workshop_status_label.grid(row=1, column=1, sticky=tk.W, pady=(5, 5), columnspan=2)
    
    # Configure grid weights
    mod_frame.columnconfigure(1, weight=1)
```

### 5.3 Mod Tree Checkbox Logic

```python
def _on_mod_tree_click(self, event) -> None:
    """Toggle checkbox state when clicking a row."""
    region = self._mod_tree.identify("region", event.x, event.y)
    
    if region == "cell":
        item_id = self._mod_tree.identify_row(event.y)
        if item_id:
            current_status = self._mod_tree.item(item_id, "values")[0]
            new_status = " ☐ " if current_status == " ☑ " else " ☑ "
            
            # Update the item
            values = list(self._mod_tree.item(item_id, "values"))
            values[0] = new_status
            self._mod_tree.item(item_id, values=values)
            
            # Track selected mods
            mod_name = values[1]
            if new_status == " ☑ ":
                if mod_name not in self._selected_mods:
                    self._selected_mods.add(mod_name)
            else:
                if mod_name in self._selected_mods:
                    self._selected_mods.remove(mod_name)

def _get_selected_mod_paths(self) -> str:
    """Get semicolon-separated list of selected mod paths."""
    mods = []
    for item_id in self._mod_tree.get_children():
        values = self._mod_tree.item(item_id, "values")
        if values[0] == " ☑ " and len(values) > 1:
            mod_name = values[1]
            # Add @ prefix if not present
            if not mod_name.startswith("@"):
                mod_name = "@" + mod_name
            mods.append(mod_name)
    
    return ";".join(mods)

def _refresh_mod_list(self) -> None:
    """Refresh the mod list from workshop directory."""
    workshop_dir = self._workshop_dir_var.get().strip()
    
    if not workshop_dir or not Path(workshop_dir).exists():
        messagebox.showwarning(
            "Workshop Directory",
            "Please set a valid Workshop directory first."
        )
        return
    
    # Clear existing items
    for item in self._mod_tree.get_children():
        self._mod_tree.delete(item)
    
    self._selected_mods.clear()
    
    # Scan workshop content folder
    content_path = Path(workshop_dir) / "content" / "223350"
    
    if not content_path.exists():
        messagebox.showinfo("No Workshop Items", 
                          f"No workshop content found at:\n{content_path}")
        return
    
    classifier = MapModClassifier()
    
    for folder in content_path.iterdir():
        if not folder.is_dir():
            continue
        
        classification = classifier.classify_workshop_item(folder)
        
        # Only show mods, not maps
        if classification == "mod":
            # Add with unchecked checkbox
            mod_name = folder.name
            self._mod_tree.insert("", tk.END, values=(" ☐ ", mod_name))
    
    self._workshop_status_label.config(
        text=f"Loaded {len(list(content_path.iterdir()))} workshop items"
    )
```

### 5.4 Multi-Instance Configuration

**Modified File:** `dayzconfigmaster/gui/app.py` (Server Control Tab)

```python
def _create_multi_instance_page(self, parent_frame: ttk.Frame) -> None:
    """Create the multi-instance configuration page."""
    
    # Enable mode checkbox
    enable_frame = ttk.LabelFrame(parent_frame, text="Multi-Instance Settings", padding=10)
    enable_frame.pack(fill=tk.X, padx=5, pady=5)
    
    ttk.Checkbutton(
        enable_frame,
        text="Enable Multi-Instance Mode",
        variable=self._multi_instance_enabled_var,
        command=self._on_multi_instance_toggled
    ).grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
    
    ttk.Label(enable_frame, text="Number of Instances:").grid(
        row=0, column=1, sticky=tk.W, padx=(30, 5), pady=2
    )
    
    spinbox = ttk.Spinbox(
        enable_frame,
        from_=1, to=9,
        width=5,
        textvariable=self._instance_count_var
    )
    spinbox.grid(row=0, column=2, sticky=tk.W, padx=5, pady=2)
    
    # Instance configuration container
    config_container = ttk.LabelFrame(parent_frame, text="Per-Instance Configuration", padding=10)
    config_container.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
    
    self._instance_config_container = ttk.Frame(config_container)
    self._instance_config_container.pack(fill=tk.BOTH, expand=True)
    
    # Build initial rows
    self._build_instance_rows()

def _build_instance_rows(self) -> None:
    """Build configuration rows for each instance."""
    # Clear existing
    for widget in self._instance_config_container.winfo_children():
        widget.destroy()
    
    self._instance_vars.clear()
    
    count = int(self._instance_count_var.get())
    
    for i in range(1, count + 1):
        row_frame = ttk.LabelFrame(
            self._instance_config_container,
            text=f"Instance {i} Configuration",
            padding=8
        )
        row_frame.pack(fill=tk.X, padx=5, pady=(0, 10))
        
        # Instance ID (read-only)
        ttk.Label(row_frame, text="Instance ID:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        instance_id_var = tk.StringVar(value=str(i))
        ttk.Entry(row_frame, width=8, textvariable=instance_id_var, state="readonly").grid(
            row=0, column=1, sticky=tk.W, padx=5, pady=2
        )
        
        # Config File
        ttk.Label(row_frame, text="Config File:").grid(row=1, column=0, sticky=tk.W, padx=5, pady=2)
        config_var = tk.StringVar(value=f"/dayz/server{i}.cfg")
        config_entry = ttk.Entry(row_frame, width=45, textvariable=config_var)
        config_entry.grid(row=1, column=1, sticky=tk.W+tk.E, padx=5, pady=2)
        
        # Root Folder
        ttk.Label(row_frame, text="Root Folder:").grid(row=2, column=0, sticky=tk.W, padx=5, pady=2)
        root_var = tk.StringVar(value=f"/dayz/server{i}")
        root_entry = ttk.Entry(row_frame, width=45, textvariable=root_var)
        root_entry.grid(row=2, column=1, sticky=tk.W+tk.E, padx=5, pady=2)
        
        # Game Port
        ttk.Label(row_frame, text="Game Port:").grid(row=3, column=0, sticky=tk.W, padx=5, pady=2)
        game_port_var = tk.StringVar(value=str(2302 + (i-1)*100))
        port_entry = ttk.Entry(row_frame, width=8, textvariable=game_port_var)
        port_entry.grid(row=3, column=1, sticky=tk.W, padx=5, pady=2)
        
        # Query Port
        ttk.Label(row_frame, text="Query Port:").grid(row=4, column=0, sticky=tk.W, padx=5, pady=2)
        query_port_var = tk.StringVar(value=str(2303 + (i-1)*100))
        ttk.Entry(row_frame, width=8, textvariable=query_port_var).grid(
            row=4, column=1, sticky=tk.W, padx=5, pady=2
        )
        
        # Map dropdown (from Server Config)
        ttk.Label(row_frame, text="Map:").grid(row=5, column=0, sticky=tk.W, padx=5, pady=2)
        map_var = tk.StringVar()
        map_combo = ttk.Combobox(row_frame, width=30, textvariable=map_var)
        self._populate_maps(map_combo)  # Reuse method from Server Config
        map_combo.grid(row=5, column=1, sticky=tk.W, padx=5, pady=2)
        
        # Profile Path
        ttk.Label(row_frame, text="Profile Path:").grid(row=6, column=0, sticky=tk.W, padx=5, pady=2)
        profile_var = tk.StringVar(value=f"/dayz/server{i}/profile")
        ttk.Entry(row_frame, width=45, textvariable=profile_var).grid(
            row=6, column=1, sticky=tk.W+tk.E, padx=5, pady=2
        )
        
        # Mod Paths
        ttk.Label(row_frame, text="Mod Paths:").grid(row=7, column=0, sticky=tk.W, padx=5, pady=2)
        mod_var = tk.StringVar()
        ttk.Entry(row_frame, width=45, textvariable=mod_var).grid(
            row=7, column=1, sticky=tk.W+tk.E, padx=5, pady=2
        )
        
        # Store references
        self._instance_vars.append({
            "id": instance_id_var,
            "config_file": config_var,
            "root_folder": root_var,
            "game_port": game_port_var,
            "query_port": query_port_var,
            "map": map_var,
            "profile": profile_var,
            "mod_paths": mod_var
        })
```

### 5.5 Tab Consolidation - Inner Notebook in Server Control

**Modified File:** `dayzconfigmaster/gui/app.py`

```python
def _create_server_control_tab(self) -> None:
    """Create the unified Server Control tab with inner tabs."""
    
    # ... existing setup code ...
    
    # Main container
    main_container = ttk.Frame(server_control_frame.frame)
    main_container.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)
    
    # Create inner notebook for sections
    control_notebook = ttk.Notebook(main_container)
    control_notebook.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
    
    # === Tab 1: General Settings (Single/Multi) ===
    general_frame = ttk.Frame(control_notebook)
    control_notebook.add(general_frame, text="General Settings")
    self._create_general_server_controls(general_frame)
    
    # === Tab 2: Mod Selection (Per-Instance) ===
    mod_selection_frame = ttk.Frame(control_notebook)
    control_notebook.add(mod_selection_frame, text="Mod Selection")
    self._create_mod_selection_tab(mod_selection_frame)
    
    # === Tab 3: Ban List & Players ===
    ban_list_frame = ttk.Frame(control_notebook)
    control_notebook.add(ban_list_frame, text="Ban List & Players")
    self._create_ban_list_tab_content(ban_list_frame)  # Move from separate tab
    
    # === Tab 4: Logs & Diagnostics ===
    logs_frame = ttk.Frame(control_notebook)
    control_notebook.add(logs_frame, text="Logs & Diagnostics")
    self._create_logs_and_diagnostics(logs_frame)
```

### 5.6 Scrolling Implementation Fix

**Pattern to Apply Across All Tabs:**

```python
def _create_tab_with_scroll(self, notebook, tab_name: str) -> ttk.Frame:
    """Create a properly scrollable tab."""
    
    # Outer frame with ScrollableFrame wrapper
    outer_frame = ScrollableFrame(notebook)
    notebook.add(outer_frame, text=tab_name)
    
    # ALL content goes in outer_frame.frame (not directly in outer_frame)
    content_container = ttk.Frame(outer_frame.frame)
    content_container.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)
    
    return content_container
```

### 5.7 Integration with Server Config

```python
def _bind_mod_selection_to_server_config(self) -> None:
    """Link mod selection between Server Config and Multi-Instance."""
    
    def on_mod_select(event):
        # When mods are selected in Server Config, update instance configs
        self._sync_mods_to_instances()
    
    if hasattr(self, '_mod_tree'):
        self._mod_tree.bind("<<TreeviewSelect>>", on_mod_select)

def _sync_mods_to_instances(self) -> None:
    """Sync mod selection to all instance configurations."""
    if not hasattr(self, '_instance_vars') or not self._instance_vars:
        return
    
    selected_mods = self._get_selected_mod_paths()
    
    for instance in self._instance_vars:
        if "mod_paths" in instance:
            instance["mod_paths"].set(selected_mods)
```

## Conclusion

This refactoring plan addresses all major architectural issues identified:

1. ✅ Maps properly connected to mods/Workshop
2. ✅ Mod selection with checkboxes
3. ✅ Multi-instance configuration support
4. ✅ Tab consolidation (Ban List, Effects & Triggers)
5. ✅ Scrolling fixes throughout

The new architecture is cleaner, more maintainable, and provides a better user experience.
