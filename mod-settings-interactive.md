# Mod Settings Interactive Editor - Audit & Fix Plan

> **Canonical document:** This file is the authoritative record of the Mod
> Settings interactive editor fixes. The widget limit described below is an
> intentional defense against X11 `BadAlloc` crashes, not a bug to remove.

## 1. Executive Summary

The "Mod Settings" tab's interactive editor had multiple issues: button placement problems, missing parameters in the parser, duplicate file detection, broken scrolling, and a critical X server crash (BadAlloc) when loading large files on Linux. This document details all fixes applied and remaining work for peer review.

---

## 2. Current Issues Reported

### Issue A: "Scan for Errors" Button Placement
**Status: FIXED (7/24)**  
The "Scan for Errors" button was placed incorrectly on the Mod Settings tab. It has been repositioned to sit next to the other buttons (Refresh, Add File, Repair Tires) in a horizontal button group at the top of the left panel.

### Issue B: `display_value` Parameter Error in parser.py
**Status: FIXED (7/24)**  
The `SettingField.__init__()` method was missing the `display_value` parameter that `_build_single_setting_row()` passes when creating boolean widgets. Added parameter 9 to the signature.

### Issue C: Duplicate Files Across Mods
**Status: PARTIALLY ADDRESSED**  
Deduplication via `resolve()` is in place; visual "(copy)" badges for same files from different mod lookups remain deferred.

### Issue D: Vertical Scrolling Broken for Large Mod Settings
**Status: FIXED (7/25)**  
The interactive editor now wraps all setting rows inside a reusable Canvas with scrollbar. Mouse wheel / trackpad scrolling works on Linux and Windows/macOS. The canvas is preserved across show/hide cycles so its internal state does not need rebuilding.

### Issue E: X11 BadAlloc Crash (Insufficient Resources)
**Status: FIXED (7/25)**  
Two-layer defense against the X server running out of Pixmap resources:

1. **Hard limit** (`_INTERACTIVE_WIDGET_LIMIT = 10`): Files with more than 10 settings are opened in text view before any widgets are created. This prevents the crash entirely for known large files on Linux.
2. **Error recovery** (`_build_single_setting_row_safe()`): Wraps individual widget creation in a try/except block. If a `BadAlloc` TclError occurs mid-build, it gracefully falls back to text view with a status message instead of crashing the whole application.

---

## 3. What's Done

| Item | Status | Details |
|------|--------|---------|
| display_value parameter bug fix | ✅ FIXED | Added to SettingField.__init__() signature |
| "Scan for Errors" button placement | ✅ FIXED | Repositioned next to other buttons in horizontal group |
| Path deduplication in _detect_mod_settings_files | ✅ FIXED | resolve() + seen_paths set prevents duplicates |
| Canvas-based scroll container (Issue D) | ✅ FIXED | Reusable canvas, scrollbar, mouse wheel support |
| Widget count guard (Issue E layer 1) | ✅ FIXED | _INTERACTIVE_WIDGET_LIMIT = 10 |
| Title label BadAlloc protection (Problem 3) | ✅ FIXED | Try/except wraps synchronous title label creation |
| BadAlloc error recovery in rendering (Issue E layer 2) | ✅ FIXED | Fallback to text view on X11 failure during row build |
| Toggle button widget limit check (Issue E layer 3) | ✅ FIXED | Re-checks limit before switching back to interactive mode |
| Cleanup unbind mouse events (Problem 5) | ✅ FIXED | Unbinds Button-4, Button-5, MouseWheel, Enter in cleanup |
| Cleanup winfo_exists guard (Problem 2) | ✅ FIXED | Guards against stale reference to destroyed canvas |

---

## 4. Remaining Problems & Root Causes

### Problem 1: `_on_mod_settings_select` lacks BadAlloc catch at startup

**Status: NOT YET ADDRESSED**  
When a file has fewer than 10 settings, `_on_mod_settings_select` calls `_show_interactive_editor`. That method then builds ~40-50 ttk widgets synchronously (deferred across time slots). If the user has already loaded many other tabs/panels with widgets, the X server may still have low pixmap budget and crash even with just 20 settings.

