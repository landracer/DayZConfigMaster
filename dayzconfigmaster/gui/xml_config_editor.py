# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Tkinter UI for editing core DayZ server XML configuration files."""

import tkinter as tk
from tkinter import ttk, messagebox, scrolledtext
from pathlib import Path
from typing import Optional, Callable

import sys


def _import_editor():
    try:
        from ..config.mod_integration import XmlConfigEditor
    except ImportError:
        from dayzconfigmaster.config.mod_integration import XmlConfigEditor
    return XmlConfigEditor


XmlConfigEditor = _import_editor()


class XmlConfigEditorTab:
    """A tab for editing events.xml, cfgspawnabletypes.xml, and types.xml."""

    def __init__(self, parent: ttk.Frame, get_mission_root: Callable[[], Optional[Path]]):
        self.parent = parent
        self.get_mission_root = get_mission_root
        self.editor: Optional[XmlConfigEditor] = None
        self.current_file: Optional[str] = None

        parent.columnconfigure(0, weight=1)
        parent.rowconfigure(1, weight=1)

        self._build_toolbar()
        self._build_editor()
        self._build_status()

    def _build_toolbar(self) -> None:
        toolbar = ttk.Frame(self.parent)
        toolbar.grid(row=0, column=0, sticky=tk.EW, padx=5, pady=5)

        ttk.Label(toolbar, text="File:").pack(side=tk.LEFT, padx=(0, 5))
        self._file_var = tk.StringVar(value="events.xml")
        self._file_combo = ttk.Combobox(
            toolbar,
            textvariable=self._file_var,
            values=XmlConfigEditor.supported_files,
            state="readonly",
            width=25,
        )
        self._file_combo.pack(side=tk.LEFT, padx=(0, 5))
        self._file_combo.bind("<<ComboboxSelected>>", lambda _e: self.load_file())

        ttk.Button(toolbar, text="Load", command=self.load_file).pack(side=tk.LEFT, padx=2)
        ttk.Button(toolbar, text="Save", command=self.save_file).pack(side=tk.LEFT, padx=2)
        ttk.Button(toolbar, text="Validate XML", command=self.validate_xml).pack(side=tk.LEFT, padx=2)

        ttk.Separator(toolbar, orient=tk.VERTICAL).pack(side=tk.LEFT, fill=tk.Y, padx=10)

        ttk.Label(toolbar, text="Vehicle:").pack(side=tk.LEFT, padx=(0, 5))
        self._vehicle_var = tk.StringVar(value="OffroadHatchback")
        vehicle_combo = ttk.Combobox(
            toolbar,
            textvariable=self._vehicle_var,
            values=sorted([
                "OffroadHatchback", "Hatchback_02", "Sedan_02",
                "CivilianVan", "Truck_01_Covered", "Offroad_02",
            ]),
            width=22,
        )
        vehicle_combo.pack(side=tk.LEFT, padx=(0, 5))

        ttk.Button(toolbar, text="Enable Spawn", command=self._enable_spawn).pack(side=tk.LEFT, padx=2)
        ttk.Button(toolbar, text="Add to Types", command=self._add_to_types).pack(side=tk.LEFT, padx=2)
        ttk.Button(toolbar, text="Add Attachments", command=self._add_attachments).pack(side=tk.LEFT, padx=2)

    def _build_editor(self) -> None:
        editor_frame = ttk.Frame(self.parent)
        editor_frame.grid(row=1, column=0, sticky=tk.NSEW, padx=5, pady=5)
        editor_frame.columnconfigure(0, weight=1)
        editor_frame.rowconfigure(0, weight=1)

        self._text = scrolledtext.ScrolledText(
            editor_frame,
            wrap=tk.NONE,
            font=("Courier", 10),
            undo=True,
        )
        self._text.grid(row=0, column=0, sticky=tk.NSEW)

    def _build_status(self) -> None:
        self._status = ttk.Label(self.parent, text="No mission root selected.", foreground="gray")
        self._status.grid(row=2, column=0, sticky=tk.EW, padx=5, pady=(0, 5))

    def _ensure_editor(self) -> bool:
        root = self.get_mission_root()
        if root is None:
            self._status.config(text="No mission root available.", foreground="red")
            return False
        if self.editor is None or self.editor.mission_root != root:
            self.editor = XmlConfigEditor(root)
        return True

    def load_file(self) -> None:
        if not self._ensure_editor():
            return
        file_name = self._file_var.get()
        self.current_file = file_name
        text = self.editor.read_text(file_name)
        if text is None:
            self._text.delete("1.0", tk.END)
            self._text.insert("1.0", f"<!-- {file_name} not found; saving will create it -->\n")
            self._status.config(text=f"{file_name} not found.", foreground="orange")
            return
        self._text.delete("1.0", tk.END)
        self._text.insert("1.0", text)
        self._status.config(text=f"Loaded {file_name}", foreground="green")

    def save_file(self) -> None:
        if not self._ensure_editor() or not self.current_file:
            return
        text = self._text.get("1.0", tk.END)
        path = self.editor.path_for(self.current_file)
        if path is None:
            return
        try:
            import xml.etree.ElementTree as ET
            ET.fromstring(text)
        except Exception as exc:
            messagebox.showerror("Invalid XML", f"Cannot save: {exc}")
            return

        backup_dir = path.parent / "backups" / "xml_editor"
        backup_dir.mkdir(parents=True, exist_ok=True)
        timestamp = __import__("datetime").datetime.now().strftime("%Y%m%d_%H%M%S")
        backup_path = backup_dir / f"{path.stem}_{timestamp}{path.suffix}"
        if path.exists():
            import shutil
            shutil.copy2(path, backup_path)
        path.write_text(text, encoding="utf-8")
        self._status.config(text=f"Saved {self.current_file}; backup: {backup_path.name}", foreground="green")

    def validate_xml(self) -> None:
        text = self._text.get("1.0", tk.END)
        try:
            import xml.etree.ElementTree as ET
            ET.fromstring(text)
            self._status.config(text="XML is valid.", foreground="green")
        except Exception as exc:
            self._status.config(text=f"XML error: {exc}", foreground="red")

    def _enable_spawn(self) -> None:
        if not self._ensure_editor():
            return
        vehicle = self._vehicle_var.get().strip()
        if not vehicle:
            return
        if self.editor.enable_vehicle_spawning(vehicle, active=True):
            self._status.config(text=f"Enabled spawning for {vehicle}", foreground="green")
            self.load_file()
        else:
            self._status.config(text=f"Failed to enable spawning for {vehicle}", foreground="red")

    def _add_to_types(self) -> None:
        if not self._ensure_editor():
            return
        vehicle = self._vehicle_var.get().strip()
        if not vehicle:
            return
        if self.editor.add_vehicle_to_types_xml(vehicle):
            self._status.config(text=f"Added {vehicle} to types.xml", foreground="green")
            self._file_var.set("types.xml")
            self.load_file()
        else:
            self._status.config(text=f"Failed to add {vehicle} to types.xml", foreground="red")

    def _add_attachments(self) -> None:
        if not self._ensure_editor():
            return
        vehicle = self._vehicle_var.get().strip()
        if not vehicle:
            return
        if self.editor.add_vehicle_attachments(vehicle):
            self._status.config(text=f"Added attachments for {vehicle}", foreground="green")
            self._file_var.set("cfgspawnabletypes.xml")
            self.load_file()
        else:
            self._status.config(text=f"Failed to add attachments for {vehicle}", foreground="red")
