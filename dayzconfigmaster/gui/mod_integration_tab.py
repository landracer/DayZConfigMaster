# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Tkinter UI tab for one-click mod XML integration workflows."""

import tkinter as tk
from tkinter import ttk, messagebox
from pathlib import Path
from typing import Optional, Callable


def _import_workflow():
    try:
        from ..config.mod_integration import ModIntegrationWorkflow
    except ImportError:
        from dayzconfigmaster.config.mod_integration import ModIntegrationWorkflow
    return ModIntegrationWorkflow


ModIntegrationWorkflow = _import_workflow()


class ModIntegrationTab:
    """Tab that detects and applies XML changes required by mods."""

    def __init__(self, parent: ttk.Frame, get_mission_root: Callable[[], Optional[Path]]):
        self.parent = parent
        self.get_mission_root = get_mission_root
        self.workflow: Optional[ModIntegrationWorkflow] = None

        parent.columnconfigure(0, weight=1)
        parent.rowconfigure(2, weight=1)

        self._build_header()
        self._build_vehicle_selector()
        self._build_actions_tree()
        self._build_status()

    def _build_header(self) -> None:
        header = ttk.Label(
            self.parent,
            text="Mod Integration Assistant",
            font=("Arial", 14, "bold"),
        )
        header.grid(row=0, column=0, sticky=tk.W, padx=10, pady=10)

        desc = ttk.Label(
            self.parent,
            text="Automatically detect and apply the XML changes required to enable vehicle and item mods.",
            wraplength=800,
            foreground="gray",
        )
        desc.grid(row=1, column=0, sticky=tk.EW, padx=10, pady=(0, 10))

    def _build_vehicle_selector(self) -> None:
        selector = ttk.Frame(self.parent)
        selector.grid(row=2, column=0, sticky=tk.EW, padx=10, pady=5)

        ttk.Label(selector, text="Vehicle Class Name:").pack(side=tk.LEFT, padx=(0, 5))
        self._vehicle_var = tk.StringVar(value="OffroadHatchback")
        self._vehicle_entry = ttk.Entry(selector, textvariable=self._vehicle_var, width=30)
        self._vehicle_entry.pack(side=tk.LEFT, padx=(0, 10))

        ttk.Button(selector, text="Detect Required Changes", command=self._detect).pack(side=tk.LEFT, padx=2)
        ttk.Button(selector, text="Apply All Changes", command=self._apply).pack(side=tk.LEFT, padx=2)

    def _build_actions_tree(self) -> None:
        tree_frame = ttk.LabelFrame(self.parent, text="Required Actions", padding=5)
        tree_frame.grid(row=3, column=0, sticky=tk.NSEW, padx=10, pady=5)
        tree_frame.columnconfigure(0, weight=1)
        tree_frame.rowconfigure(0, weight=1)

        cols = ("File", "Action", "Status")
        self._tree = ttk.Treeview(tree_frame, columns=cols, show="headings")
        for c in cols:
            self._tree.heading(c, text=c)
            self._tree.column(c, width=200 if c == "Action" else 140)
        self._tree.grid(row=0, column=0, sticky=tk.NSEW)

        scroll = ttk.Scrollbar(tree_frame, orient=tk.VERTICAL, command=self._tree.yview)
        scroll.grid(row=0, column=1, sticky=tk.NS)
        self._tree.configure(yscrollcommand=scroll.set)

    def _build_status(self) -> None:
        self._status = ttk.Label(self.parent, text="", foreground="gray")
        self._status.grid(row=4, column=0, sticky=tk.EW, padx=10, pady=(5, 10))

    def _ensure_workflow(self) -> bool:
        root = self.get_mission_root()
        if root is None:
            self._status.config(text="No mission root available.", foreground="red")
            return False
        if self.workflow is None or self.workflow.editor.mission_root != root:
            self.workflow = ModIntegrationWorkflow(root)
        return True

    def _detect(self) -> None:
        if not self._ensure_workflow():
            return
        vehicle = self._vehicle_var.get().strip()
        if not vehicle:
            self._status.config(text="Enter a vehicle class name.", foreground="red")
            return

        for item in self._tree.get_children():
            self._tree.delete(item)

        actions = self.workflow.detect_actions(vehicle)
        if not actions:
            self._tree.insert("", tk.END, values=("All files", f"{vehicle} is already fully integrated.", "OK"))
            self._status.config(text=f"{vehicle} requires no changes.", foreground="green")
            return

        for action in actions:
            status = "Missing file" if "missing" in action.description.lower() else "Pending"
            self._tree.insert("", tk.END, values=(action.file_name, action.description, status))
        self._status.config(text=f"Detected {len(actions)} required change(s) for {vehicle}.", foreground="orange")

    def _apply(self) -> None:
        if not self._ensure_workflow():
            return
        vehicle = self._vehicle_var.get().strip()
        if not vehicle:
            return

        if not messagebox.askyesno(
            "Confirm Integration",
            f"Apply all XML changes needed to enable {vehicle}?\n\n"
            "Backups will be created automatically.",
        ):
            return

        result = self.workflow.integrate_vehicle_mod(vehicle)
        for item in self._tree.get_children():
            self._tree.delete(item)

        for action in result.actions:
            status = "Applied" if action.applied else f"Failed: {action.error}"
            self._tree.insert("", tk.END, values=(action.file_name, action.description, status))

        if result.ok:
            self._status.config(text=f"{vehicle} integrated successfully.", foreground="green")
            messagebox.showinfo("Integration Complete", f"{vehicle} has been enabled.")
        else:
            self._status.config(text=f"Some changes for {vehicle} could not be applied.", foreground="red")
            messagebox.showerror("Integration Incomplete", "One or more XML changes failed. Check the action list.")
