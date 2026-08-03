# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Tkinter UI tab for one-click mod XML integration workflows."""

import tkinter as tk
from tkinter import ttk, messagebox
from pathlib import Path
from typing import Optional, Callable, Dict


def _import_workflow():
    try:
        from ..config.mod_integration import ModIntegrationWorkflow
    except ImportError:
        from dayzconfigmaster.config.mod_integration import ModIntegrationWorkflow
    return ModIntegrationWorkflow


def _import_templates():
    try:
        from ..config.mod_integration import VEHICLE_TEMPLATES
    except ImportError:
        from dayzconfigmaster.config.mod_integration import VEHICLE_TEMPLATES
    return VEHICLE_TEMPLATES


ModIntegrationWorkflow = _import_workflow()
VEHICLE_TEMPLATES = _import_templates()


class ModIntegrationTab:
    """Tab that detects and applies XML changes required by mods."""

    def __init__(
        self,
        parent: ttk.Frame,
        get_mission_root: Callable[[], Optional[Path]],
        get_workshop_dir: Optional[Callable[[], Optional[str]]] = None,
    ):
        self.parent = parent
        self.get_mission_root = get_mission_root
        self.get_workshop_dir = get_workshop_dir
        self.workflow: Optional[ModIntegrationWorkflow] = None
        self._vehicle_sources: Dict[str, str] = {}

        parent.columnconfigure(0, weight=1)
        parent.rowconfigure(3, weight=1)

        self._build_header()
        self._build_vehicle_selector()
        self._build_actions_tree()
        self._build_status()

        # Populate the vehicle picker once the UI exists.
        # Defer discovery so the GUI window appears before any I/O scan.

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
        selector = ttk.LabelFrame(self.parent, text="Vehicle Selection", padding=5)
        selector.grid(row=2, column=0, sticky=tk.EW, padx=10, pady=5)
        selector.columnconfigure(1, weight=1)

        ttk.Label(selector, text="Vehicle Class:").grid(row=0, column=0, sticky=tk.W, padx=(0, 5))
        self._vehicle_var = tk.StringVar(value="")
        self._vehicle_combo = ttk.Combobox(
            selector,
            textvariable=self._vehicle_var,
            width=40,
            state="normal",
        )
        self._vehicle_combo.grid(row=0, column=1, sticky=tk.EW, padx=(0, 10))
        self._vehicle_combo.bind("<<ComboboxSelected>>", self._on_vehicle_selected)
        self._vehicle_combo.bind("<KeyRelease>", self._on_vehicle_selected)

        ttk.Button(selector, text="Refresh List", command=self._refresh_vehicles).grid(row=0, column=2, padx=2)
        ttk.Button(selector, text="Detect Required Changes", command=self._detect).grid(row=0, column=3, padx=2)
        ttk.Button(selector, text="Apply All Changes", command=self._apply).grid(row=0, column=4, padx=2)

        self._source_var = tk.StringVar(value="")
        self._source_label = ttk.Label(
            selector,
            textvariable=self._source_var,
            foreground="gray",
            wraplength=800,
        )
        self._source_label.grid(row=1, column=0, columnspan=5, sticky=tk.EW, pady=(5, 0))

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

    def _get_workshop_path(self) -> Optional[Path]:
        if self.get_workshop_dir is None:
            return None
        raw = self.get_workshop_dir()
        if not raw:
            return None
        return Path(raw)

    def _refresh_vehicles(self, done_callback: Optional[Callable[[], None]] = None) -> None:
        if not self._ensure_workflow():
            if done_callback:
                done_callback()
            return

        self._status.config(
            text="Scanning workshop/mission for vehicles...",
            foreground="blue",
        )
        self.parent.update_idletasks()

        def _scan() -> None:
            try:
                workshop_path = self._get_workshop_path()
                self.workflow.workshop_dir = workshop_path
                self._vehicle_sources = dict(self.workflow.discover_vehicles(workshop_path))
                self._vehicle_combo["values"] = sorted(self._vehicle_sources.keys())

                if self._vehicle_sources:
                    status_text = f"Found {len(self._vehicle_sources)} vehicle class(es)."
                    status_color = "gray"
                else:
                    status_text = (
                        "No vehicle classes discovered. Type a class name manually "
                        "or check your Workshop Directory."
                    )
                    status_color = "orange"
            except Exception as exc:  # pragma: no cover - defensive UI handling
                status_text = f"Vehicle scan failed: {exc}"
                status_color = "red"

            def _apply() -> None:
                self._status.config(text=status_text, foreground=status_color)
                self._on_vehicle_selected()
                if done_callback:
                    done_callback()

            self.parent.after(0, _apply)

        import threading
        threading.Thread(target=_scan, daemon=True).start()

    def _on_vehicle_selected(self, event=None) -> None:
        name = self._vehicle_var.get().strip()
        if not name:
            self._source_var.set("Type or select a vehicle class name.")
            return

        # Import here to avoid a circular import at module load time.
        try:
            from ..config.mod_integration import _is_likely_vehicle_part
        except ImportError:
            from dayzconfigmaster.config.mod_integration import _is_likely_vehicle_part

        if _is_likely_vehicle_part(name):
            self._source_var.set(
                "Warning: this looks like a vehicle part, not a whole vehicle. "
                "Quick Setup is meant for vehicle class names only."
            )
            return

        source = self._vehicle_sources.get(name)
        if source:
            self._source_var.set(f"Source: {source}")
        elif name in VEHICLE_TEMPLATES:
            self._source_var.set("Source: Vanilla template")
        else:
            self._source_var.set(
                "Source: Custom entry (no wheel template; wheels must be configured manually)"
            )

    def _ensure_workflow(self) -> bool:
        root = self.get_mission_root()
        if root is None:
            self._status.config(text="No mission root available.", foreground="red")
            return False
        workshop_path = self._get_workshop_path()
        if self.workflow is None or self.workflow.editor.mission_root != root:
            self.workflow = ModIntegrationWorkflow(root, workshop_dir=workshop_path)
        else:
            self.workflow.workshop_dir = workshop_path
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

        self._status.config(text=f"Applying changes for {vehicle}...", foreground="blue")
        self.parent.update_idletasks()

        def _run() -> None:
            try:
                result = self.workflow.integrate_vehicle_mod(vehicle)
            except Exception as exc:  # pragma: no cover - defensive UI handling
                result = None
                error = str(exc)

            def _show() -> None:
                for item in self._tree.get_children():
                    self._tree.delete(item)

                if result is None:
                    self._status.config(text=f"Error applying changes: {error}", foreground="red")
                    messagebox.showerror("Integration Error", error)
                    return

                for action in result.actions:
                    status = "Applied" if action.applied else f"Failed: {action.error}"
                    self._tree.insert("", tk.END, values=(action.file_name, action.description, status))

                if result.ok:
                    self._status.config(text=f"{vehicle} integrated successfully.", foreground="green")
                    messagebox.showinfo("Integration Complete", f"{vehicle} has been enabled.")
                else:
                    self._status.config(text=f"Some changes for {vehicle} could not be applied.", foreground="red")
                    messagebox.showerror("Integration Incomplete", "One or more XML changes failed. Check the action list.")

            self.parent.after(0, _show)

        import threading
        threading.Thread(target=_run, daemon=True).start()
