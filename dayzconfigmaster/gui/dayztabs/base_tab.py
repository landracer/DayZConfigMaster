# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Base tab class for DayZ configuration editor

"""
BaseTab: Base class providing common functionality for all tab components.
"""

import tkinter as tk
from tkinter import ttk


class ScrollableFrame(ttk.Frame):
    """
    A scrollable frame whose scrollbars appear only when the content does not
    fit the visible area (vertical when taller, horizontal when wider). Content
    narrower than the viewport stretches to fill the width.
    """
    
    def __init__(self, parent, *args, **kwargs):
        super().__init__(parent, *args, **kwargs)
        
        self.canvas = tk.Canvas(self, borderwidth=0, highlightthickness=0)
        self.frame = ttk.Frame(self.canvas)
        self.vscrollbar = ttk.Scrollbar(self, orient=tk.VERTICAL, command=self.canvas.yview)
        self.hscrollbar = ttk.Scrollbar(self, orient=tk.HORIZONTAL, command=self.canvas.xview)
        
        self.canvas.configure(yscrollcommand=self.vscrollbar.set, xscrollcommand=self.hscrollbar.set)
        
        self.canvas.grid(row=0, column=0, sticky="nsew")
        self.vscrollbar.grid(row=0, column=1, sticky="ns")
        self.hscrollbar.grid(row=1, column=0, sticky="ew")
        self.grid_rowconfigure(0, weight=1)
        self.grid_columnconfigure(0, weight=1)
        
        self.canvas_frame = self.canvas.create_window((0, 0), window=self.frame, anchor="nw")
        self.frame.bind("<Configure>", self.on_frame_configure)
        self.canvas.bind("<Configure>", self.on_canvas_configure)
    
    def on_frame_configure(self, event=None):
        """Update scroll region and scrollbar visibility when content changes."""
        self._refresh(event)
    
    def on_canvas_configure(self, event=None):
        """Update fill width and scrollbar visibility when the canvas resizes."""
        self._refresh(event)
    
    def _refresh(self, event=None):
        """Recompute scroll region, width fill, and scrollbar visibility."""
        self.canvas.configure(scrollregion=self.canvas.bbox("all"))
        
        canvas_w = self.canvas.winfo_width()
        canvas_h = self.canvas.winfo_height()
        req_w = self.frame.winfo_reqwidth()
        req_h = self.frame.winfo_reqheight()
        
        # Stretch to fill when narrower than the viewport; keep natural width
        # (enabling horizontal scroll) when wider.
        self.canvas.itemconfig(self.canvas_frame, width=max(canvas_w, req_w))
        
        if req_h > canvas_h:
            self.vscrollbar.grid()
        else:
            self.vscrollbar.grid_remove()
        
        if req_w > canvas_w:
            self.hscrollbar.grid()
        else:
            self.hscrollbar.grid_remove()


class BaseTab(ttk.Frame):
    """
    Base class for all configuration tabs.
    
    Provides:
    - Common UI elements and layout
    - Validation helper access
    - Config model storage
    """
    
    def __init__(self, parent, notebook, validator=None, config_model=None, **kwargs):
        super().__init__(parent, **kwargs)
        
        self.notebook = notebook
        self.validator = validator
        self.config_model = config_model
        self.input_widgets = {}  # Store input widgets for validation
        
        # Configure row/column weights for proper resizing
        self.columnconfigure(0, weight=1)
        self.rowconfigure(1, weight=1)
        
        self._create_ui()
    
    def _create_ui(self):
        """Create the tab UI - to be implemented by subclasses"""
        raise NotImplementedError("Subclasses must implement _create_ui()")
    
    def add_scrollable_frame(self, parent) -> ScrollableFrame:
        """Add a scrollable frame to the parent"""
        return ScrollableFrame(parent)
    
    def create_field_with_validation(self, parent, row, field_name, default=None,
                                     min_val=None, max_val=None, valid_values=None):
        """
        Create a field with comprehensive validation info displayed next to it.
        
        Args:
            parent: Parent widget
            row: Row index for grid
            field_name: Name of the field (for validator lookup)
            default: Default value
            min_val: Minimum numeric value
            max_val: Maximum numeric value
            valid_values: List of valid string values
            
        Returns:
            Tuple of (widget, rules_dict) for the created field
        """
        # Build explanation text with all available information
        explain_parts = []
        
        if default is not None:
            explain_parts.append(f"Default: {default}")
        if min_val is not None and max_val is not None:
            explain_parts.append(f"Range: {min_val}-{max_val}")
        elif min_val is not None:
            explain_parts.append(f"Min: {min_val}")
        elif max_val is not None:
            explain_parts.append(f"Max: {max_val}")
        if valid_values:
            explain_parts.append(f"Values: {', '.join(valid_values)}")
        
        # Add field-specific explanation from validator
        if self.validator and field_name in self.validator.field_rules:
            rule = self.validator.field_rules[field_name]
            if 'explanation' in rule:
                explain_parts.insert(0, rule['explanation'])
        
        # Create validation info frame
        info_frame = ttk.Frame(parent)
        info_frame.grid(row=row, column=2, sticky=tk.W, padx=(5, 0), pady=2)
        
        if explain_parts:
            info_label = ttk.Label(info_frame, text=" | ".join(explain_parts),
                                  foreground="gray", font=("Arial", 8))
            info_label.pack()
        
        return (None, {"default": default, "min": min_val, "max": max_val, 
                      "valid_values": valid_values})