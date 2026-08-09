# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Tkinter UI tab for one-click mod XML integration workflows."""

import contextlib
import json
import random
import tkinter as tk
from tkinter import ttk, messagebox, filedialog
from pathlib import Path
from typing import Optional, Callable, Dict, Tuple, List, Any


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


def _import_spawnable():
    try:
        from ..config.mod_integration import SpawnableClass
    except ImportError:
        from dayzconfigmaster.config.mod_integration import SpawnableClass
    return SpawnableClass


ModIntegrationWorkflow = _import_workflow()
VEHICLE_TEMPLATES = _import_templates()
SpawnableClass = _import_spawnable()

# Human-readable labels for spawnable categories.
_CATEGORY_LABELS = {
    "vehicle": "Land vehicle",
    "air": "Aircraft / Helicopter",
    "water": "Boat / Water",
    "weapon": "Weapon",
    "gear": "Gear / Loadout",
    "generic": "Generic item",
}


class ModIntegrationTab:
    """Tab that detects and applies XML changes required by mods."""

    def __init__(
        self,
        parent: ttk.Frame,
        get_mission_root: Callable[[], Optional[Path]],
        get_workshop_dir: Optional[Callable[[], Optional[str]]] = None,
        get_instances: Optional[Callable[[], List[Dict[str, Any]]]] = None,
        get_instance_display_name: Optional[Callable[[Dict[str, Any]], str]] = None,
        get_instance_root: Optional[Callable[[Dict[str, Any]], Optional[Path]]] = None,
        get_instance_mission_root: Optional[Callable[[Dict[str, Any]], Optional[Path]]] = None,
    ):
        self.parent = parent
        self.get_mission_root = get_mission_root
        self.get_workshop_dir = get_workshop_dir
        self.get_instances = get_instances
        self.get_instance_display_name = get_instance_display_name
        self.get_instance_root = get_instance_root
        self.get_instance_mission_root = get_instance_mission_root
        self.workflow: Optional[ModIntegrationWorkflow] = None
        # name -> SpawnableClass metadata discovered from CE files / heuristics.
        self._spawnable_sources: Dict[str, SpawnableClass] = {}
        # Spawnables actually applied in this session (in order).
        # Each dict holds: name, category, source, spawn_count, usage, value, locations.
        self._applied_spawnables: List[Dict[str, Any]] = []
        self._selected_instance: Optional[Dict[str, Any]] = None

        parent.columnconfigure(0, weight=1)
        parent.rowconfigure(5, weight=1)

        self._build_header()
        self._build_filter_bar()
        self._build_spawnable_selector()
        self._build_loadout_queue()
        self._build_actions_tree()
        self._build_remix_bar()
        self._build_status()

        # Show an initial hint; discovery only runs when the user clicks
        # Refresh List so startup is not blocked by workshop I/O.
        self._status.config(
            text="Click 'Refresh List' to discover mod spawnables from the mission and workshop.",
            foreground="gray",
        )
        self._busy_count = 0

    @contextlib.contextmanager
    def _busy_cursor(self, message: str = "Processing..."):
        """Show a busy cursor and status message while work runs."""
        self._set_busy(message)
        try:
            yield
        finally:
            self._clear_busy()

    def _set_busy(self, message: str = "Processing...") -> None:
        self._busy_count += 1
        self._status.config(text=message, foreground="blue")
        try:
            self.parent.winfo_toplevel().config(cursor="watch")
        except tk.TclError:
            pass
        self.parent.update_idletasks()

    def _clear_busy(self) -> None:
        self._busy_count = max(0, self._busy_count - 1)
        if self._busy_count == 0:
            try:
                self.parent.winfo_toplevel().config(cursor="")
            except tk.TclError:
                pass

    def _build_header(self) -> None:
        # Slim top bar: title, instance picker, buttons, and mission path.
        top = ttk.Frame(self.parent)
        top.grid(row=0, column=0, sticky=tk.EW, padx=10, pady=(5, 2))
        top.columnconfigure(1, weight=1)

        ttk.Label(top, text="Spawn Loadout", font=("Arial", 11, "bold")).grid(
            row=0, column=0, sticky=tk.W, padx=(0, 10)
        )

        self._instance_var = tk.StringVar()
        self._instance_combo = ttk.Combobox(
            top,
            textvariable=self._instance_var,
            state="readonly",
            width=35,
        )
        self._instance_combo.grid(row=0, column=1, sticky=tk.W, padx=5)
        self._instance_combo.bind("<<ComboboxSelected>>", self._on_instance_selected)

        ttk.Button(top, text="Save Loadout", command=self._save_instance_loadout).grid(row=0, column=2, padx=2)
        ttk.Button(top, text="Load Saved Loadout", command=self._load_instance_loadout).grid(row=0, column=3, padx=2)

        self._mission_path_var = tk.StringVar(value="Mission path: not selected")
        self._mission_path_label = ttk.Label(
            top,
            textvariable=self._mission_path_var,
            foreground="gray",
        )
        self._mission_path_label.grid(row=1, column=0, columnspan=4, sticky=tk.EW, pady=(2, 0))

        self._mission_warning_var = tk.StringVar(value="")
        self._mission_warning_label = ttk.Label(
            self.parent,
            textvariable=self._mission_warning_var,
            wraplength=800,
            foreground="red",
        )
        self._mission_warning_label.grid(row=1, column=0, sticky=tk.EW, padx=10, pady=(0, 5))

    def _build_filter_bar(self) -> None:
        frame = ttk.LabelFrame(self.parent, text="Category Filters", padding=5)
        frame.grid(row=2, column=0, sticky=tk.EW, padx=10, pady=5)

        self._category_vars: Dict[str, tk.BooleanVar] = {}
        default_categories = ("vehicle", "air", "water", "weapon", "gear")
        for cat in ("vehicle", "air", "water", "weapon", "gear", "generic"):
            var = tk.BooleanVar(value=cat in default_categories)
            self._category_vars[cat] = var
            ttk.Checkbutton(
                frame,
                text=_CATEGORY_LABELS.get(cat, cat),
                variable=var,
                command=self._on_category_changed,
            ).pack(side=tk.LEFT, padx=5)

    def _build_spawnable_selector(self) -> None:
        selector = ttk.LabelFrame(self.parent, text="Spawnable Selection", padding=5)
        selector.grid(row=3, column=0, sticky=tk.EW, padx=10, pady=5)
        selector.columnconfigure(1, weight=1)

        ttk.Label(selector, text="Search:").grid(row=0, column=0, sticky=tk.W, padx=(0, 5))
        self._search_var = tk.StringVar(value="")
        self._search_entry = ttk.Entry(selector, textvariable=self._search_var, width=30)
        self._search_entry.grid(row=0, column=1, sticky=tk.EW, padx=(0, 10))
        self._search_entry.bind("<KeyRelease>", self._on_search_changed)
        self._search_entry.bind("<Return>", self._on_search_changed)
        ttk.Button(selector, text="Clear", command=self._clear_search).grid(row=0, column=2, padx=2)

        self._match_count_var = tk.StringVar(value="")
        ttk.Label(
            selector,
            textvariable=self._match_count_var,
            foreground="gray",
        ).grid(row=0, column=3, columnspan=2, sticky=tk.W, padx=(10, 0))

        ttk.Label(selector, text="Class name:").grid(row=1, column=0, sticky=tk.W, padx=(0, 5), pady=(5, 0))
        self._vehicle_var = tk.StringVar(value="")
        self._vehicle_combo = ttk.Combobox(
            selector,
            textvariable=self._vehicle_var,
            width=40,
            state="readonly",
        )
        self._vehicle_combo.grid(row=1, column=1, sticky=tk.EW, padx=(0, 10), pady=(5, 0))
        self._vehicle_combo.bind("<<ComboboxSelected>>", self._on_spawnable_selected)

        ttk.Button(selector, text="Refresh List", command=self._refresh_spawnables).grid(row=1, column=2, padx=2, pady=(5, 0))
        ttk.Button(selector, text="Detect", command=self._detect).grid(row=1, column=3, padx=2, pady=(5, 0))
        ttk.Button(selector, text="Add to Loadout", command=self._add_to_loadout).grid(row=1, column=4, padx=2, pady=(5, 0))
        ttk.Button(selector, text="Apply Now", command=self._apply).grid(row=1, column=5, padx=2, pady=(5, 0))

        # Category-aware count controls.
        self._controls_frame = ttk.Frame(selector)
        self._controls_frame.grid(row=2, column=0, columnspan=6, sticky=tk.EW, pady=(8, 0))

        # Vehicle / Air / Water controls.
        self._vehicle_controls = ttk.Frame(self._controls_frame)
        ttk.Label(self._vehicle_controls, text="Map limit:").pack(side=tk.LEFT, padx=(0, 2))
        self._map_limit_var = tk.IntVar(value=3)
        ttk.Spinbox(self._vehicle_controls, from_=0, to=100, textvariable=self._map_limit_var, width=8).pack(side=tk.LEFT, padx=(0, 8))
        ttk.Label(self._vehicle_controls, text="Event min:").pack(side=tk.LEFT, padx=(0, 2))
        self._event_min_var = tk.IntVar(value=1)
        ttk.Spinbox(self._vehicle_controls, from_=0, to=100, textvariable=self._event_min_var, width=8).pack(side=tk.LEFT, padx=(0, 8))
        ttk.Label(self._vehicle_controls, text="Event max:").pack(side=tk.LEFT, padx=(0, 2))
        self._event_max_var = tk.IntVar(value=1)
        ttk.Spinbox(self._vehicle_controls, from_=0, to=100, textvariable=self._event_max_var, width=8).pack(side=tk.LEFT, padx=(0, 8))

        # Loot (weapon/gear) controls.
        self._loot_controls = ttk.Frame(self._controls_frame)
        ttk.Label(self._loot_controls, text="Nominal:").pack(side=tk.LEFT, padx=(0, 2))
        self._loot_nominal_var = tk.IntVar(value=10)
        ttk.Spinbox(self._loot_controls, from_=0, to=500, textvariable=self._loot_nominal_var, width=8).pack(side=tk.LEFT, padx=(0, 8))
        ttk.Label(self._loot_controls, text="Min:").pack(side=tk.LEFT, padx=(0, 2))
        self._loot_min_var = tk.IntVar(value=5)
        ttk.Spinbox(self._loot_controls, from_=0, to=500, textvariable=self._loot_min_var, width=8).pack(side=tk.LEFT, padx=(0, 8))
        ttk.Label(self._loot_controls, text="Usage:").pack(side=tk.LEFT, padx=(0, 2))
        self._usage_var = tk.StringVar(value="Town")
        ttk.Entry(self._loot_controls, textvariable=self._usage_var, width=12).pack(side=tk.LEFT, padx=(0, 8))
        ttk.Label(self._loot_controls, text="Value:").pack(side=tk.LEFT, padx=(0, 2))
        self._value_var = tk.StringVar(value="Tier12")
        ttk.Entry(self._loot_controls, textvariable=self._value_var, width=12).pack(side=tk.LEFT, padx=(0, 8))
        ttk.Label(self._loot_controls, text="Tier:").pack(side=tk.LEFT, padx=(0, 2))
        self._tier_var = tk.IntVar(value=1)
        ttk.Spinbox(self._loot_controls, from_=1, to=4, textvariable=self._tier_var, width=5).pack(side=tk.LEFT)

        # Advanced loot controls (lifetime, restock, quant).
        self._advanced_loot_controls = ttk.Frame(self._controls_frame)
        ttk.Label(self._advanced_loot_controls, text="Lifetime:").pack(side=tk.LEFT, padx=(0, 2))
        self._loot_lifetime_var = tk.IntVar(value=7200)
        ttk.Spinbox(self._advanced_loot_controls, from_=0, to=3888000, textvariable=self._loot_lifetime_var, width=10).pack(side=tk.LEFT, padx=(0, 8))
        ttk.Label(self._advanced_loot_controls, text="Restock:").pack(side=tk.LEFT, padx=(0, 2))
        self._loot_restock_var = tk.IntVar(value=0)
        ttk.Spinbox(self._advanced_loot_controls, from_=0, to=86400, textvariable=self._loot_restock_var, width=10).pack(side=tk.LEFT, padx=(0, 8))
        ttk.Label(self._advanced_loot_controls, text="QuantMin:").pack(side=tk.LEFT, padx=(0, 2))
        self._loot_quantmin_var = tk.IntVar(value=30)
        ttk.Spinbox(self._advanced_loot_controls, from_=-1, to=100, textvariable=self._loot_quantmin_var, width=8).pack(side=tk.LEFT, padx=(0, 8))
        ttk.Label(self._advanced_loot_controls, text="QuantMax:").pack(side=tk.LEFT, padx=(0, 2))
        self._loot_quantmax_var = tk.IntVar(value=80)
        ttk.Spinbox(self._advanced_loot_controls, from_=-1, to=100, textvariable=self._loot_quantmax_var, width=8).pack(side=tk.LEFT)

        self._source_var = tk.StringVar(value="")
        self._source_label = ttk.Label(
            selector,
            textvariable=self._source_var,
            foreground="gray",
            wraplength=800,
        )
        self._source_label.grid(row=3, column=0, columnspan=6, sticky=tk.EW, pady=(5, 0))

        ttk.Label(
            selector,
            text="Exact spawn coordinates (optional): one position per line as x z y a. Leave blank for random event spawns.",
            foreground="gray",
            wraplength=800,
        ).grid(row=4, column=0, columnspan=6, sticky=tk.W, pady=(8, 0))

        self._locations_text = tk.Text(
            selector,
            height=3,
            width=60,
            wrap=tk.NONE,
        )
        self._locations_text.grid(row=5, column=0, columnspan=6, sticky=tk.EW, pady=(2, 0))

        self._on_spawnable_selected()

    def _build_loadout_queue(self) -> None:
        queue_frame = ttk.LabelFrame(self.parent, text="Spawn Loadout Queue", padding=5)
        queue_frame.grid(row=4, column=0, sticky=tk.EW, padx=10, pady=5)
        queue_frame.columnconfigure(0, weight=1)

        cols = ("Class", "Category", "Count", "Notes")
        self._queue_tree = ttk.Treeview(queue_frame, columns=cols, show="headings", height=5)
        for c, w in zip(cols, (220, 120, 100, 300)):
            self._queue_tree.heading(c, text=c)
            self._queue_tree.column(c, width=w, anchor=tk.CENTER if c == "Count" else tk.W)
        self._queue_tree.grid(row=0, column=0, sticky=tk.NSEW)

        scroll = ttk.Scrollbar(queue_frame, orient=tk.VERTICAL, command=self._queue_tree.yview)
        scroll.grid(row=0, column=1, sticky=tk.NS)
        self._queue_tree.configure(yscrollcommand=scroll.set)

        btn_frame = ttk.Frame(queue_frame)
        btn_frame.grid(row=1, column=0, columnspan=2, sticky=tk.EW, pady=(5, 0))
        ttk.Button(btn_frame, text="Remove Selected", command=self._remove_selected_loadout).pack(side=tk.LEFT, padx=2)
        ttk.Button(btn_frame, text="Clear Queue", command=self._clear_loadout).pack(side=tk.LEFT, padx=2)
        ttk.Button(btn_frame, text="Save Queue", command=self._save_loadout).pack(side=tk.LEFT, padx=2)
        ttk.Button(btn_frame, text="Load Queue", command=self._load_loadout).pack(side=tk.LEFT, padx=2)
        ttk.Button(btn_frame, text="Apply Loadout", command=self._apply_loadout).pack(side=tk.RIGHT, padx=2)

    def _build_actions_tree(self) -> None:
        tree_frame = ttk.LabelFrame(self.parent, text="Required Actions", padding=5)
        tree_frame.grid(row=5, column=0, sticky=tk.NSEW, padx=10, pady=5)
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

    def _build_remix_bar(self) -> None:
        frame = ttk.LabelFrame(self.parent, text="Random Remix", padding=5)
        frame.grid(row=6, column=0, sticky=tk.EW, padx=10, pady=5)

        ttk.Label(frame, text="Enable").pack(side=tk.LEFT, padx=(0, 5))
        self._remix_percent_var = tk.IntVar(value=25)
        ttk.Spinbox(
            frame,
            from_=1,
            to=100,
            textvariable=self._remix_percent_var,
            width=8,
        ).pack(side=tk.LEFT, padx=(0, 2))
        ttk.Label(frame, text="% of filtered spawnables at count").pack(side=tk.LEFT, padx=(0, 5))
        self._remix_count_var = tk.IntVar(value=10)
        ttk.Spinbox(
            frame,
            from_=1,
            to=100,
            textvariable=self._remix_count_var,
            width=8,
        ).pack(side=tk.LEFT, padx=(0, 10))
        ttk.Button(frame, text="Random Remix", command=self._random_remix).pack(side=tk.LEFT, padx=(0, 5))
        ttk.Button(frame, text="Restore Defaults", command=self._restore_defaults).pack(side=tk.LEFT)

        ttk.Label(
            frame,
            text="Randomly picks a percentage of discovered spawnables and enables them all at once. Restore Defaults removes all mod-added entries, keeps vanilla templates, and reports each file's result in Required Actions.",
            foreground="gray",
            wraplength=500,
        ).pack(side=tk.LEFT, padx=(10, 0))

    def _build_status(self) -> None:
        self._status = ttk.Label(self.parent, text="", foreground="gray")
        self._status.grid(row=7, column=0, sticky=tk.EW, padx=10, pady=(5, 10))

    def _get_spawn_options(self) -> Dict[str, Any]:
        """Return the user-selected spawn options for the current selection."""
        category = self._get_selected_category()
        is_vehicle_spawn = category in ("vehicle", "air", "water")

        if is_vehicle_spawn:
            # Vehicles, aircraft and boats are event-spawned; usage/value tags
            # would make Central Economy place them as loot inside houses.
            options: Dict[str, Any] = {
                "count": self._map_limit_var.get(),
                "event_min": self._event_min_var.get(),
                "event_max": self._event_max_var.get(),
                "usage": "",
                "value": "",
                "tier": 1,
                "locations": self._parse_locations(),
            }
        else:
            usage = self._usage_var.get().strip() or "Town"
            value = self._value_var.get().strip() or "Tier12"
            warnings: List[str] = []
            if self.workflow is not None:
                if not self.workflow.is_valid_usage(usage):
                    warnings.append(f"'{usage}' is not a declared usage in cfglimitsdefinition*.xml")
                if not self.workflow.is_valid_value(value):
                    warnings.append(f"'{value}' is not a declared usage/tier in cfglimitsdefinition*.xml")
            options = {
                "count": self._loot_nominal_var.get(),
                "min": self._loot_min_var.get(),
                "lifetime": self._loot_lifetime_var.get(),
                "restock": self._loot_restock_var.get(),
                "quantmin": self._loot_quantmin_var.get(),
                "quantmax": self._loot_quantmax_var.get(),
                "usage": usage,
                "value": value,
                "tier": self._tier_var.get(),
                "locations": self._parse_locations(),
            }
            if warnings:
                options["warnings"] = warnings
        return options

    def _parse_locations(self) -> List[Dict[str, float]]:
        """Parse the coordinates text area into a list of position dicts.

        Accepts lines with four numbers separated by whitespace or commas.
        """
        raw = self._locations_text.get("1.0", tk.END).strip()
        if not raw:
            return []

        locations: List[Dict[str, float]] = []
        for line in raw.splitlines():
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            parts = [p for p in line.replace(",", " ").split() if p]
            if len(parts) < 4:
                continue
            try:
                x = float(parts[0])
                z = float(parts[1])
                y = float(parts[2])
                a = float(parts[3])
                locations.append({"x": x, "z": z, "y": y, "a": a})
            except ValueError:
                continue
        return locations

    def _format_locations(self, locations: List[Dict[str, float]]) -> str:
        """Format position dicts for the coordinates text area."""
        lines = []
        for loc in locations:
            x = loc.get("x", 0.0)
            z = loc.get("z", 0.0)
            y = loc.get("y", 0.0)
            a = loc.get("a", 0.0)
            lines.append(f"{x} {z} {y} {a}")
        return "\n".join(lines)

    def _get_workshop_path(self) -> Optional[Path]:
        if self.get_workshop_dir is None:
            return None
        raw = self.get_workshop_dir()
        if not raw:
            return None
        return Path(raw)

    def _selected_categories(self) -> List[str]:
        return [cat for cat, var in self._category_vars.items() if var.get()]

    def _refresh_spawnables(self, done_callback: Optional[Callable[[], None]] = None) -> None:
        """Rescan and update the class-name dropdown using current filters.

        This runs synchronously on the main thread.  Discovery against local
        XML files is fast enough that the extra complexity of background
        threads is not worth the race-condition bugs it introduced with
        Tkinter variable reads and combobox updates.
        """
        if not self._ensure_workflow():
            if done_callback:
                done_callback()
            return

        categories = self._selected_categories()
        search_query = self._search_var.get().strip().lower()

        with self._busy_cursor("Scanning workshop/mission for spawnables..."):
            try:
                workshop_path = self._get_workshop_path()
                self.workflow.workshop_dir = workshop_path
                discovered = self.workflow.discover_spawnables(workshop_path, categories=categories)
                spawnable_sources = {s.name: s for s in discovered}

                if search_query:
                    spawnable_sources = {
                        name: meta
                        for name, meta in spawnable_sources.items()
                        if search_query in name.lower()
                    }

                spawnable_values = sorted(spawnable_sources.keys())

                if spawnable_sources:
                    status_text = f"Found {len(spawnable_sources)} spawnable class(es)."
                    status_color = "gray"
                else:
                    status_text = (
                        "No spawnables discovered with the current filters. "
                        "Enable more categories or check your Workshop Directory."
                    )
                    status_color = "orange"
            except Exception as exc:  # pragma: no cover - defensive UI handling
                spawnable_sources = {}
                spawnable_values = []
                status_text = f"Spawnable scan failed: {exc}"
                status_color = "red"

            self._vehicle_var.set("")
            self._spawnable_sources = spawnable_sources
            self._vehicle_combo["values"] = spawnable_values
            self._match_count_var.set(f"{len(spawnable_values)} available")
            self._status.config(text=status_text, foreground=status_color)
            self._on_spawnable_selected()
            if done_callback:
                done_callback()

    def _on_category_changed(self) -> None:
        """Refresh the spawnable list when category filters change."""
        self._refresh_spawnables()

    def _clear_search(self) -> None:
        """Clear the search field and restore the full discovered list."""
        self._search_var.set("")
        self._refresh_spawnables()

    def _on_search_changed(self, event=None) -> None:
        """Filter the class-name combobox to match the search text."""
        # Only re-scan when the user pauses typing; hitting Enter also triggers.
        if event and event.keysym in ("Return", "KP_Enter"):
            self._refresh_spawnables()
            return
        # Debounce rapid keystrokes.
        if hasattr(self, "_search_after_id"):
            self.parent.after_cancel(self._search_after_id)
        self._search_after_id = self.parent.after(250, self._refresh_spawnables)

    def _on_spawnable_selected(self, event=None) -> None:
        name = self._vehicle_var.get().strip()
        category = self._get_selected_category()
        is_vehicle = category in ("vehicle", "air", "water")

        # Toggle the relevant count controls.
        if is_vehicle:
            self._vehicle_controls.pack(side=tk.TOP, fill=tk.X, anchor=tk.W)
            self._loot_controls.pack_forget()
            self._advanced_loot_controls.pack_forget()
        else:
            self._loot_controls.pack(side=tk.TOP, fill=tk.X, anchor=tk.W)
            self._advanced_loot_controls.pack(side=tk.TOP, fill=tk.X, anchor=tk.W, pady=(4, 0))
            self._vehicle_controls.pack_forget()

        if not name:
            self._source_var.set("Type or select a mod class name.")
            return

        # Import here to avoid a circular import at module load time.
        try:
            from ..config.mod_integration import _is_likely_vehicle_part
        except ImportError:
            from dayzconfigmaster.config.mod_integration import _is_likely_vehicle_part

        if _is_likely_vehicle_part(name):
            self._source_var.set(
                "Warning: this looks like a vehicle part, not a whole spawnable. "
                "Quick Setup is meant for top-level class names only."
            )
            return

        spawnable = self._spawnable_sources.get(name)
        if spawnable:
            label = _CATEGORY_LABELS.get(spawnable.category, spawnable.category)
            parts = [f"Source: {spawnable.source}", f"Category: {label}"]
            if spawnable.confidence == "heuristic":
                parts.append("(heuristic)")
            if spawnable.declared_categories:
                parts.append(f"CE cats: {', '.join(spawnable.declared_categories)}")
            if spawnable.declared_usages:
                parts.append(f"CE usages: {', '.join(spawnable.declared_usages)}")
            if spawnable.invalid_categories or spawnable.invalid_usages:
                problems = []
                if spawnable.invalid_categories:
                    problems.append(f"invalid cats: {', '.join(spawnable.invalid_categories)}")
                if spawnable.invalid_usages:
                    problems.append(f"invalid usages: {', '.join(spawnable.invalid_usages)}")
                parts.append(f"[WARNING: {'; '.join(problems)}]")
            self._source_var.set("  |  ".join(parts))
        elif name in VEHICLE_TEMPLATES:
            self._source_var.set("Source: Vanilla template  |  Category: Land vehicle")
        else:
            self._source_var.set(
                "Source: Custom entry (no category detected; will be added to types.xml only)"
            )

    def _ensure_workflow(self) -> bool:
        root = self._get_effective_mission_root()
        self._update_mission_path_label(root)
        if root is None:
            self._status.config(text="No mission root available.", foreground="red")
            return False
        workshop_path = self._get_workshop_path()
        if self.workflow is None or self.workflow.editor.mission_root != root:
            self.workflow = ModIntegrationWorkflow(root, workshop_dir=workshop_path)
        else:
            self.workflow.workshop_dir = workshop_path
        return True

    def _get_effective_mission_root(self) -> Optional[Path]:
        """Return the mission root for the selected instance, or the global one."""
        if self._selected_instance is not None and self.get_instance_mission_root is not None:
            instance_root = self.get_instance_mission_root(self._selected_instance)
            if instance_root is not None:
                return instance_root
        return self.get_mission_root()

    def _update_mission_path_label(self, root: Optional[Path]) -> None:
        """Display the resolved mission path and warn about missing files."""
        if root is None:
            self._mission_path_var.set("Mission path: not selected")
            self._mission_warning_var.set(
                "No mission folder could be resolved. Select the correct map/instance before applying spawnables."
            )
            return

        self._mission_path_var.set(f"Mission path: {root}")
        missing: List[str] = []
        for xml_name in ("types.xml", "events.xml", "cfgspawnabletypes.xml"):
            xml_path = root / xml_name
            if not xml_path.exists():
                missing.append(xml_name)
            elif not xml_path.read_text(encoding="utf-8", errors="ignore").strip():
                missing.append(f"{xml_name} (empty)")

        if missing:
            self._mission_warning_var.set(
                f"Warning: this mission folder is missing or empty: {', '.join(missing)}. "
                "Spawning will not work until the server has created these files, or the map ships them in a different location."
            )
        else:
            self._mission_warning_var.set("")

    def _refresh_instance_selector(self) -> None:
        """Populate the instance selector and select the first instance."""
        if self.get_instances is None:
            self._instance_combo.config(values=[])
            return

        instances = self.get_instances()
        names: List[str] = []
        for inst in instances:
            name = (
                self.get_instance_display_name(inst)
                if self.get_instance_display_name
                else f"Instance {inst.get('id', {}).get() or 'unknown'}"
            )
            names.append(name)

        self._instance_combo.config(values=names)
        current = self._instance_var.get()
        if current not in names and names:
            self._instance_var.set(names[0])
            self._on_instance_selected()
        elif not names:
            self._instance_var.set("")
            self._selected_instance = None

    def _on_instance_selected(self, _event: Any = None) -> None:
        """Update the selected instance and refresh mission path display."""
        current = self._instance_var.get()
        self._selected_instance = None
        if self.get_instances is None:
            return
        for inst in self.get_instances():
            name = (
                self.get_instance_display_name(inst)
                if self.get_instance_display_name
                else f"Instance {inst.get('id', {}).get() or 'unknown'}"
            )
            if name == current:
                self._selected_instance = inst
                break
        self.refresh()

    def _get_selected_instance_root(self) -> Optional[Path]:
        """Return the file-system root of the currently selected instance."""
        if self._selected_instance is None or self.get_instance_root is None:
            return None
        return self.get_instance_root(self._selected_instance)

    def refresh(self) -> None:
        """Refresh the mission path display; call when the tab is shown."""
        self._refresh_instance_selector()
        root = self._get_effective_mission_root()
        self._update_mission_path_label(root)
        if self.workflow is not None and self.workflow.editor.mission_root != root:
            self.workflow = None

    def _save_instance_loadout(self) -> None:
        """Persist the current loadout queue to the selected instance."""
        instance_root = self._get_selected_instance_root()
        if instance_root is None:
            messagebox.showwarning(
                "No Instance",
                "Select a server instance before saving the loadout.",
            )
            return

        with self._busy_cursor("Saving loadout..."):
            from dayzconfigmaster.config.per_instance_config import (
                PerInstanceConfigManager,
                SpawnableEntry,
                InstanceSpawnLoadout,
            )

            manager = PerInstanceConfigManager(instance_root)
            loadout = InstanceSpawnLoadout()
            for item in self._queue_tree.get_children():
                entry = json.loads(self._queue_tree.item(item, "tags")[0])
                is_vehicle = entry.get("category") in ("vehicle", "air", "water")
                loadout.enabled.append(
                    SpawnableEntry(
                        name=entry["name"],
                        category=entry["category"],
                        source=entry.get("source", ""),
                        spawn_count=entry.get("count", 10),
                        min_count=entry.get("min", 0) if not is_vehicle else 0,
                        lifetime=entry.get("lifetime", 7200) if not is_vehicle else 3888000,
                        restock=entry.get("restock", 0) if not is_vehicle else 1800,
                        quantmin=entry.get("quantmin", 30) if not is_vehicle else -1,
                        quantmax=entry.get("quantmax", 80) if not is_vehicle else -1,
                        usage=entry.get("usage", "Town"),
                        value=entry.get("value", "Tier12"),
                        tier=entry.get("tier", 1),
                        locations=entry.get("locations", []),
                        event_min=entry.get("event_min", 1) if is_vehicle else 1,
                        event_max=entry.get("event_max", 1) if is_vehicle else 1,
                    )
                )
            manager.save_spawn_loadout(loadout)
            messagebox.showinfo(
                "Loadout Saved",
                f"Saved {len(loadout.enabled)} spawnable(s) to instance:\n{instance_root}",
            )

    def _load_instance_loadout(self) -> None:
        """Load the saved spawn loadout for the selected instance into the queue."""
        instance_root = self._get_selected_instance_root()
        if instance_root is None:
            messagebox.showwarning(
                "No Instance",
                "Select a server instance before loading the loadout.",
            )
            return

        from dayzconfigmaster.config.per_instance_config import (
            PerInstanceConfigManager,
        )

        manager = PerInstanceConfigManager(instance_root)
        loadout = manager.load_spawn_loadout()
        if not loadout.enabled:
            messagebox.showinfo(
                "No Saved Loadout",
                "This instance does not have a saved spawn loadout yet.",
            )
            return

        self._clear_loadout()
        for entry in loadout.enabled:
            is_vehicle = entry.category in ("vehicle", "air", "water")
            options: Dict[str, Any] = {
                "count": entry.spawn_count,
                "locations": list(entry.locations),
            }
            if is_vehicle:
                options["event_min"] = entry.event_min
                options["event_max"] = entry.event_max
            else:
                options["min"] = entry.min_count
                options["lifetime"] = entry.lifetime
                options["restock"] = entry.restock
                options["quantmin"] = entry.quantmin
                options["quantmax"] = entry.quantmax
                options["usage"] = entry.usage
                options["value"] = entry.value
                options["tier"] = entry.tier

            payload = {
                "name": entry.name,
                "category": entry.category,
                "source": entry.source,
                **options,
            }
            count_label = self._format_count_label(options, is_vehicle)
            notes = self._format_loadout_notes(options, is_vehicle)
            self._queue_tree.insert(
                "",
                tk.END,
                values=(entry.name, _CATEGORY_LABELS.get(entry.category, entry.category), count_label, notes),
                tags=(json.dumps(payload),),
            )

        self._status.config(
            text=f"Loaded {len(loadout.enabled)} saved spawnable(s) into the queue.",
            foreground="green",
        )

    def _get_selected_category(self, name: Optional[str] = None) -> str:
        if name is None:
            name = self._vehicle_var.get().strip()
        spawnable = self._spawnable_sources.get(name)
        if spawnable:
            return spawnable.category
        if name in VEHICLE_TEMPLATES:
            return "vehicle"
        # Unknown / manually typed names default to vehicle behavior so that
        # events.xml and cfgspawnabletypes.xml are still written.
        return "vehicle"

    def _detect(self) -> None:
        if not self._ensure_workflow():
            return
        name = self._vehicle_var.get().strip()
        if not name:
            self._status.config(text="Enter a mod class name.", foreground="red")
            return

        with self._busy_cursor(f"Detecting required changes for {name}..."):
            for item in self._tree.get_children():
                self._tree.delete(item)

            category = self._get_selected_category()
            actions = self.workflow.detect_actions(name, category=category)
            if not actions:
                self._tree.insert("", tk.END, values=("All files", f"{name} is already fully integrated.", "OK"))
                self._status.config(text=f"{name} requires no changes.", foreground="green")
                return

            for action in actions:
                status = "Missing file" if "missing" in action.description.lower() else "Pending"
                self._tree.insert("", tk.END, values=(action.file_name, action.description, status))
            self._status.config(text=f"Detected {len(actions)} required change(s) for {name}.", foreground="orange")

    def _apply(self) -> None:
        if not self._ensure_workflow():
            return
        name = self._vehicle_var.get().strip()
        if not name:
            return
        self._apply_single(name, self._get_spawn_options())

    def _apply_single(self, name: str, options: Dict[str, Any]) -> None:
        """Apply changes for a single class name."""
        warnings = options.get("warnings", [])
        if warnings:
            warning_text = "\n".join(f"• {w}" for w in warnings)
            if not messagebox.askyesno(
                "Confirm Integration",
                f"Apply all XML changes needed to enable {name}?\n\n"
                f"Warnings:\n{warning_text}\n\n"
                "Backups will be created automatically.",
            ):
                return
        elif not messagebox.askyesno(
            "Confirm Integration",
            f"Apply all XML changes needed to enable {name}?\n\n"
            "Backups will be created automatically.",
        ):
            return

        self._set_busy(f"Applying changes for {name}...")

        def _run() -> None:
            try:
                category = self._get_selected_category(name)
                result = self.workflow.integrate_spawnable_mod(
                    name,
                    spawn_count=options.get("count", 10),
                    category=category,
                    usage=options.get("usage", ""),
                    value=options.get("value", ""),
                    locations=options.get("locations", []),
                    event_min=options.get("event_min", 1),
                    event_max=options.get("event_max", 1),
                    min_count=options.get("min"),
                    lifetime=options.get("lifetime"),
                    restock=options.get("restock"),
                    quantmin=options.get("quantmin"),
                    quantmax=options.get("quantmax"),
                )
            except Exception as exc:  # pragma: no cover - defensive UI handling
                result = None
                error = str(exc)

            def _show() -> None:
                self._display_result(result, error, name)

            self.parent.after(0, _show)

        import threading
        threading.Thread(target=_run, daemon=True).start()

    # ------------------------------------------------------------------ #
    # Spawn loadout queue
    # ------------------------------------------------------------------ #

    def _add_to_loadout(self) -> None:
        name = self._vehicle_var.get().strip()
        if not name:
            self._status.config(text="Select a class name before adding to the loadout.", foreground="red")
            return

        category = self._get_selected_category(name)
        is_vehicle = category in ("vehicle", "air", "water")
        options = self._get_spawn_options()
        count_label = self._format_count_label(options, is_vehicle)
        notes = self._format_loadout_notes(options, is_vehicle)

        spawnable = self._spawnable_sources.get(name)
        source = spawnable.source if spawnable else ""
        payload = {"name": name, "category": category, "source": source, **options}

        # Update existing row if the class is already queued.
        for item in self._queue_tree.get_children():
            if self._queue_tree.item(item, "values")[0] == name:
                self._queue_tree.item(
                    item,
                    values=(name, _CATEGORY_LABELS.get(category, category), count_label, notes),
                    tags=(json.dumps(payload),),
                )
                self._status.config(text=f"Updated {name} in loadout queue.", foreground="green")
                return

        self._queue_tree.insert(
            "",
            tk.END,
            values=(name, _CATEGORY_LABELS.get(category, category), count_label, notes),
            tags=(json.dumps(payload),),
        )
        self._status.config(text=f"Added {name} to loadout queue.", foreground="green")

    def _format_count_label(self, options: Dict[str, Any], is_vehicle: bool) -> str:
        if is_vehicle:
            return f"map={options.get('count', 1)} / evt {options.get('event_min', 1)}-{options.get('event_max', 1)}"
        return f"nom={options.get('count', 10)} / min={options.get('min', 5)} / lt={options.get('lifetime', 7200)}"

    def _format_loadout_notes(self, options: Dict[str, Any], is_vehicle: bool) -> str:
        parts: List[str] = []
        if not is_vehicle:
            if options.get("usage"):
                parts.append(f"usage={options['usage']}")
            if options.get("value"):
                parts.append(f"value={options['value']}")
            parts.append(f"tier={options.get('tier', 1)}")
            parts.append(f"restock={options.get('restock', 0)}")
            parts.append(f"quant={options.get('quantmin', 30)}-{options.get('quantmax', 80)}")
        locs = options.get("locations", [])
        if locs:
            parts.append(f"{len(locs)} exact pos")
        warnings = options.get("warnings", [])
        if warnings:
            parts.append("warnings")
        return ", ".join(parts) if parts else "defaults"

    def _remove_selected_loadout(self) -> None:
        selection = self._queue_tree.selection()
        if not selection:
            self._status.config(text="Select a loadout row to remove.", foreground="orange")
            return
        for item in selection:
            self._queue_tree.delete(item)
        self._status.config(text="Removed selected row(s) from loadout.", foreground="green")

    def _clear_loadout(self) -> None:
        for item in self._queue_tree.get_children():
            self._queue_tree.delete(item)
        self._status.config(text="Loadout queue cleared.", foreground="gray")

    def _apply_loadout(self) -> None:
        if not self._ensure_workflow():
            return
        items = self._queue_tree.get_children()
        if not items:
            self._status.config(text="Loadout queue is empty.", foreground="orange")
            return

        entries: List[Dict[str, Any]] = []
        for item in items:
            tag = self._queue_tree.item(item, "tags")[0]
            entries.append(json.loads(tag))

        names = ", ".join(e["name"] for e in entries)
        if not messagebox.askyesno(
            "Apply Loadout",
            f"Apply loadout for {len(entries)} spawnable(s):\n{names}\n\n"
            "Backups will be created automatically.",
        ):
            return

        self._set_busy("Applying loadout...")
        self._loadout_queue = entries
        self._loadout_index = 0
        self._loadout_errors: List[str] = []
        self._loadout_results: List[Any] = []
        self._apply_next_in_loadout()

    def _apply_next_in_loadout(self) -> None:
        if self._loadout_index >= len(self._loadout_queue):
            self._finish_loadout()
            return

        entry = self._loadout_queue[self._loadout_index]
        name = entry["name"]
        options = {
            k: v for k, v in entry.items()
            if k not in ("name", "category")
        }

        def _run() -> None:
            try:
                category = entry.get("category", self._get_selected_category(name))
                result = self.workflow.integrate_spawnable_mod(
                    name,
                    spawn_count=options.get("count", 10),
                    category=category,
                    usage=options.get("usage", ""),
                    value=options.get("value", ""),
                    locations=options.get("locations", []),
                    event_min=options.get("event_min", 1),
                    event_max=options.get("event_max", 1),
                    min_count=options.get("min"),
                    lifetime=options.get("lifetime"),
                    restock=options.get("restock"),
                    quantmin=options.get("quantmin"),
                    quantmax=options.get("quantmax"),
                )
                self._loadout_results.append(result)
                if not result.success:
                    self._loadout_errors.append(f"{name}: integration reported failure")
            except Exception as exc:  # pragma: no cover
                self._loadout_errors.append(f"{name}: {exc}")

            def _next() -> None:
                self._loadout_index += 1
                self._apply_next_in_loadout()

            self.parent.after(0, _next)

        import threading
        threading.Thread(target=_run, daemon=True).start()

    def _finish_loadout(self) -> None:
        total = len(self._loadout_queue)
        errors = len(self._loadout_errors)
        if errors:
            self._status.config(
                text=f"Loadout applied with {errors} error(s). See dialog for details.",
                foreground="red",
            )
            messagebox.showerror("Loadout Errors", "\n".join(self._loadout_errors))
        else:
            self._status.config(text=f"Loadout applied: {total} spawnable(s).", foreground="green")

        for item in self._tree.get_children():
            self._tree.delete(item)
        for result in self._loadout_results:
            for action in result.actions:
                status = "Applied" if action.applied else "Failed"
                self._tree.insert("", tk.END, values=(action.file_name, action.description, status))
        self._clear_busy()

    def _save_loadout(self) -> None:
        path = filedialog.asksaveasfilename(
            defaultextension=".json",
            filetypes=(("JSON files", "*.json"), ("All files", "*.*")),
            title="Save Spawn Loadout",
        )
        if not path:
            return
        entries = [json.loads(self._queue_tree.item(item, "tags")[0]) for item in self._queue_tree.get_children()]
        try:
            with open(path, "w", encoding="utf-8") as f:
                json.dump(entries, f, indent=2)
            self._status.config(text=f"Saved loadout with {len(entries)} row(s).", foreground="green")
        except Exception as exc:  # pragma: no cover
            self._status.config(text=f"Failed to save loadout: {exc}", foreground="red")

    def _load_loadout(self) -> None:
        path = filedialog.askopenfilename(
            defaultextension=".json",
            filetypes=(("JSON files", "*.json"), ("All files", "*.*")),
            title="Load Spawn Loadout",
        )
        if not path:
            return
        try:
            with open(path, "r", encoding="utf-8") as f:
                entries = json.load(f)
        except Exception as exc:  # pragma: no cover
            self._status.config(text=f"Failed to load loadout: {exc}", foreground="red")
            return

        self._clear_loadout()
        for entry in entries:
            name = entry.get("name", "")
            category = entry.get("category", "vehicle")
            is_vehicle = category in ("vehicle", "air", "water")
            options = {k: v for k, v in entry.items() if k not in ("name", "category")}
            count_label = self._format_count_label(options, is_vehicle)
            notes = self._format_loadout_notes(options, is_vehicle)
            self._queue_tree.insert(
                "",
                tk.END,
                values=(name, _CATEGORY_LABELS.get(category, category), count_label, notes),
                tags=(json.dumps(entry),),
            )
        self._status.config(text=f"Loaded loadout with {len(entries)} row(s).", foreground="green")

    def _display_result(self, result: Optional[object], error: str, name: str) -> None:
        try:
            for item in self._tree.get_children():
                self._tree.delete(item)

            if result is None:
                self._status.config(text=f"Error applying changes: {error}", foreground="red")
                messagebox.showerror("Integration Error", error)
                return

            for action in result.actions:
                if action.applied:
                    status = "Applied"
                else:
                    status = f"Failed: {action.error}" if action.error else "Failed"
                self._tree.insert("", tk.END, values=(action.file_name, action.description, status))

            failed = [a for a in result.actions if not a.applied]
            if result.ok:
                self._status.config(text=f"{name} integrated successfully.", foreground="green")
                messagebox.showinfo("Integration Complete", f"{name} has been enabled.")
                # Track this spawnable as part of the instance loadout.
                if name in self._spawnable_sources or name in VEHICLE_TEMPLATES:
                    self._track_applied_spawnable(name)
            elif failed:
                errors = "\n".join(
                    f"• {a.file_name}: {a.error or 'Failed'}" for a in failed
                )
                status_text = f"{name}: {len(failed)} change(s) failed. {failed[0].error or ''}"
                self._status.config(text=status_text, foreground="red")
                messagebox.showerror("Integration Incomplete", f"One or more XML changes failed:\n\n{errors}")
            else:
                self._status.config(text=f"Some changes for {name} could not be applied.", foreground="red")
                messagebox.showerror("Integration Incomplete", "One or more XML changes failed. Check the action list.")
        finally:
            self._clear_busy()

    def _track_applied_spawnable(self, name: str) -> None:
        """Remember a spawnable that was successfully applied."""
        spawnable = self._spawnable_sources.get(name)
        if spawnable is not None:
            source = spawnable.source
            category = spawnable.category
        elif name in VEHICLE_TEMPLATES:
            source = "vanilla"
            category = "vehicle"
        else:
            source = "manual"
            category = "generic"
        options = self._get_spawn_options()
        is_vehicle = category in ("vehicle", "air", "water")
        # Avoid duplicates; move to end to preserve latest order.
        self._applied_spawnables = [
            x for x in self._applied_spawnables if x["name"] != name
        ]
        self._applied_spawnables.append(
            {
                "name": name,
                "category": category,
                "source": source,
                "spawn_count": options.get("count", 10),
                "min_count": 0 if is_vehicle else options.get("min", 5),
                "lifetime": options.get("lifetime", 7200) if not is_vehicle else 3888000,
                "restock": options.get("restock", 0) if not is_vehicle else 1800,
                "quantmin": options.get("quantmin", 30) if not is_vehicle else -1,
                "quantmax": options.get("quantmax", 80) if not is_vehicle else -1,
                "usage": options.get("usage", ""),
                "value": options.get("value", ""),
                "tier": options.get("tier", 1),
                "locations": options.get("locations", []),
                "event_min": options.get("event_min", 1) if is_vehicle else 1,
                "event_max": options.get("event_max", 1) if is_vehicle else 1,
            }
        )

    def _append_actions_to_tree(self, actions: List[object]) -> None:
        """Append IntegrationAction rows to the action tree."""
        for action in actions:
            if action.applied:
                status = "Applied"
            else:
                status = f"Failed: {action.error}" if action.error else "Failed"
            self._tree.insert("", tk.END, values=(action.file_name, action.description, status))

    def _random_remix(self) -> None:
        if not self._ensure_workflow():
            return

        percent = self._remix_percent_var.get()
        count = self._remix_count_var.get()
        categories = self._selected_categories()

        # Discover first so we can tell the user how many will be affected.
        spawnables = self.workflow.discover_spawnables(categories=categories)
        if not spawnables:
            messagebox.showinfo("Random Remix", "No spawnables match the current filters.")
            return
        selected_count = max(1, int(len(spawnables) * percent / 100.0))
        selected_count = min(selected_count, len(spawnables))

        if not messagebox.askyesno(
            "Confirm Random Remix",
            f"Enable approximately {selected_count} of {len(spawnables)} discovered spawnables "
            f"(filtered categories: {', '.join(categories)}) at count {count}?\n\n"
            "Backups will be created automatically.",
        ):
            return

        self._set_busy(
            f"Running random remix ({selected_count} of {len(spawnables)} at count {count})..."
        )
        for item in self._tree.get_children():
            self._tree.delete(item)
        self._tree.insert(
            "",
            tk.END,
            values=("summary", f"Random remix: enabling ~{selected_count} of {len(spawnables)} spawnables...", "In progress"),
        )

        import threading

        selected = random.sample(spawnables, selected_count)

        def _run() -> None:
            all_actions: List[object] = []
            success = True
            for idx, spawnable in enumerate(selected, start=1):
                try:
                    result = self.workflow.integrate_spawnable_mod(
                        spawnable.name,
                        spawn_count=count,
                        category=spawnable.category,
                    )
                except Exception as exc:  # pragma: no cover - defensive UI handling
                    result = type("Result", (), {
                        "ok": False,
                        "actions": [type("Action", (), {
                            "file_name": "error",
                            "description": f"{spawnable.name} crashed: {exc}",
                            "applied": False,
                            "error": str(exc),
                        })],
                    })()

                all_actions.extend(result.actions)
                if not result.ok:
                    success = False

                # Update UI after each item so the user sees progress.
                def _update(current_idx=idx, current_name=spawnable.name, ok=result.ok):
                    # Replace the in-progress summary row.
                    for child in self._tree.get_children():
                        if self._tree.item(child, "values")[0] == "summary":
                            self._tree.delete(child)
                            break
                    self._append_actions_to_tree(result.actions)
                    self._tree.insert(
                        "",
                        tk.END,
                        values=(
                            "summary",
                            f"Progress: {current_idx}/{selected_count} — {current_name} "
                            f"({'OK' if ok else 'FAILED'})",
                            "In progress" if current_idx < selected_count else "Done",
                        ),
                    )
                    self._status.config(
                        text=f"Random remix: {current_idx}/{selected_count} processed...",
                        foreground="blue",
                    )

                self.parent.after(0, _update)

            def _finish() -> None:
                try:
                    # Replace summary with final summary.
                    for child in self._tree.get_children():
                        if self._tree.item(child, "values")[0] == "summary":
                            self._tree.delete(child)
                            break
                    summary_text = (
                        f"Random remix: enabled {len(selected)} of {len(spawnables)} spawnables "
                        f"({percent}%) at count {count}."
                    )
                    self._tree.insert("", tk.END, values=("summary", summary_text, "Done"))
                    if success:
                        for spawnable in selected:
                            self._applied_spawnables = [
                                x for x in self._applied_spawnables if x["name"] != spawnable.name
                            ]
                            self._applied_spawnables.append(
                                {
                                    "name": spawnable.name,
                                    "category": spawnable.category,
                                    "source": spawnable.source,
                                    "spawn_count": count,
                                    "usage": "Town",
                                    "value": "Tier12",
                                    "tier": 1,
                                    "locations": [],
                                }
                            )
                        self._status.config(text=summary_text, foreground="green")
                        messagebox.showinfo("Random Remix Complete", summary_text)
                    else:
                        self._status.config(text=f"{summary_text} Some items failed.", foreground="red")
                        messagebox.showerror("Random Remix Incomplete", "Some spawnables could not be enabled. Check the action list.")
                finally:
                    self._clear_busy()

            self.parent.after(0, _finish)

        threading.Thread(target=_run, daemon=True).start()

    def _restore_defaults(self) -> None:
        if not self._ensure_workflow():
            return

        if not messagebox.askyesno(
            "Confirm Restore Defaults",
            "Remove all mod-added spawnables from events.xml, cfgspawnabletypes.xml, and types.xml?\n\n"
            "Vanilla templates will be kept. Backups will be created automatically.",
        ):
            return

        self._set_busy("Restoring default spawnables...")
        # Clear previous results and show an in-progress summary row.
        for item in self._tree.get_children():
            self._tree.delete(item)
        self._tree.insert(
            "",
            tk.END,
            values=("summary", "Restoring default spawnables...", "In progress"),
        )

        def _run() -> None:
            try:
                result = self.workflow.restore_defaults()
            except Exception as exc:  # pragma: no cover - defensive UI handling
                result = None
                error = str(exc)

            def _show() -> None:
                try:
                    for item in self._tree.get_children():
                        self._tree.delete(item)

                    if result is None:
                        self._tree.insert(
                            "",
                            tk.END,
                            values=("summary", f"Restore failed: {error}", "Failed"),
                        )
                        self._status.config(text=f"Restore failed: {error}", foreground="red")
                        messagebox.showerror("Restore Defaults Failed", error)
                        return

                    self._append_actions_to_tree(result.actions)

                    failed = [a for a in result.actions if not a.applied]
                    if result.ok:
                        self._applied_spawnables.clear()
                        summary = (
                            f"Restore defaults complete: {len(result.actions)} file(s) checked, "
                            f"all mod-added spawnables removed."
                        )
                        self._tree.insert("", tk.END, values=("summary", summary, "Done"))
                        self._status.config(text=summary, foreground="green")
                        messagebox.showinfo(
                            "Restore Defaults Complete",
                            "Default spawnables restored. Check the Required Actions list for details.",
                        )
                    elif failed:
                        summary = (
                            f"Restore defaults incomplete: {len(failed)} file(s) failed. "
                            f"See Required Actions for details."
                        )
                        self._tree.insert("", tk.END, values=("summary", summary, "Failed"))
                        errors = "\n".join(
                            f"• {a.file_name}: {a.error or 'Failed'}" for a in failed
                        )
                        self._status.config(
                            text=f"Restore defaults: {len(failed)} change(s) failed.",
                            foreground="red",
                        )
                        messagebox.showerror(
                            "Restore Defaults Incomplete",
                            f"One or more XML files could not be restored:\n\n{errors}",
                        )
                    else:
                        summary = "Restore defaults finished with warnings. See Required Actions."
                        self._tree.insert("", tk.END, values=("summary", summary, "Warning"))
                        self._status.config(text=summary, foreground="orange")
                        messagebox.showwarning(
                            "Restore Defaults",
                            "Some changes could not be applied. Check the Required Actions list.",
                        )
                finally:
                    self._clear_busy()

            self.parent.after(0, _show)

        import threading
        threading.Thread(target=_run, daemon=True).start()