**Fix Required:** Add a try/except wrapper in `_on_mod_settings_select` around the interactive editor call so that if `BadAlloc` happens at startup time (during widget creation for the first row), it falls back to text view:

```python
try:
    parser = ModConfigParser()
    settings = parser.parse_file(str(path))
    
    if settings and len(settings) > 0:
        if len(settings) > self._INTERACTIVE_WIDGET_LIMIT:
            # ... existing guard logic ...
            return
        
        self._show_interactive_editor(path, settings)
    else:
        # ... existing fallback logic ...
except tk.TclError as exc:
    err_str = str(exc).lower()
    if "badalloc" in err_str or "insufficient" in err_str:
        self._mod_settings_status.config(
            text="X server limit reached; showing text view.",
            foreground="red"
        )
        self._show_text_editor(path)
    else:
        raise  # Re-raise real bugs (e.g. invalid widget paths)
```

### Problem 2: `_cleanup_interactive_editor` canvas existence check — FIXED

**Status: FIXED (7/25)**  
Added `winfo_exists()` guard and stale reference cleanup in `_cleanup_interactive_editor`. The method now checks if the cached canvas has been destroyed externally before attempting to reuse it.

### Problem 3: Title label BadAlloc protection — FIXED

**Status: FIXED (7/25)**  
Wrapped synchronous title label creation in `_show_interactive_editor` with try/except. If creating even that single label triggers a `BadAlloc`, the editor gracefully falls back to text view and sets the view mode variable correctly.

### Problem 4: Canvas resize callback stale refs — NOT YET ADDRESSED

**Status: NOT YET ADDRESSED**  
The `_canvas_resize` and `_update_scrollregion` closures capture `canvas` and `scrollable` by closure variable. If those widgets are destroyed (e.g. during a rapid toggle), the callbacks still fire from old `<Configure>` bindings, causing `TclError`.

**Fix Required:** Add winfo_exists() checks:

```python
def _update_scrollregion(_evt=None):
    try:
        if not canvas.winfo_exists() or not scrollable.winfo_exists():
            return
        bbox = canvas.bbox(tk.ALL)
        if bbox is not None:
            canvas.configure(scrollregion=bbox)
    except tk.TclError:
        pass

def _canvas_resize(_evt=None):
    try:
        if not canvas.winfo_exists() or not scrollable.winfo_exists():
            return
        existing_window = canvas.find_withtag(tk.ALL)
        canvas.itemconfig(existing_window, width=_evt.width if _evt else canvas.winfo_width())
        _update_scrollregion()
    except tk.TclError:
        pass  # Widget destroyed; ignore.
```

### Problem 5: Mouse wheel bindings in cleanup — FIXED

**Status: FIXED (7/25)**  
Added unbind calls for `<Button-4>`, `<Button-5>`, `<MouseWheel>`, and `<Enter>` events in `_cleanup_interactive_editor`. This prevents callbacks on destroyed widgets during rapid toggling.

### Problem 6: `_on_tree_hover` tooltip TclError — NOT YET ADDRESSED

**Status: NOT YET ADDRESSED**  
Hovering rapidly over the mod settings file tree triggers `_on_tree_hover` which calls `self._mod_settings_tree.item(item, "values")`. During a scroll or refresh operation, the item may briefly be in an invalid state.

**Fix Required:** Add try/except around tree item access:

```python
def _on_tree_hover(event):
    try:
        item = self._mod_settings_tree.identify_row(event.y)
        if item:
            full_path = self._mod_settings_tree.item(item, "values")[2]
            mod_name = self._mod_settings_tree.item(item, "values")[0]
            display = self._mod_settings_tree.item(item, "values")[1]
            self._mod_settings_tooltip.configure(
                state=tk.NORMAL,
                text=f"{mod_name}  •  {display}\n{full_path}",
            )
            self._mod_settings_tooltip.configure(state=tk.DISABLED)
        else:
            self._mod_settings_tooltip.configure(state=tk.NORMAL, text="")
            self._mod_settings_tooltip.configure(state=tk.DISABLED)
    except (tk.TclError, IndexError):
        # Tree in invalid state during scroll/refresh; ignore.
        pass
```

