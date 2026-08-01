# Mod Settings Editor Issues and Fixes

## Current Problems

### 1. Widget Limit Issue (Critical)
The interactive settings editor has an arbitrary limit of **10 widgets** that prevents editing large configuration files. This is implemented on line 2807 in `app.py`:

```python
if len(settings) > self._INTERACTIVE_WIDGET_LIMIT:
    # ... skips interactive editor and shows text view instead
```

This causes:
- Large mod config files (like those with hundreds of settings) to be automatically shown in text view only
- Users cannot access all settings in large configuration files 
- The limit is set to an arbitrary 10 widgets which makes no sense for a configuration tool

### 2. State Management Issues
When switching between different mod settings files:
- The UI shows settings from the previous file instead of the currently selected file
- Settings don't properly update when changing selections
- Interactive editor doesn't fully reset between file switches

### 3. Memory Leak / Widget Accumulation
- Widgets aren't being properly destroyed when switching between files
- Old interactive editors can accumulate and cause memory issues

## Root Cause Analysis

The problem stems from a combination of:
1. **Arbitrary widget limit** that was designed to prevent X11 BadAlloc crashes but is too restrictive for actual use
2. **Incomplete state management** in the UI switching logic
3. **Poor cleanup mechanisms** when switching between different files

## Solutions and Fixes

### 1. Remove the Widget Limit (Primary Fix)
The widget limit should be removed entirely, or at least greatly increased (to 100+ widgets) because:
- Modern systems can handle much more than 10 widgets
- The X11 BadAlloc issue was addressed with proper crash prevention
- Users expect to see ALL settings in their configuration files

### 2. Improve State Management
The `_on_mod_settings_select` method needs better handling of state transitions between files:
- Properly reset all interactive editor widgets before loading new file
- Ensure the correct file path is used for each load operation
- Clear previous file's settings and UI elements completely

### 3. Enhanced Cleanup Mechanisms
Add comprehensive cleanup functions to ensure:
- All old widgets are destroyed properly
- No widget references are retained after switching
- Proper state transitions between interactive/text modes

## Implementation Plan

1. **Remove the widget limit** in `_on_mod_settings_select`
2. **Fix the file selection logic** to ensure correct settings are loaded 
3. **Enhance cleanup functions** to properly destroy widgets
4. **Add robust error handling** for edge cases during switching
5. **Improve UI feedback** when switching between large files

## Code Changes Required

### File: `dayzconfigmaster/gui/app.py`

#### Fix 1: Remove widget limit check (around line 2807)
```python
# BEFORE (problematic):
if len(settings) > self._INTERACTIVE_WIDGET_LIMIT:
    status_msg = (
        f"File has {len(settings)} settings — skipping interactive "
        f"editor (> limit of {self._INTERACTIVE_WIDGET_LIMIT}). Showing text view."
    )
    self._mod_settings_status.config(text=status_msg, foreground="orange")
    self._show_text_editor(path)
    return

# AFTER (fixed):
# Remove the widget limit entirely - users should be able to edit all settings
```

#### Fix 2: Improve `_cleanup_interactive_editor` method 
Ensure it properly destroys all widgets and resets state completely.

#### Fix 3: Enhance `_on_mod_settings_select`
Add better file path tracking and state reset logic.

## Expected Behavior After Fixes

1. All mod configuration files can be edited in interactive mode regardless of size
2. Switching between different mod settings files works correctly
3. No more incorrect settings showing from previous files
4. Large config files (with 100+ settings) are fully editable
5. Memory management is improved to prevent accumulation of stale widgets

## Testing Requirements

1. Test with small config files (< 10 settings)
2. Test with medium config files (10-100 settings) 
3. Test with large config files (> 100 settings)
4. Verify switching between different files works correctly
5. Verify no widget accumulation or memory leaks
6. Test that the UI properly shows current file's settings

## Long-term Considerations

1. Implement a configurable widget limit for systems with very limited resources
2. Add progress indicators for loading large config files
3. Consider lazy loading of settings for extremely large files (> 500 settings)
4. Add a setting to disable interactive mode entirely if needed