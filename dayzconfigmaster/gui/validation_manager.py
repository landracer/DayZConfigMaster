# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Validation Manager for GUI input widgets
"""
ValidationManager: Integrates field validation with Tkinter input widgets.
"""

import logging
import tkinter as tk
from tkinter import ttk, messagebox
from typing import Dict, List, Tuple, Optional, Any, Callable
from dataclasses import dataclass

logger = logging.getLogger(__name__)

try:
    from ..config.validator import FieldValidator, ValidationError
except ImportError:
    from dayzconfigmaster.config.validator import FieldValidator, ValidationError


@dataclass
class ValidationRule:
    """Represents a validation rule for an input field"""
    field_name: str
    min_val: Optional[float] = None
    max_val: Optional[float] = None
    valid_values: Optional[List[str]] = None
    min_length: Optional[int] = None
    max_length: Optional[int] = None
    format_str: Optional[str] = None


class ValidationManager:
    """
    Manages validation for GUI input widgets.
    
    Provides real-time validation feedback and error highlighting
    for all configuration fields in the application.
    """
    
    def __init__(self, validator: FieldValidator = None):
        """
        Initialize the validation manager.
        
        Args:
            validator: Optional pre-configured FieldValidator instance
        """
        self.validator = validator or FieldValidator()
        self._widget_rules: Dict[str, ValidationRule] = {}
        self._error_widgets: Dict[tk.Widget, str] = {}
        self._validation_enabled = True
    
    @property
    def validation_enabled(self) -> bool:
        """Check if validation is enabled"""
        return self._validation_enabled
    
    def enable_validation(self):
        """Enable real-time validation"""
        self._validation_enabled = True
    
    def disable_validation(self):
        """Disable real-time validation (useful during bulk loads)"""
        self._validation_enabled = False
    
    def register_widget(
        self,
        widget: tk.Widget,
        field_name: str,
        rule: ValidationRule = None
    ):
        """
        Register a widget for validation.
        
        Args:
            widget: Tkinter widget to validate
            field_name: Name of the field (for validator lookup)
            rule: Optional custom ValidationRule
        """
        if rule is None and field_name in self.validator.field_rules:
            rules = self.validator.field_rules[field_name]
            rule = ValidationRule(
                field_name=field_name,
                min_val=rules.get('min'),
                max_val=rules.get('max'),
                valid_values=rules.get('valid_values'),
                min_length=rules.get('min_length'),
                max_length=rules.get('max_length')
            )
        
        if rule:
            self._widget_rules[widget] = rule
        
        # Bind validation events based on widget type
        if isinstance(widget, (ttk.Entry, ttk.Combobox)):
            widget.bind('<FocusOut>', lambda e: self.validate_widget(widget))
            widget.bind('<KeyRelease>', lambda e: self._delayed_validate(widget))
        
        elif isinstance(widget, tk.Text):
            widget.bind('<FocusOut>', lambda e: self.validate_widget(widget))
        
        # Store error message label for this widget
        error_label = ttk.Label(
            widget.master,
            text="",
            foreground="red",
            font=("Arial", 8)
        )
        self._error_widgets[widget] = error_label
    
    def register_entry_with_rules(
        self,
        parent: tk.Widget,
        row: int,
        label_text: str,
        field_name: str,
        default: Any = None,
        **kwargs
    ) -> ttk.Entry:
        """
        Create an entry widget with full validation setup.
        
        Args:
            parent: Parent widget
            row: Grid row position
            label_text: Label text for the field
            field_name: Name of the field (for validator lookup)
            default: Default value
            **kwargs: Additional validation parameters
            
        Returns:
            The created Entry widget
        """
        # Create label and entry
        ttk.Label(parent, text=label_text).grid(row=row, column=0, sticky=tk.W, padx=5, pady=2)
        
        var = tk.StringVar(value=str(default) if default else "")
        entry = ttk.Entry(parent, textvariable=var, width=30)
        entry.grid(row=row, column=1, sticky=tk.W+tk.E, padx=5, pady=2)
        
        # Create info label for validation rules
        rule_info = self._get_rule_info(field_name, **kwargs)
        ttk.Label(parent, text=rule_info, foreground="gray", font=("Arial", 8)).grid(
            row=row, column=2, sticky=tk.W, padx=5, pady=2
        )
        
        # Register for validation
        rule = ValidationRule(
            field_name=field_name,
            min_val=kwargs.get('min'),
            max_val=kwargs.get('max'),
            valid_values=kwargs.get('valid_values')
        )
        self.register_widget(entry, field_name, rule)
        
        return entry
    
    def _get_rule_info(self, field_name: str, **kwargs) -> str:
        """Get validation info string for a field"""
        parts = []
        
        # Add default if provided
        if 'default' in kwargs:
            parts.append(f"Default: {kwargs['default']}")
        
        # Add range info
        if 'min' in kwargs and 'max' in kwargs:
            parts.append(f"Range: {kwargs['min']}-{kwargs['max']}")
        elif 'min' in kwargs:
            parts.append(f"Min: {kwargs['min']}")
        elif 'max' in kwargs:
            parts.append(f"Max: {kwargs['max']}")
        
        # Add valid values
        if 'valid_values' in kwargs and kwargs['valid_values']:
            parts.append(f"Values: {', '.join(kwargs['valid_values'])}")
        
        return " | ".join(parts) if parts else ""
    
    def _delayed_validate(self, widget: tk.Widget):
        """Validate after a brief delay to avoid excessive validation during typing"""
        if not self._validation_enabled:
            return
        
        # Schedule validation for later
        widget.after(300, lambda: self.validate_widget(widget))
    
    def validate_widget(self, widget: tk.Widget) -> Tuple[bool, Optional[str]]:
        """
        Validate a single widget's value.
        
        Args:
            widget: The widget to validate
            
        Returns:
            Tuple of (is_valid, error_message)
        """
        rule = self._widget_rules.get(widget)
        
        if not rule:
            return True, None
        
        # Get current value
        value = self._get_widget_value(widget)
        
        # Perform validation
        is_valid, message = self.validator.validate(rule.field_name, value)
        
        # Update error label
        error_label = self._error_widgets.get(widget)
        if error_label:
            error_label.config(text=message or "")
        
        return is_valid, message
    
    def _get_widget_value(self, widget: tk.Widget) -> Any:
        """Get the current value from a widget"""
        if isinstance(widget, ttk.Entry):
            return widget.get()
        elif isinstance(widget, ttk.Combobox):
            return widget.get()
        elif isinstance(widget, tk.Text):
            return widget.get("1.0", tk.END).strip()
        elif isinstance(widget, tk.BooleanVar):
            return widget.get()
        
        # Try stringvar
        var = getattr(widget, 'textvariable', None)
        if var:
            try:
                return var.get()
            except Exception as exc:
                logger.debug("Failed to get textvariable: %s", exc)
        
        return str(widget.get()) if hasattr(widget, 'get') else ""
    
    def validate_all(self, widgets: List[tk.Widget]) -> Dict[str, Tuple[bool, Optional[str]]]:
        """
        Validate multiple widgets.
        
        Args:
            widgets: List of widgets to validate
            
        Returns:
            Dictionary mapping widget name to (is_valid, error_message)
        """
        results = {}
        for widget in widgets:
            is_valid, msg = self.validate_widget(widget)
            field_name = self._widget_rules.get(widget, ValidationRule(field_name="unknown")).field_name
            results[field_name] = (is_valid, msg)
        
        return results
    
    def are_all_valid(self) -> bool:
        """
        Check if all registered widgets have valid values.
        
        Returns:
            True if all valid, False otherwise
        """
        for widget in self._widget_rules.keys():
            is_valid, _ = self.validate_widget(widget)
            if not is_valid:
                return False
        return True
    
    def get_invalid_widgets(self) -> List[Tuple[tk.Widget, str]]:
        """
        Get list of invalid widgets and their error messages.
        
        Returns:
            List of (widget, error_message) tuples
        """
        invalid = []
        for widget in self._widget_rules.keys():
            is_valid, msg = self.validate_widget(widget)
            if not is_valid:
                invalid.append((widget, msg))
        return invalid
    
    def highlight_invalid_fields(self):
        """Highlight all invalid fields with red border"""
        for widget in self.get_invalid_widgets():
            try:
                widget.config(style="Warning.TEntry")
            except Exception as exc:
                logger.debug("Failed to set warning style: %s", exc)
    
    def clear_highlights(self):
        """Clear all validation highlights"""
        for widget in self._error_widgets.keys():
            try:
                widget.config(style="")
            except Exception as exc:
                logger.debug("Failed to clear highlight style: %s", exc)