---

## 5. Deep-Dive: What It Will Take to Fix the Broken Interactive Mod Settings

### 5.1 Complete Fix for `_on_mod_settings_select` (Problem 1)

**File:** `dayzconfigmaster/gui/app.py`  
**Method:** `_on_mod_settings_select`

The current flow is:
1. User clicks a file in the tree
2. Parser loads and parses settings
3. If ≤10 settings → call `_show_interactive_editor` (deferred widget creation)
4. Otherwise → show text view with status message

**Required change:** Wrap the entire parse+show block in a try/except that catches `tk.TclError` with "badalloc" in the message, and falls back to text view:

```python
try:
    parser = ModConfigParser()
    settings = parser.parse_file(str(path))
    
    if settings and len(settings) > 0:
        if len(settings) > self._INTERACTIVE_WIDGET_LIMIT:
            status_msg = (
                f"File has {len(settings)} settings — skipping interactive "
                f"editor (> limit of {self._INTERACTIVE_WIDGET_LIMIT}). Showing text view."
            )
            self._mod_settings_status.config(text=status_msg, foreground="orange")
            self._show_text_editor(path)
            return

        # Show interactive editor with parsed settings
        self._show_interactive_editor(path, settings)
    else:
        self._mod_settings_status.config(text=f"Parsed {len(settings)} settings (interactive)")
        self._show_text_editor(path)
except tk.TclError as exc:
    err_str = str(exc).lower()
    if "badalloc" in err_str or "insufficient" in err_str:
        self._mod_settings_status.config(
            text="X server out of resources; opened in text view.",
            foreground="red"
        )
        self._show_text_editor(path)
    else:
        raise  # Re-raise real bugs (e.g. invalid widget paths)
```

### 5.2 Fix for Canvas Resize & Scrollregion Callbacks (Problem 4)

**File:** `dayzconfigmaster/gui/app.py`  
**Method:** `_show_interactive_editor` (canvas callback closures)

Update both `_update_scrollregion` and `_canvas_resize`:

```python
def _update_scrollregion(_evt=None):
    try:
        if not canvas.winfo_exists() or not scrollable.winfo_exists():
            return
        bbox = canvas.bbox(tk.ALL)
        if bbox is not None:
            canvas.configure(scrollregion=bbox)
    except tk.TclError:
        pass

def _canvas_resize(_evt=None):
    try:
        if not canvas.winfo_exists() or not scrollable.winfo_exists():
            return
        existing_window = canvas.find_withtag(tk.ALL)
        canvas.itemconfig(existing_window, width=_evt.width if _evt else canvas.winfo_width())
        _update_scrollregion()
    except tk.TclError:
        pass  # Widget destroyed; ignore.
```

### 5.3 Fix for `_on_tree_hover` Tooltip Callback (Problem 6)

**File:** `dayzconfigmaster/gui/app.py`  
**Method:** callback registered in `_create_mod_settings_content`

Update the hover handler:

```python
def _on_tree_hover(event):
    try:
        item = self._mod_settings_tree.identify_row(event.y)
        if item:
            full_path = self._mod_settings_tree.item(item, "values")[2]
            mod_name = self._mod_settings_tree.item(item, "values")[0]
            display = self._mod_settings_tree.item(item, "values")[1]
            self._mod_settings_tooltip.configure(
                state=tk.NORMAL,
                text=f"{mod_name}  •  {display}\n{full_path}",
            )
            self._mod_settings_tooltip.configure(state=tk.DISABLED)
        else:
            self._mod_settings_tooltip.configure(state=tk.NORMAL, text="")
            self._mod_settings_tooltip.configure(state=tk.DISABLED)
    except (tk.TclError, IndexError):
        # Tree in invalid state during scroll/refresh; ignore.
        pass
```

