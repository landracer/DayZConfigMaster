# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Widget module for interactive mod settings editor.

Provides Tkinter widgets for editing different types of configuration settings:
- SliderWidget: Drag bar with min/max values
- NumberInputWidget: Text input with validation
- BoolWidget: Checkbox toggle
- StringWidget: Text input field
- SelectWidget: Dropdown selection
- ColorWidget: Color picker
"""

import tkinter as tk
from tkinter import ttk, colorchooser
from typing import Optional, Callable, Any
from .parser import SettingType, SettingField


class SliderWidget(ttk.Frame):
    """
    Direct numeric input widget with min/max constraints.

    The original drag-bar slider was impractical for precise values and
    could exhaust X11 Pixmap resources on Linux. This replacement uses a
    simple validated entry field plus +/- buttons and clearly shows the
    allowed range.
    """

    def __init__(
        self,
        parent: tk.Widget,
        setting: SettingField,
        on_change: Optional[Callable[[Any], None]] = None,
        *args,
        **kwargs
    ):
        super().__init__(parent, *args, **kwargs)

        self.setting = setting
        self.on_change = on_change

        self.min_val = setting.min_val if setting.min_val is not None else 0
        self.max_val = setting.max_val if setting.max_val is not None else 100
        self.is_float = setting.setting_type == SettingType.FLOAT
        self.step = setting.step if setting.step is not None else (1.0 if self.is_float else 1)

        # Label frame with the setting name
        self.label_frame = ttk.LabelFrame(self, text=setting.name)
        self.label_frame.pack(fill=tk.X, padx=5, pady=2)

        # Default/current value
        if setting.value is not None:
            current_val = setting.value
        elif setting.default_value is not None:
            current_val = setting.default_value
        else:
            current_val = self.min_val

        self.current_value = tk.DoubleVar(value=float(current_val))

        # Range label
        range_text = f"Range: {self.min_val} - {self.max_val}"
        if setting.step is not None:
            range_text += f"  (step: {setting.step})"
        ttk.Label(self.label_frame, text=range_text, foreground="gray", font=("Arial", 8)).pack(
            anchor=tk.W, padx=5, pady=(5, 0)
        )

        # Input row
        input_frame = ttk.Frame(self.label_frame)
        input_frame.pack(fill=tk.X, padx=5, pady=5)

        ttk.Button(input_frame, text="-", width=3, command=self._decrement).pack(side=tk.LEFT)

        vcmd = (self.register(self._validate), "%P")
        self.entry = ttk.Entry(
            input_frame,
            textvariable=self.current_value,
            width=12,
            justify=tk.CENTER,
            validate="key",
            validatecommand=vcmd,
        )
        self.entry.pack(side=tk.LEFT, padx=5)

        ttk.Button(input_frame, text="+", width=3, command=self._increment).pack(side=tk.LEFT)

        # Value display
        self.value_label = ttk.Label(
            self.label_frame,
            text=self._format_value(self.current_value.get()),
            font=("Arial", 9, "bold"),
            foreground="blue",
        )
        self.value_label.pack(pady=(0, 5))

        # Apply callback on focus loss and Return
        self.entry.bind("<Return>", self._apply)
        self.entry.bind("<FocusOut>", self._apply)
        self.current_value.trace_add("write", self._on_trace_change)

    def _format_value(self, value: float) -> str:
        if self.is_float:
            return f"{value:.2f}"
        return str(int(value))

    def _validate(self, value: str) -> bool:
        if value in ("", "-", "."):
            return True
        try:
            float(value)
            return True
        except ValueError:
            return False

    def _clamp(self) -> float:
        try:
            val = self.current_value.get()
        except tk.TclError:
            val = 0.0
        val = max(self.min_val, min(self.max_val, val))
        if not self.is_float:
            val = round(val)
        else:
            # Snap to step if one is defined.
            if self.step:
                val = round(val / self.step) * self.step
        return val

    def _apply(self, event=None) -> None:
        val = self._clamp()
        self.current_value.set(val)
        self.value_label.config(text=self._format_value(val))
        if self.on_change:
            self.on_change(val)

    def _on_trace_change(self, *args) -> None:
        try:
            val = self.current_value.get()
            self.value_label.config(text=self._format_value(val))
        except tk.TclError:
            pass

    def _increment(self) -> None:
        val = self._clamp() + self.step
        self.current_value.set(min(val, self.max_val))
        self._apply()

    def _decrement(self) -> None:
        val = self._clamp() - self.step
        self.current_value.set(max(val, self.min_val))
        self._apply()

    def get_value(self) -> Any:
        return self._clamp()


class NumberInputWidget(ttk.Frame):
    """
    Numeric input widget with spinbox and validation.
    
    Features:
    - Up/down buttons for increment/decrement
    - Min/max constraints
    - Optional decimal precision
    - Keyboard support (arrow keys)
    """
    
    def __init__(
        self,
        parent: tk.Widget,
        setting: SettingField,
        on_change: Optional[Callable[[Any], None]] = None,
        *args,
        **kwargs
    ):
        super().__init__(parent, *args, **kwargs)
        
        self.setting = setting
        self.on_change = on_change
        
        # Label
        label_text = f"{setting.name} ({setting.min_val if setting.min_val is not None else 'N/A'} - {setting.max_val if setting.max_val is not None else 'N/A'})"
        ttk.Label(self, text=label_text).pack(anchor=tk.W, padx=5, pady=(5, 0))
        
        # Input frame
        input_frame = ttk.Frame(self)
        input_frame.pack(fill=tk.X, padx=5, pady=2)
        
        # Decimal or integer handling
        self.is_float = setting.setting_type == SettingType.FLOAT
        
        # Default value
        if setting.value is not None:
            current_val = setting.value
        elif setting.default_value is not None:
            current_val = setting.default_value
        else:
            current_val = 0
        
        self.current_value = tk.DoubleVar(value=float(current_val)) if self.is_float else tk.IntVar(value=int(current_val))
        
        # Min/Max values
        min_val = setting.min_val if setting.min_val is not None else -10000
        max_val = setting.max_val if setting.max_val is not None else 10000
        
        self.spinner = ttk.Spinbox(
            input_frame,
            from_=min_val,
            to=max_val,
            textvariable=self.current_value,
            increment=setting.step if setting.step is not None else (1.0 if self.is_float else 1),
            width=15,
            command=lambda: self._on_change()
        )
        self.spinner.pack(side=tk.LEFT, padx=(0, 5))
        
        # Arrow buttons for quick adjustment
        arrow_frame = ttk.Frame(input_frame)
        arrow_frame.pack(side=tk.LEFT)
        
        ttk.Button(
            arrow_frame,
            text="↑",
            command=self._increment,
            width=3
        ).pack(padx=1)
        
        ttk.Button(
            arrow_frame,
            text="↓",
            command=self._decrement,
            width=3
        ).pack()
        
        # Current value display
        self.display_label = ttk.Label(self, text="", foreground="gray", font=("Arial", 8))
        self.display_label.pack(anchor=tk.E, padx=5)
        self._update_display()
    
    def _on_change(self):
        """Handle value change."""
        try:
            val = self.current_value.get()
            
            # Clamp to min/max
            if self.setting.min_val is not None:
                val = max(self.setting.min_val, val)
            if self.setting.max_val is not None:
                val = min(self.setting.max_val, val)
            
            self.current_value.set(val)
            self._update_display()
            
            if self.on_change:
                self.on_change(val)
                
        except tk.TclError:
            pass
    
    def _increment(self):
        """Increment value by step."""
        current = self.current_value.get()
        new_val = current + (self.setting.step if self.setting.step else 1)
        
        if self.setting.max_val is not None:
            new_val = min(new_val, self.setting.max_val)
        
        self.current_value.set(new_val)
        self._update_display()
        
        if self.on_change:
            self.on_change(new_val)
    
    def _decrement(self):
        """Decrement value by step."""
        current = self.current_value.get()
        new_val = current - (self.setting.step if self.setting.step else 1)
        
        if self.setting.min_val is not None:
            new_val = max(new_val, self.setting.min_val)
        
        self.current_value.set(new_val)
        self._update_display()
        
        if self.on_change:
            self.on_change(new_val)
    
    def _update_display(self):
        """Update the display label."""
        val = self.current_value.get()
        if self.is_float:
            decimals = max(0, min(6, len(str(self.setting.step).split(".")[-1]) if "." in str(self.setting.step) else 2))
            self.display_label.config(text=f"Current: {val:.{decimals}f}")
        else:
            self.display_label.config(text=f"Current: {int(val)}")
    
    def get_value(self) -> Any:
        """Get the current value."""
        return self.current_value.get()


class BoolWidget(ttk.Frame):
    """
    Boolean toggle widget (checkbox).
    
    Features:
    - Checkbox with descriptive label
    - Custom true/false values display
    """
    
    def __init__(
        self,
        parent: tk.Widget,
        setting: SettingField,
        on_change: Optional[Callable[[Any], None]] = None,
        *args,
        **kwargs
    ):
        super().__init__(parent, *args, **kwargs)
        
        self.setting = setting
        self.on_change = on_change
        
        # Current value (default to False if None)
        current_value = bool(setting.value) if setting.value is not None else False
        
        self.var = tk.BooleanVar(value=current_value)
        
        checkbox = ttk.Checkbutton(
            self,
            text=setting.name,
            variable=self.var,
            command=self._on_change
        )
        checkbox.pack(anchor=tk.W, padx=5, pady=5)
        
        # Status label
        status_text = "Enabled (True)" if current_value else "Disabled (False)"
        self.status_label = ttk.Label(
            self,
            text=status_text,
            foreground="green" if current_value else "red",
            font=("Arial", 9),
            anchor=tk.W
        )
        self.status_label.pack(anchor=tk.W, padx=10)
    
    def _on_change(self):
        """Handle checkbox toggle."""
        state = self.var.get()
        
        status_text = "Enabled (True)" if state else "Disabled (False)"
        status_color = "green" if state else "red"
        
        self.status_label.config(text=status_text, foreground=status_color)
        
        if self.on_change:
            self.on_change(state)
    
    def get_value(self) -> bool:
        """Get the current value."""
        return self.var.get()


class StringWidget(ttk.Frame):
    """
    String input widget.
    
    Features:
    - Single-line text entry
    - Password option
    - Validation callback
    """
    
    def __init__(
        self,
        parent: tk.Widget,
        setting: SettingField,
        on_change: Optional[Callable[[Any], None]] = None,
        is_password: bool = False,
        *args,
        **kwargs
    ):
        super().__init__(parent, *args, **kwargs)
        
        self.setting = setting
        self.on_change = on_change
        
        # Label
        ttk.Label(self, text=setting.name).pack(anchor=tk.W, padx=5, pady=(5, 0))
        
        current_value = setting.value if setting.value is not None else ""
        
        self.var = tk.StringVar(value=str(current_value))
        
        entry_kwargs = {"textvariable": self.var}
        if is_password:
            entry_kwargs["show"] = "*"
        
        self.entry = ttk.Entry(self, **entry_kwargs)
        self.entry.pack(fill=tk.X, padx=5, pady=2)
        self.entry.bind("<KeyRelease>", self._on_key_release)
    
    def _on_key_release(self, event=None):
        """Handle text input."""
        if self.on_change:
            self.on_change(self.var.get())
    
    def get_value(self) -> str:
        """Get the current value."""
        return self.var.get()


class SelectWidget(ttk.Frame):
    """
    Dropdown selection widget.
    
    Features:
    - Combobox with valid values list
    - Auto-fill from SettingField.valid_values
    """
    
    def __init__(
        self,
        parent: tk.Widget,
        setting: SettingField,
        on_change: Optional[Callable[[Any], None]] = None,
        *args,
        **kwargs
    ):
        super().__init__(parent, *args, **kwargs)
        
        self.setting = setting
        self.on_change = on_change
        
        # Label
        ttk.Label(self, text=setting.name).pack(anchor=tk.W, padx=5, pady=(5, 0))
        
        # Available values
        valid_values = setting.valid_values if setting.valid_values else []
        
        current_value = str(setting.value) if setting.value is not None else ""
        
        self.var = tk.StringVar(value=current_value)
        
        combobox_kwargs = {
            "textvariable": self.var,
            "values": valid_values if valid_values else ["Default"],
        }
        
        if not valid_values:
            combobox_kwargs["state"] = "readonly"
        
        self.combobox = ttk.Combobox(self, **combobox_kwargs)
        self.combobox.pack(fill=tk.X, padx=5, pady=2)
        
        # Bind selection event
        self.combobox.bind("<<ComboboxSelected>>", self._on_select)
    
    def _on_select(self, event=None):
        """Handle selection change."""
        if self.on_change:
            self.on_change(self.var.get())
    
    def get_value(self) -> str:
        """Get the current value."""
        return self.var.get()


class ColorWidget(ttk.Frame):
    """
    Color picker widget.
    
    Features:
    - Visual color swatch
    - Open color dialog button
    - Hex value display
    """
    
    def __init__(
        self,
        parent: tk.Widget,
        setting: SettingField,
        on_change: Optional[Callable[[Any], None]] = None,
        *args,
        **kwargs
    ):
        super().__init__(parent, *args, **kwargs)
        
        self.setting = setting
        self.on_change = on_change
        
        # Label
        ttk.Label(self, text=setting.name).pack(anchor=tk.W, padx=5, pady=(5, 0))
        
        current_value = str(setting.value) if setting.value is not None else "#FF0000"
        self.current_color = tk.StringVar(value=current_value)
        
        # Color swatch frame
        swatch_frame = ttk.Frame(self)
        swatch_frame.pack(fill=tk.X, padx=5, pady=2)
        
        self.color_swatch = tk.Canvas(
            swatch_frame,
            width=30,
            height=30,
            bg=current_value,
            highlightthickness=1,
            highlightbackground="gray"
        )
        self.color_swatch.pack(side=tk.LEFT, padx=(0, 5))
        
        # Hex display
        self.hex_label = ttk.Label(swatch_frame, text=current_value.upper(), font=("Arial", 8))
        self.hex_label.pack(side=tk.LEFT)
        
        # Button frame
        btn_frame = ttk.Frame(self)
        btn_frame.pack(fill=tk.X, padx=5, pady=(2, 0))
        
        ttk.Button(
            btn_frame,
            text="Pick Color",
            command=self._pick_color
        ).pack(side=tk.LEFT)
        
        ttk.Button(
            btn_frame,
            text="Reset",
            command=self._reset_color
        ).pack(side=tk.RIGHT)
    
    def _pick_color(self):
        """Open color picker dialog."""
        current = self.current_color.get()
        color = colorchooser.askcolor(initialcolor=current, title=f"Select {self.setting.name}")
        
        if color and color[1]:  # color is (rgb_tuple, hex_string)
            new_color = color[1]
            self._set_color(new_color)
    
    def _reset_color(self):
        """Reset to default color."""
        default = "#FF0000"  # Default red
        self._set_color(default)
    
    def _set_color(self, color: str):
        """Update the color display and trigger callback."""
        self.current_color.set(color)
        
        # Update swatch
        self.color_swatch.configure(bg=color)
        
        # Update hex display (convert to uppercase)
        hex_val = color.upper() if color.startswith('#') else f"#{color}".upper()
        self.hex_label.config(text=hex_val)
        
        if self.on_change:
            self.on_change(color)


def create_widget_for_setting(
    parent: tk.Widget,
    setting: SettingField,
    on_change: Optional[Callable[[Any], None]] = None
):
    """
    Factory function to create the appropriate widget for a setting.
    
    Args:
        parent: Parent Tkinter widget
        setting: SettingField describing the configuration
        on_change: Callback when value changes
        
    Returns:
        Configured widget instance
    """
    if setting.setting_type == SettingType.BOOLEAN:
        return BoolWidget(parent, setting, on_change)
    elif setting.setting_type == SettingType.INTEGER:
        return NumberInputWidget(parent, setting, on_change)
    elif setting.setting_type == SettingType.FLOAT:
        return NumberInputWidget(parent, setting, on_change)
    elif setting.setting_type == SettingType.SELECT:
        return SelectWidget(parent, setting, on_change)
    elif setting.setting_type == SettingType.COLOR:
        return ColorWidget(parent, setting, on_change)
    elif setting.setting_type == SettingType.ARRAY:
        # For arrays, display as a multi-line text field
        return StringWidget(parent, setting, on_change)
    else:  # STRING or default
        # Check if it looks like a password
        is_password = 'password' in setting.name.lower() or 'pass' in setting.name.lower()
        return StringWidget(parent, setting, on_change, is_password=is_password)