class ValidationErrorUI:
    """
    Provides UI feedback for validation errors.
    
    Can display error messages, highlight invalid fields,
    and prevent saving when there are validation errors.
    """
    
    def __init__(self, parent: tk.Tk):
        self.parent = parent
        self._error_frame: Optional[ttk.Frame] = None
    
    def create_error_container(self, parent: tk.Widget) -> ttk.Frame:
        """Create a container for displaying error messages"""
        if not self._error_frame:
            self._error_frame = ttk.LabelFrame(
                parent,
                text="Validation Status",
                padding=10
            )
            self._status_label = ttk.Label(
                self._error_frame,
                text="All fields valid",
                foreground="green",
                font=("Arial", 9, "bold")
            )
            self._status_label.pack()
        return self._error_frame
    
    def update_status(self, is_valid: bool, message: str = None):
        """Update the validation status display"""
        if self._error_frame and hasattr(self, '_status_label'):
            if is_valid:
                self._status_label.config(text="All fields valid", foreground="green")
            else:
                self._status_label.config(text=f"Validation error: {message}", foreground="red")
    
    def show_validation_dialog(self, errors: List[Tuple[str, str]]):
        """
        Show a dialog with all validation errors.
        
        Args:
            errors: List of (field_name, error_message) tuples
        """
        if not self.parent:
            return
        
        # Build error message
        lines = ["Validation Errors:", "=" * 40]
        for field, msg in errors:
            lines.append(f"\n{field}:")
            lines.append(f"  {msg}")
        
        messagebox.showerror("Configuration Errors", "\n".join(lines))
    
    def confirm_save_if_valid(self, validation_manager: ValidationManager) -> bool:
        """
        Confirm if save is allowed based on validation status.
        
        Args:
            validation_manager: The active ValidationManager instance
            
        Returns:
            True if valid and save can proceed
        """
        if not validation_manager.are_all_valid():
            invalid = validation_manager.get_invalid_widgets()
            errors = [(widget, msg) for widget, msg in invalid]
            self.show_validation_dialog(errors)
            return False
        
        return True