---

## 6. Testing Plan

### Test 1: Small file (< 10 settings)
- Open a small mod settings file (e.g., `AILocationSettings.json` with ~5 entries)
- Verify interactive editor renders correctly with entry/slider widgets
- Toggle to text view and back
- Confirm no X errors in terminal output

### Test 2: Medium file (10+ settings, over the hard limit)
- Open a file with ~30 settings
- Verify it opens in text view (not interactive mode)
- Check that status message displays "File has N settings — skipping interactive editor"
- Verify text editor shows full file content

### Test 3: Large file (> 500 fields, flagged by scan)
- Run "Scan for Errors" on a project with large mod settings files
- Open the first large file shown in the scan results
- Click "Open First Large File in Text View" button from scan dialog
- Verify text editor opens; no crash

### Test 4: Rapid toggle stress test
- Load a small file in interactive mode
- Toggle between text and interactive views rapidly (5+ times)
- Verify no `TclError: invalid command name` or `invalid widget path` errors
- Check terminal for X11 errors

### Test 5: X server near resource limit simulation
- On Linux, start the GUI with many other ttk widgets visible (all tabs open)
- Load a file that results in exactly 8-10 settings (close to the limit)
- Verify no BadAlloc crash occurs
- Check terminal for any X protocol errors

### Test 6: Scan and repair workflow
- Run "Scan for Errors" 
- Note any files flagged with "Too many fields"
- Open one from the scan dialog's "Open First Large File in Text View" button
- Verify correct file is pre-selected in the tree and opens in text view

---

## 7. Priority & Effort Estimate

| Problem | Priority | Effort | Complexity |
|---------|----------|--------|------------|
| Problem 1: `_on_mod_settings_select` BadAlloc | **Critical** | 30 min | Low |
| Problem 4: Canvas callback stale refs | High | 25 min | Medium |
| Problem 6: Tooltip hover TclError | Low | 10 min | Low |

**Total estimated effort for remaining fixes:** ~1 hour

---

## 8. Recommended Implementation Order

1. **Problem 1** — Wrap `_on_mod_settings_select` in try/except (highest crash prevention impact)
2. **Problem 4** — Add existence checks to canvas resize callbacks
3. **Problem 6** — Wrap tooltip handler in try/except

---

## 9. Additional Recommendations

### 9.1 Consider a "Lite" interactive mode for files with 6-10 settings
Instead of all-or-nothing, implement a lazy-loading approach where widgets are created in batches of 2-3 with a 200ms delay between batches. This gives the X server time to free pixmap resources between widget creations.

### 9.2 Monitor X server resource usage
On Linux, add a diagnostic button that shows current X server resource usage:
```python
def check_x_resources():
    # Use xset -q or subprocess to query X server
    import subprocess
    result = subprocess.run(["xset", "-q"], capture_output=True, text=True)
    # Parse output for "number of objects" vs "max number objects"
```

### 9.3 Document the widget limit in the wiki
Add a note explaining that files with many settings open in text view automatically on Linux due to X11 resource constraints, and how users can adjust `_INTERACTIVE_WIDGET_LIMIT` if needed.

---

## 10. Files Modified

| File | Changes |
|------|---------|
| `dayzconfigmaster/gui/app.py` | Added `_INTERACTIVE_WIDGET_LIMIT`, `_build_single_setting_row_safe()`, canvas-based scrolling, BadAlloc guards in render path, toggle handler, title label protection, and cleanup event unbinds |
| `dayzconfigmaster/gui/mod_settings_editor/parser.py` | Added `display_value` parameter to `SettingField.__init__()` |
| `mod-settings-interactive.md` | This document |

---

## 11. Known Remaining Bugs

See Section 4 (Remaining Problems & Root Causes) for the full list of three problems that still need to be addressed. These are ranked by priority and include code snippets showing exactly what needs to change in each method.