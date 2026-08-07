# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Scalable interactive editor for DayZ mod settings files.

Layout:
- Left pane: searchable tree of settings grouped by nested category.
- Right pane: text editor (always available) or interactive editor for the
  selected setting.

No hard widget limit: only the selected setting (or a small group) is edited
interactively at a time, so files with hundreds of settings are fine.
"""

from __future__ import annotations

import json
import tkinter as tk
from copy import deepcopy
from pathlib import Path
from tkinter import messagebox, scrolledtext, ttk
from typing import Any, Callable, Dict, List, Optional, Tuple

from .parser import ConfigParser, SettingField, SettingType
from .widgets import create_widget_for_setting


class ModSettingsEditor:
    """Two-pane mod settings editor widget."""

    def __init__(
        self,
        parent: tk.Widget,
        get_instance_root: Optional[Callable[[], Optional[Path]]] = None,
    ):
        self.parent = parent
        self.get_instance_root = get_instance_root

        self.path: Optional[Path] = None
        self.raw_text: str = ""
        self.settings: List[SettingField] = []
        self.parsed_data: Any = None
        self.file_suffix: str = ""

        # Setting name -> SettingField
        self._setting_map: Dict[str, SettingField] = {}
        # Flattened JSON values (dotted names) -> current value
        self._values: Dict[str, Any] = {}

        self._build_ui()

    def _build_ui(self) -> None:
        self.frame = ttk.Frame(self.parent)
        self.frame.pack(fill=tk.BOTH, expand=True)
        self.frame.columnconfigure(1, weight=1)
        self.frame.rowconfigure(1, weight=1)

        # Header
        self._label = ttk.Label(
            self.frame,
            text="No file selected",
            foreground="gray",
        )
        self._label.grid(row=0, column=0, columnspan=2, sticky=tk.W, pady=(0, 5))

        # Left pane: search + tree
        left_frame = ttk.LabelFrame(self.frame, text="Settings", padding=5)
        left_frame.grid(row=1, column=0, sticky=tk.NSEW, padx=(0, 5))
        left_frame.columnconfigure(0, weight=1)
        left_frame.rowconfigure(1, weight=1)
        left_frame.configure(width=320)
        left_frame.grid_propagate(False)

        self._search_var = tk.StringVar()
        self._search_var.trace_add("write", lambda *_: self._filter_tree())
        search_entry = ttk.Entry(left_frame, textvariable=self._search_var)
        search_entry.grid(row=0, column=0, sticky=tk.EW, pady=(0, 5))

        tree_container = ttk.Frame(left_frame)
        tree_container.grid(row=1, column=0, sticky=tk.NSEW)
        tree_container.columnconfigure(0, weight=1)
        tree_container.rowconfigure(0, weight=1)

        self._tree = ttk.Treeview(
            tree_container,
            columns=("value",),
            show="tree",
            selectmode="browse",
        )
        self._tree.heading("#0", text="Setting")
        self._tree.heading("value", text="Value")
        self._tree.column("#0", width=200, minwidth=150)
        self._tree.column("value", width=100, minwidth=80)
        self._tree.grid(row=0, column=0, sticky=tk.NSEW)

        vscroll = ttk.Scrollbar(tree_container, orient=tk.VERTICAL, command=self._tree.yview)
        vscroll.grid(row=0, column=1, sticky=tk.NS)
        self._tree.configure(yscrollcommand=vscroll.set)

        self._tree.bind("<<TreeviewSelect>>", self._on_tree_select)

        # Right pane: text / interactive editor
        right_frame = ttk.LabelFrame(self.frame, text="Editor", padding=5)
        right_frame.grid(row=1, column=1, sticky=tk.NSEW)
        right_frame.columnconfigure(0, weight=1)
        right_frame.rowconfigure(0, weight=1)

        self._editor_container = ttk.Frame(right_frame)
        self._editor_container.grid(row=0, column=0, sticky=tk.NSEW)
        self._editor_container.columnconfigure(0, weight=1)
        self._editor_container.rowconfigure(0, weight=1)

        self._text_editor = scrolledtext.ScrolledText(
            self._editor_container,
            wrap=tk.NONE,
            font=("Courier", 10),
            undo=True,
        )
        self._text_editor.grid(row=0, column=0, sticky=tk.NSEW)

        self._interactive_container: Optional[ttk.Frame] = None
        self._interactive_editor_visible = False

        # Buttons
        btn_frame = ttk.Frame(right_frame)
        btn_frame.grid(row=1, column=0, sticky=tk.EW, pady=(5, 0))

        self._toggle_btn = ttk.Button(
            btn_frame,
            text="Interactive View",
            command=self._toggle_view,
        )
        self._toggle_btn.pack(side=tk.LEFT, padx=2)

        ttk.Button(
            btn_frame,
            text="Validate",
            command=self._validate,
        ).pack(side=tk.LEFT, padx=2)

        ttk.Button(
            btn_frame,
            text="Save",
            command=self._save,
        ).pack(side=tk.LEFT, padx=2)

        self._status = ttk.Label(right_frame, text="", foreground="gray")
        self._status.grid(row=2, column=0, sticky=tk.W, pady=(5, 0))

    def load_file(self, path: Path, mod_name: str = "") -> None:
        """Load a settings file into the editor."""
        self.path = path
        self.file_suffix = path.suffix.lower()
        try:
            self.raw_text = path.read_text(encoding="utf-8")
        except Exception as exc:
            messagebox.showerror("Read Error", f"Could not read {path}:\n{exc}")
            return

        display = f"[{mod_name}] {path.name}" if mod_name else path.name
        self._label.config(text=display)
        self._text_editor.delete("1.0", tk.END)
        self._text_editor.insert("1.0", self.raw_text)

        parser = ConfigParser()
        self.settings = parser.parse_file(str(path))
        self.parsed_data = getattr(parser, "raw_data", None)

        self._setting_map = {s.name: s for s in self.settings if not s.name.startswith("_error")}
        self._values = {s.name: deepcopy(s.value) for s in self.settings if not s.name.startswith("_error")}

        self._populate_tree()

        has_settings = bool(self._setting_map)
        self._toggle_btn.config(state=tk.NORMAL if has_settings else tk.DISABLED)

        if has_settings:
            self._show_interactive_view()
        else:
            self._show_text_view()
            if any(s.name.startswith("_error") for s in self.settings):
                self._status.config(
                    text=f"Parse error: {self.settings[0].value}",
                    foreground="red",
                )
            else:
                self._status.config(
                    text="No editable settings found; text editor only.",
                    foreground="gray",
                )

    def _populate_tree(self) -> None:
        """Build the settings tree from flattened dotted setting names."""
        self._tree.delete(*self._tree.get_children())
        if not self.settings:
            return

        # Build a nested dict from dotted names.
        root: Dict[str, Any] = {}
        for setting in self.settings:
            if setting.name.startswith("_error"):
                continue
            parts = setting.name.replace("[", ".[").split(".")
            node = root
            for idx, part in enumerate(parts):
                is_last = idx == len(parts) - 1
                if is_last:
                    node[part] = ("leaf", setting)
                else:
                    if part not in node or not isinstance(node[part], dict):
                        node[part] = {}
                    node = node[part]

        def _insert(parent: str, key: str, value: Any) -> str:
            if isinstance(value, dict):
                node_id = self._tree.insert(parent, tk.END, text=key)
                for child_key in sorted(value.keys(), key=lambda k: (not isinstance(value[k], dict), k.lower())):
                    _insert(node_id, child_key, value[child_key])
                return node_id
            elif isinstance(value, tuple) and value[0] == "leaf":
                setting: SettingField = value[1]
                display = _format_value(setting.value)
                node_id = self._tree.insert(
                    parent,
                    tk.END,
                    text=setting.name,
                    values=(display,),
                    tags=("setting",),
                )
                return node_id
            return ""

        for key in sorted(root.keys(), key=lambda k: (not isinstance(root[k], dict), k.lower())):
            _insert("", key, root[key])

        # Auto-select first leaf
        for item in self._tree.get_children():
            first_leaf = self._find_first_leaf(item)
            if first_leaf:
                self._tree.selection_set(first_leaf)
                self._tree.see(first_leaf)
                break

    def _find_first_leaf(self, item: str) -> Optional[str]:
        if "setting" in self._tree.item(item, "tags"):
            return item
        for child in self._tree.get_children(item):
            leaf = self._find_first_leaf(child)
            if leaf:
                return leaf
        return None

    def _filter_tree(self) -> None:
        """Hide tree items that don't match the search string."""
        query = self._search_var.get().strip().lower()
        if not query:
            self._populate_tree()
            return

        self._tree.delete(*self._tree.get_children())
        for setting in self.settings:
            if setting.name.startswith("_error"):
                continue
            if query in setting.name.lower() or (setting.description and query in setting.description.lower()):
                display = _format_value(setting.value)
                self._tree.insert(
                    "",
                    tk.END,
                    text=setting.name,
                    values=(display,),
                    tags=("setting",),
                )

    def _on_tree_select(self, _event: Any = None) -> None:
        if self._interactive_editor_visible:
            self._render_interactive_editor()

    def _toggle_view(self) -> None:
        if self._interactive_editor_visible:
            self._show_text_view()
        else:
            self._show_interactive_view()

    def _show_text_view(self) -> None:
        self._interactive_editor_visible = False
        if self._interactive_container is not None:
            self._interactive_container.grid_forget()
            self._interactive_container.destroy()
            self._interactive_container = None
        self._text_editor.grid(row=0, column=0, sticky=tk.NSEW)
        self._toggle_btn.config(text="Interactive View")
        self._status.config(text="Text editor active.", foreground="gray")

    def _show_interactive_view(self) -> None:
        self._interactive_editor_visible = True
        self._text_editor.grid_forget()
        self._render_interactive_editor()
        self._toggle_btn.config(text="Text Editor")
        self._status.config(text="Interactive editor active.", foreground="green")

    def _render_interactive_editor(self) -> None:
        if self._interactive_container is not None:
            self._interactive_container.destroy()

        self._interactive_container = ttk.Frame(self._editor_container)
        self._interactive_container.grid(row=0, column=0, sticky=tk.NSEW)
        self._interactive_container.columnconfigure(0, weight=1)
        self._interactive_container.rowconfigure(1, weight=1)

        selected = self._tree.selection()
        if not selected:
            ttk.Label(
                self._interactive_container,
                text="Select a setting from the list to edit.",
                foreground="gray",
            ).grid(row=0, column=0, pady=20)
            return

        setting_name = self._tree.item(selected[0], "text")
        setting = self._setting_map.get(setting_name)
        if setting is None:
            ttk.Label(
                self._interactive_container,
                text=f"Cannot edit grouped selection: {setting_name}",
                foreground="gray",
            ).grid(row=0, column=0, pady=20)
            return

        # Header with name and description
        header = ttk.Label(
            self._interactive_container,
            text=setting.name,
            font=("Arial", 11, "bold"),
        )
        header.grid(row=0, column=0, sticky=tk.W, pady=(0, 5))

        if setting.description:
            desc = ttk.Label(
                self._interactive_container,
                text=setting.description,
                foreground="gray",
                wraplength=400,
            )
            desc.grid(row=1, column=0, sticky=tk.EW, pady=(0, 10))

        # Read-only marker for version fields
        is_version = setting.name.lower().endswith("version") and "m_" in setting.name.lower()

        editor_frame = ttk.Frame(self._interactive_container)
        editor_frame.grid(row=2, column=0, sticky=tk.NSEW)
        editor_frame.columnconfigure(0, weight=1)

        current_value = self._values.get(setting.name, setting.value)
        cloned = deepcopy(setting)
        cloned.value = current_value

        if is_version:
            ttk.Label(
                editor_frame,
                text=f"Value: {current_value}",
                foreground="blue",
            ).pack(anchor=tk.W)
            ttk.Label(
                editor_frame,
                text="Version fields should not be changed unless you know what you are doing.",
                foreground="orange",
                wraplength=400,
            ).pack(anchor=tk.W, pady=(5, 0))
        else:
            widget = create_widget_for_setting(
                editor_frame,
                cloned,
                on_change=lambda v, n=setting.name: self._on_value_changed(n, v),
            )
            widget.pack(fill=tk.X, expand=True)

    def _on_value_changed(self, name: str, value: Any) -> None:
        self._values[name] = value
        # Update tree display
        for item in self._tree.get_children():
            updated = self._update_tree_value(item, name, value)
            if updated:
                break
        # Apply to text editor if JSON
        if self.file_suffix == ".json":
            self._apply_values_to_text()

    def _update_tree_value(self, item: str, name: str, value: Any) -> bool:
        text = self._tree.item(item, "text")
        if text == name and "setting" in self._tree.item(item, "tags"):
            self._tree.item(item, values=(_format_value(value),))
            return True
        for child in self._tree.get_children(item):
            if self._update_tree_value(child, name, value):
                return True
        return False

    def _apply_values_to_text(self) -> None:
        """Patch the JSON text editor with current interactive values."""
        if self.file_suffix != ".json" or self.parsed_data is None:
            return
        try:
            data = deepcopy(self.parsed_data)
            self._patch_data(data, self._values)
            text = json.dumps(data, indent=4, ensure_ascii=False)
            self._text_editor.delete("1.0", tk.END)
            self._text_editor.insert("1.0", text)
            self._status.config(text="Interactive changes applied to text editor.", foreground="green")
        except Exception as exc:
            self._status.config(text=f"Apply error: {exc}", foreground="red")

    def _patch_data(self, data: Any, values: Dict[str, Any], prefix: str = "") -> None:
        if isinstance(data, dict):
            for key, value in data.items():
                full_name = f"{prefix}.{key}" if prefix else key
                if full_name in values:
                    data[key] = values[full_name]
                elif isinstance(value, (dict, list)):
                    self._patch_data(value, values, full_name)
        elif isinstance(data, list):
            for idx, item in enumerate(data):
                full_name = f"{prefix}[{idx}]"
                if full_name in values:
                    data[idx] = values[full_name]
                elif isinstance(item, (dict, list)):
                    self._patch_data(item, values, full_name)

    def _validate(self) -> bool:
        text = self._text_editor.get("1.0", tk.END).strip()
        if not text:
            self._status.config(text="Editor is empty.", foreground="gray")
            return True

        if self.file_suffix == ".json":
            try:
                json.loads(text)
                self._status.config(text="JSON is valid.", foreground="green")
                return True
            except json.JSONDecodeError as exc:
                self._status.config(text=f"JSON error: {exc}", foreground="red")
                return False

        if self.file_suffix == ".xml":
            try:
                import xml.etree.ElementTree as ET
                ET.fromstring(text)
                self._status.config(text="XML is valid.", foreground="green")
                return True
            except Exception as exc:
                self._status.config(text=f"XML error: {exc}", foreground="red")
                return False

        self._status.config(text="Text file (no validation).", foreground="gray")
        return True

    def _save(self) -> None:
        if self.path is None:
            messagebox.showwarning("No File", "Please select a settings file first.")
            return

        if not self._validate():
            return

        text = self._text_editor.get("1.0", tk.END)
        try:
            self.path.write_text(text, encoding="utf-8")
            self._status.config(text=f"Saved {self.path.name}", foreground="green")
            self._maybe_save_instance_override(text)
        except Exception as exc:
            messagebox.showerror("Save Error", f"Could not save {self.path}:\n{exc}")

    def _maybe_save_instance_override(self, content: str) -> None:
        """If an instance root is available, store this edit as a per-instance override."""
        if self.get_instance_root is None:
            return
        instance_root = self.get_instance_root()
        if instance_root is None:
            return

        from dayzconfigmaster.config.per_instance_config import PerInstanceConfigManager

        manager = PerInstanceConfigManager(instance_root)
        relative_target = self._compute_override_target()
        try:
            manager.export_mod_settings_override(
                source_path=self.path,
                relative_target=relative_target,
                content=content,
            )
            self._status.config(
                text=f"Saved {self.path.name} and stored per-instance override.",
                foreground="green",
            )
        except Exception as exc:
            self._status.config(
                text=f"Saved file but could not store instance override: {exc}",
                foreground="orange",
            )

    def _compute_override_target(self) -> str:
        """Decide where this file should live inside the instance.

        Expansion server-profile settings belong under ExpansionMod/Settings/.
        Expansion mission settings keep their full ``mpmissions/<mission>/...``
        path so the deployment code knows to write them into the mission folder.
        Other mod folders (TraderPlus, VPPAdminTools, etc.) keep their original
        folder name under profiles/.
        """
        if self.path is None:
            return ""

        parts = [p.lower() for p in self.path.parts]
        name = self.path.name

        # Expansion profile settings: any .../expansion/settings/*.json -> ExpansionMod/Settings/*.json
        if "expansion" in parts and "settings" in parts:
            idx = parts.index("settings")
            rel = self.path.parts[idx + 1:]
            return str(Path("ExpansionMod") / "Settings" / Path(*rel))

        # Expansion mission settings under mpmissions
        if "mpmissions" in parts and "expansion" in parts:
            idx = parts.index("mpmissions")
            rel = self.path.parts[idx:]
            return str(Path(*rel))

        # TraderPlus and other known mod settings folders
        for known in ("traderplus", "vppadmintools", "zomberry", "cfgeconomycore"):
            if known in parts:
                idx = parts.index(known)
                rel = self.path.parts[idx:]
                return str(Path("profiles") / rel)

        # Default: place at profiles root using the file name.
        return name

    def get_text(self) -> str:
        return self._text_editor.get("1.0", tk.END)


def _format_value(value: Any) -> str:
    """Short display string for a setting value in the tree."""
    if value is None:
        return ""
    if isinstance(value, bool):
        return "true" if value else "false"
    if isinstance(value, list):
        if not value:
            return "[]"
        if len(value) <= 3:
            return json.dumps(value, ensure_ascii=False)
        return f"[{value[0]!r}, ... ({len(value)} items)]"
    if isinstance(value, dict):
        return f"{{... {len(value)} keys}}"
    return str(value)
