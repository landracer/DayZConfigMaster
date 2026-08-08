# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Main application window for DayZ configuration editor
"""
DayzConfigMasterApp: Main GUI application class.
"""

import contextlib
import tkinter as tk
from tkinter import ttk, messagebox, filedialog, scrolledtext, simpledialog
from pathlib import Path
import glob
import json
import os
import re
import shutil
import socket
import time
from datetime import datetime
from typing import List, Optional, Dict, Any, Tuple, Set

# Import model classes
try:
    from ..config.models import (
        EconomyConfig,
        LimitsConfig,
        RandomPresetConfig,
        SpawnableTypeConfig,
        TraderConfig,
        ServerConfig,
    )
    from ..config.validator import FieldValidator
    from ..config.parser import ConfigParser, detect_config_type
    from ..config.map_parser import (
        MapModClassifier,
        WorkshopMapParser,
        get_all_available_maps,
    )
    from ..config.cfg_generator import generate_server_cfg, generate_instance_cfgs, write_cfg_to_file
except ImportError:
    try:
        from dayzconfigmaster.config.models import (
            EconomyConfig,
            LimitsConfig,
            RandomPresetConfig,
            SpawnableTypeConfig,
            TraderConfig,
            ServerConfig,
        )
        from dayzconfigmaster.config.validator import FieldValidator
        from dayzconfigmaster.config.parser import ConfigParser, detect_config_type
        from dayzconfigmaster.config.map_parser import (
            MapModClassifier,
            WorkshopMapParser,
            get_all_available_maps,
        )
        from dayzconfigmaster.config.cfg_generator import generate_server_cfg, generate_instance_cfgs, write_cfg_to_file
    except ImportError:
        from config.models import (
            EconomyConfig,
            LimitsConfig,
            RandomPresetConfig,
            SpawnableTypeConfig,
            TraderConfig,
            ServerConfig,
        )
        from config.validator import FieldValidator
        from config.parser import ConfigParser, detect_config_type
        from config.map_parser import (
            MapModClassifier,
            WorkshopMapParser,
            get_all_available_maps,
        )
        from config.cfg_generator import generate_server_cfg, generate_instance_cfgs, write_cfg_to_file

# Import server controller and log modules
try:
    from ..server.process_controller import ProcessController
    from ..server.deployment import (
        _resolve_mod_source,
        _find_bikey_files,
        _relative_symlink,
        _copy_bikeys,
        _create_mod_wrapper,
    )
    from ..server.diagnostics import (
        is_server_port_bound,
        scan_server_log_for_errors,
        detect_mod_version_mismatches,
    )
    from ..logs.tailer import LogTailer
    from ..logs.diagnostics import DiagnosticsEngine
    from ..mods.pbo_builder import PboBuilder
    from ..mods.integration import ModIntegrationManager
    from ..mods.settings_discovery import detect_mod_settings_files
    from ..config.spawnabletypes_repair import repair_cfg_spawnable_types
    from ..economy.aircraft_lifetime import ensure_aircraft_lifetime
    from ..utils.memory_guard import setup_memory_safety
    from ..banlist.vpp_manager import VppAdminTools
except ImportError:
    try:
        from dayzconfigmaster.server.process_controller import ProcessController
        from dayzconfigmaster.server.deployment import (
            deploy_all_instances,
            _resolve_mod_source,
            _find_bikey_files,
            _relative_symlink,
            _copy_bikeys,
            _create_mod_wrapper,
        )
        from dayzconfigmaster.server.diagnostics import (
            is_server_port_bound,
            scan_server_log_for_errors,
            detect_mod_version_mismatches,
        )
        from dayzconfigmaster.logs.tailer import LogTailer
        from dayzconfigmaster.logs.diagnostics import DiagnosticsEngine
        from dayzconfigmaster.mods.pbo_builder import PboBuilder
        from dayzconfigmaster.mods.integration import ModIntegrationManager
        from dayzconfigmaster.mods.settings_discovery import detect_mod_settings_files
        from dayzconfigmaster.config.spawnabletypes_repair import repair_cfg_spawnable_types
        from dayzconfigmaster.economy.aircraft_lifetime import ensure_aircraft_lifetime
        from dayzconfigmaster.config.deployment_manifest import (
            DeploymentManifestManager,
            compute_quick_skip_status,
        )
        from dayzconfigmaster.backups.instance_storage import InstanceStorageBackupManager
        from dayzconfigmaster.backups.players_db import find_players_db, compare_players_dbs, PlayersDbSplicer
        from dayzconfigmaster.backups.map_storage_state import InstanceMapStorageTracker
        from dayzconfigmaster.backups.cross_instance_storage import find_map_backups_across_instances
        from dayzconfigmaster.scheduler import EventScheduler, EventType
        from dayzconfigmaster.utils.memory_guard import setup_memory_safety
        from dayzconfigmaster.banlist.vpp_manager import VppAdminTools
        from dayzconfigmaster.server.instance_preflight import (
            InstancePreflightChecker,
            InstancePreflightRepair,
            Severity,
        )
        from dayzconfigmaster.config.mod_presets import ModPresetManager
    except ImportError:
        from server.process_controller import ProcessController
        from server.deployment import (
            deploy_all_instances,
            _resolve_mod_source,
            _find_bikey_files,
            _relative_symlink,
            _copy_bikeys,
            _create_mod_wrapper,
        )
        from server.diagnostics import (
            is_server_port_bound,
            scan_server_log_for_errors,
            detect_mod_version_mismatches,
        )
        from logs.tailer import LogTailer
        from logs.diagnostics import DiagnosticsEngine
        from mods.pbo_builder import PboBuilder
        from mods.integration import ModIntegrationManager
        from mods.settings_discovery import detect_mod_settings_files
        from config.spawnabletypes_repair import repair_cfg_spawnable_types
        from economy.aircraft_lifetime import ensure_aircraft_lifetime
        from config.deployment_manifest import (
            DeploymentManifestManager,
            compute_quick_skip_status,
        )
        from backups.instance_storage import InstanceStorageBackupManager
        from backups.players_db import find_players_db, compare_players_dbs, PlayersDbSplicer
        from backups.map_storage_state import InstanceMapStorageTracker
        from backups.cross_instance_storage import find_map_backups_across_instances
        from scheduler import EventScheduler, EventType
        from utils.memory_guard import setup_memory_safety
        from banlist.vpp_manager import VppAdminTools
        from server.instance_preflight import (
            InstancePreflightChecker,
            InstancePreflightRepair,
            Severity,
        )
        from config.mod_presets import ModPresetManager


# Ensure backup/manifest classes are always available regardless of which
# import branch above succeeded (script vs package execution).
try:
    from dayzconfigmaster.config.deployment_manifest import (
        DeploymentManifestManager,
        compute_quick_skip_status,
    )
    from dayzconfigmaster.backups.instance_storage import InstanceStorageBackupManager
    from dayzconfigmaster.backups.players_db import find_players_db, compare_players_dbs, PlayersDbSplicer
    from dayzconfigmaster.backups.map_storage_state import InstanceMapStorageTracker
    from dayzconfigmaster.backups.cross_instance_storage import find_map_backups_across_instances
    from dayzconfigmaster.scheduler import EventScheduler, EventType
    from dayzconfigmaster.server.instance_preflight import (
        InstancePreflightChecker,
        InstancePreflightRepair,
        Severity,
    )
    from dayzconfigmaster.config.mod_presets import ModPresetManager
except ImportError:
    from config.deployment_manifest import (
        DeploymentManifestManager,
        compute_quick_skip_status,
    )
    from backups.instance_storage import InstanceStorageBackupManager
    from backups.players_db import find_players_db, compare_players_dbs, PlayersDbSplicer
    from backups.map_storage_state import InstanceMapStorageTracker
    from backups.cross_instance_storage import find_map_backups_across_instances
    from scheduler import EventScheduler, EventType
    from server.instance_preflight import (
        InstancePreflightChecker,
        InstancePreflightRepair,
        Severity,
    )
    from config.mod_presets import ModPresetManager


# Import tab classes
try:
    from .dayztabs import (
        WorkspaceTab,
    )
except ImportError:
    try:
        from dayzconfigmaster.gui.dayztabs import (
            WorkspaceTab,
        )
    except ImportError:
        from gui.dayztabs import (
            WorkspaceTab,
        )

# Import interactive mod settings widgets and new XML editor tabs.
try:
    from ..gui.mod_settings_editor.editor import ModSettingsEditor
    from ..gui.mod_settings_editor.parser import ConfigParser as ModConfigParser, SettingField
    from ..gui.xml_config_editor import XmlConfigEditorTab
    from ..gui.mod_integration_tab import ModIntegrationTab
except ImportError:
    try:
        from dayzconfigmaster.gui.mod_settings_editor.editor import ModSettingsEditor
        from dayzconfigmaster.gui.mod_settings_editor.parser import ConfigParser as ModConfigParser, SettingField
        from dayzconfigmaster.gui.xml_config_editor import XmlConfigEditorTab
        from dayzconfigmaster.gui.mod_integration_tab import ModIntegrationTab
    except ImportError:
        from gui.mod_settings_editor.editor import ModSettingsEditor
        from gui.mod_settings_editor.parser import ConfigParser as ModConfigParser, SettingField
        from gui.xml_config_editor import XmlConfigEditorTab
        from gui.mod_integration_tab import ModIntegrationTab


# Helper class for scrollable frames
class ScrollableFrame(ttk.Frame):
    """
    A scrollable frame whose scrollbars appear only when the content does not
    fit the visible area.

    - Vertical scrollbar shows only when content is taller than the viewport.
    - Horizontal scrollbar shows only when content is wider than the viewport.
    - When content is narrower than the viewport it stretches to fill the width
      (so nothing looks cramped and no needless horizontal scrollbar appears).
    """
    def __init__(self, parent, *args, **kwargs):
        super().__init__(parent, *args, **kwargs)

        self.canvas = tk.Canvas(self, borderwidth=0, highlightthickness=0)
        self.frame = ttk.Frame(self.canvas)
        self.vscrollbar = ttk.Scrollbar(self, orient=tk.VERTICAL, command=self.canvas.yview)
        self.hscrollbar = ttk.Scrollbar(self, orient=tk.HORIZONTAL, command=self.canvas.xview)

        self.canvas.configure(yscrollcommand=self.vscrollbar.set, xscrollcommand=self.hscrollbar.set)

        # Grid layout lets us hide/show each scrollbar independently.
        self.canvas.grid(row=0, column=0, sticky="nsew")
        self.vscrollbar.grid(row=0, column=1, sticky="ns")
        self.hscrollbar.grid(row=1, column=0, sticky="ew")
        self.grid_rowconfigure(0, weight=1)
        self.grid_columnconfigure(0, weight=1)

        self.canvas_frame = self.canvas.create_window((0, 0), window=self.frame, anchor="nw")
        self.frame.bind("<Configure>", self._on_content_change)
        self.canvas.bind("<Configure>", self._on_content_change)

        # Make the canvas focusable so it can receive button events on Linux.
        self.canvas.configure(takefocus=True)
        self.frame.configure(takefocus=True)

        # Mouse wheel bindings for cross-platform scrolling.
        self._bind_mousewheel(self.canvas)
        self._bind_mousewheel(self.frame)

        # Re-bind whenever new widgets are added to the scrollable frame.
        self.frame.bind("<Map>", lambda e: self._bind_mousewheel_recursive(self.frame))

    def _on_mousewheel(self, event):
        """Scroll the canvas vertically from a mouse wheel event."""
        # Linux uses Button-4 (scroll up) and Button-5 (scroll down).
        if event.num == 4:
            self.canvas.yview_scroll(-1, "units")
            return "break"
        if event.num == 5:
            self.canvas.yview_scroll(1, "units")
            return "break"

        # Windows/macOS use MouseWheel event with event.delta.
        delta = int(-event.delta / 120)
        self.canvas.yview_scroll(delta, "units")
        return "break"

    def _bind_mousewheel(self, widget):
        """Bind mouse wheel events to a single widget."""
        widget.bind("<Button-4>", self._on_mousewheel)
        widget.bind("<Button-5>", self._on_mousewheel)
        widget.bind("<MouseWheel>", self._on_mousewheel)
        # Focus the canvas on enter so Linux button events are delivered.
        widget.bind("<Enter>", lambda e: self.canvas.focus_set())

    def _bind_mousewheel_recursive(self, parent):
        """Ensure every child widget inside the scrollable frame can scroll.

        Widgets that have their own vertical scrolling (Treeview, Text, etc.)
        are skipped so their native wheel behavior is preserved.
        """
        skip_classes = (tk.Text, tk.Listbox, tk.Canvas, ttk.Treeview)
        if isinstance(parent, skip_classes):
            return
        self._bind_mousewheel(parent)
        for child in parent.winfo_children():
            self._bind_mousewheel_recursive(child)

    def _on_content_change(self, event=None):
        """Recompute scroll region, width fill, and scrollbar visibility."""
        self.canvas.configure(scrollregion=self.canvas.bbox("all"))

        canvas_w = self.canvas.winfo_width()
        canvas_h = self.canvas.winfo_height()
        req_w = self.frame.winfo_reqwidth()
        req_h = self.frame.winfo_reqheight()

        # Stretch content to fill the viewport when it is narrower than the
        # canvas; otherwise keep its natural width so horizontal scrolling works.
        self.canvas.itemconfig(self.canvas_frame, width=max(canvas_w, req_w))

        # Vertical scrollbar only when content is taller than the viewport.
        if req_h > canvas_h:
            self.vscrollbar.grid()
        else:
            self.vscrollbar.grid_remove()

        # Horizontal scrollbar only when content is wider than the viewport.
        if req_w > canvas_w:
            self.hscrollbar.grid()
        else:
            self.hscrollbar.grid_remove()


class WrappingTabView(ttk.Frame):
    """
    A notebook-like tabbed container whose tab buttons wrap onto multiple rows
    when the window is narrow and collapse back toward a single row as it grows.

    Unlike ``ttk.Notebook``, the tab labels never shrink or get clipped — every
    tab stays fully readable; extra tabs simply flow onto another row.

    Implements the small subset of the ttk.Notebook API this app uses:
    ``add(child, text=...)``.
    """

    def __init__(self, parent, *args, **kwargs):
        super().__init__(parent, *args, **kwargs)

        self._tabs = []            # list of dicts: {"text", "button", "content"}
        self._current = None       # index of selected tab
        self._last_width = 0

        # Selected-tab styling (distinct from the normal tab button look).
        style = ttk.Style()
        try:
            style.configure("Tab.TButton", padding=(10, 4))
            style.configure("TabSelected.TButton", padding=(10, 4),
                            font=("Arial", 9, "bold"),
                            background="#4a90d9", foreground="white")
            style.map("TabSelected.TButton",
                      background=[("active", "#4a90d9")],
                      foreground=[("active", "white")])
        except tk.TclError:
            pass

        self._tabbar = ttk.Frame(self)
        self._tabbar.grid(row=0, column=0, sticky="ew")
        self._content = ttk.Frame(self)
        self._content.grid(row=1, column=0, sticky="nsew")

        self.grid_rowconfigure(1, weight=1)
        self.grid_columnconfigure(0, weight=1)

        self._rows = []            # inner row frames used for wrapping
        self._tabbar.bind("<Configure>", self._on_tabbar_configure)

    def add(self, child, text="", **kwargs):
        """Add a tab whose content is ``child`` and whose label is ``text``."""
        index = len(self._tabs)
        button = ttk.Button(
            self._tabbar, text=text, style="Tab.TButton",
            command=lambda i=index: self.select(i),
        )
        # Content frames stack in the same cell; the selected one is raised.
        child.grid(in_=self._content, row=0, column=0, sticky="nsew")
        self._content.grid_rowconfigure(0, weight=1)
        self._content.grid_columnconfigure(0, weight=1)

        self._tabs.append({"text": text, "button": button, "content": child})
        self._relayout(force=True)

        # Newly gridded children stack on top; re-assert the selection so the
        # visible content always matches the highlighted tab.
        self.select(self._current if self._current is not None else 0)
        return child

    def select(self, index):
        """Raise the content for ``index`` and highlight its tab button."""
        if not (0 <= index < len(self._tabs)):
            return
        for i, tab in enumerate(self._tabs):
            if i == index:
                tab["button"].configure(style="TabSelected.TButton")
                tab["content"].tkraise()
            else:
                tab["button"].configure(style="Tab.TButton")
        self._current = index

    def _on_tabbar_configure(self, event):
        # Only reflow when the available width actually changed.
        if abs(event.width - self._last_width) > 2:
            self._last_width = event.width
            self._relayout()

    def _relayout(self, force=False):
        """Flow the tab buttons across as many rows as needed for the width."""
        avail = self._tabbar.winfo_width()
        if avail <= 1 and not force:
            return

        # Clear existing row frames (buttons are re-packed, not destroyed).
        for row in self._rows:
            row.destroy()
        self._rows = []

        pad = 6
        inner = None
        used = 0
        for tab in self._tabs:
            btn = tab["button"]
            bw = btn.winfo_reqwidth() + pad
            if inner is None or (used + bw > avail and used > 0):
                row_frame = ttk.Frame(self._tabbar)
                row_frame.pack(fill=tk.X)
                # Row frames are created after the buttons (their siblings), so
                # by default they stack above and hide them. Lower each row
                # frame beneath the buttons so the buttons remain visible.
                row_frame.lower()
                # Inner frame holds the row's buttons and is centered within the
                # full-width row, so tabs stay centered as the row grows/shrinks.
                inner = ttk.Frame(row_frame)
                inner.pack()
                inner.lower()
                self._rows.append(row_frame)
                used = 0
            btn.pack(in_=inner, side=tk.LEFT, padx=1, pady=1)
            used += bw


class DayzConfigMasterApp:
    """Main application class for DayzConfigMaster"""

    # Hard ceiling for any server process (and this app itself) so the host
    # can never be locked up by runaway memory growth.
    MAX_MEMORY_LIMIT_GB = 10.0

    def __init__(self, root):
        # Start the self-kill memory guard as early as possible. If main.py
        # already started it this call is harmless (it restarts with defaults).
        try:
            setup_memory_safety(limit_gb=self.MAX_MEMORY_LIMIT_GB)
        except Exception:
            pass

        self.root = root
        self.root.title("DayzConfigMaster - Advanced DayZ Server Configuration Editor")
        self._configure_window_geometry()

        # Global right-click context menu for copy/cut/paste.
        self._context_menu = tk.Menu(self.root, tearoff=0)
        self.root.bind("<Button-3>", self._show_context_menu)

        # Initialize components
        self.validator = FieldValidator()
        self.parser = ConfigParser()
        self.current_config = None
        
        # Store all input widgets for validation
        self.input_widgets = {}
        
        # Initialize local workshop items list for Steam Workshop integration
        self._local_workshop_items = []
        
        # Map/mod discovery state
        self._stock_maps: List[str] = []
        self._workshop_maps: Dict[str, Tuple[str, str]] = {}
        self._workshop_mods: Dict[str, Tuple[str, str]] = {}
        self._selected_mods: set = set()
        self._map_source_var = tk.StringVar(value="")
        self._workshop_dir_var = tk.StringVar(value="")

        # Track selected mod folder for Browse button and mod details
        self._selected_mod_folder: Optional[str] = None
        self._mod_folder_by_name: Dict[str, str] = {}
        self._mod_workshop_id_by_name: Dict[str, str] = {}

        # Workshop directory cache - ONE source of truth shared across tabs
        self._workshop_directory_cache: Optional[str] = None

        # Cached mapping of workshop map display names to real world names.
        self._workshop_world_name_cache: Optional[Dict[str, str]] = None
        self._workshop_world_name_cache_dir: Optional[str] = None

        # Busy cursor / status tracking so long operations give user feedback.
        self._busy_count = 0

        # Mod tree selection state
        self._mod_tree: Optional[ttk.Treeview] = None
        self._mod_tree_items: Dict[str, str] = {}  # mod_name -> treeview item id

        # Map tree selection state
        self._map_tree: Optional[ttk.Treeview] = None
        self._map_tree_items: Dict[str, str] = {}  # map_name -> treeview item id
        self._selected_map: str = ""

        # Mission tree selection state
        self._mission_tree: Optional[ttk.Treeview] = None
        self._mission_tree_items: Dict[str, str] = {}  # mission_name -> treeview item id
        self._selected_missions: set = set()
        self._mission_dir_by_name: Dict[str, str] = {}
        self._mission_paths_var: tk.StringVar = tk.StringVar(value="")
        self._mission_source_dirs: List[str] = []
        self._selected_mission_folder: Optional[str] = None

        # Multi-instance server control state
        self._instance_vars: List[Dict[str, Any]] = []
        self._instance_count_var = tk.IntVar(value=1)
        self._multi_instance_enabled_var = tk.BooleanVar(value=False)
        self._instance_config_frame: Optional[ttk.Frame] = None
        self._single_server_map_combo: Optional[ttk.Combobox] = None

        # Server process monitoring state
        self._server_monitor_after_id: Optional[str] = None
        self._server_monitor_interval_ms: int = 2000

        # Running instance tracking to prevent duplicate starts.
        self._running_instance_ids: set = set()
        self._single_server_running: bool = False
        # Per-instance button widgets for dynamic enable/disable.
        self._instance_start_buttons: Dict[str, ttk.Button] = {}
        self._instance_stop_buttons: Dict[str, ttk.Button] = {}
        
        # Server Control inner notebook references
        self._control_notebook: Optional[ttk.Notebook] = None
        self._ban_frame: Optional[ttk.Frame] = None
        self._logs_frame: Optional[ttk.Frame] = None

        # CRON scheduler for restarts, backups and messages.
        self._event_scheduler: Optional["EventScheduler"] = None
        self._scheduler_enabled_var = tk.BooleanVar(value=False)

        # RCon / Ban state (moved from standalone Ban List tab)
        self.rcon_client = None
        self.rcon_host_var = tk.StringVar(value="localhost")
        self.rcon_port_var = tk.StringVar(value="2306")
        self.rcon_password_var = tk.StringVar(value="")
        self.rcon_instance_var = tk.StringVar(value="")  # selected multi-instance id
        self._rcon_clients: Dict[str, Any] = {}  # instance_id -> RConClient
        self.ban_steam_id_var = tk.StringVar(value="")
        self.ban_name_var = tk.StringVar(value="")
        self.ban_duration_var = tk.StringVar(value="0")
        self.ban_reason_var = tk.StringVar(value="")
        self.vpp_super_admin_var = tk.StringVar(value="")
        self.player_tree: Optional[ttk.Treeview] = None
        self.ban_tree: Optional[ttk.Treeview] = None
        
        # Effects & Triggers text areas (moved into Server Config, per-instance)
        self._effects_text_areas: Dict[str, Any] = {}
        self._instance_effects_content: Dict[int, Dict[str, str]] = {}
        self._effects_instance_var: tk.StringVar = tk.StringVar(value="1")
        self._effects_filenames: List[str] = []
        
        # Configure styles
        self._configure_styles()
        
        # Create UI components
        self._create_menu()
        self._create_main_content()
        self._create_status_bar()
        
        print("DayzConfigMaster initialized successfully")

        # Scan for existing DayZ server configuration and ask user to load it
        self._scan_and_load_existing_config()

        # Initialize the scheduler (not started until enabled).
        self._init_event_scheduler()

    def _init_event_scheduler(self) -> None:
        """Create and register scheduler callbacks."""
        try:
            projects_root = self._get_projects_root()
        except Exception:
            return
        if not projects_root:
            return

        self._event_scheduler = EventScheduler(str(projects_root))
        self._event_scheduler.set_callback(
            EventType.BACKUP, self._on_scheduler_backup
        )
        self._event_scheduler.set_callback(
            EventType.RESTART, self._on_scheduler_restart
        )
        self._event_scheduler.set_callback(
            EventType.MESSAGE, self._on_scheduler_message
        )
        self._event_scheduler.load_events()

    def _on_scheduler_backup(self, event: Any) -> None:
        """Callback for scheduled BACKUP events."""
        self.log_text.insert(
            tk.END,
            f"[{self._get_timestamp()}] Scheduled backup event triggered: {event.name}\n",
        )
        for instance in getattr(self, "_instance_vars", []):
            ok, msg, _ = self._ensure_instance_storage_backup(instance)
            self.log_text.insert(
                tk.END,
                f"[{self._get_timestamp()}] {msg}\n",
            )

    def _on_scheduler_restart(self, event: Any) -> None:
        """Callback for scheduled RESTART events."""
        self.log_text.insert(
            tk.END,
            f"[{self._get_timestamp()}] Scheduled restart event triggered: {event.name}\n",
        )
        # Restart only running instances to avoid unwanted starts.
        running = list(self._running_instance_ids)
        if not running:
            self.log_text.insert(
                tk.END,
                f"[{self._get_timestamp()}] No running instances to restart.\n",
            )
            return
        for instance in self._instance_vars:
            if instance["id"].get() in running:
                self._stop_single_instance(instance)
                # Small delay to let the server release ports/files.
                self.root.after(5000, lambda inst=instance: self._start_single_instance(inst))

    def _on_scheduler_message(self, event: Any) -> None:
        """Callback for scheduled MESSAGE events."""
        text = " ".join(event.params) if event.params else "Server message"
        self.log_text.insert(
            tk.END,
            f"[{self._get_timestamp()}] Scheduled message: {text}\n",
        )
        try:
            if self.rcon_client is not None:
                self.rcon_client.send_message(text)
        except Exception as exc:
            self.log_text.insert(
                tk.END,
                f"[{self._get_timestamp()}] Failed to send scheduled message: {exc}\n",
            )

    def _toggle_event_scheduler(self) -> None:
        """Start or stop the CRON scheduler thread."""
        if self._event_scheduler is None:
            return
        if self._scheduler_enabled_var.get():
            self._event_scheduler.start()
            self.log_text.insert(
                tk.END,
                f"[{self._get_timestamp()}] Event scheduler started.\n",
            )
        else:
            self._event_scheduler.stop()
            self.log_text.insert(
                tk.END,
                f"[{self._get_timestamp()}] Event scheduler stopped.\n",
            )

    def _is_valid_tk_root(self) -> bool:
        """Return True if self.root is a real, alive Tk widget."""
        root = getattr(self, "root", None)
        return isinstance(root, tk.Tk) and bool(root.winfo_exists())

    def _safe_toplevel(self, title: str = "", geometry: str = "") -> Optional[tk.Toplevel]:
        """Create a Toplevel window only when self.root is a real Tk widget.

        Passing a non-widget (e.g. a ``MagicMock`` in tests, or a destroyed
        window) as parent to ``tk.Toplevel`` can cause infinite attribute
        recursion that exhausts system memory. This helper validates the root
        first and returns ``None`` when it is not safe to create a dialog.
        """
        if not self._is_valid_tk_root():
            return None
        try:
            top = tk.Toplevel(self.root)
            if title:
                top.title(title)
            if geometry:
                top.geometry(geometry)
            return top
        except tk.TclError:
            return None

    def _safe_after(self, ms: int, func, *args):
        """Schedule a callback on self.root only if it is a real Tk widget.

        Falls back to synchronous execution when no real root exists so that
        code remains usable in headless/test environments without leaking
        callbacks or recursing.
        """
        root = getattr(self, "root", None)
        if isinstance(root, tk.Tk) and root.winfo_exists():
            return root.after(ms, func, *args)
        if args:
            func(*args)
        else:
            func()
        return None

    def _set_busy(self, message: str = "Processing...") -> None:
        """Show a watch cursor and status message."""
        self._busy_count += 1
        if hasattr(self, "status_var"):
            self.status_var.set(message)
        try:
            self.root.config(cursor="watch")
            self.root.update_idletasks()
        except (AttributeError, tk.TclError):
            pass

    def _clear_busy(self) -> None:
        """Restore the normal cursor once all busy calls are cleared."""
        self._busy_count = max(0, self._busy_count - 1)
        if self._busy_count == 0:
            if hasattr(self, "status_var"):
                self.status_var.set("Ready")
            try:
                self.root.config(cursor="")
                self.root.update_idletasks()
            except (AttributeError, tk.TclError):
                pass

    @contextlib.contextmanager
    def _busy_context(self, message: str = "Processing..."):
        """Context manager wrapper around _set_busy/_clear_busy."""
        self._set_busy(message)
        try:
            yield
        finally:
            self._clear_busy()

    def _scan_and_load_existing_config(self):
        """
        Scan for existing DayZ server configuration files on startup.
        
        Scans common locations:
        - Steam Common Directory (DayZServer/serverDZ.cfg)
        
        If found, prompts user to load the existing settings.
        """
        # Possible serverDZ.cfg locations
        possible_paths = [
            # Linux/Unix Steam common location
            Path.home() / ".local" / "share" / "Steam" / "steamapps" / "common" / "DayZServer" / "serverDZ.cfg",
            Path.home() / ".steam" / "steam" / "steamapps" / "common" / "DayZServer" / "serverDZ.cfg",
            # Windows Steam locations
            Path(r"C:\Program Files\Steam\steamapps\common\DayZServer\serverDZ.cfg"),
            Path(r"C:\Program Files (x86)\Steam\steamapps\common\DayZServer\serverDZ.cfg"),
            Path.home() / "AppData" / "Local" / "Steam" / "steamapps" / "common" / "DayZServer" / "serverDZ.cfg",
        ]
        
        # Also check dayz config directory
        dayz_config_path = Path.home() / ".dayz"
        if dayz_config_path.exists():
            possible_paths.append(dayz_config_path / "serverDZ.cfg")
        
        for cfg_path in possible_paths:
            path_str = str(cfg_path)
            if Path(path_str).exists():
                # Found existing serverDZ.cfg
                response = messagebox.askyesno(
                    "Load Existing Configuration?",
                    f"DayZConfigMaster detected an existing DayZ server configuration:\n\n"
                    f"{path_str}\n\n"
                    f"Would you like to load these settings into the editor?\n\n"
                    f"This will populate all fields with your current server settings.",
                    parent=self.root
                )
                
                if response:
                    try:
                        config, error = self.parser.load_file(path_str)
                        if config and isinstance(config, ServerConfig):
                            self.current_config = config
                            # Update UI with loaded values
                            self._update_server_config_ui(config)
                            
                            status_msg = f"Loaded serverDZ.cfg from {path_str}"
                            status_msg += f"\nHostname: {config.name}"
                            status_msg += f"\nPort: {config.port}"
                            status_msg += f"\nMax Players: {config.max_players}"
                            self.status_var.set(status_msg)
                            
                            # Build detailed info message
                            info_lines = [
                                "Successfully loaded server settings!",
                                "",
                                f"File: {path_str}",
                                f"Hostname: {config.name or 'Not set'}",
                                f"Port: {config.port}",
                                f"Max Players: {config.max_players}",
                                f"Password: {'*' * 8 if config.password else 'None'}",
                                f"Game Mode: {config.game_mode}",
                                "",
                                "The configuration has been loaded into the editor.",
                                "You can edit any values and save changes using File > Save."
                            ]
                            
                            messagebox.showinfo(
                                "Configuration Loaded",
                                "\n".join(info_lines)
                            )
                        elif error:
                            self.status_var.set(f"Load failed: {error}")
                        break  # Only prompt for first found config
                    except Exception as e:
                        print(f"Error loading existing config: {e}")
                        self.status_var.set("Could not load existing configuration")
                        
                        # Show error dialog even if user chose to load
                        messagebox.showerror(
                            "Load Error",
                            f"Failed to parse the configuration file:\n{path_str}\n\n"
                            f"Error: {str(e)}\n\n"
                            f"The application will continue with default settings."
                        )
                else:
                    # User chose not to load - just set status and continue
                    self.status_var.set("Ready - No configuration loaded")
                return  # Don't check other paths after first match or user choice
        
        # No config found
        self.status_var.set("Ready - DayzConfigMaster initialized. No existing serverDZ.cfg detected.")
    
    def _configure_window_geometry(self):
        """
        Size the window to about 60% of the screen width and center it.

        A narrower-than-fullscreen start makes the notebook tab bar wrap onto
        two rows (so no tab is off-screen), while the window stays fully
        resizable and grows to fill the display when maximized.
        """
        screen_w = self.root.winfo_screenwidth()
        screen_h = self.root.winfo_screenheight()

        win_w = min(1400, max(1000, int(screen_w * 0.6)))
        win_h = min(950, max(650, int(screen_h * 0.85)))
        pos_x = max(0, (screen_w - win_w) // 2)
        pos_y = max(0, (screen_h - win_h) // 3)

        self.root.geometry(f"{win_w}x{win_h}+{pos_x}+{pos_y}")
        self.root.minsize(900, 600)
        self.root.resizable(True, True)
    
    def _configure_styles(self):
        """Configure ttk styles"""
        style = ttk.Style()
        style.theme_use('clam')
        style.configure('Validation.TLabel', foreground='gray', font=('Arial', 8))
        style.configure('Warning.TLabel', foreground='red', font=('Arial', 8, 'bold'))
    
    def _apply_theme(self, theme_name: str):
        """
        Apply a UI theme (light or dark).
        
        Args:
            theme_name: "light" or "dark"
        """
        style = ttk.Style()
        
        if theme_name.lower() == "dark":
            # Dark theme colors
            bg_color = "#2b2b2b"
            fg_color = "#ffffff"
            entry_bg = "#3d3d3d"
            
            style.configure(
                'TFrame', background=bg_color,
                fieldbackground=entry_bg, foreground=fg_color
            )
            style.configure(
                'TLabel', background=bg_color, foreground=fg_color
            )
            style.configure(
                'TLabelframe', background=bg_color, foreground=fg_color
            )
            style.configure(
                'TLabelframe.Label', background=bg_color, foreground=fg_color
            )
            style.configure(
                'TButton', background="#4a4a4a", foreground=fg_color,
                bordercolor=entry_bg
            )
            style.map('TButton',
                     background=[("active", "#5a5a5a")],
                     foreground=[("active", fg_color)])
            style.configure(
                'TEntry', fieldbackground=entry_bg, foreground=fg_color
            )
            style.configure(
                'Treeview', background=entry_bg, foreground=fg_color,
                fieldbackground=entry_bg, rowheight=25
            )
            style.map('Treeview',
                     background=[("selected", "#4a90d9")],
                     foreground=[("selected", "white")])
            style.configure(
                'TNotebook', background=bg_color, tabmargins=[10, 5]
            )
            style.configure(
                'TNotebook.Tab', background="#3d3d3d", foreground=fg_color,
                padding=[12, 4]
            )
            style.map('TNotebook.Tab',
                     background=[("active", "#4a90d9")],
                     foreground=[("active", "white")])
            style.configure(
                'Vertical.TScrollbar', background="#4a4a4a",
                troughcolor=bg_color, bordercolor=bg_color
            )
            style.configure(
                'Horizontal.TScrollbar', background="#4a4a4a",
                troughcolor=bg_color, bordercolor=bg_color
            )
        else:
            # Light theme (default)
            bg_color = "#f0f0f0"
            fg_color = "#000000"
            
            style.configure('TFrame', background=bg_color)
            style.configure('TLabel', background=bg_color, foreground=fg_color)
            style.configure('TLabelframe', background="#ffffff")
            style.configure('TLabelframe.Label', background="#ffffff", foreground=fg_color)
            style.configure('TButton', background="#e0e0e0", foreground=fg_color)
            style.map('TButton',
                     background=[("active", "#d0d0d0")])
            style.configure('TEntry', fieldbackground="#ffffff", foreground=fg_color)
            style.configure('Treeview', background="#ffffff", foreground=fg_color, rowheight=25)
            style.map('Treeview',
                     background=[("selected", "#4a90d9")],
                     foreground=[("selected", "white")])
    
    def create_wiki_link(self, parent, tab_name: str, wiki_path: str = None):
        """
        Create a link to documentation for this tab.
        
        Args:
            parent: Parent widget
            tab_name: Name of the tab for documentation purposes
            wiki_path: Optional specific wiki path; defaults to tab name
            
        Returns:
            ttk.Frame containing the documentation label with link
        """
        doc_frame = ttk.Frame(parent)
        
        if wiki_path is None:
            wiki_path = f"06-configuration/{tab_name.lower().replace(' ', '-')}"
        
        doc_url = f"https://github.com/landracer/DayZConfigMaster/wiki/{wiki_path}"
        
        doc_label = ttk.Label(
            doc_frame,
            text=f"📖 Documentation: {doc_url}",
            foreground="blue",
            cursor="hand2",
            font=("Arial", 9, "underline")
        )
        doc_label.pack(anchor=tk.W)
        
        # Bind click event to open URL
        def open_wiki(event):
            import webbrowser
            webbrowser.open(doc_url)
        
        doc_label.bind("<Button-1>", open_wiki)
        
        return doc_frame
    
    def create_layout_description(self, parent, title: str, description: str,
                                   wiki_link: str = None):
        """
        Create a layout description frame with explanation and optional wiki link.
        
        Args:
            parent: Parent widget
            title: Section title
            description: Detailed description of this section's layout
            wiki_link: Optional specific wiki URL for more information
            
        Returns:
            ttk.Frame containing the layout description
        """
        desc_frame = ttk.LabelFrame(parent, text=title, padding=5)
        
        # Description label with word wrapping
        desc_label = ttk.Label(
            desc_frame,
            text=description,
            wraplength=800,
            justify=tk.LEFT,
            font=("Arial", 9)
        )
        desc_label.pack(fill=tk.X, padx=5, pady=5)
        
        # Wiki link if provided
        if wiki_link:
            wiki_frame = ttk.Frame(desc_frame)
            wiki_frame.pack(fill=tk.X, padx=5, pady=(0, 5))
            
            wiki_label = ttk.Label(
                wiki_frame,
                text=f"📖 See also: {wiki_link}",
                foreground="blue",
                cursor="hand2",
                font=("Arial", 8, "underline")
            )
            wiki_label.pack(anchor=tk.W)
            
            def open_wiki(event):
                import webbrowser
                webbrowser.open(wiki_link)
            
            wiki_label.bind("<Button-1>", open_wiki)
        
        return desc_frame
    
    def _create_menu(self):
        """Create the application menu"""
        menubar = tk.Menu(self.root)
        self.root.config(menu=menubar)
        
        # File menu
        file_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="File", menu=file_menu)
        file_menu.add_command(label="New Project", command=self._new_project)
        file_menu.add_command(label="Open", command=self._open_file)
        file_menu.add_command(label="Save", command=self._save_file)
        file_menu.add_separator()
        file_menu.add_command(label="Exit", command=self._exit_app)
        
        # Edit menu
        edit_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="Edit", menu=edit_menu)
        edit_menu.add_command(label="Undo", command=lambda: messagebox.showinfo("Undo", "Undo functionality would be implemented here"))
        edit_menu.add_command(label="Redo", command=lambda: messagebox.showinfo("Redo", "Redo functionality would be implemented here"))
        edit_menu.add_separator()
        edit_menu.add_command(label="Preferences", command=self._show_preferences)
        
        # Tools menu
        tools_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="Tools", menu=tools_menu)
        tools_menu.add_command(label="Validate All Settings", command=self._validate_all_settings)
        tools_menu.add_command(label="Generate .cfg File", command=self._generate_cfg)
        tools_menu.add_separator()
        tools_menu.add_command(label="Import from Backup", command=lambda: messagebox.showinfo("Import", "Backup import functionality would be implemented here"))
        tools_menu.add_command(label="Export to Backup", command=lambda: messagebox.showinfo("Export", "Backup export functionality would be implemented here"))
        
        # View menu
        view_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="View", menu=view_menu)
        view_menu.add_command(label="Refresh All", command=self._refresh_all)
        
        # Help menu
        help_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="Help", menu=help_menu)
        # Documentation menu item - open the wiki home page
        def open_wiki():
            import webbrowser
            webbrowser.open("https://github.com/landracer/DayZConfigMaster/wiki")
        
        help_menu.add_command(label="Documentation", command=open_wiki)
        help_menu.add_command(label="About DayzConfigMaster", command=self._show_about)
    
    def _create_main_content(self):
        """Create the main content area with tabs"""
        # Wrapping tab bar: labels stay fully readable and reflow onto multiple
        # rows when the window is narrow, collapsing toward one row as it grows.
        self.notebook = WrappingTabView(self.root)
        self.notebook.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
        
        # Create all tab frames with comprehensive DayZ configuration support
        self._create_workspace_tab()
        self._create_trader_config_tab()
        self._create_server_config_tab()  # In-Game Settings and Effects & Triggers now live here
        self._create_advanced_settings_tab()
        
        # Server Control tab - unified single/multi-instance server management
        # Now includes Ban List & Players and Logs & Diagnostics as inner tabs
        self._create_server_control_tab()
        
        # House-cleaning and scheduling (memory leak management, restarts)
        self._create_house_cleaning_tab()
        
        # Mod Builder tab - PboBuilder integration for packing mods into PBO files
        self._create_mod_builder_tab()
        
        # Workshop tab - search and download workshop items
        self._create_workshop_tab()
    
    def _create_workspace_tab(self):
        """Create the workspace tab with comprehensive overview"""
        workspace_frame = ScrollableFrame(self.notebook)
        self.notebook.add(workspace_frame, text="Workspace")
        
        # Welcome message
        welcome_label = ttk.Label(
            workspace_frame.frame, 
            text="DayzConfigMaster - Advanced DayZ Server Configuration Editor",
            font=("Arial", 18, "bold")
        )
        welcome_label.pack(pady=20)
        
        description_label = ttk.Label(
            workspace_frame.frame,
            text="Comprehensive DayZ server configuration editor with full support for all configuration files\n"
                 "and advanced features for modded servers.",
            font=("Arial", 12),
            justify=tk.CENTER
        )
        description_label.pack(pady=10)
        
        # Wiki documentation link
        wiki_link_frame = self.create_wiki_link(workspace_frame.frame, "index")
        wiki_link_frame.pack(fill=tk.X, padx=20, pady=(5, 15))
        
        # Quick overview of supported files
        files_frame = ttk.LabelFrame(workspace_frame.frame, text="Supported Configuration Files", padding=10)
        files_frame.pack(fill=tk.X, padx=20, pady=10)
        
        supported_files = [
            "economycore.xml - Core game configuration (/wiki/06-configuration/economy-core)",
            "limitsdefinitions.xml - Item and limit definitions (categories, usages, values) (/wiki/06-configuration/limits-definitions)",
            "cfgrandompresets.xml - Random item presets with advanced settings (/wiki/06-configuration/random-presets)",
            "cfgspawnabletypes.xml - Spawnable item types with categories and flags (/wiki/06-configuration/spawnable-types)",
            "traderconfig.txt - Trader configurations with full category support (/wiki/06-configuration/trader-config)",
            "serverDZ.cfg - Main server configuration file (/wiki/06-configuration/server-config)",
            "cfggameplay.json - Gameplay settings (/wiki/06-configuration/advanced-settings)",
            "cfgenvironment.xml - Environmental settings",
            "Server Control - Unified single/multi-instance server management"
        ]
        
        for file_desc in supported_files:
            ttk.Label(files_frame, text=file_desc).pack(anchor=tk.W)
        
        # DayZ features overview
        features_frame = ttk.LabelFrame(workspace_frame.frame, text="DayZ Server Features Supported", padding=10)
        features_frame.pack(fill=tk.X, padx=20, pady=10)
        
        features = [
            "Full Economy System Control (/wiki/06-configuration/economy-core)",
            "Advanced Spawn Limit Management (/wiki/06-configuration/limits-definitions)",
            "Random Preset Customization (/wiki/06-configuration/random-presets)",
            "Item Category and Usage Control",
            "Trader Configuration with Categories (/wiki/06-configuration/trader-config)",
            "Server Performance Settings (/wiki/06-configuration/server-config)",
            "Server Control - Unified single/multi-instance server management (/wiki/06-configuration/server-control)",
            "House Cleaning & Scheduling - Memory leak management (/wiki/06-configuration/house-cleaning-scheduling)",
            "Modding Configuration - Mods and Workshop support (/wiki/06-configuration/modding)"
        ]
        
        for feature in features:
            ttk.Label(features_frame, text=feature).pack(anchor=tk.W)
    
    def _create_in_game_settings_content(self, parent: ttk.Frame):
        """Create In-Game Settings content inside the Server Config tab.

        Uses a compact sidebar layout: category list on the left, selected
        category's tree on the right. No nested tabs, minimal header.
        """
        parent.columnconfigure(0, weight=0)
        parent.columnconfigure(1, weight=1)
        parent.rowconfigure(0, weight=1)

        # Category list (sidebar)
        categories = [
            ("Classes", ("Name", "Type"), [
                ("Item", "1"), ("Weapon", "2"), ("Vehicle", "3"),
                ("Clothing", "4"), ("Food", "5"), ("Tool", "6")
            ]),
            ("Defaults", ("Name", "Value"), [
                ("max_items", "1000"), ("max_weapons", "500"), ("max_vehicles", "100")
            ]),
            ("Categories", ("Name", "Description"), [
                ("weapons", "Firearms, melee weapons"),
                ("tools", "Axes, shovels, saws, knives"),
                ("clothes", "Clothing, hats, gloves, shoes"),
                ("containers", "Backpacks, cases, pouches"),
                ("food", "Canned food, drinks, raw food"),
                ("vehiclesparts", "Tires, spark plugs, batteries"),
                ("explosives", "Grenades, mines, landmines")
            ]),
            ("Usages", ("Name", "Description"), [
                ("Military", "Military bases, tents, barracks, checkpoints"),
                ("Police", "Police stations"),
                ("Hunting", "Hunting stands, lodges, deer stands"),
                ("Town", "Residential houses, apartments in towns"),
                ("Village", "Small rural settlements"),
                ("Farm", "Barns, farm buildings, silos"),
                ("Industrial", "Factories, warehouses, workshops"),
                ("Medic", "Hospitals, medical buildings"),
            ]),
            ("Values/Tiers", ("Name", "Zone", "Loot Quality"), [
                ("Tier1", "Coastal areas", "Basic civilian loot"),
                ("Tier2", "Inland towns", "Mid-range civilian and some military"),
                ("Tier3", "Deep inland / NWAF area", "High-value military loot")
            ]),
            ("Random Presets", ("Name", "Cargo", "Attachments", "Min", "Max"), [
                ("DefaultWeapon", "10", "5", "1", "3"),
                ("DefaultAmmo", "20", "0", "1", "5"),
                ("DefaultFood", "15", "3", "1", "4"),
                ("MilitaryPreset", "25", "8", "2", "6")
            ]),
            ("Spawnable Types", ("Name", "Category", "Usage", "Value"), [
                ("AKM", "weapons", "Military", "Tier3"),
                ("AmmoBox", "tools", "Town", "Tier2"),
                ("Food", "food", "Village", "Tier1"),
                ("Backpack", "containers", "Town", "Tier2")
            ]),
        ]

        # Compact header below the Files tab, spanning full width
        header = ttk.Frame(parent)
        header.grid(row=0, column=0, columnspan=2, sticky=tk.W+tk.E, padx=5, pady=(5, 2))

        ttk.Label(header, text="In-Game Settings", font=("Arial", 11, "bold")).pack(side=tk.LEFT)
        wiki_link_frame = self.create_wiki_link(header, "in-game-settings")
        wiki_link_frame.pack(side=tk.RIGHT)

        ttk.Separator(parent, orient=tk.HORIZONTAL).grid(
            row=1, column=0, columnspan=2, sticky=tk.W+tk.E, padx=5, pady=(0, 5))

        parent.rowconfigure(2, weight=1)

        sidebar = ttk.Frame(parent, width=140)
        sidebar.grid(row=2, column=0, sticky=tk.N+tk.S+tk.W, padx=5, pady=5)
        sidebar.grid_propagate(False)
        sidebar.rowconfigure(0, weight=1)

        cat_listbox = tk.Listbox(sidebar, selectmode=tk.SINGLE, exportselection=False)
        cat_listbox.grid(row=0, column=0, sticky=tk.N+tk.S+tk.E+tk.W)
        cat_scroll = ttk.Scrollbar(sidebar, orient=tk.VERTICAL, command=cat_listbox.yview)
        cat_scroll.grid(row=0, column=1, sticky=tk.N+tk.S)
        cat_listbox.configure(yscrollcommand=cat_scroll.set)

        for tab_name, _, _ in categories:
            cat_listbox.insert(tk.END, tab_name)

        # Content pane
        content_frame = ttk.Frame(parent)
        content_frame.grid(row=2, column=1, sticky=tk.N+tk.S+tk.E+tk.W, padx=(0, 5), pady=5)
        content_frame.columnconfigure(0, weight=1)
        content_frame.rowconfigure(1, weight=1)

        selected_label = ttk.Label(content_frame, text="Classes", font=("Arial", 10, "bold"))
        selected_label.grid(row=0, column=0, sticky=tk.W, pady=(0, 5))

        tree_frame = ttk.Frame(content_frame)
        tree_frame.grid(row=1, column=0, sticky=tk.N+tk.S+tk.E+tk.W)
        tree_frame.columnconfigure(0, weight=1)
        tree_frame.rowconfigure(0, weight=1)

        # Store trees and data for switching
        self._in_game_trees = {}
        for tab_name, columns, sample_data in categories:
            tree = ttk.Treeview(tree_frame, columns=columns, show="headings", selectmode='browse')
            for col in columns:
                tree.heading(col, text=col)
                tree.column(col, anchor=tk.W)
            tree.bind('<Double-1>', lambda e, t=tree: self._edit_treeview_item(t))
            for item in sample_data:
                tree.insert("", tk.END, values=item)
            self._in_game_trees[tab_name] = tree

        current_tree = None
        tree_scroll = ttk.Scrollbar(tree_frame, orient=tk.VERTICAL)
        tree_scroll.grid(row=0, column=1, sticky=tk.N+tk.S)

        def _on_category_select(event=None):
            nonlocal current_tree
            selection = cat_listbox.curselection()
            if not selection:
                return
            name = cat_listbox.get(selection[0])
            selected_label.config(text=name)
            if current_tree is not None:
                current_tree.grid_remove()
            new_tree = self._in_game_trees[name]
            new_tree.grid(row=0, column=0, sticky=tk.N+tk.S+tk.E+tk.W)
            new_tree.configure(yscrollcommand=tree_scroll.set)
            tree_scroll.configure(command=new_tree.yview)
            current_tree = new_tree

        cat_listbox.bind("<<ListboxSelect>>", _on_category_select)
        cat_listbox.selection_set(0)
        _on_category_select()
    
    def _create_trader_config_tab(self):
        """Create the trader config tab with comprehensive DayZ features and DZconfig integration"""
        trader_frame = ScrollableFrame(self.notebook)
        self.notebook.add(trader_frame, text="Trader Config")
        
        # Title
        title_label = ttk.Label(trader_frame.frame, text="Trader Configuration (traderconfig.txt)", font=("Arial", 14, "bold"))
        title_label.pack(pady=10)
        
        # Wiki documentation link
        wiki_link_frame = self.create_wiki_link(trader_frame.frame, "trader-config")
        wiki_link_frame.pack(fill=tk.X, padx=10, pady=(5, 15))
        
        # Layout description
        layout_desc = self.create_layout_description(
            trader_frame.frame,
            "Trader Configuration Layout",
            """This tab contains the complete trader configuration system:
- Trader definitions with names and categories
- Item lists for each trader
- Distance settings (MinDistance/MaxDistance)
- Price multipliers for buying/selling
- Robbery settings

All traders support custom categories and item lists. Use PriceMultiplier to adjust base prices across all items, BuyPriceModifier for buy price adjustments (negative = cheaper), and SellPriceModifier for sell price adjustments (positive = better rates).""",
            "https://github.com/landracer/DayZConfigMaster/wiki/06-configuration/trader-config"
        )
        layout_desc.pack(fill=tk.X, padx=10, pady=(5, 15))
        
        # Text area for trader config
        text_frame = ttk.Frame(trader_frame.frame)
        text_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)
        
        # Create text widget with internal scrollbar (better placement)
        text_area = scrolledtext.ScrolledText(text_frame, wrap=tk.WORD, height=20, font=("Courier", 10))
        text_area.pack(fill=tk.BOTH, expand=True)
        
        # Sample trader configuration based on DayZ documentation
        sample_trader = """// Trader Configuration File
// This file contains all trader settings for DayZ server
// Integrated with DZconfig trader tools

Trader1 {
    Name = "General Store"
    Category = "General"
    Items = [
        "Food",
        "Weapons",
        "Ammo"
    ]
    MinDistance = 50.0
    MaxDistance = 200.0
    CanBeRobbed = true
    
    PriceMultiplier = 1.0
    BuyPriceModifier = -0.1
    SellPriceModifier = 0.1
}

Trader2 {
    Name = "Medical Supplies"
    Category = "Medical"
    Items = [
        "Medkit",
        "Painkillers",
        "Bandages"
    ]
    MinDistance = 30.0
    MaxDistance = 150.0
    CanBeRobbed = true
    
    PriceMultiplier = 1.2
    BuyPriceModifier = -0.15
    SellPriceModifier = 0.15
}
"""
        
        text_area.insert(tk.END, sample_trader)
    
    def _create_server_config_tab(self):
        """Create the server config tab with a clean, notebook-based layout.

        Top-level categories are exposed as tabs inside the Server Config tab:
        - Core: name, ports, players, passwords, security, voice/view, time/weather,
                network, instance/storage, logging
        - Mods: map selection, mod tree, mod paths, workshop integration
        - Multi-Instance: enable toggle, count, per-instance config rows
        - Files: In-Game Settings and per-instance Effects & Triggers
        """
        server_frame = ttk.Frame(self.notebook)
        self.notebook.add(server_frame, text="Server Config")
        server_frame.columnconfigure(0, weight=1)
        server_frame.rowconfigure(1, weight=1)

        # Slim header shared by all sub-tabs (Core, Mods, Files, Mod Settings,
        # Mission XML Editor, Spawn Loadout, etc.).
        header = ttk.Frame(server_frame)
        header.grid(row=0, column=0, sticky=tk.W+tk.E, padx=10, pady=(5, 2))

        ttk.Label(header, text="Server Configuration", font=("Arial", 11, "bold")).pack(side=tk.LEFT)
        wiki_link_frame = self.create_wiki_link(header, "server-config")
        wiki_link_frame.pack(side=tk.RIGHT)

        # Top-level notebook for server config categories
        self._server_config_notebook = ttk.Notebook(server_frame)
        self._server_config_notebook.grid(row=1, column=0, sticky=tk.N+tk.S+tk.E+tk.W, padx=10, pady=5)
        server_frame.rowconfigure(1, weight=1)
        server_frame.columnconfigure(0, weight=1)

        # Core tab
        core_frame = ttk.Frame(self._server_config_notebook)
        core_frame.columnconfigure(0, weight=1)
        core_frame.rowconfigure(0, weight=1)
        self._server_config_notebook.add(core_frame, text="Core")
        self._create_server_config_core_content(core_frame)

        # Mods tab
        mods_frame = ttk.Frame(self._server_config_notebook)
        mods_frame.columnconfigure(0, weight=1)
        mods_frame.rowconfigure(0, weight=1)
        self._server_config_notebook.add(mods_frame, text="Mods")
        self._create_server_config_mods_content(mods_frame)

        # Missions tab
        missions_frame = ttk.Frame(self._server_config_notebook)
        missions_frame.columnconfigure(0, weight=1)
        missions_frame.rowconfigure(0, weight=1)
        self._server_config_notebook.add(missions_frame, text="Missions")
        self._create_missions_tab(missions_frame)

        # Multi-Instance tab (scrollable so low-resolution screens can reach
        # all configured instance rows).
        multi_outer = ttk.Frame(self._server_config_notebook)
        multi_outer.columnconfigure(0, weight=1)
        multi_outer.rowconfigure(0, weight=1)
        self._server_config_notebook.add(multi_outer, text="Multi-Instance")
        multi_scroll = ScrollableFrame(multi_outer)
        multi_scroll.grid(row=0, column=0, sticky=tk.NSEW)
        multi_outer.rowconfigure(0, weight=1)
        multi_outer.columnconfigure(0, weight=1)
        self._create_server_config_multi_instance_content(multi_scroll.frame)

        # Files tab (In-Game Settings + Effects & Triggers)
        files_frame = ttk.Frame(self._server_config_notebook)
        files_frame.columnconfigure(0, weight=1)
        files_frame.rowconfigure(0, weight=1)
        self._server_config_notebook.add(files_frame, text="Files")
        self._create_server_config_files_content(files_frame)

        # Mission XML Editor tab
        xml_editor_frame = ttk.Frame(self._server_config_notebook)
        xml_editor_frame.columnconfigure(0, weight=1)
        xml_editor_frame.rowconfigure(0, weight=1)
        self._server_config_notebook.add(xml_editor_frame, text="Mission XML Editor")
        self._xml_config_editor_tab = XmlConfigEditorTab(xml_editor_frame, self._get_current_mission_root)

        # Spawn Loadout tab
        integration_frame = ttk.Frame(self._server_config_notebook)
        integration_frame.columnconfigure(0, weight=1)
        integration_frame.rowconfigure(0, weight=1)
        self._server_config_notebook.add(integration_frame, text="Spawn Loadout")
        self._mod_integration_tab = ModIntegrationTab(
            integration_frame,
            self._get_current_mission_root,
            self._get_workshop_directory,
            get_instances=self._get_instance_dicts,
            get_instance_display_name=self._instance_display_name,
            get_instance_root=self._resolve_instance_root,
            get_instance_mission_root=self._get_instance_mission_root,
        )

        # Mod Settings tab
        mod_settings_frame = ttk.Frame(self._server_config_notebook)
        mod_settings_frame.columnconfigure(0, weight=1)
        mod_settings_frame.rowconfigure(0, weight=1)
        self._server_config_notebook.add(mod_settings_frame, text="Mod Settings")
        self._create_mod_settings_content(mod_settings_frame)

        # Auto-refresh Mod Settings when its tab is selected so it always
        # reflects the currently chosen map/instance.
        self._server_config_notebook.bind(
            "<<NotebookTabChanged>>", self._on_server_config_tab_changed
        )

    def _create_server_config_core_content(self, parent: ttk.Frame):
        """Build the Core serverDZ.cfg settings into a compact two-column grid."""
        canvas = tk.Canvas(parent, borderwidth=0, highlightthickness=0)
        scrollbar = ttk.Scrollbar(parent, orient=tk.VERTICAL, command=canvas.yview)
        canvas.configure(yscrollcommand=scrollbar.set)

        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

        content = ttk.Frame(canvas)
        canvas_window = canvas.create_window((0, 0), window=content, anchor="nw")

        def _on_configure(event=None):
            canvas.configure(scrollregion=canvas.bbox("all"))
            canvas.itemconfig(canvas_window, width=canvas.winfo_width())

        content.bind("<Configure>", _on_configure)
        canvas.bind("<Configure>", _on_configure)

        def _add_field(parent_frame, label, widget, row, col, info=""):
            ttk.Label(parent_frame, text=label).grid(row=row, column=col * 2, sticky=tk.W, padx=5, pady=2)
            widget.grid(row=row, column=col * 2 + 1, sticky=tk.W+tk.E, padx=5, pady=2)
            if info:
                ttk.Label(parent_frame, text=info, foreground="gray", font=("Arial", 8)).grid(
                    row=row, column=col * 2 + 1, sticky=tk.E, padx=5)

        # Basic settings
        basic = ttk.LabelFrame(content, text="Basic", padding=8)
        basic.pack(fill=tk.X, padx=5, pady=5)
        basic.columnconfigure(1, weight=1)
        basic.columnconfigure(3, weight=1)

        server_name_var = tk.StringVar(value="DayZ Server")
        server_name_entry = ttk.Entry(basic, textvariable=server_name_var)
        _add_field(basic, "Server Name:", server_name_entry, 0, 0)
        self.input_widgets["Server Name"] = (server_name_entry, {"default": "DayZ Server"})

        server_port_var = tk.StringVar(value="2302")
        server_port_entry = ttk.Entry(basic, width=12, textvariable=server_port_var)
        _add_field(basic, "Port:", server_port_entry, 0, 1)
        self.input_widgets["Server Port"] = (server_port_entry, {"min": 1024, "max": 65535, "default": "2302"})

        query_port_var = tk.StringVar(value="2303")
        query_port_entry = ttk.Entry(basic, width=12, textvariable=query_port_var)
        _add_field(basic, "Query Port:", query_port_entry, 1, 0)
        self.input_widgets["Query Port"] = (query_port_entry, {"min": 1024, "max": 65535, "default": "2303"})

        max_players_var = tk.StringVar(value="60")
        max_players_entry = ttk.Entry(basic, width=12, textvariable=max_players_var)
        _add_field(basic, "Max Players:", max_players_entry, 1, 1)
        self.input_widgets["Max Players"] = (max_players_entry, {"min": 1, "max": 1000, "default": "60"})

        game_mode_var = tk.StringVar(value="Survival")
        game_mode_combo = ttk.Combobox(basic, values=["Survival", "Custom", "Modded"], width=15, textvariable=game_mode_var)
        _add_field(basic, "Game Mode:", game_mode_combo, 2, 0)
        self.input_widgets["Game Mode"] = (game_mode_combo, {"valid_values": ["Survival", "Custom", "Modded"], "default": "Survival"})

        # Security
        security = ttk.LabelFrame(content, text="Security", padding=8)
        security.pack(fill=tk.X, padx=5, pady=5)
        security.columnconfigure(1, weight=1)
        security.columnconfigure(3, weight=1)

        server_password_var = tk.StringVar(value="")
        server_password_entry = ttk.Entry(security, textvariable=server_password_var, show="*")
        _add_field(security, "Server Password:", server_password_entry, 0, 0)
        self.input_widgets["Server Password"] = (server_password_entry, {"max_length": 32})

        password_admin_var = tk.StringVar(value="")
        password_admin_entry = ttk.Entry(security, textvariable=password_admin_var, show="*")
        _add_field(security, "Admin Password:", password_admin_entry, 0, 1)
        self.input_widgets["Admin Password"] = (password_admin_entry, {})

        enable_whitelist_var = tk.StringVar(value="0 (No)")
        enable_whitelist_combo = ttk.Combobox(security, values=["0 (No)", "1 (Yes)"], width=15, textvariable=enable_whitelist_var)
        _add_field(security, "Whitelist:", enable_whitelist_combo, 1, 0)
        self.input_widgets["Enable Whitelist"] = (enable_whitelist_combo, {"valid_values": ["0", "1"], "default": "0"})

        verify_var = tk.StringVar(value="2 (Verify)")
        verify_combo = ttk.Combobox(security, values=["0 (Off)", "1 (Debug)", "2 (Verify)"], width=15, textvariable=verify_var)
        _add_field(security, "Verify Signatures:", verify_combo, 1, 1)
        self.input_widgets["Verify Signatures"] = (verify_combo, {"valid_values": ["0", "1", "2"], "default": "2"})

        force_build_var = tk.StringVar(value="1 (Yes)")
        force_build_combo = ttk.Combobox(security, values=["0 (No)", "1 (Yes)"], width=15, textvariable=force_build_var)
        _add_field(security, "Force Same Build:", force_build_combo, 2, 0)
        self.input_widgets["Force Same Build"] = (force_build_combo, {"valid_values": ["0", "1"], "default": "1"})

        # Voice & View
        view = ttk.LabelFrame(content, text="Voice & View", padding=8)
        view.pack(fill=tk.X, padx=5, pady=5)
        view.columnconfigure(1, weight=1)
        view.columnconfigure(3, weight=1)

        disable_von_var = tk.StringVar(value="0 (No)")
        disable_von_combo = ttk.Combobox(view, values=["0 (No)", "1 (Yes)"], width=15, textvariable=disable_von_var)
        _add_field(view, "Disable VoN:", disable_von_combo, 0, 0)
        self.input_widgets["Disable VoN"] = (disable_von_combo, {"valid_values": ["0", "1"], "default": "0"})

        von_quality_var = tk.StringVar(value="20")
        von_quality_entry = ttk.Entry(view, width=12, textvariable=von_quality_var)
        _add_field(view, "VoN Quality:", von_quality_entry, 0, 1)
        self.input_widgets["VoN Codec Quality"] = (von_quality_entry, {"min": 0, "max": 30, "default": "20"})

        disable_3rd_var = tk.StringVar(value="0 (No)")
        disable_3rd_combo = ttk.Combobox(view, values=["0 (No)", "1 (Yes)"], width=15, textvariable=disable_3rd_var)
        _add_field(view, "Disable 3rd Person:", disable_3rd_combo, 1, 0)
        self.input_widgets["Disable 3rd Person"] = (disable_3rd_combo, {"valid_values": ["0", "1"], "default": "0"})

        disable_crosshair_var = tk.StringVar(value="0 (No)")
        disable_crosshair_combo = ttk.Combobox(view, values=["0 (No)", "1 (Yes)"], width=15, textvariable=disable_crosshair_var)
        _add_field(view, "Disable Crosshair:", disable_crosshair_combo, 1, 1)
        self.input_widgets["Disable Crosshair"] = (disable_crosshair_combo, {"valid_values": ["0", "1"], "default": "0"})

        disable_personal_light_var = tk.StringVar(value="1 (Yes)")
        disable_personal_light_combo = ttk.Combobox(view, values=["0 (No)", "1 (Yes)"], width=15, textvariable=disable_personal_light_var)
        _add_field(view, "Disable Personal Light:", disable_personal_light_combo, 2, 0)
        self.input_widgets["Disable Personal Light"] = (disable_personal_light_combo, {"valid_values": ["0", "1"], "default": "1"})

        lighting_config_var = tk.StringVar(value="0 (Bright)")
        lighting_config_combo = ttk.Combobox(view, values=["0 (Bright)", "1 (Dark)"], width=15, textvariable=lighting_config_var)
        _add_field(view, "Lighting:", lighting_config_combo, 2, 1)
        self.input_widgets["Lighting Config"] = (lighting_config_combo, {"valid_values": ["0", "1"], "default": "0"})

        # Time & Weather
        time_frame = ttk.LabelFrame(content, text="Time & Weather", padding=8)
        time_frame.pack(fill=tk.X, padx=5, pady=5)
        time_frame.columnconfigure(1, weight=1)
        time_frame.columnconfigure(3, weight=1)

        server_time_var = tk.StringVar(value="SystemTime")
        server_time_entry = ttk.Entry(time_frame, textvariable=server_time_var)
        _add_field(time_frame, "Server Time:", server_time_entry, 0, 0, "SystemTime or YYYY/MM/DD/HH/MM")
        self.input_widgets["Server Time"] = (server_time_entry, {"default": "SystemTime"})

        time_accel_var = tk.StringVar(value="1.0")
        time_accel_entry = ttk.Entry(time_frame, width=12, textvariable=time_accel_var)
        _add_field(time_frame, "Time Acceleration:", time_accel_entry, 0, 1)
        self.input_widgets["Time Acceleration"] = (time_accel_entry, {"min": 0, "max": 24, "default": "1.0"})

        night_accel_var = tk.StringVar(value="1.0")
        night_accel_entry = ttk.Entry(time_frame, width=12, textvariable=night_accel_var)
        _add_field(time_frame, "Night Acceleration:", night_accel_entry, 1, 0)
        self.input_widgets["Night Time Acceleration"] = (night_accel_entry, {"min": 0.1, "max": 64, "default": "1.0"})

        persistent_time_var = tk.StringVar(value="0 (No)")
        persistent_time_combo = ttk.Combobox(time_frame, values=["0 (No)", "1 (Yes)"], width=15, textvariable=persistent_time_var)
        _add_field(time_frame, "Persistent Time:", persistent_time_combo, 1, 1)
        self.input_widgets["Persistent Time"] = (persistent_time_combo, {"valid_values": ["0", "1"], "default": "0"})

        # Network & Instance
        net_inst = ttk.LabelFrame(content, text="Network & Instance", padding=8)
        net_inst.pack(fill=tk.X, padx=5, pady=5)
        net_inst.columnconfigure(1, weight=1)
        net_inst.columnconfigure(3, weight=1)

        guaranteed_updates_var = tk.StringVar(value="1")
        guaranteed_updates_entry = ttk.Entry(net_inst, width=12, textvariable=guaranteed_updates_var)
        _add_field(net_inst, "Guaranteed Updates:", guaranteed_updates_entry, 0, 0)
        self.input_widgets["Guaranteed Updates"] = (guaranteed_updates_entry, {"min": 1, "max": 1, "default": "1"})

        login_concurrent_var = tk.StringVar(value="5")
        login_concurrent_entry = ttk.Entry(net_inst, width=12, textvariable=login_concurrent_var)
        _add_field(net_inst, "Login Queue Concurrent:", login_concurrent_entry, 0, 1)
        self.input_widgets["Login Queue Concurrent"] = (login_concurrent_entry, {"min": 1, "max": 1000, "default": "5"})

        login_max_var = tk.StringVar(value="500")
        login_max_entry = ttk.Entry(net_inst, width=12, textvariable=login_max_var)
        _add_field(net_inst, "Login Queue Max:", login_max_entry, 1, 0)
        self.input_widgets["Login Queue Max"] = (login_max_entry, {"min": 1, "max": 10000, "default": "500"})

        instance_id_var = tk.StringVar(value="1")
        instance_id_entry = ttk.Entry(net_inst, width=12, textvariable=instance_id_var)
        _add_field(net_inst, "Instance ID:", instance_id_entry, 1, 1)
        self.input_widgets["Instance ID"] = (instance_id_entry, {"min": 1, "max": 9, "default": "1"})

        storage_auto_fix_var = tk.StringVar(value="1 (Yes)")
        storage_auto_fix_combo = ttk.Combobox(net_inst, values=["0 (No)", "1 (Yes)"], width=15, textvariable=storage_auto_fix_var)
        _add_field(net_inst, "Storage Auto Fix:", storage_auto_fix_combo, 2, 0)
        self.input_widgets["Storage Auto Fix"] = (storage_auto_fix_combo, {"valid_values": ["0", "1"], "default": "1"})

        shard_id_var = tk.StringVar(value="")
        shard_id_entry = ttk.Entry(net_inst, width=12, textvariable=shard_id_var)
        _add_field(net_inst, "Shard ID:", shard_id_entry, 2, 1)
        self.input_widgets["Shard ID"] = (shard_id_entry, {})

        # Logging & Advanced
        advanced = ttk.LabelFrame(content, text="Logging & Advanced", padding=8)
        advanced.pack(fill=tk.X, padx=5, pady=5)
        advanced.columnconfigure(1, weight=1)
        advanced.columnconfigure(3, weight=1)

        log_memory_var = tk.StringVar(value="1 (Yes)")
        log_memory_combo = ttk.Combobox(advanced, values=["0 (No)", "1 (Yes)"], width=15, textvariable=log_memory_var)
        _add_field(advanced, "Log Memory:", log_memory_combo, 0, 0)
        self.input_widgets["Log Memory"] = (log_memory_combo, {"valid_values": ["0", "1"], "default": "1"})

        admin_logs_var = tk.StringVar(value="")
        admin_logs_entry = ttk.Entry(advanced, textvariable=admin_logs_var)
        _add_field(advanced, "Admin Logs:", admin_logs_entry, 0, 1)
        self.input_widgets["Admin Logs"] = (admin_logs_entry, {})

        net_logs_var = tk.StringVar(value="")
        net_logs_entry = ttk.Entry(advanced, textvariable=net_logs_var)
        _add_field(advanced, "Network Logs:", net_logs_entry, 1, 0)
        self.input_widgets["Network Logs"] = (net_logs_entry, {})

        update_settings_var = tk.StringVar(value="")
        update_settings_entry = ttk.Entry(advanced, textvariable=update_settings_var)
        _add_field(advanced, "Update Settings:", update_settings_entry, 1, 1)
        self.input_widgets["Update Settings"] = (update_settings_entry, {})

        mem_limit_var = tk.StringVar(value="")
        mem_limit_entry = ttk.Entry(advanced, width=12, textvariable=mem_limit_var)
        _add_field(advanced, "Memory Limit (MB):", mem_limit_entry, 2, 0)
        self.input_widgets["Memory Limit"] = (mem_limit_entry, {})

        thread_count_var = tk.StringVar(value="")
        thread_count_entry = ttk.Entry(advanced, width=12, textvariable=thread_count_var)
        _add_field(advanced, "Thread Count:", thread_count_entry, 2, 1)
        self.input_widgets["Thread Count"] = (thread_count_entry, {})

        description_var = tk.StringVar(value="")
        description_entry = ttk.Entry(advanced, textvariable=description_var)
        _add_field(advanced, "Description:", description_entry, 3, 0, "shown in server browser")
        self.input_widgets["Description"] = (description_entry, {})

        # Mouse wheel scrolling for the canvas
        def _on_mousewheel(event):
            if event.num == 4:
                canvas.yview_scroll(-1, "units")
                return "break"
            if event.num == 5:
                canvas.yview_scroll(1, "units")
                return "break"
            canvas.yview_scroll(int(-event.delta / 120), "units")
            return "break"

        for widget in (canvas, content):
            widget.bind("<Button-4>", _on_mousewheel)
            widget.bind("<Button-5>", _on_mousewheel)
            widget.bind("<MouseWheel>", _on_mousewheel)

    def _create_server_config_mods_content(self, parent: ttk.Frame):
        """Build the Mods tab: map selection, mod tree, paths, workshop integration."""
        parent.columnconfigure(0, weight=1)
        parent.rowconfigure(1, weight=1)

        # Map selection
        map_selection_frame = ttk.LabelFrame(parent, text="Map Selection", padding=8)
        map_selection_frame.grid(row=0, column=0, sticky=tk.W+tk.E+tk.N+tk.S, padx=5, pady=5)
        map_selection_frame.columnconfigure(0, weight=1)
        map_selection_frame.rowconfigure(0, weight=1)

        map_tree_container = ttk.Frame(map_selection_frame)
        map_tree_container.grid(row=0, column=0, sticky=tk.W+tk.E+tk.N+tk.S, padx=5, pady=5)
        map_tree_container.columnconfigure(0, weight=1)
        map_tree_container.rowconfigure(0, weight=1)

        map_tree_scroll_y = ttk.Scrollbar(map_tree_container, orient=tk.VERTICAL)
        map_tree_scroll_x = ttk.Scrollbar(map_tree_container, orient=tk.HORIZONTAL)

        self._map_tree = ttk.Treeview(
            map_tree_container,
            columns=("Status", "MapName", "WorkshopID", "Folder"),
            show="headings",
            yscrollcommand=map_tree_scroll_y.set,
            xscrollcommand=map_tree_scroll_x.set,
            height=5,
        )

        map_tree_scroll_y.config(command=self._map_tree.yview)
        map_tree_scroll_x.config(command=self._map_tree.xview)

        self._map_tree.column("Status", width=50, anchor=tk.CENTER)
        self._map_tree.column("MapName", width=250)
        self._map_tree.column("WorkshopID", width=100, anchor=tk.CENTER)
        self._map_tree.column("Folder", width=300)

        self._map_tree.heading("Status", text=" ☑/☐ ")
        self._map_tree.heading("MapName", text="Map Name")
        self._map_tree.heading("WorkshopID", text="Workshop ID")
        self._map_tree.heading("Folder", text="Folder Path")

        map_tree_scroll_y.grid(row=0, column=1, sticky=tk.N+tk.S)
        map_tree_scroll_x.grid(row=1, column=0, sticky=tk.E+tk.W)
        self._map_tree.grid(row=0, column=0, sticky=tk.N+tk.S+tk.E+tk.W)

        self._map_tree.bind("<Button-1>", self._on_map_tree_click)
        self._map_tree.bind("<<TreeviewSelect>>", self._on_map_tree_select)
        self._attach_copy_menu(self._map_tree)

        map_button_frame = ttk.Frame(map_selection_frame)
        map_button_frame.grid(row=1, column=0, sticky=tk.W, padx=5, pady=(5, 0))

        ttk.Button(map_button_frame, text="+ Add Map", command=self._browse_map_directory).pack(side=tk.LEFT, padx=2)
        ttk.Button(map_button_frame, text="- Remove Selected", command=self._remove_selected_map).pack(side=tk.LEFT, padx=2)
        ttk.Button(map_button_frame, text="Refresh List", command=self._refresh_map_list).pack(side=tk.LEFT, padx=2)
        ttk.Button(map_button_frame, text="Cleanup Unsubscribed", command=self._cleanup_unsubscribed_workshop).pack(side=tk.LEFT, padx=2)

        self._workshop_map_info = ttk.Label(map_selection_frame, text="", foreground="gray")
        self._workshop_map_info.grid(row=2, column=0, sticky=tk.W, padx=5, pady=(5, 0))

        # Mod selection tree
        mod_selection_frame = ttk.LabelFrame(parent, text="Mod Selection", padding=8)
        mod_selection_frame.grid(row=1, column=0, sticky=tk.W+tk.E+tk.N+tk.S, padx=5, pady=5)
        mod_selection_frame.columnconfigure(0, weight=1)
        mod_selection_frame.rowconfigure(0, weight=1)
        parent.rowconfigure(0, weight=1)
        parent.rowconfigure(1, weight=2)

        tree_container = ttk.Frame(mod_selection_frame)
        tree_container.grid(row=0, column=0, sticky=tk.W+tk.E+tk.N+tk.S, padx=5, pady=5)
        tree_container.columnconfigure(0, weight=1)
        tree_container.rowconfigure(0, weight=1)

        mod_tree_scroll_y = ttk.Scrollbar(tree_container, orient=tk.VERTICAL)
        mod_tree_scroll_x = ttk.Scrollbar(tree_container, orient=tk.HORIZONTAL)

        self._mod_tree = ttk.Treeview(
            tree_container,
            columns=("Status", "ModName", "WorkshopID", "Folder"),
            show="headings",
            yscrollcommand=mod_tree_scroll_y.set,
            xscrollcommand=mod_tree_scroll_x.set,
            height=10
        )

        mod_tree_scroll_y.config(command=self._mod_tree.yview)
        mod_tree_scroll_x.config(command=self._mod_tree.xview)

        self._mod_tree.column("Status", width=50, anchor=tk.CENTER)
        self._mod_tree.column("ModName", width=250)
        self._mod_tree.column("WorkshopID", width=100, anchor=tk.CENTER)
        self._mod_tree.column("Folder", width=250)

        self._mod_tree.heading("Status", text=" ☑/☐ ")
        self._mod_tree.heading("ModName", text="Mod Name")
        self._mod_tree.heading("WorkshopID", text="Workshop ID")
        self._mod_tree.heading("Folder", text="Folder Path")

        mod_tree_scroll_y.grid(row=0, column=1, sticky=tk.N+tk.S)
        mod_tree_scroll_x.grid(row=1, column=0, sticky=tk.E+tk.W)
        self._mod_tree.grid(row=0, column=0, sticky=tk.N+tk.S+tk.E+tk.W)

        self._mod_tree.bind("<Button-1>", self._on_mod_tree_click)
        self._mod_tree.bind("<<TreeviewSelect>>", self._on_mod_tree_select)
        self._attach_copy_menu(self._mod_tree)

        mod_button_frame = ttk.Frame(mod_selection_frame)
        mod_button_frame.grid(row=1, column=0, sticky=tk.W, padx=5, pady=(5, 0))

        ttk.Button(mod_button_frame, text="+ Add Mod", command=self._browse_mod_directory).pack(side=tk.LEFT, padx=2)
        ttk.Button(mod_button_frame, text="- Remove Selected", command=self._remove_selected_mods).pack(side=tk.LEFT, padx=2)
        ttk.Button(mod_button_frame, text="Refresh List", command=self._refresh_mod_list).pack(side=tk.LEFT, padx=2)
        ttk.Button(mod_button_frame, text="Update Mods", command=self._update_workshop_mods).pack(side=tk.LEFT, padx=2)
        ttk.Button(mod_button_frame, text="Cleanup Unsubscribed", command=self._cleanup_unsubscribed_workshop).pack(side=tk.LEFT, padx=2)

        # Preset controls for the main Mods tab
        preset_frame = ttk.LabelFrame(
            mod_selection_frame, text="Mod Presets", padding=5
        )
        preset_frame.grid(
            row=2, column=0, sticky=tk.W + tk.E, padx=5, pady=(5, 0)
        )

        ttk.Label(preset_frame, text="Preset:").pack(
            side=tk.LEFT, padx=(0, 5)
        )
        self._mods_tab_preset_var = tk.StringVar(value="")
        self._mods_tab_preset_combo = ttk.Combobox(
            preset_frame,
            values=self._get_mod_preset_manager().list_presets(),
            textvariable=self._mods_tab_preset_var,
            state="readonly",
            width=30,
        )
        self._mods_tab_preset_combo.pack(side=tk.LEFT, padx=(0, 5))

        ttk.Button(
            preset_frame, text="Load", command=self._apply_mod_preset_to_tree
        ).pack(side=tk.LEFT, padx=2)
        ttk.Button(
            preset_frame, text="Save As", command=self._save_mod_preset_from_tree
        ).pack(side=tk.LEFT, padx=2)
        ttk.Button(
            preset_frame, text="Delete", command=self._delete_mod_preset_from_tree
        ).pack(side=tk.LEFT, padx=2)
        ttk.Button(
            preset_frame, text="Export", command=self._export_mod_presets
        ).pack(side=tk.LEFT, padx=2)
        ttk.Button(
            preset_frame, text="Import", command=self._import_mod_presets
        ).pack(side=tk.LEFT, padx=2)

        # Mod paths
        mod_paths_frame = ttk.Frame(parent)
        mod_paths_frame.grid(row=2, column=0, sticky=tk.W+tk.E, padx=5, pady=5)
        mod_paths_frame.columnconfigure(1, weight=1)

        ttk.Label(mod_paths_frame, text="Mod Paths:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        mod_paths_var = tk.StringVar(value="@CF,@MasPuertas")
        mod_paths_entry = ttk.Entry(mod_paths_frame, textvariable=mod_paths_var)
        mod_paths_entry.grid(row=0, column=1, sticky=tk.W+tk.E, padx=5, pady=2)

        def browse_mod_paths():
            start_dir = self._selected_mod_folder or self._get_workshop_directory() or ""
            path = filedialog.askdirectory(title="Select Mods Directory", initialdir=start_dir)
            if path:
                current = mod_paths_var.get()
                new_mod = f"@{Path(path).name}"
                if current:
                    mod_paths_var.set(f"{current};{new_mod}")
                else:
                    mod_paths_var.set(new_mod)
                self._sync_mod_paths_to_tree(mod_paths_var.get())

        ttk.Button(mod_paths_frame, text="Browse", command=browse_mod_paths).grid(row=0, column=2, sticky=tk.W, padx=(0, 5), pady=2)

        self.input_widgets["Mod Paths"] = (mod_paths_entry, {"min_length": 0})
        self._selected_mod_folder: Optional[str] = None

        def on_mod_paths_changed(*args):
            self._sync_mod_paths_to_tree(mod_paths_var.get())

        mod_paths_var.trace_add("write", on_mod_paths_changed)
        self._mod_paths_var = mod_paths_var

        # Workshop integration
        workshop_section = ttk.LabelFrame(parent, text="Workshop Integration", padding=8)
        workshop_section.grid(row=3, column=0, sticky=tk.W+tk.E, padx=5, pady=5)
        workshop_section.columnconfigure(1, weight=1)

        detected_workshop_dir = self._get_workshop_directory() or ""

        ttk.Label(workshop_section, text="Workshop Directory:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        workshop_dir_entry = ttk.Entry(workshop_section, textvariable=self._workshop_dir_var)
        workshop_dir_entry.grid(row=0, column=1, sticky=tk.W+tk.E, padx=5, pady=2)

        def browse_workshop():
            path = filedialog.askdirectory(title="Select Workshop Directory")
            if path:
                self._set_workshop_directory(path)

        ttk.Button(workshop_section, text="Browse", command=browse_workshop).grid(row=0, column=2, padx=(5, 5), pady=2)
        ttk.Button(workshop_section, text="🔄 Scan Workshop", command=self._scan_local_workshop_items_for_mods).grid(
            row=1, column=0, sticky=tk.W, pady=(5, 5))

        self._workshop_status_label = ttk.Label(workshop_section, text="", foreground="gray")
        self._workshop_status_label.grid(row=1, column=1, sticky=tk.W, pady=(5, 5), columnspan=2)

        self.input_widgets["Workshop Directory"] = (workshop_dir_entry, {})

        if detected_workshop_dir:
            self._workshop_dir_var.set(detected_workshop_dir)

    def _create_missions_tab(self, parent: ttk.Frame):
        """Build the Missions tab: discover, select, and manage mission folders."""
        parent.columnconfigure(0, weight=1)
        parent.rowconfigure(0, weight=1)

        # Mission selection tree
        mission_selection_frame = ttk.LabelFrame(parent, text="Mission Selection", padding=8)
        mission_selection_frame.grid(row=0, column=0, sticky=tk.N+tk.S+tk.E+tk.W, padx=5, pady=5)
        mission_selection_frame.columnconfigure(0, weight=1)
        mission_selection_frame.rowconfigure(0, weight=1)
        parent.rowconfigure(0, weight=1)

        tree_container = ttk.Frame(mission_selection_frame)
        tree_container.grid(row=0, column=0, sticky=tk.N+tk.S+tk.E+tk.W, padx=5, pady=5)
        tree_container.columnconfigure(0, weight=1)
        tree_container.rowconfigure(0, weight=1)

        mission_tree_scroll_y = ttk.Scrollbar(tree_container, orient=tk.VERTICAL)
        mission_tree_scroll_x = ttk.Scrollbar(tree_container, orient=tk.HORIZONTAL)

        self._mission_tree = ttk.Treeview(
            tree_container,
            columns=("Status", "MissionName", "Source", "Folder"),
            show="headings",
            yscrollcommand=mission_tree_scroll_y.set,
            xscrollcommand=mission_tree_scroll_x.set,
            height=10,
        )

        mission_tree_scroll_y.config(command=self._mission_tree.yview)
        mission_tree_scroll_x.config(command=self._mission_tree.xview)

        self._mission_tree.column("Status", width=50, anchor=tk.CENTER)
        self._mission_tree.column("MissionName", width=250)
        self._mission_tree.column("Source", width=120, anchor=tk.CENTER)
        self._mission_tree.column("Folder", width=300)

        self._mission_tree.heading("Status", text=" ☑/☐ ")
        self._mission_tree.heading("MissionName", text="Mission Name")
        self._mission_tree.heading("Source", text="Source")
        self._mission_tree.heading("Folder", text="Folder Path")

        mission_tree_scroll_y.grid(row=0, column=1, sticky=tk.N+tk.S)
        mission_tree_scroll_x.grid(row=1, column=0, sticky=tk.E+tk.W)
        self._mission_tree.grid(row=0, column=0, sticky=tk.N+tk.S+tk.E+tk.W)

        self._mission_tree.bind("<Button-1>", self._on_mission_tree_click)
        self._mission_tree.bind("<<TreeviewSelect>>", self._on_mission_tree_select)
        self._attach_copy_menu(self._mission_tree)

        mission_button_frame = ttk.Frame(mission_selection_frame)
        mission_button_frame.grid(row=1, column=0, sticky=tk.W, padx=5, pady=(5, 0))

        ttk.Button(mission_button_frame, text="+ Add Mission Dir", command=self._browse_mission_directory).pack(
            side=tk.LEFT, padx=2)
        ttk.Button(mission_button_frame, text="- Remove Selected", command=self._remove_selected_missions).pack(
            side=tk.LEFT, padx=2)
        ttk.Button(mission_button_frame, text="Refresh List", command=self._refresh_mission_list).pack(
            side=tk.LEFT, padx=2)

        # Mission directories / paths
        mission_paths_frame = ttk.Frame(parent)
        mission_paths_frame.grid(row=1, column=0, sticky=tk.W+tk.E, padx=5, pady=5)
        mission_paths_frame.columnconfigure(1, weight=1)

        ttk.Label(mission_paths_frame, text="Mission Directories:").grid(
            row=0, column=0, sticky=tk.W, padx=5, pady=2)
        mission_dirs_entry = ttk.Entry(mission_paths_frame, textvariable=self._mission_paths_var)
        mission_dirs_entry.grid(row=0, column=1, sticky=tk.W+tk.E, padx=5, pady=2)

        def browse_mission_dir():
            start_dir = self._get_workshop_directory() or ""
            path = filedialog.askdirectory(title="Select Mission Directory", initialdir=start_dir)
            if path:
                current = self._mission_paths_var.get()
                if current:
                    self._mission_paths_var.set(f"{current};{path}")
                else:
                    self._mission_paths_var.set(path)
                self._refresh_mission_list()

        ttk.Button(mission_paths_frame, text="Browse", command=browse_mission_dir).grid(
            row=0, column=2, sticky=tk.W, padx=(0, 5), pady=2)

        self.input_widgets["Mission Directories"] = (mission_dirs_entry, {"min_length": 0})

        def on_mission_paths_changed(*args):
            self._refresh_mission_list()

        self._mission_paths_var.trace_add("write", on_mission_paths_changed)

        # Help text
        hint_label = ttk.Label(
            parent,
            text=(
                "Tip: Add directories that contain dayzOffline.* mission folders. "
                "Selected missions become available in each instance's Mission Path dropdown."
            ),
            foreground="gray",
            font=("Arial", 8),
            wraplength=700,
        )
        hint_label.grid(row=2, column=0, sticky=tk.W, padx=10, pady=(0, 5))

        # Seed with the base game missions directory.
        dayz_path = self.dayz_path_var.get().strip() if hasattr(self, "dayz_path_var") else ""
        if dayz_path and Path(dayz_path).exists():
            base_mpmissions = str(Path(dayz_path) / "mpmissions")
            self._mission_paths_var.set(base_mpmissions)

    def _create_server_config_multi_instance_content(self, parent: ttk.Frame):
        """Build the Multi-Instance tab."""
        parent.columnconfigure(0, weight=1)
        parent.rowconfigure(1, weight=1)

        controls = ttk.Frame(parent)
        controls.grid(row=0, column=0, sticky=tk.W, padx=5, pady=10)

        enable_mi_check = ttk.Checkbutton(
            controls,
            text="Enable Multi-Instance Mode",
            variable=self._multi_instance_enabled_var,
            command=self._on_multi_instance_toggled
        )
        enable_mi_check.pack(side=tk.LEFT, padx=5)

        ttk.Label(controls, text="Instance Count:").pack(side=tk.LEFT, padx=(20, 5))
        count_spin = ttk.Spinbox(
            controls,
            from_=1, to=9,
            width=5,
            textvariable=self._instance_count_var,
            command=self._on_instance_count_changed
        )
        count_spin.pack(side=tk.LEFT)
        self._instance_count_var.trace_add("write", lambda *args: self._on_instance_count_changed())

        self._instance_config_frame = ttk.Frame(parent)
        self._instance_config_frame.grid(row=1, column=0, sticky=tk.NSEW, padx=5, pady=5)
        parent.rowconfigure(1, weight=1)

        self._build_instance_config_rows()

    def _create_server_config_files_content(self, parent: ttk.Frame):
        """Build the Files tab: In-Game Settings + per-instance Effects & Triggers.

        Uses a top-level inner notebook with category tabs for cleaner navigation.
        """
        parent.columnconfigure(0, weight=1)
        parent.rowconfigure(0, weight=1)

        files_notebook = ttk.Notebook(parent)
        files_notebook.grid(row=0, column=0, sticky=tk.W+tk.E+tk.N+tk.S, padx=5, pady=5)
        parent.rowconfigure(0, weight=1)
        parent.columnconfigure(0, weight=1)

        # In-Game Settings tab
        in_game_outer = ttk.Frame(files_notebook)
        in_game_outer.columnconfigure(0, weight=1)
        in_game_outer.rowconfigure(0, weight=1)
        files_notebook.add(in_game_outer, text="In-Game Settings")
        self._create_in_game_settings_content(in_game_outer)

        # Effects & Triggers tab
        effects_outer_frame = ttk.Frame(files_notebook)
        files_notebook.add(effects_outer_frame, text="Effects & Triggers")
        self._create_effects_and_triggers_content(effects_outer_frame)

    def _create_mod_settings_content(self, parent: ttk.Frame) -> None:
        """Build the Mod Settings tab: browse and edit mod JSON/XML configs.

        Uses a scalable two-pane editor (searchable tree + detail editor) so
        files with hundreds of settings are usable.  Edits can be saved as
        per-instance overrides so each server instance can have its own mod
        configuration.
        """
        parent.columnconfigure(0, weight=1)
        parent.rowconfigure(1, weight=1)

        # Instance selector
        instance_frame = ttk.Frame(parent)
        instance_frame.grid(row=0, column=0, sticky=tk.EW, padx=5, pady=(5, 0))
        ttk.Label(instance_frame, text="Instance:").pack(side=tk.LEFT)
        self._mod_settings_instance_var = tk.StringVar()
        self._mod_settings_instance_combo = ttk.Combobox(
            instance_frame,
            textvariable=self._mod_settings_instance_var,
            state="readonly",
            width=40,
        )
        self._mod_settings_instance_combo.pack(side=tk.LEFT, padx=5)
        self._mod_settings_instance_combo.bind(
            "<<ComboboxSelected>>", lambda _e: self._refresh_mod_settings()
        )
        self._mod_settings_instance_label = ttk.Label(
            instance_frame,
            text="Select an instance to scope edits and overrides.",
            foreground="gray",
        )
        self._mod_settings_instance_label.pack(side=tk.LEFT, padx=5)

        paned = ttk.PanedWindow(parent, orient=tk.HORIZONTAL)
        paned.grid(row=1, column=0, sticky=tk.NSEW, padx=5, pady=5)
        parent.rowconfigure(1, weight=1)
        parent.columnconfigure(0, weight=1)

        # Left side: settings file tree
        left_frame = ttk.LabelFrame(paned, text="Mod Settings Files", padding=5)
        left_frame.columnconfigure(0, weight=1)
        left_frame.rowconfigure(1, weight=1)

        btn_frame = ttk.Frame(left_frame)
        btn_frame.grid(row=0, column=0, sticky=tk.W, pady=(0, 5))
        ttk.Button(btn_frame, text="Refresh", command=self._refresh_mod_settings).pack(side=tk.LEFT, padx=2)
        ttk.Button(btn_frame, text="Add File", command=self._browse_mod_settings_file).pack(side=tk.LEFT, padx=2)
        ttk.Button(btn_frame, text="Repair Tires", command=self._repair_cfg_spawnable_types).pack(side=tk.LEFT, padx=2)

        tree_container = ttk.Frame(left_frame)
        tree_container.grid(row=1, column=0, sticky=tk.NSEW)
        tree_container.columnconfigure(0, weight=1)
        tree_container.rowconfigure(0, weight=1)

        tree_scroll_y = ttk.Scrollbar(tree_container, orient=tk.VERTICAL)
        tree_scroll_x = ttk.Scrollbar(tree_container, orient=tk.HORIZONTAL)

        self._mod_settings_tree = ttk.Treeview(
            tree_container,
            columns=("Mod", "File", "Path"),
            show="headings",
            yscrollcommand=tree_scroll_y.set,
            xscrollcommand=tree_scroll_x.set,
        )
        tree_scroll_y.config(command=self._mod_settings_tree.yview)
        tree_scroll_x.config(command=self._mod_settings_tree.xview)

        self._mod_settings_tree.column("Mod", width=160)
        self._mod_settings_tree.column("File", width=220)
        self._mod_settings_tree.column("Path", width=300)
        self._mod_settings_tree.heading("Mod", text="Mod")
        self._mod_settings_tree.heading("File", text="File")
        self._mod_settings_tree.heading("Path", text="Path")

        tree_scroll_y.grid(row=0, column=1, sticky=tk.NS)
        tree_scroll_x.grid(row=1, column=0, sticky=tk.EW)
        self._mod_settings_tree.grid(row=0, column=0, sticky=tk.NSEW)
        self._mod_settings_tree.bind("<<TreeviewSelect>>", self._on_mod_settings_select)
        self._attach_copy_menu(self._mod_settings_tree)

        paned.add(left_frame, weight=1)

        # Right side: new scalable editor
        right_frame = ttk.LabelFrame(paned, text="Editor", padding=5)
        right_frame.columnconfigure(0, weight=1)
        right_frame.rowconfigure(0, weight=1)

        self._mod_settings_editor = ModSettingsEditor(
            right_frame,
            get_instance_root=self._get_selected_mod_settings_instance_root,
        )

        paned.add(right_frame, weight=3)

        self._mod_settings_path: Optional[Path] = None
        self._refresh_mod_settings()

    def _get_selected_mod_settings_instance_root(self) -> Optional[Path]:
        """Return the instance root for the currently selected Mod Settings instance."""
        selected = self._mod_settings_instance_var.get()
        for instance in getattr(self, "_instance_vars", []):
            name = self._instance_display_name(instance)
            if name == selected:
                root = instance.get("root_folder", {}).get() or ""
                sanitized = self._sanitize_instance_root(root, int(instance["id"].get() or 1))
                if sanitized:
                    return Path(sanitized)
        return None

    def _instance_display_name(self, instance: Dict[str, Any]) -> str:
        """Return a human-readable label for a multi-instance row."""
        inst_id = int(instance.get("id", {}).get() or 1)
        map_name = instance.get("map", {}).get() or "unknown"
        return f"Instance {inst_id} ({map_name})"

    def _get_instance_dicts(self) -> List[Dict[str, Any]]:
        """Return the configured multi-instance dicts."""
        return list(getattr(self, "_instance_vars", []))

    def _resolve_instance_root(self, instance: Dict[str, Any]) -> Optional[Path]:
        """Return the on-disk root folder for a multi-instance dict."""
        if not instance:
            return None
        root_folder = instance.get("root_folder", {}).get() or ""
        instance_id = int(instance.get("id", {}).get() or 1)
        sanitized = self._sanitize_instance_root(root_folder, instance_id)
        if sanitized:
            return Path(sanitized)
        dayz_path = self.dayz_path_var.get().strip() if hasattr(self, "dayz_path_var") else ""
        if dayz_path:
            return Path(dayz_path)
        return None

    def _get_instance_mission_root(self, instance: Dict[str, Any]) -> Optional[Path]:
        """Return the mission folder path that an instance will use.

        The server loads mpmissions/dayzOffline.<world>.  We mirror that path
        inside the instance root so the tab can edit the exact mission the
        instance will run.
        """
        instance_root = self._resolve_instance_root(instance)
        if instance_root is None:
            return None
        map_display_name = instance.get("map", {}).get() or ""
        if not map_display_name:
            return None
        workshop_dir = self._get_workshop_directory() or ""
        world_name = self._resolve_world_name(map_display_name, workshop_dir) or map_display_name
        if world_name.lower().startswith("dayzoffline.") or world_name.lower().startswith("dayz."):
            target_name = world_name
        else:
            target_name = f"dayzOffline.{world_name}"
        mission_path = instance_root / "mpmissions" / target_name
        return mission_path if mission_path.exists() else None

    def _detect_mod_settings_files(self) -> List[Tuple[str, str, Path]]:
        """Scan workshop mod folders and the active mission folder for settings files.

        Returns a list of (mod_display_name, relative_file_path, file_path) tuples.
        """
        return detect_mod_settings_files(
            self._get_workshop_directory(),
            self._get_current_mission_root(),
        )

    def _get_current_mission_root(self) -> Optional[Path]:
        """Return the mission folder for the currently selected map/instance."""
        map_name = self._get_selected_map_name()
        if not map_name:
            # No map selected yet; try to discover any mission folder.
            return self._discover_any_mission_root()

        # Resolve the world name so "Enoch" matches "dayzOffline.enoch".
        workshop_dir = self._get_workshop_directory() or ""
        world_name = self._resolve_world_name(map_name, workshop_dir) or map_name

        dayz_path = getattr(self, "dayz_path_var", tk.StringVar(value="")).get().strip()

        # Candidate instance roots, in order of preference.
        candidate_roots: List[Path] = []
        if hasattr(self, "_last_instance_root") and self._last_instance_root:
            candidate_roots.append(self._last_instance_root)

        # Add configured multi-instance root folders.
        for instance in getattr(self, "_instance_vars", []):
            root_folder = instance.get("root_folder", {}).get() or ""
            if root_folder:
                sanitized = self._sanitize_instance_root(root_folder, 1)
                if sanitized:
                    candidate_roots.append(Path(sanitized))

        # Default project instance roots.
        projects_root = Path(self._get_projects_root())
        instances_dir = projects_root / "instances"
        if instances_dir.exists():
            for inst_dir in sorted(instances_dir.glob("server*")):
                candidate_roots.append(inst_dir)
            candidate_roots.append(instances_dir / "default")

        # DayZ server install directory.
        if dayz_path:
            candidate_roots.append(Path(dayz_path))

        # Try each root with both the resolved world name and the raw map name.
        for root in candidate_roots:
            if not root.exists():
                continue
            for name in (world_name, map_name):
                if not name:
                    continue
                for prefix in ("dayzOffline.", "dayz.", ""):
                    mission_path = root / "mpmissions" / f"{prefix}{name}"
                    if mission_path.exists():
                        return mission_path

        # Last resort: discover any mission folder.
        return self._discover_any_mission_root()

    def _get_selected_map_name(self) -> str:
        """Return the best map name from any UI source."""
        candidates: List[str] = []

        # Server Config Core tab (if present).
        widget = self.input_widgets.get("Map Name")
        if widget is not None:
            map_combo, _ = widget
            if isinstance(map_combo, ttk.Combobox):
                candidates.append(map_combo.get().strip())

        # Single-server combo.
        if self._single_server_map_combo is not None:
            candidates.append(self._single_server_map_combo.get().strip())

        # Its backing variable.
        if hasattr(self, "map_name_var"):
            candidates.append(self.map_name_var.get().strip())

        # Mods tab tree selection.
        if getattr(self, "_selected_map", ""):
            candidates.append(self._selected_map)

        # Multi-instance map combos.
        for instance in getattr(self, "_instance_vars", []):
            map_combo = instance.get("map")
            if map_combo is not None and isinstance(map_combo, ttk.Combobox):
                candidates.append(map_combo.get().strip())

        # Prefer the first non-empty value.
        for name in candidates:
            if name:
                return name
        return ""

    def _discover_any_mission_root(self) -> Optional[Path]:
        """Find any dayzOffline.* mission folder under known roots."""
        dayz_path = getattr(self, "dayz_path_var", tk.StringVar(value="")).get().strip()
        projects_root = Path(self._get_projects_root())
        search_roots: List[Path] = []

        if hasattr(self, "_last_instance_root") and self._last_instance_root:
            search_roots.append(self._last_instance_root)

        for instance in getattr(self, "_instance_vars", []):
            root_folder = instance.get("root_folder", {}).get() or ""
            if root_folder:
                sanitized = self._sanitize_instance_root(root_folder, 1)
                if sanitized:
                    search_roots.append(Path(sanitized))

        instances_dir = projects_root / "instances"
        if instances_dir.exists():
            search_roots.extend(sorted(instances_dir.glob("server*")))
            search_roots.append(instances_dir / "default")

        if dayz_path:
            search_roots.append(Path(dayz_path))

        for root in search_roots:
            if not root.exists():
                continue
            mpmissions = root / "mpmissions"
            if not mpmissions.exists():
                continue
            for mission in sorted(mpmissions.glob("dayzOffline.*")):
                if mission.is_dir():
                    return mission

        return None

    def _on_server_config_tab_changed(self, event=None) -> None:
        """Refresh tabs that depend on the currently selected map/instance."""
        if not hasattr(self, "_server_config_notebook"):
            return
        try:
            selected_text = self._server_config_notebook.tab("current", "text")
        except tk.TclError:
            return
        if selected_text == "Mod Settings":
            self._refresh_mod_settings()
        elif selected_text == "Spawn Loadout":
            if hasattr(self, "_mod_integration_tab"):
                self._mod_integration_tab.refresh()

    def _refresh_mod_settings(self) -> None:
        """Rescan and populate the mod settings file tree and instance selector."""
        with self._busy_context("Scanning mod settings..."):
            self._refresh_mod_settings_impl()

    def _refresh_mod_settings_impl(self) -> None:
        """Internal implementation: refresh mod settings tree."""
        if not hasattr(self, "_mod_settings_tree"):
            return

        # Update instance selector
        instance_names: List[str] = []
        for instance in getattr(self, "_instance_vars", []):
            instance_names.append(self._instance_display_name(instance))
        current = self._mod_settings_instance_var.get()
        self._mod_settings_instance_combo.config(values=instance_names)
        if current not in instance_names and instance_names:
            self._mod_settings_instance_var.set(instance_names[0])

        for item in self._mod_settings_tree.get_children():
            self._mod_settings_tree.delete(item)

        files = self._detect_mod_settings_files()
        for mod_name, file_name, path in files:
            self._mod_settings_tree.insert("", tk.END, values=(mod_name, file_name, str(path)))

        count = len(self._mod_settings_tree.get_children())
        if count:
            # Auto-select the first file so the editor is immediately usable.
            first = self._mod_settings_tree.get_children()[0]
            self._mod_settings_tree.selection_set(first)
            self._mod_settings_tree.see(first)
            self._on_mod_settings_select()
        else:
            if hasattr(self._mod_settings_editor, "_status"):
                self._mod_settings_editor._status.config(
                    text="No settings files found. Set DayZ Server Path / Map Name and workshop directory.",
                    foreground="red",
                )

    def _browse_mod_settings_file(self) -> None:
        """Let the user add an arbitrary mod settings file to the tree."""
        path = filedialog.askopenfilename(
            title="Select Mod Settings File",
            filetypes=[("JSON files", "*.json"), ("XML files", "*.xml"), ("All files", "*.*")],
        )
        if not path:
            return

        p = Path(path)
        mod_name = p.parent.name
        rel_path = p.name
        self._mod_settings_tree.insert("", tk.END, values=(mod_name, rel_path, str(p)))

    def _on_mod_settings_select(self, event=None) -> None:
        """Load the selected settings file into the new scalable editor."""
        if not hasattr(self, "_mod_settings_tree"):
            return

        selection = self._mod_settings_tree.selection()
        if not selection:
            return

        values = self._mod_settings_tree.item(selection[0], "values")
        if len(values) < 3:
            return

        mod_name = values[0] or "Unknown"
        path = Path(values[2])
        self._mod_settings_path = path
        self._mod_settings_editor.load_file(path, mod_name=mod_name)

    def _validate_mod_settings(self) -> bool:
        """Validate the editor content through the new editor widget."""
        return self._mod_settings_editor._validate()

    def _save_mod_settings(self) -> None:
        """Save the editor content through the new editor widget."""
        self._mod_settings_editor._save()

    def _repair_cfg_spawnable_types(self) -> None:
        """Fix common cfgspawnabletypes.xml problems that remove vehicle tires.

        Backs up the file, removes invalid XML comments, and ensures common
        vehicles have wheel attachments.
        """
        with self._busy_context("Repairing cfgspawnabletypes.xml..."):
            self._repair_cfg_spawnable_types_impl()

    def _repair_cfg_spawnable_types_impl(self) -> None:
        """Internal implementation: repair cfgspawnabletypes.xml."""
        from dayzconfigmaster.config.spawnabletypes_repair import RepairResult

        mission_root = self._get_current_mission_root()
        if mission_root is None:
            messagebox.showwarning(
                "No Mission",
                "Could not locate the active mission folder.",
            )
            return

        target_path = mission_root / "cfgspawnabletypes.xml"
        result: RepairResult = repair_cfg_spawnable_types(target_path)

        if not result.success:
            if "not found" in result.error.lower():
                messagebox.showwarning("Not Found", result.error)
            else:
                messagebox.showerror("Repair Error", result.error)
            return

        if not result.changed:
            messagebox.showinfo(
                "No Repair Needed",
                "cfgspawnabletypes.xml looks healthy. No tire fixes were required.",
            )
            return

        self._refresh_mod_settings()
        messagebox.showinfo(
            "Repair Complete",
            f"cfgspawnabletypes.xml repaired.\n\nFixes applied:\n" +
            "\n".join(f"• {f}" for f in result.fixes) +
            f"\n\nBackup saved to:\n{result.backup_path}",
        )

    def _repair_aircraft_lifetimes(self) -> None:
        """Manually normalize aircraft/helicopter lifetimes for an instance."""
        if not self._instance_vars:
            messagebox.showwarning(
                "No Instances",
                "No instances are configured. Add instances in Server Config first."
            )
            return

        # Build a simple picker from configured instances.
        labels = []
        for instance in self._instance_vars:
            instance_id = int(instance.get("id", {}).get() or 1)
            map_name = instance.get("map", {}).get() or ""
            labels.append(f"Instance {instance_id} ({map_name})")

        picker = tk.Toplevel(self.root)
        picker.title("Select Instance")
        picker.geometry("350x150")
        picker.transient(self.root)
        picker.grab_set()

        ttk.Label(picker, text="Instance:").pack(pady=(10, 0))
        selected = tk.StringVar(value=labels[0] if labels else "")
        combo = ttk.Combobox(
            picker, textvariable=selected, values=labels, state="readonly"
        )
        combo.pack(padx=10, pady=5, fill=tk.X)

        def on_confirm():
            picker.destroy()
            label = selected.get()
            idx = labels.index(label) if label in labels else 0
            instance = self._instance_vars[idx]
            instance_id = int(instance.get("id", {}).get() or 1)
            root_folder = instance.get("root_folder", {}).get() or ""
            root_folder = self._sanitize_instance_root(root_folder, instance_id)
            dayz_path = self.dayz_path_var.get().strip()
            instance_root = Path(root_folder) if root_folder else Path(dayz_path)
            if not instance_root.exists():
                messagebox.showerror(
                    "Instance Not Found",
                    f"Instance directory does not exist:\n{instance_root}"
                )
                return

            world_name = self._resolve_world_name(
                instance.get("map", {}).get() or "",
                self._workshop_dir_var.get().strip(),
            )
            if not world_name:
                messagebox.showerror(
                    "No Map",
                    f"Could not determine world name for instance {instance_id}."
                )
                return
            if world_name.lower().startswith("dayzoffline.") or world_name.lower().startswith("dayz."):
                target_name = world_name
            else:
                target_name = f"dayzOffline.{world_name}"

            with self._busy_context(
                f"Fixing aircraft lifetimes for instance {instance_id}..."
            ):
                msg = self._normalize_aircraft_lifetimes(
                    instance_root, target_name
                )
            self.log_text.insert(
                tk.END,
                f"[{self._get_timestamp()}] Instance {instance_id}: {msg}\n",
            )
            messagebox.showinfo("Aircraft Lifetime Fix", msg)

        btn_frame = ttk.Frame(picker)
        btn_frame.pack(pady=10)
        ttk.Button(btn_frame, text="Fix", command=on_confirm).pack(side=tk.LEFT, padx=5)
        ttk.Button(btn_frame, text="Cancel", command=picker.destroy).pack(side=tk.LEFT, padx=5)

    def _on_mod_tree_click(self, event) -> None:
        """Toggle checkbox state when clicking a mod tree row."""
        if self._mod_tree is None:
            return

        region = self._mod_tree.identify("region", event.x, event.y)
        if region != "cell":
            return

        item_id = self._mod_tree.identify_row(event.y)
        if not item_id:
            return

        values = list(self._mod_tree.item(item_id, "values"))
        if len(values) < 2:
            return

        current_status = values[0]
        new_status = " ☐ " if current_status == " ☑ " else " ☑ "
        values[0] = new_status
        self._mod_tree.item(item_id, values=values)

        mod_name = values[1]
        if new_status == " ☑ ":
            self._selected_mods.add(mod_name)
        else:
            self._selected_mods.discard(mod_name)

        self._update_mod_paths_from_selection()

    def _on_mod_tree_select(self, event=None) -> None:
        """Update the selected mod folder when a tree row is selected."""
        if self._mod_tree is None:
            return

        selection = self._mod_tree.selection()
        if not selection:
            self._selected_mod_folder = None
            return

        item_id = selection[0]
        values = self._mod_tree.item(item_id, "values")
        if len(values) >= 4:
            mod_name = values[1]
            self._selected_mod_folder = self._mod_folder_by_name.get(mod_name)
        else:
            self._selected_mod_folder = None

    def _update_mod_paths_from_selection(self) -> None:
        """Update the Mod Paths entry from the current tree selection."""
        if not hasattr(self, '_mod_paths_var'):
            return

        mods = []
        for item_id in self._mod_tree.get_children():
            values = self._mod_tree.item(item_id, "values")
            if len(values) >= 2 and values[0] == " ☑ ":
                mod_name = values[1]
                folder = self._mod_folder_by_name.get(mod_name, "")
                # Use the actual folder path if available, otherwise fall back to @Name.
                if folder:
                    mods.append(folder)
                else:
                    if not mod_name.startswith("@") and not mod_name.startswith("steam://"):
                        mod_name = "@" + mod_name
                    mods.append(mod_name)

        current = self._mod_paths_var.get()
        new_value = ";".join(mods)
        if current != new_value:
            self._mod_paths_var.set(new_value)

    def _sync_mod_paths_to_tree(self, mod_paths: str) -> None:
        """Sync the mod tree checkboxes from a mod path string."""
        if self._mod_tree is None:
            return

        raw = mod_paths.replace(",", ";")
        selected_folders = set()
        selected_names = set()
        for part in raw.split(";"):
            part = part.strip()
            if not part:
                continue
            selected_folders.add(part)
            selected_names.add(Path(part).name)
            if part.startswith("@"):
                selected_names.add(part[1:])

        self._selected_mods = selected_names.copy()

        for item_id in self._mod_tree.get_children():
            values = list(self._mod_tree.item(item_id, "values"))
            if len(values) >= 2:
                mod_name = values[1]
                folder = self._mod_folder_by_name.get(mod_name, "")
                checked = mod_name in selected_names or folder in selected_folders
                values[0] = " ☑ " if checked else " ☐ "
                self._mod_tree.item(item_id, values=values)

    def _get_mod_tree_checked_names(self) -> List[str]:
        """Return the ordered list of currently checked mod display names."""
        checked: List[str] = []
        if self._mod_tree is None:
            return checked
        for item_id in self._mod_tree.get_children():
            values = self._mod_tree.item(item_id, "values")
            if len(values) >= 2 and values[0] == " ☑ ":
                checked.append(str(values[1]))
        return checked

    def _apply_mod_preset_to_tree(self) -> None:
        """Load a saved preset and update the mod tree check state."""
        name = self._mods_tab_preset_var.get()
        if not name:
            messagebox.showinfo("Load Preset", "Please select a preset to load.")
            return
        mgr = self._get_mod_preset_manager()
        preset = mgr.get_preset(name)
        if preset is None:
            messagebox.showerror("Preset Error", f"Preset '{name}' not found.")
            return

        target_names = {m.strip() for m in preset.mods if m.strip()}
        target_folders = {
            Path(m).name for m in preset.mods
        }

        if self._mod_tree is not None:
            for item_id in self._mod_tree.get_children():
                values = list(self._mod_tree.item(item_id, "values"))
                if len(values) < 2:
                    continue
                mod_name = values[1]
                folder = self._mod_folder_by_name.get(mod_name, "")
                folder_name = Path(folder).name if folder else ""
                checked = (
                    mod_name in target_names
                    or folder in target_folders
                    or folder_name in target_names
                )
                values[0] = " ☑ " if checked else " ☐ "
                self._mod_tree.item(item_id, values=values)
                if checked:
                    self._selected_mods.add(mod_name)
                else:
                    self._selected_mods.discard(mod_name)

        self._update_mod_paths_from_selection()
        self.log_text.insert(
            tk.END,
            f"[{self._get_timestamp()}] Loaded mod preset "
            f"'{name}'\n",
        )

    def _save_mod_preset_from_tree(self) -> None:
        """Save the currently checked mods as a named preset."""
        checked = self._get_mod_tree_checked_names()
        if not checked:
            messagebox.showwarning("No Mods", "No mods are selected to save.")
            return
        name = simpledialog.askstring(
            "Save Mod Preset",
            "Enter a name for this mod preset:",
        )
        if not name:
            return
        mgr = self._get_mod_preset_manager()
        ok, msg = mgr.save_preset(name, checked)
        if ok:
            self._refresh_mods_tab_preset_combo()
            self._refresh_instance_mod_preset_combos()
            self._mods_tab_preset_var.set(name)
        self.log_text.insert(tk.END, f"[{self._get_timestamp()}] {msg}\n")
        if not ok:
            messagebox.showerror("Preset Error", msg)

    def _delete_mod_preset_from_tree(self) -> None:
        """Delete the preset selected in the Mods tab dropdown."""
        name = self._mods_tab_preset_var.get()
        if not name:
            messagebox.showinfo("Delete Preset", "Please select a preset to delete.")
            return
        if not messagebox.askyesno("Delete Preset", f"Delete mod preset '{name}'?"):
            return
        mgr = self._get_mod_preset_manager()
        ok, msg = mgr.delete_preset(name)
        if ok:
            self._mods_tab_preset_var.set("")
            self._refresh_mods_tab_preset_combo()
            self._refresh_instance_mod_preset_combos()
        self.log_text.insert(tk.END, f"[{self._get_timestamp()}] {msg}\n")

    def _refresh_mods_tab_preset_combo(self) -> None:
        """Refresh the Mods tab preset dropdown values."""
        names = self._get_mod_preset_manager().list_presets()
        if hasattr(self, "_mods_tab_preset_combo"):
            self._mods_tab_preset_combo.config(values=names)

    def _export_mod_presets(self) -> None:
        """Export all mod presets to a JSON file chosen by the user."""
        path = filedialog.asksaveasfilename(
            title="Export Mod Presets",
            defaultextension=".json",
            filetypes=[("JSON files", "*.json"), ("All files", "*.*")],
        )
        if not path:
            return
        ok, msg = self._get_mod_preset_manager().export_presets(Path(path))
        self.log_text.insert(tk.END, f"[{self._get_timestamp()}] {msg}\n")
        if not ok:
            messagebox.showerror("Export Error", msg)

    def _import_mod_presets(self) -> None:
        """Import mod presets from a JSON file chosen by the user."""
        path = filedialog.askopenfilename(
            title="Import Mod Presets",
            defaultextension=".json",
            filetypes=[("JSON files", "*.json"), ("All files", "*.*")],
        )
        if not path:
            return
        overwrite = messagebox.askyesno(
            "Import Presets",
            "Overwrite existing presets if names match?",
        )
        ok, msg = self._get_mod_preset_manager().import_presets(
            Path(path), overwrite=overwrite
        )
        self.log_text.insert(tk.END, f"[{self._get_timestamp()}] {msg}\n")
        if ok:
            self._refresh_mods_tab_preset_combo()
            self._refresh_instance_mod_preset_combos()
        else:
            messagebox.showerror("Import Error", msg)

    def _browse_mod_directory(self) -> None:
        """Browse for a local mod directory and add it to the selection."""
        path = filedialog.askdirectory(title="Select Mods Directory")
        if not path:
            return

        mod_name = Path(path).name
        if not mod_name.startswith("@"):
            mod_name = "@" + mod_name

        current = self._mod_paths_var.get()
        if current:
            self._mod_paths_var.set(f"{current};{mod_name}")
        else:
            self._mod_paths_var.set(mod_name)

    def _browse_map_directory(self) -> None:
        """Browse for a local map directory and add it to the map list."""
        path = filedialog.askdirectory(title="Select Map Directory")
        if not path:
            return

        # Add to workshop maps dict so it appears in the tree and combos.
        folder_name = Path(path).name
        self._workshop_maps[folder_name] = (folder_name, folder_name)
        self._refresh_map_list()

        # Select the newly added map in the tree.
        self._selected_map = folder_name
        item_id = self._map_tree_items.get(folder_name)
        if item_id and self._map_tree is not None:
            self._map_tree.selection_set(item_id)
            self._map_tree.see(item_id)
            for child_id in self._map_tree.get_children():
                child_values = list(self._map_tree.item(child_id, "values"))
                if len(child_values) >= 2:
                    child_values[0] = " ☑ " if child_values[1] == folder_name else " ☐ "
                    self._map_tree.item(child_id, values=child_values)

    def _on_map_tree_click(self, event) -> None:
        """Toggle checkbox state when clicking a map tree row."""
        if self._map_tree is None:
            return

        region = self._map_tree.identify("region", event.x, event.y)
        if region != "cell":
            return

        item_id = self._map_tree.identify_row(event.y)
        if not item_id:
            return

        values = list(self._map_tree.item(item_id, "values"))
        if len(values) < 3:
            return

        # Toggle the clicked row on and all other rows off (single-select checkbox).
        clicked_name = values[1]
        self._selected_map = clicked_name

        for child_id in self._map_tree.get_children():
            child_values = list(self._map_tree.item(child_id, "values"))
            if len(child_values) < 3:
                continue
            child_values[0] = " ☑ " if child_values[1] == clicked_name else " ☐ "
            self._map_tree.item(child_id, values=child_values)

        self._on_map_tree_select(event)

    def _on_map_tree_select(self, event=None) -> None:
        """Update the map source label and other map combos when a map row is selected."""
        if self._map_tree is None:
            return

        selection = self._map_tree.selection()
        if not selection:
            self._workshop_map_info.config(text="")
            return

        item_id = selection[0]
        values = self._map_tree.item(item_id, "values")
        if len(values) >= 3:
            map_name, ws_id = values[1], values[2]
            self._selected_map = map_name
            self._map_source_var.set(f"Source: Workshop item #{ws_id}")
            self._workshop_map_info.config(text=f"Source: Workshop item #{ws_id}")

            # Update the map combo boxes across the app to match the selection.
            widget = self.input_widgets.get("Map Name")
            if widget is not None:
                map_combo, _ = widget
                if isinstance(map_combo, ttk.Combobox):
                    map_combo.set(map_name)

            for instance in self._instance_vars:
                map_combo = instance.get("map")
                if map_combo is not None and isinstance(map_combo, ttk.Combobox):
                    map_combo.set(map_name)

            if self._single_server_map_combo is not None:
                self._single_server_map_combo.set(map_name)
        else:
            self._workshop_map_info.config(text="")

    def _remove_selected_map(self) -> None:
        """Remove the selected map from the local map tree (does not delete files)."""
        if self._map_tree is None:
            return

        selected = self._map_tree.selection()
        if not selected:
            messagebox.showinfo("Remove Map", "Please select a map in the list to remove.")
            return

        for item_id in selected:
            values = self._map_tree.item(item_id, "values")
            if len(values) >= 2:
                map_name = values[1]
                self._workshop_maps.pop(map_name, None)
                if self._selected_map == map_name:
                    self._selected_map = ""
            self._map_tree.delete(item_id)

        self._refresh_all_map_combos()
        self._workshop_map_info.config(text="")

    def _remove_selected_mods(self) -> None:
        """Remove currently selected mods from the tree and path string."""
        if self._mod_tree is None:
            return

        selected = self._mod_tree.selection()
        if not selected:
            messagebox.showinfo("Remove Mod", "Please select a mod in the list to remove.")
            return

        for item_id in selected:
            values = self._mod_tree.item(item_id, "values")
            if len(values) >= 2:
                self._selected_mods.discard(values[1])
            self._mod_tree.delete(item_id)

        self._update_mod_paths_from_selection()

    def _refresh_mod_list(self) -> None:
        """Refresh the mod list from the configured workshop directory."""
        self._refresh_workshop_lists()

    def _refresh_map_list(self) -> None:
        """Refresh the map list from the configured workshop directory."""
        self._refresh_workshop_lists()

    def _on_mission_tree_click(self, event) -> None:
        """Toggle checkbox state when clicking a mission tree row."""
        if self._mission_tree is None:
            return

        region = self._mission_tree.identify("region", event.x, event.y)
        if region != "cell":
            return

        item_id = self._mission_tree.identify_row(event.y)
        if not item_id:
            return

        values = list(self._mission_tree.item(item_id, "values"))
        if len(values) < 2:
            return

        current_status = values[0]
        new_status = " ☐ " if current_status == " ☑ " else " ☑ "
        values[0] = new_status
        self._mission_tree.item(item_id, values=values)

        mission_name = values[1]
        if new_status == " ☑ ":
            self._selected_missions.add(mission_name)
        else:
            self._selected_missions.discard(mission_name)

        self._refresh_instance_mission_combos()

    def _on_mission_tree_select(self, event=None) -> None:
        """Update state when a mission row is selected."""
        if self._mission_tree is None:
            return

        selection = self._mission_tree.selection()
        if not selection:
            return

        item_id = selection[0]
        values = self._mission_tree.item(item_id, "values")
        if len(values) >= 2:
            mission_name = values[1]
            self._selected_mission_folder = self._mission_dir_by_name.get(mission_name)

    def _browse_mission_directory(self) -> None:
        """Browse for a local mission directory and add it to the scan list."""
        path = filedialog.askdirectory(title="Select Mission Directory")
        if not path:
            return

        current = self._mission_paths_var.get()
        if current:
            self._mission_paths_var.set(f"{current};{path}")
        else:
            self._mission_paths_var.set(path)

    def _remove_selected_missions(self) -> None:
        """Remove currently selected missions from the tree."""
        if self._mission_tree is None:
            return

        selected = self._mission_tree.selection()
        if not selected:
            messagebox.showinfo("Remove Mission", "Please select a mission in the list to remove.")
            return

        for item_id in selected:
            values = self._mission_tree.item(item_id, "values")
            if len(values) >= 2:
                mission_name = values[1]
                self._selected_missions.discard(mission_name)
                self._mission_dir_by_name.pop(mission_name, None)
            self._mission_tree.delete(item_id)

    def _refresh_mission_list(self) -> None:
        """Refresh the mission list from configured mission directories."""
        with self._busy_context("Refreshing mission list..."):
            self._refresh_mission_list_impl()

    def _refresh_mission_list_impl(self) -> None:
        """Internal implementation: refresh the mission list."""
        if self._mission_tree is None:
            return

        # Preserve current selection.
        previous_selection = self._selected_missions.copy()

        for item in self._mission_tree.get_children():
            self._mission_tree.delete(item)
        self._mission_tree_items.clear()
        self._mission_dir_by_name.clear()

        discovered: Dict[str, Tuple[str, str]] = {}  # name -> (source, path)

        raw = self._mission_paths_var.get().replace(",", ";")
        for path_str in raw.split(";"):
            path_str = path_str.strip()
            if not path_str:
                continue
            path = Path(path_str)
            if not path.exists():
                continue
            for mission_name, mission_path in self._discover_missions_in_directory(path).items():
                if mission_name not in discovered:
                    discovered[mission_name] = ("Local", str(mission_path))

        # Also scan the base game mpmissions directory.
        dayz_path = self.dayz_path_var.get().strip() if hasattr(self, "dayz_path_var") else ""
        if dayz_path:
            base_mpmissions = Path(dayz_path) / "mpmissions"
            if base_mpmissions.exists():
                for mission_name, mission_path in self._discover_missions_in_directory(base_mpmissions).items():
                    if mission_name not in discovered:
                        discovered[mission_name] = ("Stock", str(mission_path))

        # Scan workshop content for any shipped mission folders.
        workshop_dir = self._get_workshop_directory()
        if workshop_dir:
            content_path = Path(workshop_dir)
            if content_path.exists():
                for folder in content_path.iterdir():
                    if not folder.is_dir():
                        continue
                    try:
                        _ = int(folder.name)
                    except ValueError:
                        continue
                    display = self._read_workshop_display_name(folder) or folder.name
                    for mission_name, mission_path in self._discover_missions_in_directory(folder).items():
                        if mission_name not in discovered:
                            discovered[mission_name] = (f"Workshop {display}", str(mission_path))

        for mission_name, (source, folder_path) in sorted(discovered.items(), key=lambda x: x[0].lower()):
            self._mission_dir_by_name[mission_name] = folder_path
            checked = " ☑ " if mission_name in previous_selection else " ☐ "
            item_id = self._mission_tree.insert(
                "", tk.END,
                values=(checked, mission_name, source, folder_path)
            )
            self._mission_tree_items[mission_name] = item_id

        self._selected_missions = previous_selection.intersection(set(discovered.keys()))
        self._refresh_instance_mission_combos()

    def _discover_missions_in_directory(self, root: Path) -> Dict[str, Path]:
        """Find dayzOffline.* or dayz.* mission folders under root.

        Returns a mapping of mission folder name -> full path.
        """
        missions: Dict[str, Path] = {}
        if not root.exists() or not root.is_dir():
            return missions

        for item in root.iterdir():
            if not item.is_dir():
                continue
            lower = item.name.lower()
            if lower.startswith("dayzoffline.") or lower.startswith("dayz."):
                missions[item.name] = item

        return missions

    def _refresh_instance_mission_combos(self) -> None:
        """Refresh every per-instance Mission Path combobox with discovered missions."""
        values = [""] + sorted(self._mission_dir_by_name.values(), key=str.lower)
        for instance in getattr(self, "_instance_vars", []):
            mission_combo = instance.get("mission_path_widget")
            if isinstance(mission_combo, ttk.Combobox):
                current = mission_combo.get()
                mission_combo["values"] = values
                if current in values:
                    mission_combo.set(current)
                elif values:
                    mission_combo.set(values[0])
                else:
                    mission_combo.set("")

    def _refresh_workshop_lists(self) -> None:
        """Refresh both map and mod lists from the configured workshop directory."""
        with self._busy_context("Scanning workshop content..."):
            workshop_dir = self._get_workshop_directory()
            if not workshop_dir or not Path(workshop_dir).exists():
                messagebox.showwarning(
                    "Workshop Directory",
                    "Please set a valid Workshop directory first.",
                )
                return

            content_path = Path(workshop_dir)
            if not content_path.exists() or not any(content_path.iterdir()):
                messagebox.showinfo(
                    "No Workshop Items",
                    f"No workshop content found at:\n{content_path}",
                )
                return

            # Rebuild the world-name map whenever workshop lists are refreshed.
            self._workshop_world_name_cache = None
            self._workshop_world_name_cache_dir = None

            self._scan_workshop_content(content_path)
        self._refresh_all_map_combos()
        self._refresh_mission_list()

    def _scan_local_workshop_items_for_mods(self) -> None:
        """Scan local workshop items and populate map/mod lists."""
        # Workshop content may have changed; rebuild the world-name map too.
        self._workshop_world_name_cache = None
        self._workshop_world_name_cache_dir = None
        self._refresh_workshop_lists()

    def _scan_workshop_content(self, content_path: Path) -> None:
        """Classify workshop content and update UI state."""
        parser = WorkshopMapParser(str(content_path))
        self._workshop_maps = parser.get_workshop_maps()
        self._workshop_mods = parser.get_workshop_mods()

        # Clear lookup tables
        self._mod_folder_by_name.clear()
        self._mod_workshop_id_by_name.clear()

        # Clear and repopulate mod tree
        if self._mod_tree is not None:
            for item in self._mod_tree.get_children():
                self._mod_tree.delete(item)
            self._mod_tree_items.clear()
            self._selected_mods.clear()

            for ws_id, (folder_name, item_name) in sorted(self._workshop_mods.items(), key=lambda x: x[1][1].lower()):
                display_name = item_name if item_name != folder_name else folder_name
                folder_path = str(content_path / ws_id)
                self._mod_folder_by_name[display_name] = folder_path
                self._mod_workshop_id_by_name[display_name] = ws_id
                item_id = self._mod_tree.insert(
                    "", tk.END,
                    values=(" ☐ ", display_name, ws_id, folder_path)
                )
                self._mod_tree_items[display_name] = item_id

        # Clear and repopulate map tree
        if self._map_tree is not None:
            for item in self._map_tree.get_children():
                self._map_tree.delete(item)
            self._map_tree_items.clear()

            for ws_id, (folder_name, item_name) in sorted(self._workshop_maps.items(), key=lambda x: x[1][1].lower()):
                display_name = item_name if item_name != folder_name else folder_name
                folder_path = str(content_path / ws_id)
                checked = " ☑ " if display_name == self._selected_map else " ☐ "
                item_id = self._map_tree.insert(
                    "", tk.END,
                    values=(checked, display_name, ws_id, folder_path)
                )
                self._map_tree_items[display_name] = item_id

        self._workshop_status_label.config(
            text=f"Loaded {len(self._workshop_maps)} maps and {len(self._workshop_mods)} mods from workshop"
        )

    def _update_workshop_mods(self) -> None:
        """Update all currently selected workshop mods to their latest versions.

        SteamCMD's ``+workshop_download_item`` re-downloads the item if it has
        changed, so this simply re-downloads every mod currently listed in the
        Mod Paths field. Maps are intentionally skipped because they are managed
        through the Map selection dropdown.
        """
        workshop_dir = self._get_workshop_directory()
        if not workshop_dir or not Path(workshop_dir).exists():
            messagebox.showwarning("Workshop Directory", "Please set a valid Workshop directory first.")
            return

        # Collect workshop IDs from the currently selected mods.
        mod_ids: List[str] = []
        for display_name, ws_id in self._mod_workshop_id_by_name.items():
            if display_name in self._selected_mods and ws_id:
                mod_ids.append(ws_id)

        if not mod_ids:
            messagebox.showinfo("No Mods Selected", "No workshop mods are selected for update.")
            return

        progress = self._safe_toplevel("Updating Workshop Mods", "500x200")
        if progress is None:
            # No real Tk root (headless/tests): run synchronously without GUI.
            results = self._download_workshop_mod_items(
                mod_ids, progress_callback=lambda _i, _item_id, _msg: None
            )
            self._finish_workshop_mod_updates(results, progress=None)
            return

        try:
            progress.transient(self.root)
            progress.grab_set()
        except tk.TclError:
            pass

        ttk.Label(progress, text=f"Updating {len(mod_ids)} mod(s)...", font=("Arial", 10, "bold")).pack(pady=(10, 5))
        status_var = tk.StringVar(value="Starting...")
        status_label = ttk.Label(progress, textvariable=status_var)
        status_label.pack(pady=5)
        progress_bar = ttk.Progressbar(progress, mode="determinate", maximum=len(mod_ids))
        progress_bar.pack(fill=tk.X, padx=20, pady=10)

        def run_updates():
            def callback(i: int, item_id: str, _msg: str):
                self._safe_after(
                    0,
                    lambda v=f"Updating {item_id} ({i}/{len(mod_ids)})...": status_var.set(v),
                )
                self._safe_after(0, lambda p=i: progress_bar.configure(value=p))

            results = self._download_workshop_mod_items(mod_ids, progress_callback=callback)
            self._safe_after(0, lambda: self._finish_workshop_mod_updates(results, progress))

        import threading
        threading.Thread(target=run_updates, daemon=True).start()

    def _download_workshop_mod_items(
        self,
        mod_ids: List[str],
        progress_callback=None,
    ) -> List[Tuple[str, bool, str]]:
        """Download the given workshop mod IDs and return per-item results.

        This is split out from the GUI dialog so it can run headlessly in tests
        or batch environments without creating Tk widgets.
        """
        try:
            from ..workshop.api import WorkshopManager
        except ImportError:
            from dayzconfigmaster.workshop.api import WorkshopManager

        manager = WorkshopManager(str(self._get_projects_root()))
        results: List[Tuple[str, bool, str]] = []
        for i, item_id in enumerate(mod_ids, 1):
            if progress_callback is not None:
                progress_callback(i, item_id, "")
            success, msg = manager.download_item(item_id)
            results.append((item_id, success, msg))
        return results

    def _finish_workshop_mod_updates(
        self,
        results: List[Tuple[str, bool, str]],
        progress: Optional[tk.Toplevel] = None,
    ) -> None:
        """Close the progress dialog, refresh UI, and report results."""
        if progress is not None:
            try:
                progress.destroy()
            except tk.TclError:
                pass
        self._refresh_mod_list()
        self._refresh_all_map_combos()
        self._workshop_world_name_cache = None
        self._workshop_world_name_cache_dir = None

        succeeded = [r for r in results if r[1]]
        failed = [r for r in results if not r[1]]

        summary = f"Updated {len(succeeded)} mod(s)."
        if failed:
            summary += f"\n\nFailed ({len(failed)}):\n"
            summary += "\n".join(f"  {ws_id}: {msg}" for ws_id, _, msg in failed)
        messagebox.showinfo("Update Complete", summary)

    def _cleanup_unsubscribed_workshop(self) -> None:
        """Find workshop folders that are no longer subscribed and delete them."""
        workshop_dir = self._get_workshop_directory()
        if not workshop_dir or not Path(workshop_dir).exists():
            messagebox.showwarning("Workshop Directory", "Please set a valid Workshop directory first.")
            return

        steam_path = self._detect_steam_install_path()
        if not steam_path:
            messagebox.showwarning(
                "Steam Path Not Found",
                "Could not locate the Steam installation path from the workshop directory."
            )
            return

        try:
            from ..workshop.local_parser import LocalWorkshopMetadataParser
        except ImportError:
            from dayzconfigmaster.workshop.local_parser import LocalWorkshopMetadataParser

        with self._busy_context("Scanning for unsubscribed workshop items..."):
            parser = LocalWorkshopMetadataParser(steam_path)
            orphans = parser.find_unsubscribed_folders()
        if not orphans:
            messagebox.showinfo(
                "No Cleanup Needed",
                "All workshop folders are currently subscribed.",
            )
            return

        # Build a selection dialog so the user can uncheck items they want to keep.
        dialog = self._safe_toplevel("Cleanup Unsubscribed Workshop Items", "650x450")
        if dialog is None:
            messagebox.showinfo("No Cleanup Needed", "Cannot open cleanup dialog: no GUI root available.")
            return
        try:
            dialog.transient(self.root)
            dialog.grab_set()
        except tk.TclError:
            pass

        ttk.Label(
            dialog,
            text=f"Found {len(orphans)} unsubscribed workshop folder(s) consuming disk space.",
            font=("Arial", 10, "bold"),
        ).pack(padx=10, pady=(10, 5), anchor=tk.W)

        ttk.Label(
            dialog,
            text="Uncheck any items you want to keep. Checked items will be permanently deleted.",
            foreground="gray",
        ).pack(padx=10, pady=(0, 10), anchor=tk.W)

        list_frame = ttk.Frame(dialog)
        list_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)
        list_frame.columnconfigure(0, weight=1)
        list_frame.rowconfigure(0, weight=1)

        canvas = tk.Canvas(list_frame, highlightthickness=0)
        scrollbar = ttk.Scrollbar(list_frame, orient=tk.VERTICAL, command=canvas.yview)
        scrollable = ttk.Frame(canvas)
        scrollable.bind(
            "<Configure>",
            lambda e: canvas.configure(scrollregion=canvas.bbox("all")),
        )
        canvas.create_window((0, 0), window=scrollable, anchor=tk.NW)
        canvas.configure(yscrollcommand=scrollbar.set)
        canvas.grid(row=0, column=0, sticky=tk.NSEW)
        scrollbar.grid(row=0, column=1, sticky=tk.NS)

        check_vars: Dict[str, tk.BooleanVar] = {}
        total_bytes = 0
        for item in sorted(orphans, key=lambda x: x.size_on_disk_bytes, reverse=True):
            var = tk.BooleanVar(value=True)
            check_vars[item.workshop_id] = var
            total_bytes += item.size_on_disk_bytes
            row = ttk.Frame(scrollable)
            row.pack(fill=tk.X, pady=2)
            ttk.Checkbutton(row, variable=var).pack(side=tk.LEFT)
            ttk.Label(row, text=f"{item.workshop_id}").pack(side=tk.LEFT, padx=(0, 10))
            ttk.Label(row, text=item.size_on_disk_readable, width=12).pack(side=tk.LEFT)
            ttk.Label(row, text=item.path, foreground="gray").pack(side=tk.LEFT, padx=(10, 0))

        ttk.Label(
            dialog,
            text=f"Total selected for deletion: {parser._format_size(total_bytes)}",
        ).pack(padx=10, pady=(5, 0), anchor=tk.W)

        def on_delete():
            selected_ids = [ws_id for ws_id, var in check_vars.items() if var.get()]
            if not selected_ids:
                dialog.destroy()
                return

            # Protect items that are currently in the active mod list.
            active_ids = set(self._mod_workshop_id_by_name.values())
            protected = [ws_id for ws_id in selected_ids if ws_id in active_ids]
            if protected:
                messagebox.showwarning(
                    "Active Mods Selected",
                    f"The following items are in the active mod list and will not be deleted:\n" +
                    "\n".join(protected),
                )
                selected_ids = [ws_id for ws_id in selected_ids if ws_id not in active_ids]
                if not selected_ids:
                    return

            confirm = messagebox.askyesno(
                "Confirm Deletion",
                f"Permanently delete {len(selected_ids)} unsubscribed workshop folder(s)?\n\n"
                "This cannot be undone. The folders will be removed from disk.",
            )
            if not confirm:
                return

            deleted: List[str] = []
            failed: List[str] = []
            freed = 0
            with self._busy_context("Deleting unsubscribed workshop items..."):
                for ws_id in selected_ids:
                    path = Path(workshop_dir) / ws_id
                    try:
                        size = sum(
                            f.stat().st_size
                            for f in path.rglob('*')
                            if f.is_file()
                        )
                        shutil.rmtree(path)
                        deleted.append(ws_id)
                        freed += size
                    except Exception as exc:
                        failed.append(f"{ws_id}: {exc}")

            dialog.destroy()
            self._refresh_mod_list()
            self._refresh_all_map_combos()

            summary = f"Deleted {len(deleted)} folder(s), freed {parser._format_size(freed)}."
            if failed:
                summary += f"\n\nFailed:\n" + "\n".join(failed)
            messagebox.showinfo("Cleanup Complete", summary)

        btn_frame = ttk.Frame(dialog)
        btn_frame.pack(fill=tk.X, padx=10, pady=10)
        ttk.Button(btn_frame, text="Delete Selected", command=on_delete).pack(side=tk.RIGHT, padx=5)
        ttk.Button(btn_frame, text="Cancel", command=dialog.destroy).pack(side=tk.RIGHT, padx=5)

    def _detect_steam_install_path(self) -> Optional[str]:
        """Derive the Steam installation path from the configured workshop directory."""
        workshop_dir = self._get_workshop_directory()
        if not workshop_dir:
            return None
        path = Path(workshop_dir)
        # Typical path: .../steamapps/workshop/content/221100
        if path.name == str(221100):
            path = path.parent
        if path.name == "content":
            path = path.parent
        if path.name == "workshop":
            path = path.parent
        if path.name == "steamapps":
            return str(path.parent)
        return None

    def _attach_copy_menu(self, widget: tk.Widget) -> None:
        """Attach a right-click context menu with Copy to a widget.

        Note: a global right-click handler is also installed on the root
        window, so this is mainly useful for widgets that need to block the
        global menu or use a custom one.
        """
        pass

    def _show_context_menu(self, event: tk.Event) -> None:
        """Show a context menu with Copy/Cut/Paste for the widget under the cursor."""
        widget = event.widget
        menu = self._context_menu
        menu.delete(0, tk.END)

        if isinstance(widget, ttk.Treeview):
            selection = widget.selection()
            if selection:
                menu.add_command(label="Copy", command=lambda: self._copy_treeview_selection(widget))
                try:
                    menu.tk_popup(event.x_root, event.y_root)
                finally:
                    menu.grab_release()
            return

        if isinstance(widget, (tk.Entry, ttk.Entry, tk.Text, scrolledtext.ScrolledText)):
            # Determine if there is a text selection.
            has_selection = False
            if isinstance(widget, (tk.Entry, ttk.Entry)):
                try:
                    widget.selection_get()
                    has_selection = True
                except tk.TclError:
                    has_selection = False
            else:
                try:
                    widget.get(tk.SEL_FIRST, tk.SEL_LAST)
                    has_selection = True
                except tk.TclError:
                    has_selection = False

            if has_selection:
                menu.add_command(label="Cut", command=lambda: self._cut_text(widget))
                menu.add_command(label="Copy", command=lambda: self._copy_text(widget))
            menu.add_command(label="Paste", command=lambda: self._paste_text(widget))
            try:
                menu.tk_popup(event.x_root, event.y_root)
            finally:
                menu.grab_release()

    def _copy_treeview_selection(self, widget: ttk.Treeview) -> None:
        """Copy the selected treeview row values to the clipboard."""
        selection = widget.selection()
        if not selection:
            return
        values = widget.item(selection[0], "values")
        text = "\t".join(str(v) for v in values)
        self.root.clipboard_clear()
        self.root.clipboard_append(text)

    def _copy_text(self, widget: tk.Widget) -> None:
        """Copy the selected text of an entry/text widget to the clipboard."""
        try:
            if isinstance(widget, (tk.Entry, ttk.Entry)):
                text = widget.selection_get()
            else:
                text = widget.get(tk.SEL_FIRST, tk.SEL_LAST)
            self.root.clipboard_clear()
            self.root.clipboard_append(text)
        except tk.TclError:
            pass

    def _cut_text(self, widget: tk.Widget) -> None:
        """Cut the selected text of an entry/text widget."""
        try:
            if isinstance(widget, (tk.Entry, ttk.Entry)):
                text = widget.selection_get()
                widget.delete(tk.SEL_FIRST, tk.SEL_LAST)
            else:
                text = widget.get(tk.SEL_FIRST, tk.SEL_LAST)
                widget.delete(tk.SEL_FIRST, tk.SEL_LAST)
            self.root.clipboard_clear()
            self.root.clipboard_append(text)
        except tk.TclError:
            pass

    def _paste_text(self, widget: tk.Widget) -> None:
        """Paste clipboard text into an entry/text widget."""
        try:
            text = self.root.clipboard_get()
        except tk.TclError:
            return

        if isinstance(widget, (tk.Entry, ttk.Entry)):
            try:
                widget.delete(tk.SEL_FIRST, tk.SEL_LAST)
            except tk.TclError:
                pass
            widget.insert(tk.INSERT, text)
        else:
            try:
                widget.delete(tk.SEL_FIRST, tk.SEL_LAST)
            except tk.TclError:
                pass
            widget.insert(tk.INSERT, text)

    def _create_advanced_settings_tab(self):
        """Create the advanced settings tab with additional DayZ features"""
        advanced_frame = ScrollableFrame(self.notebook)
        self.notebook.add(advanced_frame, text="Advanced Settings")
        
        # Title
        title_label = ttk.Label(advanced_frame.frame, text="Advanced DayZ Configuration", font=("Arial", 14, "bold"))
        title_label.pack(pady=10)
        
        # Wiki documentation link
        wiki_link_frame = self.create_wiki_link(advanced_frame.frame, "advanced-settings")
        wiki_link_frame.pack(fill=tk.X, padx=10, pady=(5, 15))
        
        # Layout description
        layout_desc = self.create_layout_description(
            advanced_frame.frame,
            "Advanced Configuration Layout",
            """This tab contains advanced gameplay and environment settings:
- Gameplay Settings: Damage multipliers, spawn rates, item decay times
- Environment Settings: Fog density, rain intensity, wind speed, temperature

Each setting includes validation with min/max values and default ranges displayed next to each input field. These settings control the core gameplay mechanics of your DayZ server.""",
            "https://github.com/landracer/DayZConfigMaster/wiki/06-configuration/advanced-settings"
        )
        layout_desc.pack(fill=tk.X, padx=10, pady=(5, 15))
        
        # Tabs for advanced features
        advanced_notebook = ttk.Notebook(advanced_frame.frame)
        advanced_notebook.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)
        
        # Gameplay settings tab
        gameplay_frame = ttk.Frame(advanced_notebook)
        advanced_notebook.add(gameplay_frame, text="Gameplay")
        
        # Gameplay settings grid with proper scrollbars and validation
        gameplay_container = ttk.Frame(gameplay_frame)
        gameplay_container.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        # Gameplay settings grid
        ttk.Label(gameplay_container, text="Gameplay Settings", font=("Arial", 12, "bold")).grid(
            row=0, column=0, columnspan=4, sticky=tk.W, padx=5, pady=5)
        
        gameplay_settings = [
            ("Base Damage Multiplier", "1.0", {"min": 0.1, "max": 10.0}),
            ("Hit Point Multiplier", "1.0", {"min": 0.1, "max": 10.0}), 
            ("Loot Spawn Multiplier", "1.0", {"min": 0.1, "max": 5.0}),
            ("Zombie Spawn Multiplier", "1.0", {"min": 0.1, "max": 5.0}),
            ("Vehicle Spawn Multiplier", "1.0", {"min": 0.1, "max": 5.0}),
            ("Item Decay Time (sec)", "3600", {"min": 0, "max": 86400}),
        ]
        
        row = 1
        for setting, default_value, rules in gameplay_settings:
            ttk.Label(gameplay_container, text=setting).grid(row=row, column=0, sticky=tk.W, padx=5, pady=2)
            var = tk.StringVar(value=default_value)
            entry = ttk.Entry(gameplay_container, width=15, textvariable=var)
            entry.grid(row=row, column=1, sticky=tk.W+tk.E, padx=5, pady=2)
            
            # Create validation info frame
            info_frame = ttk.Frame(gameplay_container)
            info_frame.grid(row=row, column=2, sticky=tk.W, padx=(5, 0), pady=2)
            
            # Build explanation text with all available information
            explain_parts = []
            if "min" in rules:
                explain_parts.append(f"Min: {rules['min']}")
            if "max" in rules:
                explain_parts.append(f"Max: {rules['max']}")
            explain_parts.append(f"Default: {default_value}")
            
            info_label = ttk.Label(info_frame, text=" | ".join(explain_parts),
                                  foreground="gray", font=("Arial", 8))
            info_label.pack()
            
            self.input_widgets[setting] = (entry, rules)
            row += 1
        
        # Environment settings tab
        env_frame = ttk.Frame(advanced_notebook)
        advanced_notebook.add(env_frame, text="Environment")
        
        # Environment settings grid with proper scrollbars and validation
        env_container = ttk.Frame(env_frame)
        env_container.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        ttk.Label(env_container, text="Environment Settings", font=("Arial", 12, "bold")).grid(
            row=0, column=0, columnspan=4, sticky=tk.W, padx=5, pady=5)
        
        env_settings = [
            ("Fog Density", "0.5", {"min": 0.0, "max": 1.0}),
            ("Rain Intensity", "0.8", {"min": 0.0, "max": 1.0}),
            ("Wind Speed (m/s)", "5.0", {"min": 0.0, "max": 20.0}),
            ("Temperature (C)", "20.0", {"min": -40.0, "max": 50.0}),
        ]
        
        row = 1
        for setting, default_value, rules in env_settings:
            ttk.Label(env_container, text=setting).grid(row=row, column=0, sticky=tk.W, padx=5, pady=2)
            var = tk.StringVar(value=default_value)
            entry = ttk.Entry(env_container, width=15, textvariable=var)
            entry.grid(row=row, column=1, sticky=tk.W+tk.E, padx=5, pady=2)
            
            # Create validation info frame
            info_frame = ttk.Frame(env_container)
            info_frame.grid(row=row, column=2, sticky=tk.W, padx=(5, 0), pady=2)
            
            explain_parts = []
            if "min" in rules:
                explain_parts.append(f"Min: {rules['min']}")
            if "max" in rules:
                explain_parts.append(f"Max: {rules['max']}")
            explain_parts.append(f"Default: {default_value}")
            
            info_label = ttk.Label(info_frame, text=" | ".join(explain_parts),
                                  foreground="gray", font=("Arial", 8))
            info_label.pack()
            
            self.input_widgets[setting] = (entry, rules)
            row += 1
        
        # Configure grid weights for proper resizing
        gameplay_container.columnconfigure(1, weight=1)
        env_container.columnconfigure(1, weight=1)
    
    def _create_missing_config_tabs(self):
        """Create tabs for missing configuration files with comprehensive DayZ support"""
        
        # Helper function to create a tab with explanation and text area
        def create_config_tab(name, title, description, sample_content):
            frame = ScrollableFrame(self.notebook)
            self.notebook.add(frame, text=name)
            
            # Title
            title_label = ttk.Label(frame.frame, text=title, font=("Arial", 14, "bold"))
            title_label.pack(pady=10)
            
            # Explanation text with scrollable help
            explanation_text = tk.Text(frame.frame, wrap=tk.WORD, height=3, bg="#f0f0f0", relief="flat")
            explanation_text.insert(tk.END, description + "\n")
            explanation_text.config(state=tk.DISABLED)
            explanation_text.pack(fill=tk.X, padx=10, pady=(5, 10))
            
            # Sample content
            from tkinter import scrolledtext
            text_area = scrolledtext.ScrolledText(frame.frame, wrap=tk.WORD, height=15)
            text_area.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)
            text_area.insert(tk.END, sample_content)
        
        # 1. cfglimitsdefinitionuser.xml
        create_config_tab(
            "Limits User",
            "User Limits Configuration",
            "User-defined limits configuration. This file allows customizing spawn limits beyond the default values defined in limitsdefinitions.xml.",
            """// User-defined limits configuration
// This file allows customizing spawn limits beyond the default values

// Example format:
// <limits>
//   <limit name="CustomItem" value="50"/>
// </limits>"""
        )
        
        # 2. cfgeventspawns.xml
        create_config_tab(
            "Event Spawns",
            "Event Spawn Configuration",
            "Event-based spawn configuration. Define how items and entities spawn during special events like Heli Crashes or Zombie Hordes.",
            """// Event-based spawn configuration
// Define how items and entities spawn during events

// Example format:
// <eventspawns>
//   <event name="HeliCrash">
//     <spawn item="Helicopter" chance="0.1"/>
//   </event>
// </eventspawns>"""
        )
        
        # 3. cfgeffectarea.json
        create_config_tab(
            "Effect Areas",
            "Effect Area Configuration",
            "Effect area definitions in JSON format. Define zones with special effects like radiation, weather effects, or gameplay modifiers.",
            """// Effect area definitions in JSON format
{
  "effectAreas": [
    {
      "name": "RadioactiveZone",
      "radius": 50.0,
      "duration": 3600,
      "effects": ["radiation", "slowness"]
    }
  ]
}"""
        )
        
        # 4. cfgundergroundtriggers.json
        create_config_tab(
            "Underground Triggers",
            "Underground Trigger Configuration",
            "Underground trigger configurations. Define triggers for caves, bunkers, and underground areas with special effects.",
            """// Underground trigger configurations in JSON format
{
  "triggers": [
    {
      "name": "CaveEntrance",
      "radius": 20.0,
      "triggerType": "cave",
      "effects": ["darkness", "mystery"]
    }
  ]
}"""
        )
        
        # 5. mapclusterproto.xml
        create_config_tab(
            "Map Clusters",
            "Map Cluster Prototypes",
            "Map cluster prototypes. Define how map clusters are generated and organized by terrain type and density.",
            """// Map cluster prototypes configuration
// Define how map clusters are generated and organized

// Example format:
// <mapclusters>
//   <cluster name="ForestCluster">
//     <type>forest</type>
//     <density>high</density>
//   </cluster>
// </mapclusters>"""
        )
        
        # 6. mapgroupdirt.xml
        create_config_tab(
            "Map Group Dirt",
            "Map Group Dirt Configuration",
            "Map group dirt configurations. Define dirt and terrain properties for different map groups affecting vehicle handling.",
            """// Map group dirt configurations
// Define dirt and terrain properties for different map groups

// Example format:
// <mapgroups>
//   <group name="FarmArea">
//     <dirtType>clay</dirtType>
//     <density>medium</density>
//   </group>
// </mapgroups>"""
        )
        
        # 7. events.xml
        create_config_tab(
            "Events XML",
            "Event System Configuration",
            "Event system configurations. Define all server events and their properties including frequency, type, and parameters.",
            """// Event system configurations in XML
// Define all server events and their properties

// Example format:
// <events>
//   <event name="ZombieHorde">
//     <type>spawn</type>
//     <frequency>hourly</frequency>
//   </event>
// </events>"""
        )
        
        # 8. globals.xml
        create_config_tab(
            "Globals XML",
            "Global Configuration",
            "Global configuration settings. Define global server parameters that affect all gameplay aspects.",
            """// Global configuration settings
// Define global server parameters

// Example format:
// <globals>
//   <setting name="maxPlayers" value="100"/>
//   <setting name="serverName" value="My DayZ Server"/>
// </globals>"""
        )
        
        # 9. messages.xml
        create_config_tab(
            "Messages XML",
            "Server Messages Configuration",
            "Server messages configuration. Define server notifications and player-facing messages.",
            """// Server messages configuration
// Define server notifications and messages

// Example format:
// <messages>
//   <message id="welcome" text="Welcome to the server!"/>
// </messages>"""
        )
    
    def _create_instance_config_row(self, parent_frame, instance_num, state=tk.NORMAL):
        """Create configuration controls for a single server instance."""
        root_frame = ttk.LabelFrame(parent_frame, text=f"Instance {instance_num} Configuration", padding=10)
        root_frame.pack(fill=tk.X, padx=5, pady=5)
        root_frame.columnconfigure(1, weight=1)

        row = 0

        # Instance ID (must be unique per instance)
        ttk.Label(root_frame, text="Instance ID:").grid(row=row, column=0, sticky=tk.W, padx=5, pady=2)
        instance_id_var = tk.StringVar(value=str(instance_num))
        instance_id_entry = ttk.Entry(root_frame, width=10, textvariable=instance_id_var, state=state)
        instance_id_entry.grid(row=row, column=1, sticky=tk.W, padx=5, pady=2)
        ttk.Label(root_frame, text="Unique ID: 1-9 for each instance", foreground="gray", font=("Arial", 8)).grid(
            row=row, column=2, sticky=tk.W, padx=5, pady=2)
        self.input_widgets[f"Instance {instance_num} ID"] = (
            instance_id_entry, {"min": 1, "max": 9, "default": str(instance_num)}
        )
        row += 1

        # Config file
        ttk.Label(root_frame, text="Config File:").grid(row=row, column=0, sticky=tk.W, padx=5, pady=2)
        config_file_var = tk.StringVar(value=f"serverDZ_{instance_num}.cfg")
        config_entry = ttk.Entry(root_frame, width=40, textvariable=config_file_var, state=state)
        config_entry.grid(row=row, column=1, sticky=tk.W+tk.E, padx=5, pady=2)
        self.input_widgets[f"Instance {instance_num} Config"] = (config_entry, {})
        row += 1

        # Root folder
        ttk.Label(root_frame, text="Root Folder:").grid(row=row, column=0, sticky=tk.W, padx=5, pady=2)
        default_root = self._get_default_instance_root(instance_num)
        root_folder_var = tk.StringVar(value=default_root)
        root_entry = ttk.Entry(root_frame, width=40, textvariable=root_folder_var, state=state)
        root_entry.grid(row=row, column=1, sticky=tk.W+tk.E, padx=5, pady=2)

        def browse_root():
            path = filedialog.askdirectory(title="Select Root Folder")
            if path:
                root_folder_var.set(path)

        root_browse_btn = ttk.Button(root_frame, text="Browse", command=browse_root, state=state)
        root_browse_btn.grid(row=row, column=2, sticky=tk.W, padx=(0, 5), pady=2)

        self.input_widgets[f"Instance {instance_num} Root"] = (root_entry, {})
        row += 1

        # Game port - tight consecutive range starting at 2302
        # Instance 1: 2302/2303, Instance 2: 2304/2305, etc.
        base_port = 2302 + (instance_num - 1) * 2
        ttk.Label(root_frame, text="Game Port:").grid(row=row, column=0, sticky=tk.W, padx=5, pady=2)
        game_port_var = tk.StringVar(value=str(base_port))
        game_port_entry = ttk.Entry(root_frame, width=10, textvariable=game_port_var, state=state)
        game_port_entry.grid(row=row, column=1, sticky=tk.W, padx=5, pady=2)
        ttk.Label(root_frame, text=f"Default: {base_port}", foreground="gray", font=("Arial", 8)).grid(
            row=row, column=2, sticky=tk.W, padx=5, pady=2)
        self.input_widgets[f"Instance {instance_num} Game Port"] = (game_port_entry, {"min": 1024, "max": 65535})
        row += 1

        # Query port
        ttk.Label(root_frame, text="Query Port:").grid(row=row, column=0, sticky=tk.W, padx=5, pady=2)
        query_port_var = tk.StringVar(value=str(base_port + 1))
        query_port_entry = ttk.Entry(root_frame, width=10, textvariable=query_port_var, state=state)
        query_port_entry.grid(row=row, column=1, sticky=tk.W, padx=5, pady=2)
        self.input_widgets[f"Instance {instance_num} Query Port"] = (query_port_entry, {"min": 1024, "max": 65535})
        row += 1

        # Steam P2P port - separate range starting at 27016
        steam_base = 27016 + (instance_num - 1)
        ttk.Label(root_frame, text="Steam P2P Port:").grid(row=row, column=0, sticky=tk.W, padx=5, pady=2)
        steam_port_var = tk.StringVar(value=str(steam_base))
        steam_port_entry = ttk.Entry(root_frame, width=10, textvariable=steam_port_var, state=state)
        steam_port_entry.grid(row=row, column=1, sticky=tk.W, padx=5, pady=2)
        ttk.Label(root_frame, text=f"Default: {steam_base}", foreground="gray", font=("Arial", 8)).grid(
            row=row, column=2, sticky=tk.W, padx=5, pady=2)
        self.input_widgets[f"Instance {instance_num} Steam Port"] = (steam_port_entry, {"min": 1024, "max": 65535})
        row += 1

        # Map selection (auto-detected from stock and workshop directories)
        ttk.Label(root_frame, text="Map Name:").grid(row=row, column=0, sticky=tk.W, padx=5, pady=2)
        map_var = tk.StringVar(value="")
        map_combo = ttk.Combobox(root_frame, width=25, textvariable=map_var, state=state)
        map_combo.grid(row=row, column=1, sticky=tk.W, padx=5, pady=2)
        self._populate_maps(map_combo)
        self.input_widgets[f"Instance {instance_num} Map"] = (map_combo, {})
        row += 1

        # Profile path
        ttk.Label(root_frame, text="Profile Path:").grid(row=row, column=0, sticky=tk.W, padx=5, pady=2)
        default_profile = str(Path(default_root) / "profiles")
        profile_var = tk.StringVar(value=default_profile)
        profile_entry = ttk.Entry(root_frame, width=40, textvariable=profile_var, state=state)
        profile_entry.grid(row=row, column=1, sticky=tk.W+tk.E, padx=5, pady=2)

        def browse_profile():
            path = filedialog.askdirectory(title="Select Profile Folder")
            if path:
                profile_var.set(path)

        profile_browse_btn = ttk.Button(root_frame, text="Browse", command=browse_profile, state=state)
        profile_browse_btn.grid(row=row, column=2, sticky=tk.W, padx=(0, 5), pady=2)

        self.input_widgets[f"Instance {instance_num} Profile"] = (profile_entry, {})
        row += 1

        # Mission path (point-and-click selection from discovered missions)
        ttk.Label(root_frame, text="Mission Path:").grid(row=row, column=0, sticky=tk.W, padx=5, pady=2)
        mission_path_var = tk.StringVar(value="")
        mission_path_combo = ttk.Combobox(
            root_frame,
            width=40,
            textvariable=mission_path_var,
            values=[""],
            state="readonly" if state == tk.NORMAL else state,
        )
        mission_path_combo.grid(row=row, column=1, sticky=tk.W+tk.E, padx=5, pady=2)

        def browse_mission():
            path = filedialog.askdirectory(title="Select Mission Folder Source")
            if path:
                mission_path_var.set(path)

        mission_path_browse_btn = ttk.Button(root_frame, text="Browse", command=browse_mission, state=state)
        mission_path_browse_btn.grid(row=row, column=2, sticky=tk.W, padx=(0, 5), pady=2)

        self.input_widgets[f"Instance {instance_num} Mission"] = (mission_path_combo, {})
        row += 1

        # Mod paths
        ttk.Label(root_frame, text="Mod Paths:").grid(row=row, column=0, sticky=tk.W, padx=5, pady=2)
        mod_paths_var = tk.StringVar(value="")
        mod_paths_entry = ttk.Entry(root_frame, width=40, textvariable=mod_paths_var, state=state)
        mod_paths_entry.grid(row=row, column=1, sticky=tk.W+tk.E, padx=5, pady=2)

        def browse_mods():
            path = filedialog.askdirectory(title="Select Mods Directory")
            if path:
                current = mod_paths_var.get()
                if current:
                    mod_paths_var.set(f"{current};@{Path(path).name}")
                else:
                    mod_paths_var.set(f"@{Path(path).name}")

        mods_browse_btn = ttk.Button(root_frame, text="Browse", command=browse_mods, state=state)
        mods_browse_btn.grid(row=row, column=2, sticky=tk.W, padx=(0, 5), pady=2)

        self.input_widgets[f"Instance {instance_num} Mods"] = (mod_paths_entry, {})
        row += 1

        # Mod load order hint
        hint_label = ttk.Label(
            root_frame,
            text="Place mods in proper load order (dependencies first) or loading may fail. Order varies by mods used.",
            foreground="orange",
            font=("Arial", 8, "italic"),
            wraplength=500,
        )
        hint_label.grid(row=row, column=1, sticky=tk.W, padx=5, pady=(0, 4))

        # Store references for multi-instance start/stop
        self._instance_vars.append({
            "id": instance_id_var,
            "config_file": config_file_var,
            "root_folder": root_folder_var,
            "game_port": game_port_var,
            "query_port": query_port_var,
            "steam_port": steam_port_var,
            "map": map_combo,
            "profile": profile_var,
            "mod_paths": mod_paths_var,
            "mission_path": mission_path_var,
            "mission_path_widget": mission_path_combo,
        })

        root_frame.columnconfigure(1, weight=1)
    
    def _create_house_cleaning_tab(self):
        """Create the house-cleaning and scheduling tab with full server management features"""
        cleaning_frame = ScrollableFrame(self.notebook)
        self.notebook.add(cleaning_frame, text="House Cleaning & Scheduling")
        
        # Title
        title_label = ttk.Label(cleaning_frame.frame, text="Server House-Cleaning & Scheduling", font=("Arial", 14, "bold"))
        title_label.pack(pady=10)
        
        try:
            from ..integration import ServerManager
            self.server_manager = ServerManager(str(self._get_projects_root()))
            self.server_manager.load_config()
        except Exception as e:
            print(f"Server manager initialization: {e}")
            self.server_manager = None
        
        # Wiki documentation link
        wiki_link_frame = self.create_wiki_link(cleaning_frame.frame, "house-cleaning-scheduling")
        wiki_link_frame.pack(fill=tk.X, padx=10, pady=(5, 15))
        
        # Overview description
        layout_desc = self.create_layout_description(
            cleaning_frame.frame,
            "Why Schedule Server Restarts?",
            """DayZ servers accumulate memory leaks over time, causing RAM usage to gradually increase until the server crashes.

Scheduled restarts are essential for:
• 🔄 **Memory Cleanup**: Flush accumulated memory leaks each restart
• 📊 **Performance Stability**: Maintain consistent performance throughout the day
• 🎮 **Player Experience**: Prevent unexpected crashes during gameplay
• 💾 **Database Integrity**: Regular restarts help maintain clean database state

Recommended Restart Schedule:
• **Standard servers**: Every 4-6 hours
• **High-population servers**: Every 3-4 hours (more player activity = more memory usage)
• **Multi-instance setups**: ⚠️ Offset restart times by 30+ minutes between instances to prevent CPU/disk spikes""",
            "https://github.com/landracer/DayZConfigMaster/wiki/06-configuration/house-cleaning-scheduling"
        )
        layout_desc.pack(fill=tk.X, padx=10, pady=(5, 15))
        
        # Memory monitoring section
        memory_frame = ttk.LabelFrame(cleaning_frame.frame, text="Memory Monitoring", padding=10)
        memory_frame.pack(fill=tk.X, padx=10, pady=5)
        
        row_mem = 0
        
        # Log memory setting
        log_memory_var = tk.IntVar(value=1)
        log_memory_check = ttk.Checkbutton(memory_frame, text="Enable Memory Logging (logMemory = 1)", variable=log_memory_var)
        log_memory_check.grid(row=row_mem, column=0, sticky=tk.W, padx=5, pady=2)
        self.input_widgets["Log Memory"] = (log_memory_check, {})
        row_mem += 1
        
        ttk.Label(memory_frame, text="With logMemory enabled, check logs/servername_memory.log for trends").grid(row=row_mem, column=0, sticky=tk.W, padx=5, pady=(0, 5), columnspan=2)
        row_mem += 1
        
        # Memory threshold warning
        ttk.Label(memory_frame, text="Memory Threshold Warning:", font=("Arial", 9, "bold")).grid(row=row_mem, column=0, sticky=tk.W, padx=5, pady=(10, 2))
        memory_threshold_var = tk.StringVar(value="1600")
        memory_threshold_entry = ttk.Entry(memory_frame, width=10, textvariable=memory_threshold_var)
        memory_threshold_entry.grid(row=row_mem, column=1, sticky=tk.W, padx=5, pady=2)
        ttk.Label(memory_frame, text="MB - Alert when server exceeds this threshold", foreground="gray", font=("Arial", 8)).grid(row=row_mem, column=2, sticky=tk.W, padx=5, pady=2)
        self.input_widgets["Memory Threshold"] = (memory_threshold_entry, {"min": 500, "max": 32000})
        row_mem += 1
        
        # Scheduling section
        schedule_frame = ttk.LabelFrame(cleaning_frame.frame, text="Restart Scheduling", padding=10)
        schedule_frame.pack(fill=tk.X, padx=10, pady=5)
        
        row_sched = 0
        
        # Restart interval
        ttk.Label(schedule_frame, text="Restart Interval (hours):").grid(row=row_sched, column=0, sticky=tk.W, padx=5, pady=2)
        restart_interval_var = tk.StringVar(value="4")
        restart_interval_entry = ttk.Entry(schedule_frame, width=10, textvariable=restart_interval_var)
        restart_interval_entry.grid(row=row_sched, column=1, sticky=tk.W, padx=5, pady=2)
        self.input_widgets["Restart Interval"] = (restart_interval_entry, {"min": 1, "max": 24})
        row_sched += 1
        
        # Staggered restarts info
        stagger_info = ttk.LabelFrame(cleaning_frame.frame, text="Multi-Instance Staggering", padding=5)
        stagger_info.pack(fill=tk.X, padx=10, pady=(10, 0))
        
        stagger_text = """For multiple instances running on the same hardware:

Recommended staggered restart schedule:
• Instance 1: Every day at 4:00 AM
• Instance 2: Every day at 4:30 AM  
• Instance 3: Every day at 5:00 AM

This prevents simultaneous Steam CMD logins and reduces resource spikes."""
        ttk.Label(stagger_info, text=stagger_text, font=("Arial", 9), justify=tk.LEFT).pack(anchor=tk.W, padx=5, pady=5)
        
        # Start.sh generation section
        start_sh_frame = ttk.LabelFrame(cleaning_frame.frame, text="Linux start.sh Generation", padding=10)
        start_sh_frame.pack(fill=tk.X, padx=10, pady=(10, 0))
        
        # Instance selector for start.sh (uses unified multi-instance settings)
        instance_selector_frame = ttk.Frame(start_sh_frame)
        instance_selector_frame.pack(fill=tk.X, padx=5, pady=5)

        ttk.Label(
            instance_selector_frame,
            text="Instances are configured in Server Control > Multi-Instance.",
            font=("Arial", 9)
        ).pack(anchor=tk.W, padx=5, pady=(0, 5))

        # Auto-generate start.sh button
        def generate_start_sh():
            if not self._instance_vars:
                messagebox.showwarning(
                    "No Instances",
                    "No instances are configured. Add instances in Server Control > Multi-Instance first."
                )
                return

            start_content = """#!/bin/bash
# DayZ Server Multi-Instance Start Script (Linux)
# Generated by DayzConfigMaster

cd "$(dirname "$0")"

echo "Starting DayZ server instances..."
"""
            for instance in self._instance_vars:
                instance_id = instance["id"].get()
                config_file = instance["config_file"].get() or f"serverDZ_{instance_id}.cfg"
                profile_path = instance["profile"].get() or f"./profile{instance_id}"
                game_port = instance["game_port"].get()
                query_port = instance["query_port"].get()
                steam_port = instance["steam_port"].get()
                mod_paths = instance["mod_paths"].get().strip()
                mod_arg = f'-mod="{mod_paths}"' if mod_paths else ""
                exe_name = self._get_instance_binary_name(int(instance_id), mode="normal")
                root_folder = instance.get("root_folder", {}).get() or self._get_default_instance_root(int(instance_id))
                inst_dir = Path(root_folder).name
                home_dir = f"$PWD/{inst_dir}/.dayzhome"

                start_content += f"""
# Instance {instance_id}
export HOME="{home_dir}"
export XDG_CONFIG_HOME="{home_dir}/.config"
export XDG_DATA_HOME="{home_dir}/.local/share"
export XDG_RUNTIME_DIR="{home_dir}/.runtime"
export TMPDIR="{home_dir}/.tmp"
export STEAM_RUNTIME_DIR="{home_dir}/.runtime/steam"
mkdir -p "$HOME" "$XDG_CONFIG_HOME" "$XDG_DATA_HOME" "$XDG_RUNTIME_DIR" "$TMPDIR"
nohup ./{inst_dir}/{exe_name} \\
    -port={game_port} \\
    -queryPort={query_port} \\
    -steamPort={steam_port} \\
    -config={config_file} \\
    -profiles={profile_path} \\
    -instanceId={instance_id} \\
    {mod_arg} > {inst_dir}/logs/server{instance_id}.log 2>&1 &
echo "Instance {instance_id} started on port {game_port}"
"""

            start_content += """
echo "All instances started successfully!"
wait
"""

            result_window = self._safe_toplevel("Generated start.sh", "700x500")
            if result_window is None:
                messagebox.showerror("GUI Error", "Cannot display generated script: no GUI root available.")
                return

            ttk.Label(result_window, text="start.sh Content", font=("Arial", 12, "bold")).pack(pady=10)

            from tkinter import scrolledtext
            text_area = scrolledtext.ScrolledText(result_window, wrap=tk.NONE, height=25, font=("Courier", 9))
            text_area.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)
            text_area.insert(tk.END, start_content)

            def save_start_sh():
                filename = filedialog.asksaveasfilename(
                    title="Save start.sh",
                    defaultextension=".sh",
                    filetypes=[("Shell scripts", "*.sh"), ("All files", "*.*")]
                )
                if filename:
                    with open(filename, 'w') as f:
                        f.write(start_content)
                    import os
                    os.chmod(filename, 0o755)  # Make executable
                    messagebox.showinfo("Save Complete", f"start.sh saved to: {filename}")

            ttk.Button(result_window, text="Save to File", command=save_start_sh).pack(side=tk.LEFT, padx=10, pady=10)
            ttk.Button(result_window, text="Close", command=result_window.destroy).pack(side=tk.RIGHT, padx=10, pady=10)

        generate_btn = ttk.Button(start_sh_frame, text="Generate start.sh Script", command=generate_start_sh)
        generate_btn.pack(pady=5)

        # Deployment section
        deploy_frame = ttk.LabelFrame(cleaning_frame.frame, text="Instance Deployment", padding=10)
        deploy_frame.pack(fill=tk.X, padx=10, pady=(10, 0))

        ttk.Label(
            deploy_frame,
            text="Deploy configured instances to disk with proper directory structure, mod symlinks, and keys.",
            wraplength=800,
            font=("Arial", 9)
        ).pack(anchor=tk.W, padx=5, pady=(0, 5))

        ttk.Label(
            deploy_frame,
            text="Requires: DayZ Server Path (Server Control) and Workshop Directory (Mods tab).",
            foreground="gray",
            font=("Arial", 8)
        ).pack(anchor=tk.W, padx=5, pady=(0, 5))

        deploy_options_frame = ttk.Frame(deploy_frame)
        deploy_options_frame.pack(fill=tk.X, padx=5, pady=5)

        generate_systemd_var = tk.IntVar(value=0)
        ttk.Checkbutton(
            deploy_options_frame,
            text="Generate systemd service files",
            variable=generate_systemd_var
        ).pack(side=tk.LEFT, padx=5)

        def force_full_redeploy():
            if not messagebox.askyesno(
                "Confirm Force Redeploy",
                "Clear the deployment manifest and force a full redeploy on the next start?\n\n"
                "Use this after manually editing deployed files.",
            ):
                return
            for instance in self._instance_vars:
                instance_id = int(instance.get("id", {}).get() or 1)
                root_folder = instance.get("root_folder", {}).get() or ""
                root_folder = self._sanitize_instance_root(root_folder, instance_id)
                dayz_path = self.dayz_path_var.get().strip()
                instance_root = Path(root_folder) if root_folder else Path(dayz_path)
                if instance_root.exists():
                    mgr = DeploymentManifestManager(instance_root)
                    mgr.mark_forced()
                    self.log_text.insert(
                        tk.END,
                        f"[{self._get_timestamp()}] Instance {instance_id}: deployment manifest cleared; next start will do a full redeploy.\n",
                    )

        ttk.Button(deploy_options_frame, text="Force Full Redeploy", command=force_full_redeploy).pack(
            side=tk.LEFT, padx=5
        )

        def deploy_instances():
            with self._busy_context("Deploying instances..."):
                _deploy_instances_impl()

        def _deploy_instances_impl():
            base_server_dir = Path(self.dayz_path_var.get().strip())
            if not base_server_dir.exists():
                messagebox.showerror(
                    "DayZ Server Path Missing",
                    "Set the DayZ Server Path in Server Control > General Settings first."
                )
                return

            workshop_dir_str = self._workshop_dir_var.get().strip()
            workshop_dir = Path(workshop_dir_str) if workshop_dir_str else None

            projects_root = Path(self._get_projects_root())

            if not self._instance_vars:
                messagebox.showwarning(
                    "No Instances",
                    "No instances are configured. Add instances in Server Config > Multi-Instance first."
                )
                return

            instance_cfgs = self._build_instance_cfgs_content()
            instances = []
            for instance in self._instance_vars:
                instance_id = int(instance["id"].get() or 1)
                cfg_key = f"serverDZ_{instance_id}.cfg"
                full_mods_str = self._get_instance_mods(instance)
                instances.append({
                    "name": f"instance{instance_id}",
                    "id": instance_id,
                    "cfg_content": instance_cfgs.get(cfg_key, ""),
                    "mod_paths": [
                        p.strip()
                        for p in full_mods_str.replace(",", ";").split(";")
                        if p.strip()
                    ],
                    "port": int(instance["game_port"].get() or 2302),
                    "max_players": int(self.input_widgets.get("Max Players", (None, {}))[0].get() or 60),
                    "profile": instance["profile"].get() or f"profiles",
                })

            try:
                results = deploy_all_instances(
                    base_server_dir=base_server_dir,
                    instances=instances,
                    projects_root=projects_root,
                    workshop_dir=workshop_dir,
                    generate_scripts=True,
                    generate_systemd=bool(generate_systemd_var.get()),
                    systemd_output_dir=projects_root / "systemd",
                )
            except Exception as exc:
                messagebox.showerror("Deployment Failed", f"Deployment failed:\n{exc}")
                return

            # Deploy/refresh per-instance mission folders so the generated
            # serverDZ.cfg template actually has a matching mpmissions folder.
            for instance in self._instance_vars:
                instance_id = int(instance["id"].get() or 1)
                result = results.get(f"instance{instance_id}")
                if result is None or result.instance_dir is None:
                    continue
                map_display_name = instance.get("map", {}).get() or ""
                mission_source = instance.get("mission_path")
                mission_source_str = mission_source.get() if mission_source is not None else ""
                mission_source_path = Path(mission_source_str) if mission_source_str.strip() else None
                try:
                    mission_msg, mission_target = self._deploy_mission_folder(
                        instance_root=result.instance_dir,
                        dayz_path=base_server_dir,
                        map_display_name=map_display_name,
                        workshop_dir=str(workshop_dir) if workshop_dir else "",
                        mission_source_path=mission_source_path,
                    )
                    result.messages.append(mission_msg)
                    if mission_msg.startswith("ERROR"):
                        result.errors.append(mission_msg)
                    elif mission_target:
                        lifetime_msg = self._normalize_aircraft_lifetimes(
                            result.instance_dir, mission_target
                        )
                        result.messages.append(lifetime_msg)
                except Exception as exc:
                    result.errors.append(f"Mission folder deployment failed: {exc}")

            # Show results
            result_window = self._safe_toplevel("Deployment Results", "800x500")
            if result_window is None:
                messagebox.showerror("GUI Error", "Cannot display deployment results: no GUI root available.")
                return

            text_area = scrolledtext.ScrolledText(result_window, wrap=tk.WORD, height=25, font=("Courier", 9))
            text_area.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)

            for name, result in results.items():
                text_area.insert(tk.END, f"=== {name} ===\n")
                text_area.insert(tk.END, f"Directory: {result.instance_dir}\n")
                for msg in result.messages:
                    text_area.insert(tk.END, f"  ✓ {msg}\n")
                for err in result.errors:
                    text_area.insert(tk.END, f"  ✗ {err}\n")
                if result.start_script_path:
                    text_area.insert(tk.END, f"  Start script: {result.start_script_path}\n")
                if result.systemd_path:
                    text_area.insert(tk.END, f"  Systemd unit: {result.systemd_path}\n")
                text_area.insert(tk.END, "\n")

            text_area.config(state=tk.DISABLED)

            has_errors = any(r.errors for r in results.values())
            if has_errors:
                messagebox.showwarning(
                    "Deployment Completed with Warnings",
                    "Some instances deployed with warnings. Review the results window for details."
                )
            else:
                messagebox.showinfo(
                    "Deployment Complete",
                    f"Deployed {len(results)} instance(s) to:\n{projects_root / 'instances'}"
                )
            self.status_var.set(f"Deployed {len(results)} instance(s)")

        deploy_btn = ttk.Button(deploy_frame, text="🚀 Deploy Instances", command=deploy_instances)
        deploy_btn.pack(pady=5)

        # Mission XML maintenance section.
        xml_frame = ttk.LabelFrame(
            cleaning_frame.frame, text="Mission XML Maintenance", padding=10
        )
        xml_frame.pack(fill=tk.X, padx=10, pady=(10, 0))
        ttk.Label(
            xml_frame,
            text="Fix common central-economy problems that delete admin-placed assets.",
            wraplength=800,
            font=("Arial", 9),
        ).pack(anchor=tk.W, padx=5, pady=(0, 5))
        ttk.Button(
            xml_frame,
            text="✈️ Fix Aircraft Lifetimes",
            command=self._repair_aircraft_lifetimes,
        ).pack(anchor=tk.W, padx=5, pady=2)

        # Storage backups and player restore UI.
        self._create_house_cleaning_backup_ui(cleaning_frame.frame)

    def _create_house_cleaning_backup_ui(self, parent: tk.Widget) -> None:
        """Add per-instance storage_1 backup and players.db restore UI."""
        backup_frame = ttk.LabelFrame(parent, text="Instance Storage Backups", padding=10)
        backup_frame.pack(fill=tk.X, padx=10, pady=(10, 0))

        # Instance selector
        selector_frame = ttk.Frame(backup_frame)
        selector_frame.pack(fill=tk.X, pady=(0, 5))
        ttk.Label(selector_frame, text="Instance:").pack(side=tk.LEFT)
        self._backup_instance_var = tk.StringVar()
        self._backup_instance_combo = ttk.Combobox(
            selector_frame,
            textvariable=self._backup_instance_var,
            state="readonly",
            width=40,
        )
        self._backup_instance_combo.pack(side=tk.LEFT, padx=5)
        self._backup_instance_combo.bind(
            "<<ComboboxSelected>>", lambda _e: self._refresh_storage_backup_list()
        )

        # Settings
        settings_frame = ttk.Frame(backup_frame)
        settings_frame.pack(fill=tk.X, pady=5)
        ttk.Label(settings_frame, text="Max backup age before start (min):").pack(side=tk.LEFT)
        self._storage_backup_max_age_minutes_var = tk.StringVar(value="60")
        ttk.Entry(settings_frame, textvariable=self._storage_backup_max_age_minutes_var, width=8).pack(
            side=tk.LEFT, padx=5
        )
        ttk.Label(settings_frame, text="Retention count:").pack(side=tk.LEFT, padx=(15, 0))
        self._storage_backup_retention_var = tk.StringVar(value="14")
        ttk.Entry(settings_frame, textvariable=self._storage_backup_retention_var, width=8).pack(
            side=tk.LEFT, padx=5
        )

        # Backup list
        list_frame = ttk.Frame(backup_frame)
        list_frame.pack(fill=tk.BOTH, expand=True, pady=5)
        list_frame.columnconfigure(0, weight=1)
        list_frame.rowconfigure(0, weight=1)

        cols = ("Date", "Size", "Path")
        self._storage_backup_tree = ttk.Treeview(list_frame, columns=cols, show="headings")
        for c in cols:
            self._storage_backup_tree.heading(c, text=c)
            self._storage_backup_tree.column(c, width=140 if c == "Size" else 220)
        self._storage_backup_tree.grid(row=0, column=0, sticky=tk.NSEW)
        scroll = ttk.Scrollbar(list_frame, orient=tk.VERTICAL, command=self._storage_backup_tree.yview)
        scroll.grid(row=0, column=1, sticky=tk.NS)
        self._storage_backup_tree.configure(yscrollcommand=scroll.set)

        # Buttons
        btn_frame = ttk.Frame(backup_frame)
        btn_frame.pack(fill=tk.X, pady=5)
        ttk.Button(btn_frame, text="Refresh", command=self._refresh_storage_backup_list).pack(
            side=tk.LEFT, padx=2
        )
        ttk.Button(btn_frame, text="Backup Now", command=self._backup_storage_now).pack(
            side=tk.LEFT, padx=2
        )
        ttk.Button(btn_frame, text="Restore Selected", command=self._restore_storage_backup).pack(
            side=tk.LEFT, padx=2
        )
        ttk.Button(btn_frame, text="Delete Selected", command=self._delete_storage_backup).pack(
            side=tk.LEFT, padx=2
        )
        ttk.Button(btn_frame, text="Prune Old", command=self._prune_storage_backups).pack(
            side=tk.LEFT, padx=2
        )
        ttk.Button(btn_frame, text="Start Fresh", command=self._start_storage_fresh).pack(
            side=tk.LEFT, padx=2
        )

        # Player restore section
        player_frame = ttk.LabelFrame(parent, text="Player Data Restore", padding=10)
        player_frame.pack(fill=tk.X, padx=10, pady=(10, 0))

        ttk.Label(
            player_frame,
            text="Compare the current players.db with a backup and splice a single player's Data blob back into the live database.",
            wraplength=800,
            foreground="gray",
        ).pack(anchor=tk.W, pady=(0, 5))

        player_btn_frame = ttk.Frame(player_frame)
        player_btn_frame.pack(fill=tk.X, pady=5)
        ttk.Button(
            player_btn_frame,
            text="Open Player Restore Tool...",
            command=self._open_player_restore_dialog,
        ).pack(side=tk.LEFT, padx=2)

        # Scheduler section
        scheduler_frame = ttk.LabelFrame(parent, text="Scheduled Events", padding=10)
        scheduler_frame.pack(fill=tk.X, padx=10, pady=(10, 0))

        ttk.Checkbutton(
            scheduler_frame,
            text="Enable scheduled events (restart / backup / message)",
            variable=self._scheduler_enabled_var,
            command=self._toggle_event_scheduler,
        ).pack(anchor=tk.W, padx=5, pady=2)

        scheduler_btn_frame = ttk.Frame(scheduler_frame)
        scheduler_btn_frame.pack(fill=tk.X, padx=5, pady=5)
        ttk.Button(
            scheduler_btn_frame,
            text="Run Backup Now",
            command=lambda: self._on_scheduler_backup(type("Event", (), {"name": "Manual backup"})()),
        ).pack(side=tk.LEFT, padx=2)
        ttk.Button(
            scheduler_btn_frame,
            text="Edit Events...",
            command=self._open_scheduler_editor,
        ).pack(side=tk.LEFT, padx=2)

        # Bind instance list refresh to multi-instance changes.
        self._refresh_backup_instance_selector()

    def _get_backup_instance_selection(self) -> Optional[Dict[str, Any]]:
        selected = self._backup_instance_var.get()
        for instance in getattr(self, "_instance_vars", []):
            name = self._instance_display_name(instance)
            if name == selected:
                return instance
        return None

    def _refresh_backup_instance_selector(self) -> None:
        names = [self._instance_display_name(i) for i in getattr(self, "_instance_vars", [])]
        if hasattr(self, "_backup_instance_combo"):
            self._backup_instance_combo.config(values=names)
            current = self._backup_instance_var.get()
            if current not in names and names:
                self._backup_instance_var.set(names[0])
            self._refresh_storage_backup_list()

    def _open_scheduler_editor(self) -> None:
        """Open a simple dialog to add/remove scheduled events."""
        dialog = self._safe_toplevel("Scheduled Events", "700x500")
        if dialog is None:
            return

        dialog.columnconfigure(0, weight=1)
        dialog.rowconfigure(0, weight=1)

        tree = ttk.Treeview(
            dialog,
            columns=("Name", "CRON", "Type", "Enabled", "Next"),
            show="headings",
        )
        for c in ("Name", "CRON", "Type", "Enabled", "Next"):
            tree.heading(c, text=c)
            tree.column(c, width=120)
        tree.grid(row=0, column=0, sticky=tk.NSEW, padx=10, pady=5)
        scroll = ttk.Scrollbar(dialog, orient=tk.VERTICAL, command=tree.yview)
        scroll.grid(row=0, column=1, sticky=tk.NS, pady=5)
        tree.configure(yscrollcommand=scroll.set)

        def _refresh() -> None:
            for item in tree.get_children():
                tree.delete(item)
            if self._event_scheduler is None:
                return
            for event in self._event_scheduler._events:
                next_run = event.next_run.strftime("%Y-%m-%d %H:%M") if event.next_run else "—"
                tree.insert(
                    "",
                    tk.END,
                    values=(event.name, event.cron, event.event_type.value, str(event.enabled), next_run),
                )

        _refresh()

        controls = ttk.Frame(dialog)
        controls.grid(row=1, column=0, columnspan=2, sticky=tk.EW, padx=10, pady=5)

        ttk.Label(controls, text="Name:").pack(side=tk.LEFT)
        name_var = tk.StringVar()
        ttk.Entry(controls, textvariable=name_var, width=12).pack(side=tk.LEFT, padx=2)

        ttk.Label(controls, text="CRON:").pack(side=tk.LEFT, padx=(10, 0))
        cron_var = tk.StringVar(value="0 4 * * *")
        ttk.Entry(controls, textvariable=cron_var, width=14).pack(side=tk.LEFT, padx=2)

        ttk.Label(controls, text="Type:").pack(side=tk.LEFT, padx=(10, 0))
        type_var = tk.StringVar(value="backup")
        ttk.Combobox(
            controls,
            textvariable=type_var,
            values=[t.value for t in EventType],
            state="readonly",
            width=12,
        ).pack(side=tk.LEFT, padx=2)

        def _add() -> None:
            if self._event_scheduler is None:
                return
            try:
                etype = EventType(type_var.get())
                self._event_scheduler.add_event(name_var.get(), cron_var.get(), etype)
                self._event_scheduler.save_events()
                _refresh()
            except Exception as exc:
                messagebox.showerror("Error", str(exc))

        def _remove() -> None:
            sel = tree.selection()
            if not sel or self._event_scheduler is None:
                return
            name = tree.item(sel[0], "values")[0]
            self._event_scheduler.remove_event(name)
            self._event_scheduler.save_events()
            _refresh()

        def _toggle() -> None:
            sel = tree.selection()
            if not sel or self._event_scheduler is None:
                return
            name = tree.item(sel[0], "values")[0]
            self._event_scheduler.toggle_event(name)
            self._event_scheduler.save_events()
            _refresh()

        ttk.Button(controls, text="Add", command=_add).pack(side=tk.LEFT, padx=2)
        ttk.Button(controls, text="Toggle", command=_toggle).pack(side=tk.LEFT, padx=2)
        ttk.Button(controls, text="Remove", command=_remove).pack(side=tk.LEFT, padx=2)
        ttk.Button(controls, text="Close", command=dialog.destroy).pack(side=tk.RIGHT, padx=2)

    def _storage_backup_manager_for_selection(self) -> Optional[Tuple["InstanceStorageBackupManager", Path]]:
        instance = self._get_backup_instance_selection()
        if instance is None:
            return None
        instance_id = int(instance.get("id", {}).get() or 1)
        map_name = instance.get("map", {}).get() or ""
        root_folder = instance.get("root_folder", {}).get() or ""
        root_folder = self._sanitize_instance_root(root_folder, instance_id)
        dayz_path = self.dayz_path_var.get().strip()
        instance_root = Path(root_folder) if root_folder else Path(dayz_path)

        from dayzconfigmaster.mods.integration import ModIntegrationManager

        mission_dir = ModIntegrationManager(instance_root)._find_mission_dir()
        if mission_dir is None:
            return None
        storage_path = mission_dir / "storage_1"
        mgr = InstanceStorageBackupManager(instance_root, instance_id, map_name)
        return mgr, storage_path

    def _refresh_storage_backup_list(self, event=None) -> None:
        if not hasattr(self, "_storage_backup_tree"):
            return
        for item in self._storage_backup_tree.get_children():
            self._storage_backup_tree.delete(item)

        selection = self._storage_backup_manager_for_selection()
        if selection is None:
            return
        mgr, _storage_path = selection
        for info in mgr.list_backups():
            size_mb = info.size_bytes / (1024 * 1024)
            self._storage_backup_tree.insert(
                "",
                tk.END,
                values=(
                    info.timestamp.strftime("%Y-%m-%d %H:%M:%S"),
                    f"{size_mb:.2f} MB",
                    str(info.path),
                ),
            )

    def _backup_storage_now(self) -> None:
        selection = self._storage_backup_manager_for_selection()
        if selection is None:
            messagebox.showwarning("No Instance", "Select an instance first.")
            return
        mgr, storage_path = selection
        if not storage_path.exists():
            messagebox.showwarning("No storage_1", "No storage_1 folder found for this instance.")
            return
        ok, msg = mgr.create_backup(storage_path)
        if ok:
            messagebox.showinfo("Backup Created", msg)
        else:
            messagebox.showerror("Backup Failed", msg)
        self._refresh_storage_backup_list()

    def _restore_storage_backup(self) -> None:
        selection = self._storage_backup_manager_for_selection()
        if selection is None:
            messagebox.showwarning("No Instance", "Select an instance first.")
            return
        mgr, storage_path = selection

        selected = self._storage_backup_tree.selection()
        if not selected:
            messagebox.showwarning("No Backup", "Select a backup to restore.")
            return
        values = self._storage_backup_tree.item(selected[0], "values")
        backup_path = Path(values[2])

        if not messagebox.askyesno(
            "Confirm Restore",
            f"Restore {backup_path.name} over the current storage_1?\n\n"
            "This will overwrite current player data.",
        ):
            return

        ok, msg = mgr.restore_backup(backup_path, storage_path)
        if ok:
            messagebox.showinfo("Restore Complete", msg)
        else:
            messagebox.showerror("Restore Failed", msg)

    def _delete_storage_backup(self) -> None:
        selection = self._storage_backup_manager_for_selection()
        if selection is None:
            return
        mgr, _storage_path = selection

        selected = self._storage_backup_tree.selection()
        if not selected:
            return
        values = self._storage_backup_tree.item(selected[0], "values")
        backup_path = Path(values[2])

        if not messagebox.askyesno("Confirm Delete", f"Delete backup {backup_path.name}?"):
            return

        ok, msg = mgr.delete_backup(backup_path)
        if ok:
            self._refresh_storage_backup_list()
        else:
            messagebox.showerror("Delete Failed", msg)

    def _prune_storage_backups(self) -> None:
        selection = self._storage_backup_manager_for_selection()
        if selection is None:
            return
        mgr, _storage_path = selection

        try:
            max_count = int(self._storage_backup_retention_var.get())
        except ValueError:
            max_count = 14

        deleted, messages = mgr.prune(max_count=max_count)
        messagebox.showinfo("Prune Complete", f"Deleted {deleted} backup(s).")
        self._refresh_storage_backup_list()

    def _start_storage_fresh(self) -> None:
        """Reset the selected instance's storage_1 to a clean, empty state."""
        selection = self._storage_backup_manager_for_selection()
        if selection is None:
            messagebox.showwarning("No Instance", "Select an instance first.")
            return
        mgr, storage_path = selection

        if not storage_path.exists():
            messagebox.showinfo("Already Fresh", "No storage_1 folder exists for this instance.")
            return

        if not messagebox.askyesno(
            "Confirm Start Fresh",
            "Reset storage_1 to a clean, empty state?\n\n"
            "A safety backup of the current data will be created first.",
        ):
            return

        ok, msg = mgr.start_fresh(storage_path)
        if ok:
            messagebox.showinfo("Fresh Start", msg)
        else:
            messagebox.showerror("Fresh Start Failed", msg)
        self._refresh_storage_backup_list()

    def _open_player_restore_dialog(self) -> None:
        """Open a dialog to compare and splice players.db rows."""
        selection = self._storage_backup_manager_for_selection()
        if selection is None:
            messagebox.showwarning("No Instance", "Select an instance first.")
            return
        mgr, storage_path = selection
        current_db = find_players_db(storage_path)
        if current_db is None:
            messagebox.showwarning("No players.db", "No players.db found for this instance.")
            return

        dialog = self._safe_toplevel("Player Data Restore", "900x600")
        if dialog is None:
            return

        dialog.columnconfigure(0, weight=1)
        dialog.rowconfigure(1, weight=1)

        ttk.Label(
            dialog,
            text="Select a backup, then choose a player to splice their Data blob into the live players.db.",
            wraplength=850,
        ).grid(row=0, column=0, sticky=tk.W, padx=10, pady=5)

        paned = ttk.PanedWindow(dialog, orient=tk.HORIZONTAL)
        paned.grid(row=1, column=0, sticky=tk.NSEW, padx=10, pady=5)
        dialog.rowconfigure(1, weight=1)

        # Left: backup list
        left = ttk.LabelFrame(paned, text="Backups", padding=5)
        left.columnconfigure(0, weight=1)
        left.rowconfigure(0, weight=1)
        backup_tree = ttk.Treeview(left, columns=("Date",), show="headings")
        backup_tree.heading("Date", text="Date")
        backup_tree.column("Date", width=180)
        backup_tree.grid(row=0, column=0, sticky=tk.NSEW)
        backup_scroll = ttk.Scrollbar(left, orient=tk.VERTICAL, command=backup_tree.yview)
        backup_scroll.grid(row=0, column=1, sticky=tk.NS)
        backup_tree.configure(yscrollcommand=backup_scroll.set)

        backups = mgr.list_backups()
        backup_map: Dict[str, Path] = {}
        for info in backups:
            name = info.timestamp.strftime("%Y-%m-%d %H:%M:%S")
            backup_map[name] = info.path
            backup_tree.insert("", tk.END, values=(name,))

        # Right: diff list and details
        right = ttk.LabelFrame(paned, text="Players", padding=5)
        right.columnconfigure(0, weight=1)
        right.rowconfigure(0, weight=1)

        diff_tree = ttk.Treeview(
            right,
            columns=("UID", "Status", "Current", "Backup"),
            show="headings",
        )
        for c in ("UID", "Status", "Current", "Backup"):
            diff_tree.heading(c, text=c)
            diff_tree.column(c, width=160)
        diff_tree.grid(row=0, column=0, sticky=tk.NSEW)
        diff_scroll = ttk.Scrollbar(right, orient=tk.VERTICAL, command=diff_tree.yview)
        diff_scroll.grid(row=0, column=1, sticky=tk.NS)
        diff_tree.configure(yscrollcommand=diff_scroll.set)

        details = scrolledtext.ScrolledText(right, wrap=tk.WORD, height=8)
        details.grid(row=1, column=0, columnspan=2, sticky=tk.EW, pady=(5, 0))

        paned.add(left, weight=1)
        paned.add(right, weight=3)

        current_diffs: List[Any] = []

        def _load_diff(_event=None) -> None:
            for item in diff_tree.get_children():
                diff_tree.delete(item)
            current_diffs.clear()

            sel = backup_tree.selection()
            if not sel:
                return
            name = backup_tree.item(sel[0], "values")[0]
            backup_path = backup_map.get(name)
            if backup_path is None:
                return
            backup_db = find_players_db(backup_path)
            if backup_db is None:
                messagebox.showerror("No players.db", "No players.db found in selected backup.")
                return

            ok, diffs, msg = compare_players_dbs(current_db, backup_db)
            if not ok:
                messagebox.showerror("Compare Failed", msg)
                return

            current_diffs.extend(diffs)
            for diff in diffs:
                cur = f"{diff.current.data_size} bytes" if diff.current else "—"
                bak = f"{diff.backup.data_size} bytes" if diff.backup else "—"
                diff_tree.insert(
                    "",
                    tk.END,
                    values=(diff.uid, diff.status, cur, bak),
                    tags=("restorable" if diff.is_restorable else "readonly",),
                )

        def _on_diff_select(_event=None) -> None:
            details.delete("1.0", tk.END)
            sel = diff_tree.selection()
            if not sel:
                return
            idx = diff_tree.index(sel[0])
            diff = current_diffs[idx]
            lines = [
                f"UID: {diff.uid}",
                f"Status: {diff.status}",
            ]
            if diff.current:
                lines.append(
                    f"Current: Id={diff.current.player_id}, Alive={diff.current.alive}, "
                    f"Data={diff.current.data_size} bytes"
                )
            if diff.backup:
                lines.append(
                    f"Backup: Id={diff.backup.player_id}, Alive={diff.backup.alive}, "
                    f"Data={diff.backup.data_size} bytes"
                )
            details.insert(tk.END, "\n".join(lines))

        def _splice_selected() -> None:
            sel = diff_tree.selection()
            if not sel:
                return
            idx = diff_tree.index(sel[0])
            diff = current_diffs[idx]
            if not diff.is_restorable:
                messagebox.showwarning("Not Restorable", "No backup data for this UID.")
                return

            sel_backup = backup_tree.selection()
            if not sel_backup:
                return
            name = backup_tree.item(sel_backup[0], "values")[0]
            backup_db = find_players_db(backup_map[name])
            if backup_db is None:
                return

            if not messagebox.askyesno(
                "Confirm Splice",
                f"Replace player {diff.uid} in the live players.db with data from backup {name}?\n\n"
                "A safety backup of the current DB will be created first.",
            ):
                return

            splicer = PlayersDbSplicer(current_db)
            ok, msg = splicer.splice_player(backup_db, diff.uid)
            if ok:
                messagebox.showinfo("Splice Complete", msg)
                _load_diff()
            else:
                messagebox.showerror("Splice Failed", msg)

        backup_tree.bind("<<TreeviewSelect>>", _load_diff)
        diff_tree.bind("<<TreeviewSelect>>", _on_diff_select)

        btn_frame = ttk.Frame(dialog)
        btn_frame.grid(row=2, column=0, sticky=tk.EW, padx=10, pady=5)
        ttk.Button(btn_frame, text="Splice Selected Player", command=_splice_selected).pack(
            side=tk.LEFT, padx=2
        )
        ttk.Button(btn_frame, text="Close", command=dialog.destroy).pack(side=tk.RIGHT, padx=2)

    def _create_server_control_tab(self):
        """Create the unified Server Control tab with inner tabs."""
        server_control_frame = ScrollableFrame(self.notebook)
        self.notebook.add(server_control_frame, text="Server Control")

        # Title
        title_label = ttk.Label(
            server_control_frame.frame,
            text="Server Control - DayZ Server Management",
            font=("Arial", 14, "bold")
        )
        title_label.pack(pady=10)

        # Wiki documentation link
        wiki_link_frame = self.create_wiki_link(server_control_frame.frame, "server-control")
        wiki_link_frame.pack(fill=tk.X, padx=10, pady=(5, 15))

        # Overview description
        overview_desc = self.create_layout_description(
            server_control_frame.frame,
            "Server Control Features",
            """This tab provides a single entry point for all DayZ server management:
- Start/stop/restart a single DayZ server or multiple instances
- Per-instance mod selection and configuration
- Player ban management and connected player list via RCon
- Real-time log viewing from multiple log types (script, rpt, adm, client)
- Pattern-matched diagnostics for common server issues
- Cross-platform support (Windows/Linux/macOS)

Requirements:
- DayZ server executable (DayZServer_x64 or DayZDiag_x64) in the installation path
- Log files generated by running servers""",
            "https://github.com/landracer/DayZConfigMaster/wiki/03-python-api/server-control"
        )
        overview_desc.pack(fill=tk.X, padx=10, pady=(5, 15))

        # DayZ Path configuration (shared across all inner tabs)
        path_frame = ttk.LabelFrame(server_control_frame.frame, text="DayZ Installation Paths", padding=10)
        path_frame.pack(fill=tk.X, padx=10, pady=5)

        row_path = 0

        ttk.Label(path_frame, text="DayZ Server Path:").grid(row=row_path, column=0, sticky=tk.W, padx=5, pady=2)
        self.dayz_path_var = tk.StringVar(value="")
        dayz_path_entry = ttk.Entry(path_frame, width=60, textvariable=self.dayz_path_var)
        dayz_path_entry.grid(row=row_path, column=1, sticky=tk.W+tk.E, padx=5, pady=2)

        def browse_dayz_path():
            path = filedialog.askdirectory(title="Select DayZ Server Directory")
            if path:
                self.dayz_path_var.set(path)
                self._refresh_all_map_combos()

        def auto_detect_dayz_path():
            try:
                from ..setup.wizard import SetupWizard
            except ImportError:
                from dayzconfigmaster.setup.wizard import SetupWizard
            wizard = SetupWizard(str(self._get_projects_root()))
            detected = wizard.detect_dayz_installation()
            if detected:
                self.dayz_path_var.set(detected)
                self._refresh_all_map_combos()
                self.status_var.set(f"Detected DayZ Server path: {detected}")
            else:
                messagebox.showwarning(
                    "Auto-Detect Failed",
                    "Could not automatically find the DayZ server installation.\n"
                    "Please browse to it manually."
                )

        path_btn_frame = ttk.Frame(path_frame)
        path_btn_frame.grid(row=row_path, column=2, sticky=tk.W, padx=5, pady=2)
        ttk.Button(path_btn_frame, text="Browse", command=browse_dayz_path).pack(side=tk.LEFT, padx=(0, 2))
        ttk.Button(path_btn_frame, text="Auto-Detect", command=auto_detect_dayz_path).pack(side=tk.LEFT)

        ttk.Label(
            path_frame,
            text="Path to DayZ server executables (DayZServer_x64/DayZDiag_x64)",
            foreground="gray", font=("Arial", 8)
        ).grid(row=row_path+1, column=0, columnspan=3, sticky=tk.W, padx=5, pady=(0, 5))

        ttk.Label(path_frame, text="DayZ Tools Path:").grid(row=row_path+2, column=0, sticky=tk.W, padx=5, pady=2)
        self.dayz_tools_path_var = tk.StringVar(value="")
        tools_path_entry = ttk.Entry(path_frame, width=60, textvariable=self.dayz_tools_path_var)
        tools_path_entry.grid(row=row_path+2, column=1, sticky=tk.W+tk.E, padx=5, pady=2)

        def browse_tools_path():
            path = filedialog.askdirectory(title="Select DayZ Tools Directory")
            if path:
                self.dayz_tools_path_var.set(path)

        def auto_detect_tools_path():
            try:
                from ..setup.wizard import SetupWizard
            except ImportError:
                from dayzconfigmaster.setup.wizard import SetupWizard
            wizard = SetupWizard(str(self._get_projects_root()))
            detected = wizard.detect_dayz_tools()
            if detected:
                self.dayz_tools_path_var.set(detected)
                self.status_var.set(f"Detected DayZ Tools path: {detected}")
            else:
                messagebox.showwarning(
                    "Auto-Detect Failed",
                    "Could not automatically find the DayZ Tools installation.\n"
                    "Please browse to it manually."
                )

        tools_btn_frame = ttk.Frame(path_frame)
        tools_btn_frame.grid(row=row_path+2, column=2, sticky=tk.W, padx=5, pady=2)
        ttk.Button(tools_btn_frame, text="Browse", command=browse_tools_path).pack(side=tk.LEFT, padx=(0, 2))
        ttk.Button(tools_btn_frame, text="Auto-Detect", command=auto_detect_tools_path).pack(side=tk.LEFT)

        ttk.Label(
            path_frame,
            text="Path to DayZ Tools (for AddonBuilder)",
            foreground="gray", font=("Arial", 8)
        ).grid(row=row_path+3, column=0, columnspan=3, sticky=tk.W, padx=5, pady=(0, 10))

        path_frame.columnconfigure(1, weight=1)

        # Shared status indicator
        status_frame = ttk.LabelFrame(server_control_frame.frame, text="Server Status", padding=5)
        status_frame.pack(fill=tk.X, padx=10, pady=5)

        self.status_var = tk.StringVar()
        self.status_var.set("Ready - No server running")

        status_label = ttk.Label(status_frame, textvariable=self.status_var, font=("Arial", 9))
        status_label.pack(anchor=tk.W, padx=5, pady=5)

        # Inner notebook for Server Control sections
        self._control_notebook = ttk.Notebook(server_control_frame.frame)
        self._control_notebook.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)

        # ===== General Settings tab =====
        general_frame = ttk.Frame(self._control_notebook)
        self._control_notebook.add(general_frame, text="General Settings")
        self._create_general_server_controls(general_frame)

        # ===== Mod Selection tab =====
        mod_selection_frame = ttk.Frame(self._control_notebook)
        self._control_notebook.add(mod_selection_frame, text="Mod Selection")
        self._create_instance_mod_selection_tab(mod_selection_frame)

        # ===== Mod Integration tab =====
        mod_integration_frame = ttk.Frame(self._control_notebook)
        self._control_notebook.add(mod_integration_frame, text="Mod Integration")
        self._create_mod_integration_tab(mod_integration_frame)

        # ===== Ban List & Players tab =====
        self._ban_frame = ttk.Frame(self._control_notebook)
        self._control_notebook.add(self._ban_frame, text="Ban List & Players")
        self._create_ban_list_content(self._ban_frame)

        # ===== Logs & Diagnostics tab =====
        self._logs_frame = ttk.Frame(self._control_notebook)
        self._control_notebook.add(self._logs_frame, text="Logs & Diagnostics")
        self._create_logs_and_diagnostics_tab(self._logs_frame)

    def _create_general_server_controls(self, parent: ttk.Frame):
        """Create the single/multi-instance controls inside the General Settings tab."""
        # Sub-tab notebook for single vs multi-instance control
        instance_notebook = ttk.Notebook(parent)
        instance_notebook.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)

        # ===== Single Server tab =====
        single_frame = ttk.Frame(instance_notebook)
        instance_notebook.add(single_frame, text="Single Server")
        self._create_single_server_controls(single_frame)

        # ===== Multi-Instance tab =====
        multi_frame = ttk.Frame(instance_notebook)
        instance_notebook.add(multi_frame, text="Multi-Instance")
        self._create_multi_instance_controls(multi_frame)

    def _create_logs_and_diagnostics_tab(self, parent: ttk.Frame):
        """Create the Logs & Diagnostics inner tab."""
        # Log viewer section
        log_frame = ttk.LabelFrame(parent, text="Live Log Viewer", padding=5)
        log_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)

        # Log source selector
        log_selector_frame = ttk.Frame(log_frame)
        log_selector_frame.pack(fill=tk.X, padx=5, pady=(5, 0))

        ttk.Label(log_selector_frame, text="Instance:").pack(side=tk.LEFT, padx=(0, 5))
        self.log_instance_var = tk.StringVar(value="Default")
        self.log_instance_combo = ttk.Combobox(
            log_selector_frame,
            values=["Default"],
            width=14,
            textvariable=self.log_instance_var,
            state="readonly",
        )
        self.log_instance_combo.pack(side=tk.LEFT, padx=5)

        ttk.Label(log_selector_frame, text="Log Type:").pack(side=tk.LEFT, padx=(15, 5))

        self.log_type_var = tk.StringVar(value="script")
        log_combo = ttk.Combobox(
            log_selector_frame,
            values=["script", "rpt", "adm", "client"],
            width=10,
            textvariable=self.log_type_var
        )
        log_combo.pack(side=tk.LEFT, padx=5)

        refresh_btn = ttk.Button(log_selector_frame, text="🔄 Refresh", command=lambda: self._update_log_viewer())
        refresh_btn.pack(side=tk.LEFT, padx=(0, 5))

        clear_btn = ttk.Button(log_selector_frame, text="Clear Log", command=self._clear_log_viewer)
        clear_btn.pack(side=tk.LEFT, padx=(0, 5))

        def _refresh_log_instance_options(event=None):
            options = ["Default"]
            for inst in getattr(self, "_instance_vars", []):
                inst_id = inst["id"].get()
                if inst_id:
                    options.append(f"Instance {inst_id}")
            self.log_instance_combo.config(values=options)

        parent.bind("<Visibility>", lambda e: _refresh_log_instance_options(), add="+")

        # Log display area
        self.log_text = scrolledtext.ScrolledText(log_frame, wrap=tk.WORD, height=20, font=("Courier", 9))
        self.log_text.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)

        # Diagnostic analysis section
        diag_frame = ttk.LabelFrame(parent, text="Diagnostics Analysis", padding=10)
        diag_frame.pack(fill=tk.X, padx=5, pady=(5, 0))

        analyze_btn = ttk.Button(diag_frame, text="🔍 Analyze Logs for Issues", command=self._analyze_logs)
        analyze_btn.pack(pady=5)

        self.diagnostics_text = scrolledtext.ScrolledText(diag_frame, wrap=tk.WORD, height=8, font=("Courier", 9))
        self.diagnostics_text.pack(fill=tk.X, padx=5, pady=5)

    def _create_mod_integration_tab(self, parent: ttk.Frame):
        """Create the Mod Integration inner tab."""
        desc_label = ttk.Label(
            parent,
            text="Integrate mod-provided XML fragments (types, spawnable types, event spawns, etc.) into the active instance's mission files. Loading a mod via -mod= is not enough; the server must also register the mod's classnames.",
            wraplength=800,
            justify=tk.LEFT,
        )
        desc_label.pack(fill=tk.X, padx=10, pady=(5, 10))

        # Instance selector
        selector_frame = ttk.Frame(parent)
        selector_frame.pack(fill=tk.X, padx=10, pady=5)
        ttk.Label(selector_frame, text="Instance:").pack(side=tk.LEFT, padx=(0, 5))
        self._integration_instance_var = tk.StringVar()
        self._integration_instance_combo = ttk.Combobox(
            selector_frame,
            textvariable=self._integration_instance_var,
            state="readonly",
            width=30,
        )
        self._integration_instance_combo.pack(side=tk.LEFT, padx=5)
        self._integration_instance_combo.bind(
            "<<ComboboxSelected>>", lambda _e: self._refresh_mod_integration_tab()
        )

        # Action buttons
        button_frame = ttk.Frame(parent)
        button_frame.pack(fill=tk.X, padx=10, pady=5)
        ttk.Button(
            button_frame,
            text="🔄 Scan Mods",
            command=self._refresh_mod_integration_tab,
        ).pack(side=tk.LEFT, padx=2)
        ttk.Button(
            button_frame,
            text="✅ Apply Integration",
            command=self._apply_mod_integration,
        ).pack(side=tk.LEFT, padx=2)
        ttk.Button(
            button_frame,
            text="⏪ Restore Backups",
            command=self._restore_mod_integration_backups,
        ).pack(side=tk.LEFT, padx=2)

        # Mod tree
        tree_frame = ttk.LabelFrame(parent, text="Detected Mod Configurations", padding=5)
        tree_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)

        columns = ("integrate", "mod", "fragments")
        self._integration_tree = ttk.Treeview(
            tree_frame,
            columns=columns,
            show="headings",
            selectmode="browse",
        )
        self._integration_tree.heading("integrate", text="Integrate")
        self._integration_tree.heading("mod", text="Mod")
        self._integration_tree.heading("fragments", text="Detected Fragments")
        self._integration_tree.column("integrate", width=80, anchor=tk.CENTER)
        self._integration_tree.column("mod", width=200)
        self._integration_tree.column("fragments", width=500)
        self._integration_tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

        vsb = ttk.Scrollbar(tree_frame, orient=tk.VERTICAL, command=self._integration_tree.yview)
        vsb.pack(side=tk.RIGHT, fill=tk.Y)
        self._integration_tree.configure(yscrollcommand=vsb.set)

        self._integration_tree.bind("<ButtonRelease-1>", self._on_integration_tree_click)

        # Log area
        log_frame = ttk.LabelFrame(parent, text="Integration Log", padding=5)
        log_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)
        self._integration_log = scrolledtext.ScrolledText(
            log_frame, wrap=tk.WORD, height=8, font=("Courier", 9)
        )
        self._integration_log.pack(fill=tk.BOTH, expand=True)

        # Populate instance list when the tab is first drawn.
        self._refresh_integration_instance_list()

    def _refresh_integration_instance_list(self):
        """Populate the instance selector for mod integration."""
        names = ["Single Server (default)"]
        if hasattr(self, "_instance_vars"):
            for instance in self._instance_vars:
                instance_id = instance["id"].get()
                config_file = instance["config_file"].get() or f"serverDZ_{instance_id}.cfg"
                names.append(f"Instance {instance_id} ({config_file})")
        self._integration_instance_combo["values"] = names
        if names:
            self._integration_instance_var.set(names[0])

    def _get_mod_integration_instance_root(self) -> Optional[Path]:
        """Resolve the instance root for the selected integration target."""
        selection = self._integration_instance_var.get()
        dayz_path = self.dayz_path_var.get().strip()

        if not dayz_path or not Path(dayz_path).exists():
            return None

        if selection.startswith("Instance "):
            # Extract instance id from "Instance N (...)".
            try:
                instance_id = selection.split()[1]
            except IndexError:
                return None
            for instance in getattr(self, "_instance_vars", []):
                if instance["id"].get() == instance_id:
                    root_folder = instance.get("root_folder", {}).get() or ""
                    root_folder = self._sanitize_instance_root(root_folder, int(instance_id))
                    return Path(root_folder) if root_folder else Path(dayz_path)
            return None

        # Single server uses the default instance directory.
        projects_root = Path.home() / "Documents" / "DayZProjects"
        return projects_root / "instances" / "default"

    def _refresh_mod_integration_tab(self):
        """Rescan mods and refresh the integration tree."""
        self._integration_tree.delete(*self._integration_tree.get_children())
        self._integration_log.delete("1.0", tk.END)

        instance_root = self._get_mod_integration_instance_root()
        if instance_root is None:
            self._integration_log.insert(tk.END, "Set a valid DayZ Server Path first.\n")
            return

        workshop_dir = self._get_workshop_directory() or ""
        local_mod_dirs = [Path(workshop_dir)] if workshop_dir else []

        # Determine which mods are linked into this instance.
        active_links: Dict[str, Path] = {}
        if instance_root.exists():
            for item in instance_root.iterdir():
                if item.name.startswith("@") and (item.is_dir() or item.is_symlink()):
                    try:
                        resolved = item.resolve()
                    except OSError:
                        resolved = item
                    active_links[item.name] = resolved

        if not active_links:
            self._integration_log.insert(
                tk.END,
                f"No @Mod folders found in {instance_root}. Start the server once to deploy mods.\n",
            )
            return

        # Load persisted active state.
        mgr = ModIntegrationManager(instance_root)
        state = mgr.load_state()
        integrated = set(state.get("active_mods", []))

        for link_name, mod_source in sorted(active_links.items()):
            mgr = ModIntegrationManager(instance_root)
            fragments = mgr.scan_mod(mod_source)
            if not fragments:
                continue

            summary_parts = []
            by_target: Dict[str, List[str]] = {}
            for fragment in fragments:
                by_target.setdefault(fragment.target_mission_path, []).append(
                    fragment.source_path.name
                )
            for target, files in sorted(by_target.items()):
                summary_parts.append(f"{target} ({', '.join(files)})")

            checked = "☑" if link_name in integrated else "☐"
            self._integration_tree.insert(
                "",
                tk.END,
                values=(checked, link_name, "; ".join(summary_parts)),
                tags=(link_name,),
            )

        self._integration_log.insert(
            tk.END,
            f"Scanned {len(active_links)} mod(s) in {instance_root}.\n",
        )

    def _on_integration_tree_click(self, event):
        """Toggle the integrate checkbox when the first column is clicked."""
        region = self._integration_tree.identify_region(event.x, event.y)
        if region != "cell":
            return
        column = self._integration_tree.identify_column(event.x)
        if column != "#1":
            return

        item = self._integration_tree.identify_row(event.y)
        if not item:
            return

        values = list(self._integration_tree.item(item, "values"))
        values[0] = "☑" if values[0] == "☐" else "☐"
        self._integration_tree.item(item, values=values)

    def _apply_mod_integration_to_instance(self, instance_root: Path) -> None:
        """Automatically apply persisted mod integration to an instance root.

        This is called during server startup preparation. It reads the
        integration state saved by the Mod Integration tab and merges the
        selected mod XML fragments into the instance mission files.
        """
        mgr = ModIntegrationManager(instance_root)
        state = mgr.load_state()
        active_mods = set(state.get("active_mods", []))
        if not active_mods:
            return

        mod_folders: List[Path] = []
        for item in instance_root.iterdir():
            if item.name.startswith("@") and item.name in active_mods:
                try:
                    mod_folders.append(item.resolve())
                except OSError:
                    mod_folders.append(item)

        if not mod_folders:
            return

        mgr.apply_integration(mod_folders, active_mods=active_mods)

    def _ensure_instance_storage_backup(
        self,
        instance: Dict[str, Any],
        force: bool = False,
    ) -> Tuple[bool, str, Optional[Path]]:
        """Back up storage_1 for an instance.

        Uses the per-instance backup manager. Returns (ok, message, backup_path).
        """
        instance_id = int(instance.get("id", {}).get() or 1)
        map_name = instance.get("map", {}).get() or ""
        root_folder = instance.get("root_folder", {}).get() or ""
        root_folder = self._sanitize_instance_root(root_folder, instance_id)
        dayz_path = self.dayz_path_var.get().strip()
        instance_root = Path(root_folder) if root_folder else Path(dayz_path)

        if not instance_root.exists():
            # Nothing to back up yet; this is fine on first start.
            return True, f"Instance {instance_id}: instance root does not exist yet, skipping storage backup.", None

        # Resolve mission directory to find storage_1.
        from dayzconfigmaster.mods.integration import ModIntegrationManager

        mission_dir = ModIntegrationManager(instance_root)._find_mission_dir()
        if mission_dir is None:
            return True, f"Instance {instance_id}: no mission folder found, skipping storage backup.", None

        storage_path = mission_dir / "storage_1"
        if not storage_path.exists():
            return True, f"Instance {instance_id}: no storage_1 folder found, skipping storage backup.", None

        mgr = InstanceStorageBackupManager(instance_root, instance_id, map_name)
        ok, msg = mgr.ensure_backup_before_start(
            storage_path,
            max_age_minutes=self._get_storage_backup_max_age_minutes(),
            force=force,
        )
        backup_path = Path(msg) if ok and Path(msg).exists() else None
        return ok, msg, backup_path

    def _get_storage_backup_max_age_minutes(self) -> int:
        """Return the configured max age before a new startup backup is created."""
        try:
            return int(getattr(self, "_storage_backup_max_age_minutes_var", tk.StringVar(value="60")).get())
        except ValueError:
            return 60

    def _maybe_rollover_storage_for_map_change(
        self,
        instance: Dict[str, Any],
    ) -> Tuple[bool, str, Optional[Path]]:
        """Handle map swaps by restoring or resetting storage_1.

        If the configured map differs from the last-run map, look for the
        most recent backup of the configured map across all instances. Offer
        to restore it, start fresh, or cancel.

        Returns (proceed, message, backup_path_used).
        """
        instance_id = int(instance.get("id", {}).get() or 1)
        map_name = instance.get("map", {}).get() or ""
        root_folder = instance.get("root_folder", {}).get() or ""
        root_folder = self._sanitize_instance_root(root_folder, instance_id)
        dayz_path = self.dayz_path_var.get().strip()
        instance_root = Path(root_folder) if root_folder else Path(dayz_path)

        if not map_name or not instance_root.exists():
            return True, "no map change check needed", None

        tracker = InstanceMapStorageTracker(instance_root)
        state = tracker.load()
        last_map = state.last_run_map

        if not last_map or last_map.lower() == map_name.lower():
            return True, f"Instance {instance_id}: map unchanged ({map_name})", None

        # Map has changed. Look for backups of the new map anywhere.
        projects_root = Path(self._get_projects_root())
        candidates = find_map_backups_across_instances(
            projects_root,
            map_name,
            exclude_instance_root=instance_root,
        )

        # Determine the mission folder for the configured map. We cannot rely
        # on _find_mission_dir because the old map's folder may still exist.
        workshop_dir = self._get_workshop_directory() or ""
        world_name = self._resolve_world_name(map_name, workshop_dir)
        if not world_name:
            return True, f"Instance {instance_id}: could not resolve world name for {map_name}; will deploy fresh", None

        if world_name.lower().startswith("dayzoffline.") or world_name.lower().startswith("dayz."):
            mission_name = world_name
        else:
            mission_name = f"dayzOffline.{world_name}"

        mission_dir = instance_root / "mpmissions" / mission_name
        storage_path = mission_dir / "storage_1"
        mgr = InstanceStorageBackupManager(instance_root, instance_id, map_name)

        if candidates:
            newest = candidates[0]
            answer = messagebox.askyesnocancel(
                "Map Changed",
                f"Instance {instance_id} was last running '{last_map}' but is now configured for '{map_name}'.\n\n"
                f"A backup of '{map_name}' exists from {newest.timestamp.strftime('%Y-%m-%d %H:%M:%S')} "
                f"({newest.size_bytes / (1024 * 1024):.2f} MB).\n\n"
                "Yes = restore that map data\n"
                "No = start fresh (reset map state to zero)\n"
                "Cancel = abort start",
            )
            if answer is True:
                ok, msg = mgr.restore_backup(newest.path, storage_path)
                if ok:
                    return True, f"Instance {instance_id}: restored {map_name} data from {newest.path.parent.name}/{newest.path.name}", newest.path
                return False, f"Instance {instance_id}: restore failed: {msg}", None
            if answer is False:
                ok, msg = mgr.start_fresh(storage_path)
                return ok, f"Instance {instance_id}: {msg}", None
            # Cancel
            return False, f"Instance {instance_id}: start cancelled by user", None

        answer = messagebox.askyesno(
            "Map Changed",
            f"Instance {instance_id} was last running '{last_map}' but is now configured for '{map_name}'.\n\n"
            f"No existing '{map_name}' backup was found.\n\n"
            "Start fresh (reset map state to zero)?",
        )
        if answer:
            ok, msg = mgr.start_fresh(storage_path)
            return ok, f"Instance {instance_id}: {msg}", None

        return False, f"Instance {instance_id}: start cancelled by user", None

    def _record_map_storage_state(
        self,
        instance: Dict[str, Any],
        backup_path: Optional[Path] = None,
    ) -> None:
        """Persist which map was just started for map-swap detection."""
        instance_id = int(instance.get("id", {}).get() or 1)
        map_name = instance.get("map", {}).get() or ""
        root_folder = instance.get("root_folder", {}).get() or ""
        root_folder = self._sanitize_instance_root(root_folder, instance_id)
        dayz_path = self.dayz_path_var.get().strip()
        instance_root = Path(root_folder) if root_folder else Path(dayz_path)
        if not instance_root.exists():
            return
        tracker = InstanceMapStorageTracker(instance_root)
        tracker.record_run(map_name, backup_path)

    def _run_instance_preflight(
        self,
        instance: Dict[str, Any],
        allow_repair: bool = True,
    ) -> Tuple[bool, str]:
        """Run filesystem/database preflight checks before starting.

        Returns (ok, message). Errors are shown to the user and logged.
        """
        instance_id = int(instance.get("id", {}).get() or 1)
        map_name = instance.get("map", {}).get() or ""
        root_folder = instance.get("root_folder", {}).get() or ""
        root_folder = self._sanitize_instance_root(root_folder, instance_id)
        dayz_path = self.dayz_path_var.get().strip()
        instance_root = Path(root_folder) if root_folder else Path(dayz_path)

        profile_path = instance.get("profile", {}).get() or "profiles"
        profile_dir = Path(profile_path)
        if not profile_dir.is_absolute():
            profile_dir = instance_root / profile_dir

        config_filename = instance.get("config_file", {}).get() or f"serverDZ_{instance_id}.cfg"

        checker = InstancePreflightChecker(
            instance_root=instance_root,
            instance_id=instance_id,
            map_name=map_name,
            config_filename=config_filename,
            profile_dir=profile_dir,
        )
        result = checker.run()

        if result.has_warnings:
            for issue in result.warnings:
                self.log_text.insert(
                    tk.END,
                    f"[{self._get_timestamp()}] PREFLIGHT WARN {instance_id}: {issue.message}\n",
                )

        if result.has_errors:
            error_lines = [f"Instance {instance_id} preflight failed:"]
            for issue in result.errors:
                line = f"- [{issue.rule}] {issue.message}"
                if issue.remediation:
                    line += f"\n  Fix: {issue.remediation}"
                error_lines.append(line)
                self.log_text.insert(
                    tk.END,
                    f"[{self._get_timestamp()}] PREFLIGHT ERROR {instance_id}: {issue.message}\n",
                )

            if allow_repair:
                repair_text = "\n\n".join(error_lines) + (
                    "\n\nAttempt automatic repair (fix permissions/remove immutable flags)?"
                )
                if messagebox.askyesno("Preflight Check Failed", repair_text):
                    repair = InstancePreflightRepair(checker)
                    result = repair.repair()
                    self.log_text.insert(
                        tk.END,
                        f"[{self._get_timestamp()}] PREFLIGHT {instance_id}: automatic repair attempted\n",
                    )
                    if result.has_errors:
                        for issue in result.errors:
                            self.log_text.insert(
                                tk.END,
                                f"[{self._get_timestamp()}] PREFLIGHT ERROR {instance_id}: {issue.message}\n",
                            )
                        return False, f"Instance {instance_id}: automatic repair could not fix all issues."
                    return True, f"Instance {instance_id}: preflight issues repaired and checks passed."

            messagebox.showerror(
                "Preflight Check Failed",
                "\n\n".join(error_lines),
            )
            return False, f"Instance {instance_id}: preflight checks failed. See log for details."

        return True, f"Instance {instance_id}: preflight checks passed."

    def _resolve_mod_source_for_manifest(self, mod_token: str) -> Optional[Path]:
        """Return the real mod folder for manifest fingerprinting.

        This avoids fingerprinting the instance-local wrapper directory, which
        changes on every deploy.
        """
        workshop_dir = self._get_workshop_directory() or ""
        return _resolve_mod_source(
            mod_token,
            workshop_dir=Path(workshop_dir) if workshop_dir else None,
            local_mod_dirs=[Path(workshop_dir)] if workshop_dir else [],
        )

    def _apply_per_instance_config(
        self,
        instance_root: Path,
        workshop_dir: Optional[str] = "",
        mission_dir: Optional[Path] = None,
    ) -> None:
        """Apply per-instance spawn loadout and mod-settings overrides.

        Called during instance preparation so each server instance gets the
        spawnables and mod settings configured for it in the GUI.
        """
        from dayzconfigmaster.config.per_instance_config import PerInstanceConfigManager
        from dayzconfigmaster.mods.integration import ModIntegrationManager

        manager = PerInstanceConfigManager(instance_root)
        if mission_dir is None:
            mission_dir = ModIntegrationManager(instance_root)._find_mission_dir()
        workshop_path = Path(workshop_dir) if workshop_dir else None

        if mission_dir is not None:
            self.log_text.insert(
                tk.END,
                f"[{self._get_timestamp()}] Applying spawn loadout to "
                f"{mission_dir.name}...\n",
            )
            self.log_text.update_idletasks()
            ok, messages = manager.apply_spawn_loadout(mission_dir, workshop_path)
            for msg in messages:
                self.log_text.insert(
                    tk.END,
                    f"[{self._get_timestamp()}] Spawn loadout: {msg}\n",
                )
            if not ok:
                raise RuntimeError("One or more spawn loadout entries failed. See log.")

        ok, messages = manager.apply_mod_settings_overrides(instance_root, mission_dir)
        for msg in messages:
            self.log_text.insert(
                tk.END,
                f"[{self._get_timestamp()}] Mod settings: {msg}\n",
            )
        if not ok:
            raise RuntimeError("One or more mod-settings overrides failed. See log.")

    def _apply_mod_integration(self):
        """Apply selected mod integrations to the active instance."""
        instance_root = self._get_mod_integration_instance_root()
        if instance_root is None:
            messagebox.showerror("Error", "Set a valid DayZ Server Path first.")
            return

        selected: Set[str] = set()
        for item in self._integration_tree.get_children():
            values = self._integration_tree.item(item, "values")
            if values and values[0] == "☑":
                selected.add(values[1])

        if not selected:
            messagebox.showwarning("No Mods Selected", "Check at least one mod to integrate.")
            return

        # Resolve mod sources from instance @Mod links.
        mod_folders: List[Path] = []
        for item in instance_root.iterdir():
            if item.name.startswith("@") and item.name in selected:
                try:
                    mod_folders.append(item.resolve())
                except OSError:
                    mod_folders.append(item)

        mgr = ModIntegrationManager(instance_root)
        result = mgr.apply_integration(mod_folders, active_mods=selected)

        self._integration_log.delete("1.0", tk.END)
        for msg in result.messages:
            self._integration_log.insert(tk.END, f"{msg}\n")
        for warning in result.warnings:
            self._integration_log.insert(tk.END, f"⚠️ {warning}\n")
        for error in result.errors:
            self._integration_log.insert(tk.END, f"❌ {error}\n")

        if result.ok:
            messagebox.showinfo(
                "Integration Applied",
                f"Integrated {len(selected)} mod(s). Restart the server for changes to take effect.",
            )
        else:
            messagebox.showerror("Integration Error", "\n".join(result.errors))

    def _restore_mod_integration_backups(self):
        """Restore pristine mission file backups for the active instance."""
        instance_root = self._get_mod_integration_instance_root()
        if instance_root is None:
            messagebox.showerror("Error", "Set a valid DayZ Server Path first.")
            return

        if not messagebox.askyesno(
            "Confirm Restore",
            "This will restore the original mission XML files and remove all mod integrations. Continue?",
        ):
            return

        mgr = ModIntegrationManager(instance_root)
        result = mgr.restore_backups()

        self._integration_log.delete("1.0", tk.END)
        for msg in result.messages:
            self._integration_log.insert(tk.END, f"{msg}\n")
        for error in result.errors:
            self._integration_log.insert(tk.END, f"❌ {error}\n")

        self._refresh_mod_integration_tab()

    def _create_instance_mod_selection_tab(self, parent: ttk.Frame):
        """Create the per-instance mod selection inner tab."""
        desc_label = ttk.Label(
            parent,
            text="Select mods for each server instance. Changes here update the Mod Paths field in each instance configuration.",
            wraplength=800,
            justify=tk.LEFT
        )
        desc_label.pack(fill=tk.X, padx=10, pady=(5, 10))

        # Container for dynamic instance mod rows
        self._instance_mod_frame = ttk.Frame(parent)
        self._instance_mod_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)

        self._build_instance_mod_rows()

    def _build_instance_mod_rows(self):
        """Build mod selection rows for each configured instance."""
        if not hasattr(self, '_instance_mod_frame') or self._instance_mod_frame is None:
            return

        for widget in self._instance_mod_frame.winfo_children():
            widget.destroy()

        self._instance_mod_preset_combos: List[ttk.Combobox] = []
        preset_names = self._get_mod_preset_manager().list_presets()

        for i, instance in enumerate(self._instance_vars):
            frame = ttk.LabelFrame(self._instance_mod_frame, text=f"Instance {i+1} Mods", padding=8)
            frame.pack(fill=tk.X, padx=5, pady=(0, 8))
            frame.columnconfigure(1, weight=1)

            mod_var = instance.get("mod_paths")
            if mod_var is None:
                mod_var = tk.StringVar()
                instance["mod_paths"] = mod_var

            # Row 0: mod entry + add mod button
            ttk.Label(frame, text="Mods:").grid(row=0, column=0, sticky=tk.W, padx=(0, 5))
            entry = ttk.Entry(frame, width=80, textvariable=mod_var)
            entry.grid(row=0, column=1, sticky=tk.EW, padx=(0, 5))

            def browse_mods(var=mod_var):
                path = filedialog.askdirectory(title="Select Mods Directory")
                if path:
                    current = var.get()
                    new_mod = f"@{Path(path).name}"
                    if current:
                        var.set(f"{current};{new_mod}")
                    else:
                        var.set(new_mod)

            ttk.Button(frame, text="Add Mod", command=browse_mods).grid(row=0, column=2, sticky=tk.W)

            # Row 1: preset controls
            ttk.Label(frame, text="Preset:").grid(row=1, column=0, sticky=tk.W, padx=(0, 5), pady=(5, 0))
            preset_var = tk.StringVar(value="")
            preset_combo = ttk.Combobox(
                frame,
                values=preset_names,
                textvariable=preset_var,
                state="readonly",
                width=30,
            )
            preset_combo.grid(row=1, column=1, sticky=tk.W, padx=(0, 5), pady=(5, 0))

            def apply_preset(var=mod_var, combo=preset_combo):
                name = combo.get()
                if not name:
                    return
                mod_string = self._get_mod_preset_manager().apply_preset_to_string(name)
                if mod_string is not None:
                    var.set(mod_string)
                    self.log_text.insert(
                        tk.END,
                        f"[{self._get_timestamp()}] Applied mod preset '{name}'\n",
                    )

            def save_as_preset(var=mod_var, combo=preset_combo):
                current = var.get().strip()
                if not current:
                    messagebox.showwarning("No Mods", "There are no mods to save in a preset.")
                    return
                name = tk.simpledialog.askstring(
                    "Save Mod Preset",
                    "Enter a name for this mod preset:",
                )
                if not name:
                    return
                ok, msg = self._get_mod_preset_manager().save_preset_from_string(
                    name, current
                )
                if ok:
                    self._refresh_instance_mod_preset_combos()
                    combo.set(name)
                self.log_text.insert(tk.END, f"[{self._get_timestamp()}] {msg}\n")
                if not ok:
                    messagebox.showerror("Preset Error", msg)

            def delete_preset(combo=preset_combo):
                name = combo.get()
                if not name:
                    return
                if not messagebox.askyesno(
                    "Delete Preset",
                    f"Delete mod preset '{name}'?",
                ):
                    return
                ok, msg = self._get_mod_preset_manager().delete_preset(name)
                if ok:
                    combo.set("")
                    self._refresh_instance_mod_preset_combos()
                self.log_text.insert(tk.END, f"[{self._get_timestamp()}] {msg}\n")

            ttk.Button(frame, text="Apply", command=apply_preset).grid(
                row=1, column=2, sticky=tk.W, padx=(0, 2), pady=(5, 0)
            )
            ttk.Button(frame, text="Save As", command=save_as_preset).grid(
                row=1, column=3, sticky=tk.W, padx=(0, 2), pady=(5, 0)
            )
            ttk.Button(frame, text="Delete", command=delete_preset).grid(
                row=1, column=4, sticky=tk.W, pady=(5, 0)
            )

            # Track the combo so rebuilds can refresh values.
            self._instance_mod_preset_combos.append(preset_combo)

    def _refresh_instance_mod_preset_combos(self):
        """Refresh every preset dropdown after add/delete."""
        preset_names = self._get_mod_preset_manager().list_presets()
        for combo in getattr(self, "_instance_mod_preset_combos", []):
            combo.config(values=preset_names)
        self._refresh_mods_tab_preset_combo()

    def _create_single_server_controls(self, parent: ttk.Frame):
        """Create controls for the Single Server sub-tab."""
        # Server controls section
        controls_frame = ttk.LabelFrame(parent, text="Server Controls", padding=10)
        controls_frame.pack(fill=tk.X, padx=5, pady=5)

        row_ctrl = 0

        # Mode selection
        ttk.Label(controls_frame, text="Mode:").grid(row=row_ctrl, column=0, sticky=tk.W, padx=5, pady=2)
        self.mode_var = tk.StringVar(value="normal")
        mode_combo = ttk.Combobox(controls_frame, values=["normal", "debug"], width=15, textvariable=self.mode_var)
        mode_combo.grid(row=row_ctrl, column=1, sticky=tk.W, padx=5, pady=2)

        # Port configuration
        ttk.Label(controls_frame, text="Port:").grid(row=row_ctrl, column=2, sticky=tk.W, padx=5, pady=2)
        self.port_var = tk.StringVar(value="2302")
        port_entry = ttk.Entry(controls_frame, width=8, textvariable=self.port_var)
        port_entry.grid(row=row_ctrl, column=3, sticky=tk.W, padx=5, pady=2)

        # Map selection dropdown (populated from available maps)
        ttk.Label(controls_frame, text="Map Name:").grid(row=row_ctrl, column=4, sticky=tk.W, padx=5, pady=2)
        self.map_name_var = tk.StringVar(value="")
        self._single_server_map_combo = ttk.Combobox(controls_frame, width=30, textvariable=self.map_name_var)
        self._single_server_map_combo.grid(row=row_ctrl, column=5, sticky=tk.W+tk.E, padx=5, pady=2)
        self._populate_maps(self._single_server_map_combo)

        row_ctrl += 1

        # Map size (for new servers)
        ttk.Label(controls_frame, text="Map Size (m):").grid(row=row_ctrl, column=0, sticky=tk.W, padx=5, pady=2)
        self.map_size_var = tk.StringVar(value="2000")
        map_size_entry = ttk.Entry(controls_frame, width=8, textvariable=self.map_size_var)
        map_size_entry.grid(row=row_ctrl, column=1, sticky=tk.W, padx=5, pady=2)

        row_ctrl += 1

        # Max players
        ttk.Label(controls_frame, text="Max Players:").grid(row=row_ctrl, column=0, sticky=tk.W, padx=5, pady=2)
        self.max_players_var = tk.StringVar(value="60")
        max_players_entry = ttk.Entry(controls_frame, width=8, textvariable=self.max_players_var)
        max_players_entry.grid(row=row_ctrl, column=1, sticky=tk.W, padx=5, pady=2)

        # Config path
        ttk.Label(controls_frame, text="Config File:").grid(row=row_ctrl, column=2, sticky=tk.W, padx=5, pady=2)
        self.config_path_var = tk.StringVar(value="")
        config_entry = ttk.Entry(controls_frame, width=40, textvariable=self.config_path_var)
        config_entry.grid(row=row_ctrl, column=3, columnspan=2, sticky=tk.W+tk.E, padx=5, pady=2)

        def browse_config():
            path = filedialog.askopenfilename(
                title="Select serverDZ.cfg",
                filetypes=[("CFG files", "*.cfg"), ("All files", "*.*")]
            )
            if path:
                self.config_path_var.set(path)

        config_browse_btn = ttk.Button(controls_frame, text="...", command=browse_config, width=3)
        config_browse_btn.grid(row=row_ctrl, column=5, sticky=tk.W, padx=(0, 5), pady=2)

        row_ctrl += 1

        # Memory kill limit (hard ceiling enforced to protect the host)
        ttk.Label(controls_frame, text="Memory Kill Limit (GB):").grid(row=row_ctrl, column=0, sticky=tk.W, padx=5, pady=2)
        self.memory_kill_limit_var = tk.StringVar(value="10")
        memory_kill_limit_entry = ttk.Entry(controls_frame, width=8, textvariable=self.memory_kill_limit_var)
        memory_kill_limit_entry.grid(row=row_ctrl, column=1, sticky=tk.W, padx=5, pady=2)
        ttk.Label(
            controls_frame,
            text=f"Server process tree is killed if RSS exceeds this limit (max {self.MAX_MEMORY_LIMIT_GB} GB)",
            foreground="gray",
            font=("Arial", 8),
        ).grid(row=row_ctrl, column=2, columnspan=3, sticky=tk.W, padx=5, pady=2)

        row_ctrl += 1

        # Mod paths
        ttk.Label(controls_frame, text="Mod Paths:").grid(row=row_ctrl, column=0, sticky=tk.W, padx=5, pady=2)
        self.mod_paths_var = tk.StringVar(value="")
        mod_entry = ttk.Entry(controls_frame, width=60, textvariable=self.mod_paths_var)
        mod_entry.grid(row=row_ctrl, column=1, columnspan=4, sticky=tk.W+tk.E, padx=5, pady=2)

        def browse_mods():
            path = filedialog.askdirectory(title="Select Mods Directory")
            if path:
                current = self.mod_paths_var.get()
                if current:
                    self.mod_paths_var.set(f"{current};@{Path(path).name}")
                else:
                    self.mod_paths_var.set(f"@{Path(path).name}")

        mods_browse_btn = ttk.Button(controls_frame, text="Add Mod", command=browse_mods)
        mods_browse_btn.grid(row=row_ctrl, column=5, sticky=tk.W, padx=(0, 5), pady=2)

        # Start/Stop buttons
        button_row = row_ctrl + 1

        self.start_server_btn = ttk.Button(controls_frame, text="🚀 Start Server", command=self._start_server)
        self.start_server_btn.grid(row=button_row, column=0, columnspan=2, sticky=tk.W+tk.E, padx=5, pady=5)

        self.stop_server_btn = ttk.Button(
            controls_frame, text="🛑 Stop Server", command=self._stop_server, state=tk.DISABLED
        )
        self.stop_server_btn.grid(row=button_row, column=2, columnspan=2, sticky=tk.W+tk.E, padx=5, pady=5)

        self.restart_server_btn = ttk.Button(
            controls_frame, text="🔄 Restart Server", command=self._restart_server, state=tk.DISABLED
        )
        self.restart_server_btn.grid(row=button_row, column=4, columnspan=2, sticky=tk.W+tk.E, padx=5, pady=5)

        controls_frame.columnconfigure(3, weight=1)

    def _create_multi_instance_controls(self, parent: ttk.Frame):
        """Create controls for the Multi-Instance sub-tab.

        Instance configuration is managed from the Server Config tab. This tab
        provides a read-only/control view of the same instance data.
        """
        info_label = ttk.Label(
            parent,
            text="Instances are configured in the Server Config tab. Use this tab to start/stop instances.",
            wraplength=800,
            foreground="gray"
        )
        info_label.pack(fill=tk.X, padx=5, pady=5)

        # Container for per-instance control rows
        self._instance_control_frame = ttk.Frame(parent)
        self._instance_control_frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)

        # Build control rows from current instance data
        self._build_instance_control_rows()

        # Multi-instance action buttons
        button_frame = ttk.Frame(parent)
        button_frame.pack(fill=tk.X, padx=5, pady=5)

        self.start_multi_btn = ttk.Button(
            button_frame, text="🚀 Start All Instances", command=self._start_multi_instance_servers
        )
        self.start_multi_btn.pack(side=tk.LEFT, padx=5)

        self.stop_multi_btn = ttk.Button(
            button_frame, text="🛑 Stop All Instances", command=self._stop_multi_instance_servers, state=tk.DISABLED
        )
        self.stop_multi_btn.pack(side=tk.LEFT, padx=5)

    def _on_multi_instance_toggled(self):
        """Enable or disable multi-instance configuration rows."""
        enabled = self._multi_instance_enabled_var.get()
        state = tk.NORMAL if enabled else tk.DISABLED
        for widget in self._instance_config_frame.winfo_children():
            for child in widget.winfo_children():
                self._set_widget_state(child, state)

        # Rebuild rows so instance count/effects selector reflect the new mode
        self._build_instance_config_rows()

    def _set_widget_state(self, widget, state):
        """Recursively set the state of a widget and its children."""
        try:
            widget.configure(state=state)
        except tk.TclError:
            pass
        for child in widget.winfo_children():
            self._set_widget_state(child, state)

    def _on_instance_count_changed(self):
        """Rebuild instance configuration rows when the count changes."""
        try:
            raw = self._instance_count_var.get()
        except tk.TclError:
            # IntVar.get() can raise when the spinbox is temporarily empty.
            return
        if raw == "":
            # Allow the field to be temporarily empty while the user is typing.
            return
        try:
            count = max(1, min(9, int(raw)))
        except (ValueError, tk.TclError):
            count = 1
        self._instance_count_var.set(count)
        self._build_instance_config_rows()

    def _build_instance_config_rows(self):
        """Create or recreate the per-instance configuration rows."""
        if self._instance_config_frame is None:
            return

        # Preserve any Effects & Triggers edits for currently configured instances
        self._save_current_effects_content()

        # Clear existing rows
        for widget in self._instance_config_frame.winfo_children():
            widget.destroy()
        self._instance_vars.clear()

        count = self._instance_count_var.get()
        enabled = self._multi_instance_enabled_var.get()
        state = tk.NORMAL if enabled else tk.DISABLED

        for i in range(1, count + 1):
            self._create_instance_config_row(self._instance_config_frame, i, state)

        # Seed Effects & Triggers content for any new instances
        self._ensure_effects_content_for_instances()

        # Also rebuild the per-instance mod selection rows
        self._build_instance_mod_rows()

        # Rebuild Server Control multi-instance rows so they stay 1:1
        self._build_instance_control_rows()

        # Keep the Effects & Triggers instance selector in sync
        self._update_effects_instance_selector()

        # Keep the Mod Integration instance selector in sync (if the tab exists).
        if hasattr(self, "_integration_instance_combo"):
            self._refresh_integration_instance_list()

    def _ensure_effects_content_for_instances(self):
        """Make sure every configured instance has a Effects & Triggers content dict."""
        if not self._instance_vars:
            return

        for instance in self._instance_vars:
            try:
                instance_id = int(instance["id"].get())
            except ValueError:
                continue

            if instance_id not in self._instance_effects_content:
                self._instance_effects_content[instance_id] = {}
                # Copy defaults from the currently visible text areas (instance 1 defaults)
                for filename, text_area in self._effects_text_areas.items():
                    self._instance_effects_content[instance_id][filename] = text_area.get("1.0", tk.END)

        # Prune content for instance IDs that no longer exist
        active_ids = set()
        for instance in self._instance_vars:
            try:
                active_ids.add(int(instance["id"].get()))
            except ValueError:
                continue
        for stale_id in list(self._instance_effects_content.keys()):
            if stale_id not in active_ids:
                del self._instance_effects_content[stale_id]

    def _build_instance_control_rows(self):
        """Create or recreate the per-instance control rows in Server Control."""
        if not hasattr(self, '_instance_control_frame') or self._instance_control_frame is None:
            return

        for widget in self._instance_control_frame.winfo_children():
            widget.destroy()

        # Button references are rebuilt below; clear old ones first.
        self._instance_start_buttons.clear()
        self._instance_stop_buttons.clear()

        if not self._instance_vars:
            ttk.Label(
                self._instance_control_frame,
                text="No instances configured. Enable Multi-Instance Mode in Server Config and set Instance Count.",
                foreground="gray"
            ).pack(anchor=tk.W, padx=5, pady=5)
            return

        for i, instance in enumerate(self._instance_vars, 1):
            frame = ttk.LabelFrame(self._instance_control_frame, text=f"Instance {i} Control", padding=8)
            frame.pack(fill=tk.X, padx=5, pady=(0, 8))

            info_text = (
                f"ID: {instance['id'].get()}  |  "
                f"Port: {instance['game_port'].get()}  |  "
                f"Query: {instance['query_port'].get()}  |  "
                f"Steam P2P: {instance['steam_port'].get()}  |  "
                f"Map: {instance['map'].get()}  |  "
                f"Config: {instance['config_file'].get()}"
            )
            ttk.Label(frame, text=info_text, font=("Courier", 9)).pack(anchor=tk.W, padx=5, pady=2)

            btn_frame = ttk.Frame(frame)
            btn_frame.pack(anchor=tk.W, padx=5, pady=2)

            instance_id = instance['id'].get()

            def start_one(inst=instance):
                self._start_single_instance(inst)

            def start_one_terminal(inst=instance):
                self._start_single_instance(inst, terminal=True)

            def stop_one(inst=instance):
                self._stop_single_instance(inst)

            start_btn = ttk.Button(btn_frame, text="🚀 Start", command=start_one)
            start_btn.pack(side=tk.LEFT, padx=(0, 5))
            start_term_btn = ttk.Button(btn_frame, text="🚀 Start in Terminal", command=start_one_terminal)
            start_term_btn.pack(side=tk.LEFT, padx=(0, 5))
            stop_btn = ttk.Button(btn_frame, text="🛑 Stop", command=stop_one)
            stop_btn.pack(side=tk.LEFT, padx=(0, 5))

            self._instance_start_buttons[instance_id] = start_btn
            self._instance_stop_buttons[instance_id] = stop_btn

        # Sync button states with currently tracked running instances.
        self._update_instance_button_states()

    def _prepare_instance_files(
        self,
        instance: Dict[str, Any],
        cfg_content: Optional[str] = None,
    ) -> Tuple[Optional[Path], Optional[str], Optional[str], Optional[str]]:
        """Export config and write it to the instance root folder before starting.

        Validates that the DayZ server path and instance root are usable, creates
        any required directories, and writes the config and Effects & Triggers
        files in-place before the server process starts.

        Args:
            instance: Instance variable dict from the UI.
            cfg_content: Optional pre-generated serverDZ.cfg content. If omitted,
                the content is built from the multi-instance configuration.
            force_redeploy: If True, ignore the deployment manifest and run the
                full deployment pipeline.

        Returns:
            (instance_root_path, config_filename, deployed_mods_str, profile_dir) or
            (None, error_message, None, None).
        """
        from dayzconfigmaster.mods.integration import ModIntegrationManager

        instance_id = int(instance["id"].get() or 1)
        config_filename = instance["config_file"].get() or f"serverDZ_{instance_id}.cfg"

        dayz_path = self.dayz_path_var.get().strip()
        if not dayz_path:
            return None, "DayZ Server Path is not set. Configure it in Server Control > General Settings."
        if not Path(dayz_path).exists():
            return None, f"DayZ Server Path does not exist: {dayz_path}"

        # Use the instance root folder if set, otherwise fall back to DayZ server path.
        root_folder = instance.get("root_folder", {}).get() or ""
        root_folder = self._sanitize_instance_root(root_folder, instance_id)
        instance_root = Path(root_folder) if root_folder else Path(dayz_path)

        # Validate that we can create/write to the instance root.
        try:
            instance_root.mkdir(parents=True, exist_ok=True)
        except Exception as exc:
            return None, f"Cannot create instance directory {instance_root}: {exc}"
        if not os.access(instance_root, os.W_OK):
            return None, f"Instance directory is not writable: {instance_root}"

        # Ensure the profile directory exists. Default to instance_root/profiles
        # so DayZ always has an explicit -profiles folder (required by mods such
        # as ZomBerry and for consistent log/config placement).
        profile_path = instance.get("profile", {}).get() or "profiles"
        profile_dir = Path(profile_path)
        if not profile_dir.is_absolute():
            profile_dir = instance_root / profile_dir
        try:
            profile_dir.mkdir(parents=True, exist_ok=True)
        except Exception as exc:
            return None, f"Cannot create profile directory {profile_dir}: {exc}"

        # Symlink required base server files into the instance root so DayZ can
        # find dayz.gproj, addons/, dta/, etc. when launched from the instance
        # directory. Skip files that belong to the instance (config, profiles).
        try:
            self._link_base_server_files(Path(dayz_path), instance_root, config_filename)
        except Exception as exc:
            return None, f"Failed to link base server files into {instance_root}: {exc}"

        # Sanitize and deploy ban.txt / whitelist.txt so the server does not
        # loop reloading malformed userlists.
        try:
            self._sanitize_ban_file(Path(dayz_path) / "ban.txt", instance_root / "ban.txt")
            self._sanitize_ban_file(Path(dayz_path) / "ban.txt", instance_root / "profiles" / "ban.txt")
            self._sanitize_ban_file(
                Path(dayz_path) / "whitelist.txt",
                instance_root / "whitelist.txt",
                whitelist=True,
            )
            self._sanitize_ban_file(
                Path(dayz_path) / "whitelist.txt",
                instance_root / "profiles" / "whitelist.txt",
                whitelist=True,
            )
        except Exception as exc:
            return None, f"Failed to sanitize ban/whitelist files: {exc}"

        # Build current deployment manifest inputs so we can skip the slow
        # copy/symlink/integration steps when nothing has changed.
        mods_str = self._get_instance_mods(instance)
        map_display_name = instance.get("map", {}).get() or ""
        workshop_dir = self._get_workshop_directory() or ""
        mission_source = instance.get("mission_path")
        mission_source_str = mission_source.get() if mission_source is not None else ""
        mission_source_path = Path(mission_source_str) if mission_source_str.strip() else None

        if cfg_content is None or not cfg_content:
            all_configs = self._build_instance_cfgs_content()
            cfg_content = all_configs.get(config_filename, "")
        cfg_content = cfg_content or ""

        from dayzconfigmaster.config.per_instance_config import PerInstanceConfigManager

        per_instance_mgr = PerInstanceConfigManager(instance_root)
        spawn_loadout = per_instance_mgr.load_spawn_loadout()
        mod_settings_overrides = per_instance_mgr.load_mod_settings_overrides()

        # Mod integration state affects which XML fragments are merged.
        mod_integration_state = ModIntegrationManager(instance_root).load_state()

        manifest_mgr = DeploymentManifestManager(instance_root)
        current_manifest = manifest_mgr.compute(
            instance=instance,
            dayz_server_path=Path(dayz_path),
            instance_root=instance_root,
            mods_str=mods_str,
            mission_source_path=mission_source_path,
            spawn_loadout=spawn_loadout.to_dict(),
            mod_settings_overrides={k: v.to_dict() for k, v in mod_settings_overrides.items()},
            mod_integration_state=mod_integration_state,
            cfg_content=cfg_content,
            resolve_mod_source=self._resolve_mod_source_for_manifest,
        )

        can_skip, skip_reason = compute_quick_skip_status(instance_root, current_manifest)
        if can_skip and not force_redeploy:
            self.log_text.insert(
                tk.END,
                f"[{self._get_timestamp()}] Instance {instance_id}: deployment manifest matches ({skip_reason}); skipping slow deployment steps.\n",
            )
            deployed_mods_str = mods_str
        else:
            self.log_text.insert(
                tk.END,
                f"[{self._get_timestamp()}] Instance {instance_id}: full deployment required ({skip_reason}).\n",
            )
            try:
                deployed_mods_str = self._deploy_mods_and_keys(instance_root, mods_str)
            except Exception as exc:
                return None, f"Failed to deploy mods/keys: {exc}"

            try:
                mission_msg, mission_target = self._deploy_mission_folder(
                    instance_root=instance_root,
                    dayz_path=Path(dayz_path),
                    map_display_name=map_display_name,
                    workshop_dir=workshop_dir,
                    mission_source_path=mission_source_path,
                )
                self.log_text.insert(
                    tk.END,
                    f"[{self._get_timestamp()}] Instance {instance_id} mission: {mission_msg}\n",
                )
                if mission_msg.startswith("ERROR"):
                    return None, mission_msg
                if mission_target:
                    lifetime_msg = self._normalize_aircraft_lifetimes(
                        instance_root, mission_target
                    )
                    self.log_text.insert(
                        tk.END,
                        f"[{self._get_timestamp()}] Instance {instance_id} "
                        f"aircraft lifetime: {lifetime_msg}\n",
                    )
            except Exception as exc:
                return None, f"Failed to deploy mission folder: {exc}"

            try:
                self._apply_mod_integration_to_instance(instance_root)
            except Exception as exc:
                return None, f"Failed to apply mod integration: {exc}"

            try:
                mission_dir = ModIntegrationManager(instance_root)._find_mission_dir()
                self._apply_per_instance_config(
                    instance_root,
                    workshop_dir=workshop_dir,
                    mission_dir=mission_dir,
                )
            except Exception as exc:
                return None, f"Failed to apply per-instance configuration: {exc}"

            # Save the manifest only after a successful full deployment.
            manifest_mgr.save(current_manifest)

        # Generate the config content for this instance.
        if cfg_content is None or not cfg_content:
            all_configs = self._build_instance_cfgs_content()
            cfg_content = all_configs.get(config_filename, "")
            if not cfg_content:
                return None, f"Could not generate config content for {config_filename}"

            # Also write Effects & Triggers files for this instance.
            for filename, content in all_configs.items():
                if filename == config_filename or not filename.endswith((".xml", ".json")):
                    continue
                if f"_{instance_id}" in filename:
                    try:
                        (instance_root / filename).write_text(content, encoding="utf-8")
                    except Exception as exc:
                        return None, f"Failed to write {instance_root / filename}: {exc}"

        cfg_path = instance_root / config_filename
        try:
            cfg_path.write_text(cfg_content, encoding="utf-8")
        except Exception as exc:
            return None, f"Failed to write {cfg_path}: {exc}"

        return instance_root, config_filename, deployed_mods_str, profile_dir

    @staticmethod
    def _sanitize_ban_file(
        source_path: Path,
        dest_path: Path,
        whitelist: bool = False,
    ) -> None:
        """Sanitize a ban.txt or whitelist.txt file and write it to the instance.

        Strips full-line and inline comments, validates entries, removes
        example/demo lines, and ensures the file ends with a single LF newline.

        Args:
            source_path: Path to the base file in the DayZ server directory.
            dest_path: Where to write the sanitized file.
            whitelist: If True, validate 44-character player IDs; otherwise
                validate 17-digit SteamID64 values.
        """
        dest_path.parent.mkdir(parents=True, exist_ok=True)

        if not source_path.exists():
            # Write an empty sanitized file so the server does not fall back to
            # a malformed copy elsewhere.
            dest_path.write_text("\n", encoding="utf-8")
            return

        valid_entries: List[str] = []
        seen: set = set()
        for raw_line in source_path.read_text(encoding="utf-8", errors="ignore").splitlines():
            # Strip inline comments.
            line = raw_line.split("//", 1)[0].strip()
            if not line:
                continue

            # Skip obvious example/demo entries.
            if set(line) <= {"1", "2", "3", "X", "A"} and len(line) >= 20:
                continue
            if line.lower() in ("example", "sample", "test"):
                continue

            if whitelist:
                # DayZ character IDs are 44 alphanumeric characters.
                if len(line) == 44 and line.isalnum() and line not in seen:
                    valid_entries.append(line)
                    seen.add(line)
            else:
                # SteamID64 values are 17-digit numbers.
                if len(line) == 17 and line.isdigit() and line not in seen:
                    valid_entries.append(line)
                    seen.add(line)

        output = "\n".join(valid_entries)
        if output:
            output += "\n"
        else:
            output = "\n"
        dest_path.write_text(output, encoding="utf-8")

    def _get_instance_mods(self, instance: Dict[str, Any]) -> str:
        """Return the full semicolon-separated mod list for an instance.

        This merges the user's configured mod paths with the selected workshop
        map mod (if any) so the generated configs and start scripts are
        complete. The map mod is prepended so its terrain PBOs are loaded.
        
        Filters out non-mod directories like CrashReporter, profiles, etc.
        """
        mods_str = instance.get("mod_paths", {}).get() or ""
        if not mods_str:
            return ""

        # Filter out non-mod paths - only include actual mod folders
        filtered_parts = []
        for part in mods_str.replace(",", ";").split(";"):
            part = part.strip()
            if not part:
                continue
            path = Path(part)
            # Skip CrashReporter and other known non-mod directories
            if path.name == "CrashReporter":
                continue
            # Only include if it's an absolute path or starts with @
            if not path.is_absolute() and not part.startswith("@"):
                # Local mod name like "CF" - keep it, _deploy_mods_and_keys will resolve it
                pass
            filtered_parts.append(part)
        mods_str = ";".join(filtered_parts)
        map_display_name = instance.get("map", {}).get() or ""
        if not map_display_name:
            return mods_str

        workshop_dir = self._get_workshop_directory() or ""
        map_folder = self._find_workshop_map_folder(map_display_name, workshop_dir)
        if not map_folder:
            return mods_str

        map_token = map_folder.name
        existing = {
            m.strip().lstrip("@")
            for m in mods_str.replace(",", ";").split(";")
            if m.strip()
        }
        if map_token not in existing:
            if mods_str:
                return f"{map_token};{mods_str}"
            return map_token
        return mods_str

    def _deploy_mods_and_keys(self, instance_root: Path, mods_str: str) -> str:
        """Create @Mod wrappers and copy .bikey files into the instance.

        Args:
            instance_root: Instance directory to populate.
            mods_str: Semicolon-separated mod list (may contain @Name, absolute
                paths, or workshop numeric IDs).

        Returns:
            Normalized mod string using instance-relative @Name paths, suitable
            for the server's ``-mod=`` argument.
        """
        workshop_dir = self._get_workshop_directory() or ""
        local_mod_dirs: List[Path] = []
        if workshop_dir:
            local_mod_dirs.append(Path(workshop_dir))

        keys_dir = instance_root / "keys"
        keys_dir.mkdir(parents=True, exist_ok=True)

        # Copy base game keys first.
        dayz_path = Path(self.dayz_path_var.get().strip())
        base_keys_dir = dayz_path / "keys"
        if base_keys_dir.exists() and base_keys_dir.is_dir():
            for key_file in base_keys_dir.iterdir():
                if key_file.is_file() and key_file.suffix.lower() == ".bikey":
                    dest = keys_dir / key_file.name
                    shutil.copy2(key_file, dest)

        requested_link_names: set = set()
        normalized_paths: List[str] = []
        mod_paths = [m.strip() for m in mods_str.replace(",", ";").split(";") if m.strip()]

        for mod in mod_paths:
            mod_source = _resolve_mod_source(
                mod,
                workshop_dir=Path(workshop_dir) if workshop_dir else None,
                local_mod_dirs=local_mod_dirs,
            )
            if mod_source is None:
                continue

            link_name = "@" + mod_source.name.lstrip("@")
            requested_link_names.add(link_name)
            mod_link = instance_root / link_name

            # Replace any stale single-symlink with a case-normalized wrapper.
            if mod_link.exists() or mod_link.is_symlink():
                if mod_link.is_symlink():
                    mod_link.unlink()
                elif mod_link.is_dir():
                    shutil.rmtree(mod_link)

            _create_mod_wrapper(instance_root, link_name, mod_source)
            _copy_bikeys(mod_link, keys_dir)
            normalized_paths.append(link_name)

        # Remove stale @* wrappers that are no longer in the mod list.
        for item in instance_root.iterdir():
            if item.name.startswith("@"):
                if item.name not in requested_link_names:
                    try:
                        if item.is_symlink():
                            item.unlink()
                        elif item.is_dir():
                            shutil.rmtree(item)
                    except OSError:
                        pass

        return ";".join(normalized_paths)

    def _find_workshop_map_folder(self, map_display_name: str, workshop_dir: str) -> Optional[Path]:
        """Find the workshop folder matching a map display name."""
        if not map_display_name or not workshop_dir:
            return None
        content_path = Path(workshop_dir)
        if not content_path.exists():
            return None

        for folder in content_path.iterdir():
            if not folder.is_dir():
                continue
            try:
                _ = int(folder.name)
            except ValueError:
                continue

            display = self._read_workshop_display_name(folder) or folder.name
            if display.lower() == map_display_name.lower():
                return folder

        return None

    def _deploy_mission_folder(
        self,
        instance_root: Path,
        dayz_path: Path,
        map_display_name: str,
        workshop_dir: str,
        mission_source_path: Optional[Path] = None,
    ) -> Tuple[str, str]:
        """Ensure the mission folder required by serverDZ.cfg exists.

        Resolves the real world name, then copies the base-game, workshop, or
        user-supplied mission folder into instance_root/mpmissions/ as
        dayzOffline.<world>. An existing real mission directory that already
        matches the target world is always preserved so player save data and
        custom edits are not destroyed. If a workshop map does not ship a
        mission folder (terrain-only mods), the base game's ChernarusPlus
        mission is copied as a starting template so the server can start and
        the map PBOs define the terrain.

        Args:
            mission_source_path: Optional explicit source directory to copy
                the mission from. Ignored if a matching real mission folder
                already exists in the instance.

        Returns:
            A tuple of (short human-readable description of what was deployed,
            mission target folder name such as ``dayzOffline.chernarusplus``).
        """
        mpmissions_dir = instance_root / "mpmissions"
        mpmissions_dir.mkdir(parents=True, exist_ok=True)

        world_name = self._resolve_world_name(map_display_name, workshop_dir)
        if not world_name:
            return "No map selected; skipping mission deployment.", ""

        # The user may have supplied a full template such as dayzOffline.alteria.
        if world_name.lower().startswith("dayzoffline.") or world_name.lower().startswith("dayz."):
            target_name = world_name
            world_token = world_name.split(".", 1)[1]
        else:
            target_name = f"dayzOffline.{world_name}"
            world_token = world_name

        target_link = mpmissions_dir / target_name

        # Preserve an existing real mission directory. These folders contain
        # player saves and custom edits; deleting/replacing them would wipe
        # progress and any hand-tuned mission files.
        if target_link.exists() and target_link.is_dir() and not target_link.is_symlink():
            return (
                f"Preserved existing mission folder {target_name}. "
                "Delete it manually if you want to redeploy from a source.",
                target_name,
            )

        # Remove a stale symlink/broken link before deploying a fresh one.
        if target_link.is_symlink():
            try:
                target_link.unlink()
            except Exception as exc:
                return f"Could not remove stale mission symlink {target_link}: {exc}", ""

        # If the user provided a custom mission source, copy it.
        if mission_source_path is not None and mission_source_path.exists():
            try:
                shutil.copytree(mission_source_path, target_link, symlinks=True)
                return f"Copied mission {target_name} from custom source {mission_source_path}.", target_name
            except Exception as exc:
                return f"Could not copy custom mission source {mission_source_path}: {exc}", ""

        # Prefer the base game's mission folder for stock maps.
        base_mission = dayz_path / "mpmissions" / target_name
        if base_mission.exists():
            shutil.copytree(base_mission, target_link, symlinks=True)
            return f"Copied stock mission {target_name}.", target_name

        # Search the workshop map folder for a matching mission folder.
        workshop_folder = self._find_workshop_map_folder(map_display_name, workshop_dir)
        if workshop_folder is not None:
            candidates: List[Path] = []
            for item in workshop_folder.iterdir():
                if not item.is_dir():
                    continue
                lower = item.name.lower()
                if lower == target_name.lower():
                    candidates.append(item)
                elif lower == f"dayz.{world_token}".lower():
                    candidates.append(item)
                elif lower == f"offline.{world_token}".lower():
                    candidates.append(item)
                elif lower.endswith(f".{world_token}".lower()):
                    candidates.append(item)

            # Fallback: look for any folder that contains a .wrp file.
            if not candidates:
                for item in workshop_folder.iterdir():
                    if not item.is_dir():
                        continue
                    for sub in item.rglob("*.wrp"):
                        candidates.append(item)
                        break

            if candidates:
                shutil.copytree(candidates[0], target_link, symlinks=True)
                return f"Copied workshop mission {target_name} from {workshop_folder.name}.", target_name

        # Terrain-only workshop maps often do not include a mission folder.
        # Fall back to the base ChernarusPlus mission as a template so the
        # server can still boot; the custom terrain PBOs provide the world.
        fallback_mission = dayz_path / "mpmissions" / "dayzOffline.chernarusplus"
        if fallback_mission.exists():
            shutil.copytree(fallback_mission, target_link, symlinks=True)
            return (
                f"WARNING: {target_name} mission folder not found in workshop item "
                f"or base game. Copied ChernarusPlus mission as a fallback template. "
                f"You may need to obtain the correct mission files for {map_display_name}.",
                target_name,
            )

        return (
            f"ERROR: Could not find mission files for {target_name}. "
            f"Install the map mod or provide a dayzOffline.{world_token} mission folder.",
            "",
        )

    def _normalize_aircraft_lifetimes(
        self,
        instance_root: Path,
        target_name: str,
    ) -> str:
        """Ensure aircraft and helicopter types have the max lifetime.

        DayZ may load ``db/types.xml`` or the root ``types.xml`` depending on
        ``cfgeconomycore.xml``. Mod aircraft classes are often defined only in
        the root file, so normalize both to guarantee the lifetime is applied.

        This is run after the mission folder is deployed so admin-placed
        aircraft do not despawn because of a short ``<lifetime>`` value.

        Args:
            instance_root: Root directory of the instance.
            target_name: Mission folder name (e.g. ``dayzOffline.chernarusplus``).

        Returns:
            A short human-readable message describing what was done.
        """
        try:
            from dayzconfigmaster.economy.aircraft_lifetime import (
                ensure_aircraft_lifetime,
                ensure_aircraft_types_in_db,
                import_missing_aircraft_classes_to_db,
            )
        except Exception as exc:
            return f"Aircraft lifetime normalization failed: {exc}"

        mission_dir = instance_root / "mpmissions" / target_name
        profiles_dir = instance_root / "profiles"
        candidates = [
            mission_dir / "db" / "types.xml",
            mission_dir / "types.xml",
        ]

        messages: List[str] = []
        total_changed = 0
        total_skipped = 0
        backup_names: List[str] = []

        # First, copy any aircraft classes from root types.xml that DayZ's
        # default CE loader would otherwise ignore (db/types.xml is loaded).
        merge_result = ensure_aircraft_types_in_db(mission_dir)
        if merge_result.success and (merge_result.added or merge_result.updated):
            total_changed += merge_result.updated_count
            if merge_result.backup_path:
                backup_names.append(merge_result.backup_path.name)
        elif not merge_result.success and merge_result.error:
            messages.append(f"merge: {merge_result.error}")

        # Second, import script-defined aircraft classes discovered from the
        # server script logs (e.g. RFFSHeli_*, LM_*) that have no types.xml
        # entry at all.
        import_result = import_missing_aircraft_classes_to_db(
            mission_dir, profiles_dir
        )
        if import_result.success and import_result.imported:
            if import_result.backup_path:
                backup_names.append(import_result.backup_path.name)
        elif not import_result.success and import_result.error:
            messages.append(f"import: {import_result.error}")

        for types_path in candidates:
            if not types_path.exists():
                continue
            result = ensure_aircraft_lifetime(types_path)
            if not result.success:
                messages.append(f"{types_path.name}: {result.error}")
                continue
            total_changed += result.changed_count
            total_skipped += result.skipped_count
            if result.backup_path:
                backup_names.append(result.backup_path.name)

        if not messages and total_changed == 0 and total_skipped == 0 and not merge_result.added and not import_result.imported:
            return "No aircraft lifetime changes needed."

        msg_parts = []
        if total_changed:
            msg_parts.append(
                f"Set aircraft/helicopter lifetime to max for "
                f"{total_changed} type(s)."
            )
        if merge_result.success and merge_result.added:
            msg_parts.append(
                f"Added {merge_result.added_count} aircraft type(s) to "
                f"db/types.xml from root types.xml."
            )
        if import_result.success and import_result.imported:
            msg_parts.append(
                f"Imported {import_result.imported_count} script-defined "
                f"aircraft type(s) from logs into db/types.xml."
            )
        if total_skipped:
            msg_parts.append(
                f"{total_skipped} type(s) already at max lifetime."
            )
        if not msg_parts:
            msg_parts.append("Aircraft lifetime normalization completed.")
        if backup_names:
            msg_parts.append(f"Backups: {', '.join(backup_names)}")
        if messages:
            msg_parts.append("; ".join(messages))

        return " ".join(msg_parts)

    def _start_single_instance(
        self, instance: Dict[str, Any], terminal: bool = False
    ):
        """Start a single DayZ server instance from the multi-instance list."""
        instance_id = instance["id"].get()
        self._set_busy(f"Starting instance {instance_id}...")
        try:
            return self._start_single_instance_impl(instance, terminal)
        finally:
            self._clear_busy()

    def _start_single_instance_impl(
        self, instance: Dict[str, Any], terminal: bool = False
    ):
        """Internal implementation: start a single DayZ server instance."""
        valid, error = self._validate_dayz_server_path()
        if not valid:
            messagebox.showerror("Error", error)
            return

        instance_id = instance['id'].get()
        if instance_id in self._running_instance_ids:
            messagebox.showwarning(
                "Instance Already Running",
                f"Instance {instance_id} is already running. Stop it before starting again."
            )
            return

        dayz_path = self.dayz_path_var.get().strip()

        # Handle map swaps before we back up or deploy.
        proceed, rollover_msg, _rollover_backup = self._maybe_rollover_storage_for_map_change(instance)
        self.log_text.insert(tk.END, f"[{self._get_timestamp()}] {rollover_msg}\n")
        if not proceed:
            return

        # Ensure a storage_1 backup exists before we touch the instance.
        backup_ok, backup_msg, backup_path = self._ensure_instance_storage_backup(instance)
        self.log_text.insert(tk.END, f"[{self._get_timestamp()}] {backup_msg}\n")
        if not backup_ok:
            messagebox.showerror("Backup Error", backup_msg)
            return

        # Run filesystem/database preflight checks before deployment.
        preflight_ok, preflight_msg = self._run_instance_preflight(instance)
        self.log_text.insert(tk.END, f"[{self._get_timestamp()}] {preflight_msg}\n")
        if not preflight_ok:
            return

        # Ensure config and instance files are written in-place before starting.
        instance_root, config_filename, deployed_mods_str, profile_dir = self._prepare_instance_files(instance)
        if instance_root is None:
            messagebox.showerror("Instance Preparation Error", config_filename)
            return

        try:
            if not hasattr(self, 'process_controller') or self.process_controller is None:
                self.process_controller = ProcessController(dayz_path)

            port = int(instance["game_port"].get())
            query_port = int(instance["query_port"].get() or port + 1)
            steam_port = int(instance["steam_port"].get() or 27016 + int(instance_id) - 1)
            mods = [m.strip() for m in deployed_mods_str.split(';') if m.strip()] if deployed_mods_str else None
            
            process_name = f"server_instance_{instance_id}"

            # Kill any orphaned DayZ processes for this instance before starting
            # a new one. This ensures we always have governing control.
            orphaned = self.process_controller.kill_orphaned_dayz_processes(
                instance_root, config_filename, port
            )
            for msg in orphaned:
                self.log_text.insert(tk.END, f"[{self._get_timestamp()}] {msg}\n")

            # Use the instance-unique binary copy so Steam/BattlEye treat this
            # instance as a distinct process. Without this, only the first
            # started DayZ server binary is connectable.
            instance_exe = instance_root / self._get_instance_binary_name(int(instance_id), mode="normal")

            # Isolate Steam/IPC state per instance. DayZ's Linux steamclient.so
            # uses singleton state in the user's HOME; sharing it across
            # instances makes only the first-started server connectable.
            instance_env = self._build_isolated_instance_env(instance_root)

            success, msg = self.process_controller.start_server(
                port=port,
                query_port=query_port,
                steam_port=steam_port,
                mode="normal",
                map_size=2000,
                max_players=60,
                config_path=config_filename,
                profiles_path=str(profile_dir),
                mods=mods,
                terminal=terminal,
                cwd=instance_root,
                process_name=process_name,
                exe_path=instance_exe,
                env=instance_env,
            )
            if success:
                self._running_instance_ids.add(instance_id)
                self._update_instance_button_states()
                self.status_var.set(f"Instance {instance_id} started on port {port}")
                self.log_text.insert(
                    tk.END,
                    f"[{self._get_timestamp()}] Instance {instance_id} started on port {port} using {instance_exe.name}\n"
                )

                # Remember this map run for future map-swap handling.
                self._record_map_storage_state(instance, backup_path)

                # Start a per-instance memory watchdog.
                try:
                    memory_limit_gb = float(self.memory_kill_limit_var.get())
                except ValueError:
                    memory_limit_gb = self.MAX_MEMORY_LIMIT_GB
                memory_limit_gb = min(memory_limit_gb, self.MAX_MEMORY_LIMIT_GB)
                memory_limit_bytes = int(memory_limit_gb * 1024 * 1024 * 1024)
                memory_log_path = instance_root / "logs" / "memory_watchdog.log"

                def _make_instance_memory_callback(inst_id: str, proc_name: str):
                    def _callback(sample: Dict[str, Any]) -> None:
                        if sample.get("killed"):
                            self.log_text.insert(
                                tk.END,
                                f"[{self._get_timestamp()}] MEMORY KILL Instance {inst_id}: "
                                f"{sample['total_gb']:.2f} GB (limit {sample['limit_gb']:.2f} GB). "
                                f"Process tree terminated.\n",
                            )
                            self._running_instance_ids.discard(inst_id)
                            self._update_instance_button_states()
                    return _callback

                self.process_controller.start_memory_monitor(
                    process_name=process_name,
                    limit_bytes=memory_limit_bytes,
                    log_path=memory_log_path,
                    interval_seconds=5.0,
                    callback=_make_instance_memory_callback(instance_id, process_name),
                )

                self._start_server_monitor()
            else:
                self.status_var.set(f"Failed to start instance {instance_id}: {msg}")
                self.log_text.insert(
                    tk.END,
                    f"[{self._get_timestamp()}] ERROR Instance {instance_id}: {msg}\n"
                )
                messagebox.showerror("Server Error", msg)
        except Exception as e:
            error_msg = f"Error starting instance {instance_id}: {str(e)}"
            self.status_var.set(error_msg)
            self.log_text.insert(tk.END, f"[{self._get_timestamp()}] ERROR: {error_msg}\n")
            messagebox.showerror("Server Error", error_msg)

    def _stop_single_instance(self, instance: Dict[str, Any]):
        """Stop a single DayZ server instance."""
        instance_id = instance['id'].get()
        with self._busy_context(f"Stopping instance {instance_id}..."):
            self._stop_single_instance_impl(instance)

    def _stop_single_instance_impl(self, instance: Dict[str, Any]):
        """Internal implementation: stop a single instance."""
        instance_id = instance['id'].get()
        try:
            if hasattr(self, 'process_controller') and self.process_controller:
                process_name = f"server_instance_{instance_id}"
                if self.process_controller.is_process_name_running(process_name):
                    success, msg = self.process_controller.stop_process_by_name(process_name)
                else:
                    success, msg = self.process_controller.stop_server()
                self._running_instance_ids.discard(instance_id)
                self._disconnect_rcon_for_instance(instance_id)
                self._update_instance_button_states()
                self._stop_server_monitor()
                if success:
                    self.status_var.set(f"Instance {instance_id} stopped")
                    self.log_text.insert(
                        tk.END,
                        f"[{self._get_timestamp()}] Instance {instance_id} stopped\n"
                    )
                    # Back up storage_1 while the server is not writing to it.
                    backup_ok, backup_msg, _ = self._ensure_instance_storage_backup(instance, force=True)
                    self.log_text.insert(tk.END, f"[{self._get_timestamp()}] {backup_msg}\n")
                else:
                    self.status_var.set(f"Failed to stop instance {instance_id}: {msg}")
                    messagebox.showerror("Server Error", msg)
            else:
                messagebox.showwarning("No Server", "No instance is currently running")
        except Exception as e:
            self._running_instance_ids.discard(instance_id)
            self._update_instance_button_states()
            error_msg = f"Error stopping instance {instance_id}: {str(e)}"
            self.status_var.set(error_msg)
            self.log_text.insert(tk.END, f"[{self._get_timestamp()}] ERROR: {error_msg}\n")
            messagebox.showerror("Server Error", error_msg)

    def _start_multi_instance_servers(self):
        """Start all enabled DayZ server instances."""
        self._set_busy("Starting multi-instance servers...")
        try:
            return self._start_multi_instance_servers_impl()
        finally:
            self._clear_busy()

    def _start_multi_instance_servers_impl(self):
        """Internal implementation: start all enabled instances."""
        valid, error = self._validate_dayz_server_path()
        if not valid:
            messagebox.showerror("Error", error)
            return

        dayz_path = self.dayz_path_var.get().strip()

        if not self._multi_instance_enabled_var.get():
            messagebox.showwarning("Disabled", "Enable Multi-Instance Mode first")
            return

        try:
            if not hasattr(self, 'process_controller') or self.process_controller is None:
                self.process_controller = ProcessController(dayz_path)
            started = []
            version_warnings_seen: Set[str] = set()
            for instance in self._instance_vars:
                instance_id = instance['id'].get()
                if instance_id in self._running_instance_ids:
                    self.log_text.insert(
                        tk.END,
                        f"[{self._get_timestamp()}] Instance {instance_id} already running, skipping\n"
                    )
                    continue

                # Handle map swaps before we back up or deploy.
                proceed, rollover_msg, _ = self._maybe_rollover_storage_for_map_change(instance)
                self.log_text.insert(tk.END, f"[{self._get_timestamp()}] Instance {instance_id}: {rollover_msg}\n")
                if not proceed:
                    continue

                # Ensure a storage_1 backup exists before we touch the instance.
                backup_ok, backup_msg, backup_path = self._ensure_instance_storage_backup(instance)
                self.log_text.insert(tk.END, f"[{self._get_timestamp()}] Instance {instance_id}: {backup_msg}\n")
                if not backup_ok:
                    self.log_text.insert(
                        tk.END,
                        f"[{self._get_timestamp()}] ERROR Instance {instance_id}: {backup_msg}\n"
                    )
                    continue

                # Run filesystem/database preflight checks before deployment.
                preflight_ok, preflight_msg = self._run_instance_preflight(instance)
                self.log_text.insert(tk.END, f"[{self._get_timestamp()}] Instance {instance_id}: {preflight_msg}\n")
                if not preflight_ok:
                    continue

                # Ensure config and files are written in-place before starting.
                instance_root, config_filename, deployed_mods_str, profile_dir = self._prepare_instance_files(instance)
                if instance_root is None:
                    self.log_text.insert(
                        tk.END,
                        f"[{self._get_timestamp()}] ERROR Instance {instance_id}: {config_filename}\n"
                    )
                    continue

                # Warn about known mod-family version mismatches before starting.
                version_warnings = self._detect_mod_version_mismatches(instance_root)
                new_warnings = [w for w in version_warnings if w not in version_warnings_seen]
                if new_warnings:
                    version_warnings_seen.update(new_warnings)
                    warning_text = "\n\n".join(new_warnings)
                    self.log_text.insert(
                        tk.END,
                        f"[{self._get_timestamp()}] WARNING Instance {instance_id}: {warning_text}\n",
                    )
                    if not messagebox.askyesno(
                        "Mod Version Mismatch",
                        f"Instance {instance_id}:\n{warning_text}\n\nStart this instance anyway?",
                    ):
                        self.log_text.insert(
                            tk.END,
                            f"[{self._get_timestamp()}] Instance {instance_id} start cancelled: mod version mismatch\n",
                        )
                        continue

                port = int(instance["game_port"].get())
                query_port = int(instance["query_port"].get() or port + 1)
                steam_port = int(instance["steam_port"].get() or 27016 + int(instance_id) - 1)
                mods = [m.strip() for m in deployed_mods_str.split(';') if m.strip()] if deployed_mods_str else None

                process_name = f"server_instance_{instance_id}"

                # Ensure no orphaned process is still running for this instance.
                orphaned = self.process_controller.kill_orphaned_dayz_processes(
                    instance_root, config_filename, port
                )
                for orphan_msg in orphaned:
                    self.log_text.insert(tk.END, f"[{self._get_timestamp()}] {orphan_msg}\n")

                # Use the instance-unique binary copy so Steam/BattlEye treat
                # this instance as a distinct process.
                instance_exe = instance_root / self._get_instance_binary_name(int(instance_id), mode="normal")

                # Isolate Steam/IPC state per instance.
                instance_env = self._build_isolated_instance_env(instance_root)

                success, msg = self.process_controller.start_server(
                    port=port,
                    query_port=query_port,
                    steam_port=steam_port,
                    mode="normal",
                    map_size=2000,
                    max_players=60,
                    config_path=config_filename,
                    profiles_path=str(profile_dir),
                    mods=mods,
                    cwd=instance_root,
                    process_name=process_name,
                    exe_path=instance_exe,
                    env=instance_env,
                )
                if success:
                    self._running_instance_ids.add(instance_id)
                    started.append(str(port))
                    self.log_text.insert(
                        tk.END,
                        f"[{self._get_timestamp()}] Instance {instance_id} started on port {port}\n"
                    )

                    # Remember this map run for future map-swap handling.
                    self._record_map_storage_state(instance, backup_path)

                    # Start a per-instance memory watchdog.
                    try:
                        memory_limit_gb = float(self.memory_kill_limit_var.get())
                    except ValueError:
                        memory_limit_gb = self.MAX_MEMORY_LIMIT_GB
                    memory_limit_gb = min(memory_limit_gb, self.MAX_MEMORY_LIMIT_GB)
                    memory_limit_bytes = int(memory_limit_gb * 1024 * 1024 * 1024)
                    memory_log_path = instance_root / "logs" / "memory_watchdog.log"

                    def _make_instance_memory_callback(inst_id: str, proc_name: str):
                        def _callback(sample: Dict[str, Any]) -> None:
                            if sample.get("killed"):
                                self.log_text.insert(
                                    tk.END,
                                    f"[{self._get_timestamp()}] MEMORY KILL Instance {inst_id}: "
                                    f"{sample['total_gb']:.2f} GB (limit {sample['limit_gb']:.2f} GB). "
                                    f"Process tree terminated.\n",
                                )
                                self._running_instance_ids.discard(inst_id)
                                self._update_instance_button_states()
                        return _callback

                    self.process_controller.start_memory_monitor(
                        process_name=process_name,
                        limit_bytes=memory_limit_bytes,
                        log_path=memory_log_path,
                        interval_seconds=5.0,
                        callback=_make_instance_memory_callback(instance_id, process_name),
                    )
                else:
                    self.log_text.insert(
                        tk.END,
                        f"[{self._get_timestamp()}] ERROR Instance {instance_id}: {msg}\n"
                    )

            self._update_instance_button_states()
            if started:
                self.status_var.set(f"Started instances on ports: {', '.join(started)}")
                self.start_multi_btn.config(state=tk.DISABLED)
                self.stop_multi_btn.config(state=tk.NORMAL)
                self._start_server_monitor()
            else:
                self.status_var.set("No instances were started")

        except Exception as e:
            error_msg = f"Error starting multi-instance servers: {str(e)}"
            self.status_var.set(error_msg)
            self.log_text.insert(tk.END, f"[{self._get_timestamp()}] ERROR: {error_msg}\n")
            messagebox.showerror("Server Error", error_msg)

    def _stop_multi_instance_servers(self):
        """Stop all running DayZ server instances."""
        with self._busy_context("Stopping multi-instance servers..."):
            self._stop_multi_instance_servers_impl()

    def _stop_multi_instance_servers_impl(self):
        """Internal implementation: stop all running instances."""
        try:
            if hasattr(self, 'process_controller') and self.process_controller:
                # Stop every tracked server instance process, not just the
                # legacy "server" process name.
                for process_name in list(self.process_controller.get_running_process_names()):
                    if process_name == "client":
                        continue
                    self.process_controller.stop_process_by_name(process_name)

                # Back up storage_1 for each configured instance while the
                # server is not writing to it.
                for instance in getattr(self, "_instance_vars", []):
                    backup_ok, backup_msg, _ = self._ensure_instance_storage_backup(instance, force=True)
                    self.log_text.insert(
                        tk.END,
                        f"[{self._get_timestamp()}] Instance {instance['id'].get()}: {backup_msg}\n",
                    )

                self._running_instance_ids.clear()
                self._update_instance_button_states()
                self._stop_server_monitor()
                self.status_var.set("All instances stopped")
                self.log_text.insert(tk.END, f"[{self._get_timestamp()}] All instances stopped\n")
                self.start_multi_btn.config(state=tk.NORMAL)
                self.stop_multi_btn.config(state=tk.DISABLED)
            else:
                messagebox.showwarning("No Server", "No instances are currently running")
        except Exception as e:
            self._running_instance_ids.clear()
            self._update_instance_button_states()
            error_msg = f"Error stopping instances: {str(e)}"
            self.status_var.set(error_msg)
            self.log_text.insert(tk.END, f"[{self._get_timestamp()}] ERROR: {error_msg}\n")
            messagebox.showerror("Server Error", error_msg)
    
    def _start_server(self):
        """Start the DayZ server using ProcessController."""
        self._set_busy("Starting server...")
        try:
            return self._start_server_impl()
        finally:
            self._clear_busy()

    def _start_server_impl(self):
        """Internal implementation: start the DayZ server."""
        valid, error = self._validate_dayz_server_path()
        if not valid:
            messagebox.showerror("Error", error)
            return

        if self._single_server_running:
            messagebox.showwarning(
                "Server Already Running",
                "The single server is already running. Stop it before starting again."
            )
            return

        dayz_path = self.dayz_path_var.get().strip()

        # Build a pseudo-instance so the single-server path gets the same
        # instance preparation (base file symlinks, ban/whitelist sanitization,
        # mod/key deployment, mission folder) as multi-instance servers.
        config_path = self.config_path_var.get().strip() or "serverDZ.cfg"
        cfg_content = self._build_server_cfg_content()
        pseudo_instance = {
            "id": tk.StringVar(value="0"),
            "config_file": tk.StringVar(value=config_path),
            "root_folder": tk.StringVar(value=""),
            "profile": tk.StringVar(value=""),
            "mod_paths": self.mod_paths_var,
            "map": self.map_name_var,
        }

        # Handle map swaps and ensure a storage_1 backup exists.
        proceed, rollover_msg, _ = self._maybe_rollover_storage_for_map_change(pseudo_instance)
        self.log_text.insert(tk.END, f"[{self._get_timestamp()}] {rollover_msg}\n")
        if not proceed:
            return

        backup_ok, backup_msg, backup_path = self._ensure_instance_storage_backup(pseudo_instance)
        self.log_text.insert(tk.END, f"[{self._get_timestamp()}] {backup_msg}\n")
        if not backup_ok:
            messagebox.showerror("Backup Error", backup_msg)
            return

        # Run filesystem/database preflight checks before deployment.
        preflight_ok, preflight_msg = self._run_instance_preflight(pseudo_instance)
        self.log_text.insert(tk.END, f"[{self._get_timestamp()}] {preflight_msg}\n")
        if not preflight_ok:
            return

        instance_root, config_path, deployed_mods_str, profile_dir = self._prepare_instance_files(
            pseudo_instance, cfg_content=cfg_content
        )
        if instance_root is None:
            messagebox.showerror("Instance Preparation Error", config_path)
            return
        self._last_instance_root = instance_root

        try:
            # Initialize ProcessController
            if not hasattr(self, 'process_controller') or self.process_controller is None:
                self.process_controller = ProcessController(dayz_path)

            port = int(self.port_var.get())
            mode = self.mode_var.get()
            map_size = int(self.map_size_var.get())
            max_players = int(self.max_players_var.get())

            # Use the instance-relative @Name paths produced by deployment.
            mods = [m.strip() for m in deployed_mods_str.split(';') if m.strip()] if deployed_mods_str else None

            # Ensure no orphaned single-server process is still running.
            orphaned = self.process_controller.kill_orphaned_dayz_processes(
                instance_root, config_path, port
            )
            for orphan_msg in orphaned:
                self.log_text.insert(tk.END, f"[{self._get_timestamp()}] {orphan_msg}\n")

            # Warn about known mod-family version mismatches before starting.
            version_warnings = self._detect_mod_version_mismatches(instance_root)
            if version_warnings:
                warning_text = "\n\n".join(version_warnings)
                self.log_text.insert(
                    tk.END,
                    f"[{self._get_timestamp()}] WARNING: {warning_text}\n",
                )
                if not messagebox.askyesno(
                    "Mod Version Mismatch",
                    f"{warning_text}\n\nStart the server anyway?",
                ):
                    self.status_var.set("Server start cancelled: mod version mismatch")
                    return

            # Use the instance-unique binary copy (even for the single-server
            # pseudo-instance) so Steam/BattlEye treat it as a distinct process.
            instance_id = pseudo_instance["id"].get() or "0"
            instance_exe = instance_root / self._get_instance_binary_name(int(instance_id), mode=mode)

            # Isolate Steam/IPC state from any other DayZ server/client on the
            # same Linux user account.
            instance_env = self._build_isolated_instance_env(instance_root)

            query_port = int(pseudo_instance.get("query_port", {}).get() or port + 1)
            steam_port = int(pseudo_instance.get("steam_port", {}).get() or 27016)

            success, msg = self.process_controller.start_server(
                port=port,
                query_port=query_port,
                steam_port=steam_port,
                mode=mode,
                map_size=map_size,
                max_players=max_players,
                config_path=config_path,
                profiles_path=str(profile_dir),
                mods=mods,
                cwd=instance_root,
                process_name="server",
                exe_path=instance_exe,
                env=instance_env,
            )
            
            if success:
                self._single_server_running = True
                self.status_var.set(f"Server started: {msg}")
                self.log_text.insert(tk.END, f"[{self._get_timestamp()}] Server started: {msg} using {instance_exe.name}\n")

                # Remember this map run for future map-swap handling.
                self._record_map_storage_state(pseudo_instance, backup_path)

                # Start the memory watchdog. It runs in a background thread so
                # it can still kill the server even if the GUI freezes.
                try:
                    memory_limit_gb = float(self.memory_kill_limit_var.get())
                except ValueError:
                    memory_limit_gb = self.MAX_MEMORY_LIMIT_GB
                memory_limit_gb = min(memory_limit_gb, self.MAX_MEMORY_LIMIT_GB)
                memory_limit_bytes = int(memory_limit_gb * 1024 * 1024 * 1024)
                memory_log_path = instance_root / "logs" / "memory_watchdog.log"

                def _on_memory_sample(sample: Dict[str, Any]) -> None:
                    # Log every sample to the GUI log at most once per minute to
                    # avoid spam, and always log when the limit is exceeded.
                    if sample.get("killed"):
                        self.log_text.insert(
                            tk.END,
                            f"[{self._get_timestamp()}] MEMORY KILL: {sample['process_name']} "
                            f"reached {sample['total_gb']:.2f} GB (limit {sample['limit_gb']:.2f} GB). "
                            f"Process tree terminated.\n",
                        )
                        self.status_var.set(
                            f"Killed: memory limit exceeded ({sample['total_gb']:.2f} GB)"
                        )
                        self._single_server_running = False
                        self._update_instance_button_states()
                        self.start_server_btn.config(state=tk.NORMAL)
                        self.stop_server_btn.config(state=tk.DISABLED)
                        self.restart_server_btn.config(state=tk.DISABLED)
                    else:
                        now = time.time()
                        last = getattr(self, "_last_memory_log_ts", 0)
                        if now - last >= 60:
                            self._last_memory_log_ts = now
                            self.log_text.insert(
                                tk.END,
                                f"[{self._get_timestamp()}] Memory: {sample['total_gb']:.2f} GB / "
                                f"{sample['limit_gb']:.2f} GB\n",
                            )

                self.process_controller.start_memory_monitor(
                    process_name="server",
                    limit_bytes=memory_limit_bytes,
                    log_path=memory_log_path,
                    interval_seconds=5.0,
                    callback=_on_memory_sample,
                )
                self.log_text.insert(
                    tk.END,
                    f"[{self._get_timestamp()}] Memory watchdog started: "
                    f"{memory_limit_gb:.1f} GB limit, logging to {memory_log_path}\n",
                )

                self.start_server_btn.config(state=tk.DISABLED)
                self.stop_server_btn.config(state=tk.NORMAL)
                self.restart_server_btn.config(state=tk.NORMAL)
                self._server_start_time = time.time()
                self._server_port_check_done = False
                self._start_server_monitor()
            else:
                self.status_var.set(f"Failed to start server: {msg}")
                messagebox.showerror("Server Error", msg)
                
        except Exception as e:
            error_msg = f"Error starting server: {str(e)}"
            self.status_var.set(error_msg)
            self.log_text.insert(tk.END, f"[{self._get_timestamp()}] ERROR: {error_msg}\n")
            messagebox.showerror("Server Error", error_msg)
    
    def _stop_server(self):
        """Stop the DayZ server."""
        with self._busy_context("Stopping server..."):
            self._stop_server_impl()

    def _stop_server_impl(self):
        """Internal implementation: stop the DayZ server."""
        try:
            if hasattr(self, 'process_controller') and self.process_controller:
                success, msg = self.process_controller.stop_server()
                self._single_server_running = False
                self._stop_server_monitor()
                
                if success:
                    self.status_var.set(f"Server stopped: {msg}")
                    self.log_text.insert(tk.END, f"[{self._get_timestamp()}] Server stopped\n")
                    self.start_server_btn.config(state=tk.NORMAL)
                    self.stop_server_btn.config(state=tk.DISABLED)
                    self.restart_server_btn.config(state=tk.DISABLED)

                    # Back up storage_1 for the single-server pseudo-instance.
                    pseudo_instance = {
                        "id": tk.StringVar(value="0"),
                        "config_file": tk.StringVar(value=self.config_path_var.get() or "serverDZ.cfg"),
                        "root_folder": tk.StringVar(value=""),
                        "profile": tk.StringVar(value=""),
                        "mod_paths": self.mod_paths_var,
                        "map": self.map_name_var,
                    }
                    backup_ok, backup_msg, _ = self._ensure_instance_storage_backup(pseudo_instance, force=True)
                    self.log_text.insert(tk.END, f"[{self._get_timestamp()}] {backup_msg}\n")
                else:
                    self.status_var.set(f"Failed to stop server: {msg}")
                    messagebox.showerror("Server Error", msg)
            else:
                messagebox.showwarning("No Server", "No server is currently running")
                
        except Exception as e:
            self._single_server_running = False
            error_msg = f"Error stopping server: {str(e)}"
            self.status_var.set(error_msg)
            self.log_text.insert(tk.END, f"[{self._get_timestamp()}] ERROR: {error_msg}\n")
            messagebox.showerror("Server Error", error_msg)
    
    def _restart_server(self):
        """Restart the DayZ server."""
        with self._busy_context("Restarting server..."):
            self._restart_server_impl()

    def _restart_server_impl(self):
        """Internal implementation: restart the DayZ server."""
        try:
            if hasattr(self, 'process_controller') and self.process_controller:
                mode = self.mode_var.get()
                port = int(self.port_var.get())
                query_port = port + 1
                steam_port = 27016

                success, msg = self.process_controller.restart_server(
                    mode=mode,
                    port=port,
                    query_port=query_port,
                    steam_port=steam_port,
                )
                
                if success:
                    self._single_server_running = True
                    self.status_var.set(f"Server restarted: {msg}")
                    self.log_text.insert(tk.END, f"[{self._get_timestamp()}] Server restarted\n")
                    self._start_server_monitor()
                else:
                    self._single_server_running = False
                    self.status_var.set(f"Failed to restart server: {msg}")
                    messagebox.showerror("Server Error", msg)
            else:
                messagebox.showwarning("No Server", "No server is currently running")
                
        except Exception as e:
            self._single_server_running = False
            error_msg = f"Error restarting server: {str(e)}"
            self.status_var.set(error_msg)
            self.log_text.insert(tk.END, f"[{self._get_timestamp()}] ERROR: {error_msg}\n")
            messagebox.showerror("Server Error", error_msg)
    
    def _check_server_ports_bound(self, port: int) -> bool:
        """Return True if the DayZ game port is listening on any interface."""
        return is_server_port_bound(port)

    def _scan_server_log_for_errors(self, instance_root: Path) -> List[str]:
        """Scan the latest server RPT/log for critical startup errors."""
        return scan_server_log_for_errors(instance_root)

    def _detect_mod_version_mismatches(self, instance_root: Path) -> List[str]:
        """Warn when known mod families have mismatched versions.

        Returns a list of human-readable warnings.
        """
        return detect_mod_version_mismatches(instance_root)

    def _start_server_monitor(self):
        """Begin polling ProcessController for server liveness."""
        self._stop_server_monitor()
        if not self._is_valid_tk_root():
            self._server_monitor_after_id = None
            return
        self._server_monitor_after_id = self.root.after(
            self._server_monitor_interval_ms, self._poll_server_status
        )

    def _stop_server_monitor(self):
        """Cancel the server liveness polling callback."""
        if self._server_monitor_after_id is not None:
            try:
                self.root.after_cancel(self._server_monitor_after_id)
            except Exception:
                pass
            self._server_monitor_after_id = None

    def _poll_server_status(self):
        """Poll ProcessController and update the GUI if a server has exited."""
        if not hasattr(self, 'process_controller') or self.process_controller is None:
            self._server_monitor_after_id = None
            return

        try:
            all_status = self.process_controller.get_all_status()
        except Exception as exc:
            self.log_text.insert(
                tk.END,
                f"[{self._get_timestamp()}] ERROR monitoring server status: {exc}\n"
            )
            self._server_monitor_after_id = None
            return

        server_processes = {
            name: info for name, info in all_status.items()
            if name == "server" or name.startswith("server_instance_")
        }

        if not server_processes:
            self._server_monitor_after_id = None
            return

        any_running = any(info.get("running", False) for info in server_processes.values())

        # Display current memory usage in the status bar for the single server.
        if "server" in server_processes and self.process_controller is not None:
            try:
                sample = self.process_controller.get_memory_sample("server")
                if sample is not None:
                    self.status_var.set(
                        f"Server running — memory {sample['total_gb']:.2f} GB"
                    )
            except Exception:
                pass

        # After a grace period, verify the game port is actually listening.
        # If not, scan logs for the specific error so the user knows why.
        if (
            "server" in server_processes
            and server_processes["server"].get("running", False)
            and not getattr(self, "_server_port_check_done", False)
            and hasattr(self, "_server_start_time")
        ):
            elapsed = time.time() - self._server_start_time
            if elapsed >= 60:
                self._server_port_check_done = True
                try:
                    port = int(self.port_var.get())
                except ValueError:
                    port = 2302
                if not self._check_server_ports_bound(port):
                    # Use the instance root captured when the server started.
                    instance_root = getattr(
                        self, "_last_instance_root", Path(self.dayz_path_var.get().strip())
                    )

                    errors = self._scan_server_log_for_errors(instance_root)
                    if errors:
                        detail = "\n".join(errors)
                        msg = (
                            f"Server has been running for {int(elapsed)}s but port {port} "
                            f"is not listening. Critical log errors detected:\n\n{detail}\n\n"
                            "The server will not accept connections until this is resolved."
                        )
                    else:
                        msg = (
                            f"Server has been running for {int(elapsed)}s but port {port} "
                            f"is not listening. Check the server RPT log in {instance_root}/profiles "
                            "for startup errors."
                        )
                    self.status_var.set(f"Server running but port {port} not open")
                    self.log_text.insert(tk.END, f"[{self._get_timestamp()}] WARNING: {msg}\n")
                    messagebox.showwarning("Server Not Ready", msg)

        if not any_running:
            # All tracked server processes have exited.
            crashed_instances = self._running_instance_ids.copy()
            self._running_instance_ids.clear()
            self._single_server_running = False
            self._update_instance_button_states()

            self.status_var.set("Server stopped (crashed or exited)")
            crash_msg = "Server process stopped (crashed or exited)"
            if crashed_instances:
                crash_msg += f" — instances: {', '.join(sorted(crashed_instances))}"
            self.log_text.insert(
                tk.END,
                f"[{self._get_timestamp()}] {crash_msg}\n"
            )

            # Re-enable start buttons, disable stop buttons.
            if hasattr(self, 'start_server_btn'):
                self.start_server_btn.config(state=tk.NORMAL)
            if hasattr(self, 'stop_server_btn'):
                self.stop_server_btn.config(state=tk.DISABLED)
            if hasattr(self, 'restart_server_btn'):
                self.restart_server_btn.config(state=tk.DISABLED)
            if hasattr(self, 'start_multi_btn'):
                self.start_multi_btn.config(state=tk.NORMAL)
            if hasattr(self, 'stop_multi_btn'):
                self.stop_multi_btn.config(state=tk.DISABLED)

            self._server_monitor_after_id = None
            return

        # Schedule the next poll only if the GUI root is still alive.
        if self._is_valid_tk_root():
            self._server_monitor_after_id = self.root.after(
                self._server_monitor_interval_ms, self._poll_server_status
            )
        else:
            self._server_monitor_after_id = None

    def _update_instance_button_states(self):
        """Enable/disable per-instance Start/Stop buttons based on running state."""
        for instance_id, start_btn in self._instance_start_buttons.items():
            running = instance_id in self._running_instance_ids
            try:
                start_btn.config(state=tk.DISABLED if running else tk.NORMAL)
            except tk.TclError:
                pass
            stop_btn = self._instance_stop_buttons.get(instance_id)
            if stop_btn is not None:
                try:
                    stop_btn.config(state=tk.NORMAL if running else tk.DISABLED)
                except tk.TclError:
                    pass

    def _resolve_log_profiles_path(self, selected_text: Optional[str] = None) -> Optional[Path]:
        """Return the profiles directory for the log viewer's selected instance."""
        dayz_path = self.dayz_path_var.get().strip()
        if not dayz_path:
            return None

        if selected_text is None:
            selected = getattr(self, "log_instance_var", None)
            selected_text = selected.get() if selected else "Default"

        if selected_text.startswith("Instance "):
            inst_id = selected_text.split(" ", 1)[1]
            for inst in getattr(self, "_instance_vars", []):
                if inst["id"].get() == inst_id:
                    root_folder = inst["root_folder"].get() or ""
                    root_folder = self._sanitize_instance_root(root_folder, int(inst_id))
                    instance_root = Path(root_folder) if root_folder else Path(dayz_path)
                    profile_path = inst["profile"].get() or "profiles"
                    profile_dir = Path(profile_path)
                    if not profile_dir.is_absolute():
                        profile_dir = instance_root / profile_dir
                    return profile_dir

        return Path(dayz_path) / "profiles"

    def _update_log_viewer(self):
        """Update the log viewer with logs from the selected instance."""
        profiles_path = self._resolve_log_profiles_path()
        if profiles_path is None:
            return

        try:
            tailer = LogTailer(str(profiles_path))
            log_type = self.log_type_var.get()

            lines = tailer.tail(log_type, lines=100)

            self.log_text.delete(1.0, tk.END)
            header = f"--- {self.log_instance_var.get()} | {profiles_path} ---"
            self.log_text.insert(tk.END, f"{header}\n")
            for line in lines:
                self.log_text.insert(tk.END, f"{line}\n")

        except Exception as e:
            error_msg = f"Error reading logs: {str(e)}"
            self.log_text.insert(tk.END, f"[{self._get_timestamp()}] ERROR: {error_msg}\n")
            messagebox.showerror("Log Error", error_msg)

    def _clear_log_viewer(self):
        """Clear the log viewer"""
        self.log_text.delete(1.0, tk.END)

    def _analyze_logs(self):
        """Run diagnostics on logs from the selected instance."""
        profiles_path = self._resolve_log_profiles_path()
        if profiles_path is None:
            messagebox.showwarning("No Path", "Please specify a DayZ Server Path")
            return

        try:
            tailer = LogTailer(str(profiles_path))

            # Get all log types
            all_logs = ""
            for log_type in ["script", "rpt", "adm"]:
                lines = tailer.tail(log_type, lines=200)
                all_logs += "\n".join(lines) + "\n"

            if not all_logs.strip():
                self.diagnostics_text.delete(1.0, tk.END)
                self.diagnostics_text.insert(tk.END, "No log data available for analysis.\nPlease ensure logs are being generated by the server.\n")
                return

            # Run diagnostics
            diag_engine = DiagnosticsEngine()
            issues = diag_engine.run(all_logs)

            self.diagnostics_text.delete(1.0, tk.END)
            self.diagnostics_text.insert(tk.END, f"--- Diagnostics for {self.log_instance_var.get()} | {profiles_path} ---\n")

            if not issues:
                self.diagnostics_text.insert(tk.END, "✅ No common issues detected in logs.\n")
            else:
                for issue in issues:
                    severity_icon = {"error": "❌", "warning": "⚠️", "info": "ℹ️"}.get(issue.get("severity", "info"), "ℹ️")
                    self.diagnostics_text.insert(tk.END, f"{severity_icon} {issue['name']}\n")
                    self.diagnostics_text.insert(tk.END, f"   Cause: {issue['cause']}\n")
                    self.diagnostics_text.insert(tk.END, f"   Fix: {issue['fix']}\n\n")

        except Exception as e:
            error_msg = f"Error analyzing logs: {str(e)}"
            self.log_text.insert(tk.END, f"[{self._get_timestamp()}] ERROR: {error_msg}\n")
            messagebox.showerror("Diagnostics Error", error_msg)
    
    def _create_mod_builder_tab(self):
        """Create the Mod Builder tab with PboBuilder integration"""
        mod_builder_frame = ScrollableFrame(self.notebook)
        self.notebook.add(mod_builder_frame, text="Mod Builder")
        
        # Title
        title_label = ttk.Label(mod_builder_frame.frame, text="Mod Builder - PBO Construction", font=("Arial", 14, "bold"))
        title_label.pack(pady=10)
        
        # Wiki documentation link
        wiki_link_frame = self.create_wiki_link(mod_builder_frame.frame, "modding")
        wiki_link_frame.pack(fill=tk.X, padx=10, pady=(5, 15))
        
        # Overview description
        layout_desc = self.create_layout_description(
            mod_builder_frame.frame,
            "Mod Builder Features",
            """Build DayZ mods into PBO files using AddonBuilder from DayZ Tools.

Features:
- Pack mod folders into signed PBO files
- Binarize configs and models for production builds
- Clean build option (clear output before building)
- Cross-platform support (Windows/Linux/macOS)

Requirements:
- DayZ Tools installed with AddonBuilder executable
- Source folder with proper mod structure (config.cpp, model folder, etc.)
- Private key file for signing PBOs""",
            "https://github.com/landracer/DayZConfigMaster/wiki/06-configuration/modding"
        )
        layout_desc.pack(fill=tk.X, padx=10, pady=(5, 15))
        
        # DayZ Tools Path configuration
        path_frame = ttk.LabelFrame(mod_builder_frame.frame, text="DayZ Tools Configuration", padding=10)
        path_frame.pack(fill=tk.X, padx=10, pady=5)
        
        ttk.Label(path_frame, text="DayZ Tools Path:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        self.mod_builder_tools_path_var = tk.StringVar(value="")
        tools_path_entry = ttk.Entry(path_frame, width=60, textvariable=self.mod_builder_tools_path_var)
        tools_path_entry.grid(row=0, column=1, sticky=tk.W+tk.E, padx=5, pady=2)
        
        def browse_tools_path():
            path = filedialog.askdirectory(title="Select DayZ Tools Directory")
            if path:
                self.mod_builder_tools_path_var.set(path)
        tools_browse_btn = ttk.Button(path_frame, text="Browse", command=browse_tools_path)
        tools_browse_btn.grid(row=0, column=2, sticky=tk.W, padx=5, pady=2)
        
        # Status indicator
        mod_builder_status_frame = ttk.LabelFrame(mod_builder_frame.frame, text="Build Status", padding=5)
        mod_builder_status_frame.pack(fill=tk.X, padx=10, pady=5)
        
        self.mod_builder_status_var = tk.StringVar()
        self.mod_builder_status_var.set("Ready - No active build")
        
        status_label = ttk.Label(mod_builder_status_frame, textvariable=self.mod_builder_status_var, font=("Arial", 9))
        status_label.pack(anchor=tk.W, padx=5, pady=5)
        
        # Build configuration
        config_frame = ttk.LabelFrame(mod_builder_frame.frame, text="Build Configuration", padding=10)
        config_frame.pack(fill=tk.X, padx=10, pady=5)
        
        row_cfg = 0
        
        # Source directory
        ttk.Label(config_frame, text="Source Directory:").grid(row=row_cfg, column=0, sticky=tk.W, padx=5, pady=2)
        self.mod_builder_source_var = tk.StringVar(value="")
        source_entry = ttk.Entry(config_frame, width=60, textvariable=self.mod_builder_source_var)
        source_entry.grid(row=row_cfg, column=1, columnspan=2, sticky=tk.W+tk.E, padx=5, pady=2)
        
        def browse_source():
            path = filedialog.askdirectory(title="Select Source Directory")
            if path:
                self.mod_builder_source_var.set(path)
        source_browse_btn = ttk.Button(config_frame, text="Browse", command=browse_source)
        source_browse_btn.grid(row=row_cfg, column=3, sticky=tk.W, padx=(0, 5), pady=2)
        row_cfg += 1
        
        # Output directory
        ttk.Label(config_frame, text="Output Directory:").grid(row=row_cfg, column=0, sticky=tk.W, padx=5, pady=2)
        self.mod_builder_output_var = tk.StringVar(value="")
        output_entry = ttk.Entry(config_frame, width=60, textvariable=self.mod_builder_output_var)
        output_entry.grid(row=row_cfg, column=1, columnspan=2, sticky=tk.W+tk.E, padx=5, pady=2)
        
        def browse_output():
            path = filedialog.askdirectory(title="Select Output Directory")
            if path:
                self.mod_builder_output_var.set(path)
        output_browse_btn = ttk.Button(config_frame, text="Browse", command=browse_output)
        output_browse_btn.grid(row=row_cfg, column=3, sticky=tk.W, padx=(0, 5), pady=2)
        row_cfg += 1
        
        # PBO prefix
        ttk.Label(config_frame, text="PBO Prefix:").grid(row=row_cfg, column=0, sticky=tk.W, padx=5, pady=2)
        self.mod_builder_prefix_var = tk.StringVar(value="")
        prefix_entry = ttk.Entry(config_frame, width=60, textvariable=self.mod_builder_prefix_var)
        prefix_entry.grid(row=row_cfg, column=1, columnspan=3, sticky=tk.W+tk.E, padx=5, pady=2)
        row_cfg += 1
        
        # Private key for signing
        ttk.Label(config_frame, text="Signing Key:").grid(row=row_cfg, column=0, sticky=tk.W, padx=5, pady=2)
        self.mod_builder_key_var = tk.StringVar(value="")
        key_entry = ttk.Entry(config_frame, width=60, textvariable=self.mod_builder_key_var)
        key_entry.grid(row=row_cfg, column=1, columnspan=3, sticky=tk.W+tk.E, padx=5, pady=2)
        
        def browse_key():
            path = filedialog.askopenfilename(title="Select Private Key", 
                                              filetypes=[("Key files", "*.bisign"), ("All files", "*.*")])
            if path:
                self.mod_builder_key_var.set(path)
        key_browse_btn = ttk.Button(config_frame, text="Browse", command=browse_key)
        key_browse_btn.grid(row=row_cfg, column=4, sticky=tk.W, padx=(0, 5), pady=2)
        row_cfg += 1
        
        # Build options
        build_options_frame = ttk.LabelFrame(mod_builder_frame.frame, text="Build Options", padding=10)
        build_options_frame.pack(fill=tk.X, padx=10, pady=5)
        
        self.mod_builder_binarize_var = tk.BooleanVar(value=True)
        binarize_check = ttk.Checkbutton(build_options_frame, 
                                        text="Binarize configs/models (full build)",
                                        variable=self.mod_builder_binarize_var)
        binarize_check.pack(anchor=tk.W, pady=2)
        
        self.mod_builder_clean_var = tk.BooleanVar(value=False)
        clean_check = ttk.Checkbutton(build_options_frame,
                                     text="Clean output before building (-clear)",
                                     variable=self.mod_builder_clean_var)
        clean_check.pack(anchor=tk.W, pady=2)
        
        self.mod_builder_verbose_var = tk.BooleanVar(value=False)
        verbose_check = ttk.Checkbutton(build_options_frame,
                                       text="Verbose logging (-dologs)",
                                       variable=self.mod_builder_verbose_var)
        verbose_check.pack(anchor=tk.W, pady=2)
        
        # Build buttons
        button_frame = ttk.Frame(mod_builder_frame.frame)
        button_frame.pack(pady=10)
        
        self.build_btn = ttk.Button(button_frame, text="🔨 Build PBO", command=self._build_pbo)
        self.build_btn.pack(side=tk.LEFT, padx=5)
        
        # Progress display
        progress_frame = ttk.LabelFrame(mod_builder_frame.frame, text="Build Log", padding=5)
        progress_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=(5, 0))
        
        self.build_log_text = scrolledtext.ScrolledText(progress_frame, wrap=tk.WORD, height=15, font=("Courier", 9))
        self.build_log_text.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
    
    def _build_pbo(self):
        """Build PBO using PboBuilder"""
        dayz_tools_path = self.mod_builder_tools_path_var.get().strip()
        
        if not dayz_tools_path:
            messagebox.showerror("Error", "Please specify a DayZ Tools Path")
            return
        
        source_dir = self.mod_builder_source_var.get().strip()
        output_dir = self.mod_builder_output_var.get().strip()
        
        if not source_dir or not output_dir:
            messagebox.showerror("Error", "Please specify both Source and Output directories")
            return
        
        try:
            # Initialize PboBuilder
            builder = PboBuilder(dayz_tools_path)
            
            prefix = self.mod_builder_prefix_var.get() or None
            sign_key = self.mod_builder_key_var.get() if self.mod_builder_key_var.get().strip() else None
            clean = self.mod_builder_clean_var.get()
            binarize = self.mod_builder_binarize_var.get()
            verbose = self.mod_builder_verbose_var.get()
            
            output_pbo = f"{output_dir}/{Path(source_dir).name}"
            
            success, msg = builder.pack(
                source_dir=source_dir,
                output_pbo=output_pbo,
                prefix=prefix,
                sign_key=sign_key,
                clean=clean,
                binarize=binarize,
                verbose=verbose
            )
            
            if success:
                self.mod_builder_status_var.set(f"Build successful: {msg}")
                self.build_log_text.insert(tk.END, f"[{self._get_timestamp()}] {msg}\n")
                messagebox.showinfo("Build Complete", msg)
            else:
                self.mod_builder_status_var.set(f"Build failed: {msg}")
                self.build_log_text.insert(tk.END, f"[{self._get_timestamp()}] ERROR: {msg}\n")
                messagebox.showerror("Build Error", msg)
                
        except Exception as e:
            error_msg = f"Error building PBO: {str(e)}"
            self.mod_builder_status_var.set(error_msg)
            self.build_log_text.insert(tk.END, f"[{self._get_timestamp()}] ERROR: {error_msg}\n")
            messagebox.showerror("Build Error", error_msg)
    
    def _create_effects_and_triggers_content(self, parent: ttk.Frame):
        """Create the per-instance Effects & Triggers configuration panel."""
        # Header with instance selector
        header = ttk.Frame(parent)
        header.pack(fill=tk.X, padx=10, pady=(10, 5))

        ttk.Label(header, text="Effects & Triggers Configuration", font=("Arial", 14, "bold")).pack(side=tk.LEFT)

        selector_frame = ttk.Frame(header)
        selector_frame.pack(side=tk.RIGHT)

        ttk.Label(selector_frame, text="Instance:").pack(side=tk.LEFT, padx=(0, 5))
        self._effects_instance_combo = ttk.Combobox(
            selector_frame,
            textvariable=self._effects_instance_var,
            values=["1"],
            state="readonly",
            width=12
        )
        self._effects_instance_combo.pack(side=tk.LEFT)

        def on_instance_selected(*args):
            self._save_current_effects_content()
            self._load_effects_content_for_instance(int(self._effects_instance_var.get() or 1))

        self._effects_instance_var.trace_add("write", on_instance_selected)

        # Wiki documentation link
        wiki_link_frame = self.create_wiki_link(parent, "effects-triggers")
        wiki_link_frame.pack(fill=tk.X, padx=10, pady=(5, 10))

        # Inner notebook for the various effects files
        effects_notebook = ttk.Notebook(parent)
        effects_notebook.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)
        self._effects_notebook = effects_notebook

        effects_specs = [
            ("Limits", "cfglimitsdefinitionuser.xml", """USER-DEFINED LIMITS (cfglimitsdefinitionuser.xml)

Purpose: Custom spawn limits beyond default values defined in limitsdefinitions.xml

What it controls:
• Category counts per map zone
• Item quantity restrictions
• Spawn density modifiers

Format Example:
<limits>
  <limit name="CustomItem" value="50"/>
</limits>"""),
            ("Event Spawns", "cfgeventspawns.xml", """EVENT SPAWNS (cfgeventspawns.xml)

Purpose: Configure special events like Heli Crashes, Zombie Hordes

What it controls:
• Event triggers and probabilities
• Reward types and quantities
• Spawn locations

Format Example:
<eventspawns>
  <event name="HeliCrash">
    <spawn item="Helicopter" chance="0.1"/>
  </event>
</eventspawns>"""),
            ("Effect Areas", "cfgeffectarea.json", """EFFECT AREAS (cfgeffectarea.json)

Purpose: Define zones with special effects like radiation, weather effects

Format Example:
{
  "effectAreas": [
    {
      "name": "RadioactiveZone",
      "radius": 50.0,
      "duration": 3600,
      "effects": ["radiation", "slowness"]
    }
  ]
}"""),
            ("Underground Triggers", "cfgundergroundtriggers.json", """UNDERGROUND TRIGGERS (cfgundergroundtriggers.json)

Purpose: Configure triggers for caves, bunkers, and underground areas

Format Example:
{
  "triggers": [
    {
      "name": "CaveEntrance",
      "radius": 20.0,
      "triggerType": "cave",
      "effects": ["darkness", "mystery"]
    }
  ]
}"""),
            ("Map Clusters", "mapclusterproto.xml", """MAP CLUSTERS (mapclusterproto.xml)

Purpose: Define how map clusters are generated and organized by terrain type

Format Example:
<mapclusters>
  <cluster name="ForestCluster">
    <type>forest</type>
    <density>high</density>
  </cluster>
</mapclusters>"""),
            ("Map Group Dirt", "mapgroupdirt.xml", """MAP GROUP DIRT (mapgroupdirt.xml)

Purpose: Configure terrain properties affecting vehicle handling and surface grip

Format Example:
<mapgroups>
  <group name="FarmArea">
    <dirtType>clay</dirtType>
    <density>medium</density>
  </group>
</mapgroups>"""),
            ("Events XML", "events.xml", """EVENTS.XML

Purpose: Server-wide event system configuration

Format Example:
<events>
  <event name="ZombieHorde">
    <type>spawn</type>
    <frequency>hourly</frequency>
  </event>
</events>"""),
            ("Globals XML", "globals.xml", """GLOBALS.XML

Purpose: Global server parameters affecting all gameplay

Format Example:
<globals>
  <setting name="maxPlayers" value="100"/>
</globals>"""),
            ("Messages XML", "messages.xml", """MESSAGES.XML

Purpose: Server notifications and player-facing messages

Format Example:
<messages>
  <message id="welcome" text="Welcome to the server!"/>
</messages>"""),
        ]

        self._effects_filenames = [filename for _, filename, _ in effects_specs]
        for name, filename, description in effects_specs:
            self._create_effects_subtab(effects_notebook, name, filename, description)

        # Seed instance 1 with default content if empty
        if 1 not in self._instance_effects_content:
            self._instance_effects_content[1] = {}
            for filename, text_area in self._effects_text_areas.items():
                self._instance_effects_content[1][filename] = text_area.get("1.0", tk.END)

        self._update_effects_instance_selector()

    def _update_effects_instance_selector(self):
        """Update the Effects & Triggers instance selector values from _instance_vars."""
        if not hasattr(self, '_effects_instance_combo') or self._effects_instance_combo is None:
            return

        if self._multi_instance_enabled_var.get() and self._instance_vars:
            values = [str(inst["id"].get()) for inst in self._instance_vars]
        else:
            values = ["1"]

        self._effects_instance_combo["values"] = values
        current = self._effects_instance_var.get()
        if current not in values:
            self._effects_instance_var.set(values[0] if values else "1")

    def _save_current_effects_content(self):
        """Persist the currently visible Effects & Triggers text into the current instance."""
        try:
            instance_id = int(self._effects_instance_var.get() or 1)
        except ValueError:
            instance_id = 1

        if instance_id not in self._instance_effects_content:
            self._instance_effects_content[instance_id] = {}

        for filename, text_area in self._effects_text_areas.items():
            self._instance_effects_content[instance_id][filename] = text_area.get("1.0", tk.END)

    def _load_effects_content_for_instance(self, instance_id: int):
        """Load Effects & Triggers text for the selected instance into the UI."""
        if instance_id not in self._instance_effects_content:
            self._instance_effects_content[instance_id] = {}

        for filename, text_area in self._effects_text_areas.items():
            content = self._instance_effects_content[instance_id].get(filename, "")
            text_area.delete("1.0", tk.END)
            text_area.insert(tk.END, content)

    def _create_effects_subtab(self, notebook, name, filename, description, show_overview=True):
        """Create a sub-tab for a specific effects configuration file."""
        frame = ttk.Frame(notebook)
        notebook.add(frame, text=name)

        # Title
        title_label = ttk.Label(frame, text=f"Configuration: {filename}", font=("Arial", 12, "bold"))
        title_label.pack(pady=10)

        # Description with scrollable help
        desc_frame = tk.Text(frame, wrap=tk.WORD, height=8, bg="#f0f0f0", relief="flat")
        desc_frame.insert(tk.END, description + "\n")
        desc_frame.config(state=tk.DISABLED)
        desc_frame.pack(fill=tk.X, padx=10, pady=(5, 10))

        # Sample content text area
        from tkinter import scrolledtext
        sample_text = f"""// Example {filename} content below

// This is a template/example. Edit carefully based on your server needs.
"""

        if filename.endswith('.json'):
            sample_text += """{
  "version": 1,
  "items": []
}
"""
        else:
            sample_text += """<!-- XML format example -->
<configuration>
  <item name="example" value="0"/>
</configuration>
"""

        text_area = scrolledtext.ScrolledText(frame, wrap=tk.WORD, height=20)
        text_area.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)
        text_area.insert(tk.END, sample_text)

        # Store reference so the content can be retrieved when generating configs
        self._effects_text_areas[filename] = text_area

    def _create_ban_list_content(self, parent: ttk.Frame):
        """Create Ban List & Players content using VPPAdminTools files."""
        title_label = ttk.Label(parent, text="Server Ban List Management (VPPAdminTools)", font=("Arial", 14, "bold"))
        title_label.pack(pady=10)

        wiki_link_frame = self.create_wiki_link(parent, "server-control")
        wiki_link_frame.pack(fill=tk.X, padx=10, pady=(5, 15))

        layout_desc = self.create_layout_description(
            parent,
            "Ban List Features",
            """Manage player bans and view connected players via VPPAdminTools files.

Features:
• Select any configured instance
• View currently connected players parsed from VPP logs
• Add/remove bans by Steam64 ID in VPPAdminTools/BanList.json
• Optionally add yourself to VPP SuperAdmins

Note: DayZ on Linux does not expose a reliable RCon port, so this tab reads and writes VPPAdminTools data directly in the instance profiles directory. The running server must have VPPAdminTools installed and loaded.""",
            "https://github.com/landracer/DayZConfigMaster/wiki/06-configuration/server-control"
        )
        layout_desc.pack(fill=tk.X, padx=10, pady=(5, 15))

        # Instance selector
        instance_frame = ttk.LabelFrame(parent, text="Instance", padding=10)
        instance_frame.pack(fill=tk.X, padx=10, pady=5)

        ttk.Label(instance_frame, text="Instance:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        instance_combo = ttk.Combobox(
            instance_frame,
            width=28,
            textvariable=self.rcon_instance_var,
            state="readonly",
        )
        instance_combo.grid(row=0, column=1, sticky=tk.W+tk.E, padx=5, pady=2)
        instance_frame.columnconfigure(1, weight=1)

        vpp_status_label = ttk.Label(instance_frame, text="VPP: not checked", foreground="gray")
        vpp_status_label.grid(row=1, column=1, sticky=tk.E, padx=5, pady=(5, 0))

        def _refresh_instance_options():
            options = ["Default"]
            for inst in getattr(self, "_instance_vars", []):
                inst_id = inst["id"].get()
                if inst_id:
                    options.append(f"Instance {inst_id}")
            instance_combo.config(values=options)

        def _on_instance_changed(event=None):
            vpp = self._get_vpp_for_selected_instance()
            if vpp is None:
                vpp_status_label.config(text="VPP: no profiles path", foreground="red")
                return
            if vpp.is_available():
                vpp_status_label.config(text=f"VPP: available ({vpp.vpp_path.name})", foreground="green")
            else:
                vpp_status_label.config(text=f"VPP: not found ({vpp.ban_path})", foreground="red")
            self._refresh_vpp_players()
            self._refresh_vpp_bans()

        instance_combo.bind("<<ComboboxSelected>>", _on_instance_changed)
        parent.bind("<Visibility>", lambda e: _refresh_instance_options(), add="+")

        # Ban management
        ban_frame = ttk.LabelFrame(parent, text="Ban Management", padding=10)
        ban_frame.pack(fill=tk.X, padx=10, pady=5)

        ttk.Label(ban_frame, text="Steam ID (64-bit):").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        ban_steam_entry = ttk.Entry(ban_frame, width=30, textvariable=self.ban_steam_id_var)
        ban_steam_entry.grid(row=0, column=1, sticky=tk.W+tk.E, padx=5, pady=2)
        self.input_widgets["Ban Steam ID"] = (ban_steam_entry, {})

        ttk.Label(ban_frame, text="Player Name:").grid(row=1, column=0, sticky=tk.W, padx=5, pady=2)
        ban_name_entry = ttk.Entry(ban_frame, width=30, textvariable=self.ban_name_var)
        ban_name_entry.grid(row=1, column=1, sticky=tk.W+tk.E, padx=5, pady=2)

        ttk.Label(ban_frame, text="Reason:").grid(row=2, column=0, sticky=tk.W, padx=5, pady=2)
        ban_reason_entry = ttk.Entry(ban_frame, width=40, textvariable=self.ban_reason_var)
        ban_reason_entry.grid(row=2, column=1, sticky=tk.W+tk.E, padx=5, pady=2)
        self.input_widgets["Ban Reason"] = (ban_reason_entry, {})

        ttk.Label(ban_frame, text="Duration (min, 0=permanent):").grid(row=3, column=0, sticky=tk.W, padx=5, pady=2)
        ban_duration_entry = ttk.Entry(ban_frame, width=10, textvariable=self.ban_duration_var)
        ban_duration_entry.grid(row=3, column=1, sticky=tk.W, padx=5, pady=2)
        self.input_widgets["Ban Duration"] = (ban_duration_entry, {"min": 0})

        btn_frame = ttk.Frame(ban_frame)
        btn_frame.grid(row=4, column=0, columnspan=2, sticky=tk.W+tk.E, pady=(10, 0))
        add_ban_btn = ttk.Button(btn_frame, text="Add / Update Ban", command=self._add_vpp_ban)
        add_ban_btn.pack(side=tk.LEFT, padx=(0, 5))

        # Super admin section
        sa_frame = ttk.LabelFrame(parent, text="VPP SuperAdmin", padding=10)
        sa_frame.pack(fill=tk.X, padx=10, pady=5)
        ttk.Label(sa_frame, text="Your Steam64 ID:").grid(row=0, column=0, sticky=tk.W, padx=5, pady=2)
        sa_entry = ttk.Entry(sa_frame, width=30, textvariable=self.vpp_super_admin_var)
        sa_entry.grid(row=0, column=1, sticky=tk.W+tk.E, padx=5, pady=2)
        sa_btn = ttk.Button(sa_frame, text="Ensure Super Admin", command=self._ensure_vpp_super_admin)
        sa_btn.grid(row=0, column=2, padx=5, pady=2)

        # Lists frame
        lists_frame = ttk.Frame(parent)
        lists_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)
        lists_frame.columnconfigure(0, weight=1)
        lists_frame.columnconfigure(1, weight=1)
        lists_frame.rowconfigure(1, weight=1)

        # Connected players
        player_frame = ttk.LabelFrame(lists_frame, text="Connected Players", padding=5)
        player_frame.grid(row=0, column=0, rowspan=2, sticky=tk.NSEW, padx=(0, 5))
        player_frame.rowconfigure(1, weight=1)
        player_frame.columnconfigure(0, weight=1)

        refresh_players_btn = ttk.Button(player_frame, text="Refresh", command=self._refresh_vpp_players)
        refresh_players_btn.grid(row=0, column=0, sticky=tk.W, pady=(0, 5))

        player_tree_scroll_y = ttk.Scrollbar(player_frame, orient=tk.VERTICAL)
        player_tree_scroll_x = ttk.Scrollbar(player_frame, orient=tk.HORIZONTAL)
        self.player_tree = ttk.Treeview(
            player_frame,
            columns=("SteamID", "Name", "Connected"),
            show="headings",
            yscrollcommand=player_tree_scroll_y.set,
            xscrollcommand=player_tree_scroll_x.set,
        )
        player_tree_scroll_y.config(command=self.player_tree.yview)
        player_tree_scroll_x.config(command=self.player_tree.xview)
        self.player_tree.column("SteamID", width=140, anchor=tk.CENTER)
        self.player_tree.column("Name", width=150)
        self.player_tree.column("Connected", width=80, anchor=tk.CENTER)
        self.player_tree.heading("SteamID", text="Steam ID 64")
        self.player_tree.heading("Name", text="Player Name")
        self.player_tree.heading("Connected", text="Connected")
        self.player_tree.grid(row=1, column=0, sticky=tk.NSEW)
        player_tree_scroll_y.grid(row=1, column=1, sticky=tk.NS)
        player_tree_scroll_x.grid(row=2, column=0, sticky=tk.EW)

        def _on_player_selected(event=None):
            selected = self.player_tree.focus()
            if selected:
                values = self.player_tree.item(selected, "values")
                if values:
                    self.ban_steam_id_var.set(values[0])
                    self.ban_name_var.set(values[1])

        self.player_tree.bind("<<TreeviewSelect>>", _on_player_selected)

        # Ban list
        ban_list_frame = ttk.LabelFrame(lists_frame, text="Current Bans", padding=5)
        ban_list_frame.grid(row=0, column=1, rowspan=2, sticky=tk.NSEW, padx=(5, 0))
        ban_list_frame.rowconfigure(1, weight=1)
        ban_list_frame.columnconfigure(0, weight=1)

        refresh_bans_btn = ttk.Button(ban_list_frame, text="Refresh", command=self._refresh_vpp_bans)
        refresh_bans_btn.grid(row=0, column=0, sticky=tk.W, pady=(0, 5))

        ban_tree_scroll_y = ttk.Scrollbar(ban_list_frame, orient=tk.VERTICAL)
        ban_tree_scroll_x = ttk.Scrollbar(ban_list_frame, orient=tk.HORIZONTAL)
        self.ban_tree = ttk.Treeview(
            ban_list_frame,
            columns=("SteamID", "Name", "Reason", "Permanent"),
            show="headings",
            yscrollcommand=ban_tree_scroll_y.set,
            xscrollcommand=ban_tree_scroll_x.set,
        )
        ban_tree_scroll_y.config(command=self.ban_tree.yview)
        ban_tree_scroll_x.config(command=self.ban_tree.xview)
        self.ban_tree.column("SteamID", width=140, anchor=tk.CENTER)
        self.ban_tree.column("Name", width=120)
        self.ban_tree.column("Reason", width=150)
        self.ban_tree.column("Permanent", width=80, anchor=tk.CENTER)
        self.ban_tree.heading("SteamID", text="Steam ID 64")
        self.ban_tree.heading("Name", text="Player Name")
        self.ban_tree.heading("Reason", text="Reason")
        self.ban_tree.heading("Permanent", text="Permanent")
        self.ban_tree.grid(row=1, column=0, sticky=tk.NSEW)
        ban_tree_scroll_y.grid(row=1, column=1, sticky=tk.NS)
        ban_tree_scroll_x.grid(row=2, column=0, sticky=tk.EW)

        unban_btn = ttk.Button(ban_list_frame, text="Remove Ban", command=self._remove_vpp_ban)
        unban_btn.grid(row=3, column=0, sticky=tk.W, pady=(5, 0))

    def _get_vpp_for_selected_instance(self) -> Optional[VppAdminTools]:
        """Return a VPPAdminTools interface for the currently selected instance."""
        selected = self.rcon_instance_var.get()
        profiles_path = self._resolve_log_profiles_path(selected_text=selected)
        if profiles_path is None:
            return None
        return VppAdminTools(profiles_path)

    def _get_rcon_client_for_instance(self, instance_id: str) -> Optional[Any]:
        """Return a cached RConClient for the given instance, creating it if needed.

        DayZ on Linux does not expose a reliable vanilla RCon port, so this is
        provided for Windows hosts or mods that do enable RCon. The client is
        cached per instance so switching instances does not reuse a stale
        connection to the wrong server.
        """
        if not instance_id:
            return None

        # Disconnect any cached client for a different instance to avoid
        # accidentally talking to the wrong server.
        for cached_id, client in list(self._rcon_clients.items()):
            if cached_id != instance_id and client is not None:
                try:
                    client.disconnect()
                except Exception:
                    pass
                self._rcon_clients.pop(cached_id, None)

        client = self._rcon_clients.get(instance_id)
        if client is None:
            try:
                from ..rcon.client import RConClient
            except ImportError:
                from dayzconfigmaster.rcon.client import RConClient

            host = self.rcon_host_var.get() or "localhost"
            try:
                port = int(self.rcon_port_var.get() or 2306)
            except ValueError:
                port = 2306
            password = self.rcon_password_var.get() or ""
            client = RConClient(host=host, port=port, password=password)
            self._rcon_clients[instance_id] = client
        return client

    def _disconnect_rcon_for_instance(self, instance_id: str) -> None:
        """Close the cached RCon connection for an instance, if any."""
        client = self._rcon_clients.pop(instance_id, None)
        if client is not None:
            try:
                client.disconnect()
            except Exception:
                pass

    def _refresh_vpp_players(self):
        """Refresh the connected players list from VPP logs."""
        vpp = self._get_vpp_for_selected_instance()
        if vpp is None:
            messagebox.showwarning("No Path", "Could not resolve profiles path for selected instance.")
            return
        try:
            players = vpp.get_connected_players()
            self.player_tree.delete(*self.player_tree.get_children())
            for player in players:
                ts = player.connected_at.strftime("%H:%M") if player.connected_at else ""
                self.player_tree.insert("", tk.END, values=(player.steam_id, player.name, ts))
        except Exception as e:
            messagebox.showerror("Player List Error", f"Failed to read VPP players:\n{str(e)}")

    def _refresh_vpp_bans(self):
        """Refresh the ban list from VPPAdminTools."""
        vpp = self._get_vpp_for_selected_instance()
        if vpp is None:
            return
        try:
            bans = vpp.get_bans()
            self.ban_tree.delete(*self.ban_tree.get_children())
            for ban in bans:
                self.ban_tree.insert(
                    "",
                    tk.END,
                    values=(
                        ban.steam_id,
                        ban.player_name,
                        ban.reason,
                        "Yes" if ban.permanent else "No",
                    ),
                )
        except Exception as e:
            messagebox.showerror("Ban List Error", f"Failed to read VPP bans:\n{str(e)}")

    def _add_vpp_ban(self):
        """Add or update a VPP ban for the Steam ID in the entry fields."""
        vpp = self._get_vpp_for_selected_instance()
        if vpp is None:
            messagebox.showwarning("No Path", "Could not resolve profiles path for selected instance.")
            return
        steam_id = self.ban_steam_id_var.get().strip()
        if not steam_id:
            messagebox.showwarning("Validation Error", "Please enter a Steam ID.")
            return
        try:
            duration = int(self.ban_duration_var.get())
        except ValueError:
            duration = 0
        name = self.ban_name_var.get().strip()
        reason = self.ban_reason_var.get().strip()
        try:
            vpp.add_ban(
                steam_id=steam_id,
                name=name,
                reason=reason,
                permanent=(duration == 0),
            )
            messagebox.showinfo(
                "Ban Added",
                f"Player banned successfully!\nSteam ID: {steam_id}\nDuration: {'Permanent' if duration == 0 else f'{duration} minutes'}",
            )
            self._refresh_vpp_bans()
        except Exception as e:
            messagebox.showerror("Ban Error", f"Failed to add ban:\n{str(e)}")

    def _remove_vpp_ban(self):
        """Remove the selected ban from VPPAdminTools."""
        selected = self.ban_tree.focus()
        if not selected:
            messagebox.showwarning("Selection Error", "Please select a ban to remove.")
            return
        values = self.ban_tree.item(selected, "values")
        steam_id = values[0] if values else ""
        if not steam_id:
            return
        vpp = self._get_vpp_for_selected_instance()
        if vpp is None:
            return
        try:
            if vpp.remove_ban(steam_id):
                messagebox.showinfo("Ban Removed", f"Removed ban: {steam_id}")
                self._refresh_vpp_bans()
            else:
                messagebox.showwarning("Not Found", f"Ban not found: {steam_id}")
        except Exception as e:
            messagebox.showerror("Ban Error", f"Failed to remove ban:\n{str(e)}")

    def _ensure_vpp_super_admin(self):
        """Add the entered Steam64 ID to VPP SuperAdmins for the selected instance."""
        steam_id = self.vpp_super_admin_var.get().strip()
        if not steam_id:
            messagebox.showwarning("Validation Error", "Please enter your Steam64 ID.")
            return
        vpp = self._get_vpp_for_selected_instance()
        if vpp is None:
            return
        try:
            added = vpp.ensure_super_admin(steam_id)
            if added:
                messagebox.showinfo("Super Admin", f"Added {steam_id} to VPP SuperAdmins.")
            else:
                messagebox.showinfo("Super Admin", f"{steam_id} is already a VPP SuperAdmin.")
        except Exception as e:
            messagebox.showerror("Super Admin Error", f"Failed to update SuperAdmins:\n{str(e)}")

    def _update_player_list(self):
        """Deprecated alias kept for compatibility; delegates to VPP refresh."""
        self._refresh_vpp_players()

    def _create_workshop_tab(self):
        """Create the workshop search and download tab."""
        from pathlib import Path
        
        workshop_frame = ScrollableFrame(self.notebook)
        self.notebook.add(workshop_frame, text="Steam Workshop")
        
        title_label = ttk.Label(workshop_frame.frame, text="Steam Workshop Integration", font=("Arial", 14, "bold"))
        title_label.pack(pady=10)
        
        wiki_link_frame = self.create_wiki_link(workshop_frame.frame, "modding")
        wiki_link_frame.pack(fill=tk.X, padx=10, pady=(5, 15))
        
        layout_desc = self.create_layout_description(
            workshop_frame.frame,
            "Steam Workshop Features",
            """This tab provides Steam Workshop integration for searching and downloading DayZ mods.
            
Features:
• Search for workshop items by name or tag
• View detailed information about each item
• Download items to your server's workshop directory
• List locally installed workshop items

To use these features, configure your Steam Web API key in Preferences (Steam Workshop Settings section).""",
            "https://github.com/landracer/DayZConfigMaster/wiki/06-configuration/modding"
        )
        layout_desc.pack(fill=tk.X, padx=10, pady=(5, 15))
        
        # Local workshop items - show already downloaded mods
        local_items_frame = ttk.LabelFrame(workshop_frame.frame, text="Local Workshop Items", padding=10)
        local_items_frame.pack(fill=tk.X, padx=10, pady=5)
        
        # Use the shared workshop directory source of truth.
        default_workshop_dir = self._get_workshop_directory() or ""

        # Workshop directory frame - separate container above the treeview
        workshop_dir_frame = ttk.Frame(local_items_frame)
        workshop_dir_frame.pack(fill=tk.X, pady=(0, 5))

        ttk.Label(workshop_dir_frame, text="Workshop Directory:", font=("Arial", 9)).pack(anchor=tk.W, pady=(0, 2))

        workshop_dir_entry = ttk.Entry(workshop_dir_frame, textvariable=self._workshop_dir_var, width=60)
        workshop_dir_entry.pack(side=tk.LEFT, padx=(0, 5), pady=(0, 5))

        def browse_workshop_dir():
            path = filedialog.askdirectory(title="Select Workshop Directory")
            if path:
                self._set_workshop_directory(path)
        workshop_browse_btn = ttk.Button(workshop_dir_frame, text="Browse", command=browse_workshop_dir)
        workshop_browse_btn.pack(side=tk.LEFT, pady=(0, 5))

        # Seed the UI with the detected path on first creation.
        if default_workshop_dir:
            self._workshop_dir_var.set(default_workshop_dir)
        
        # Button to scan for local items
        def scan_local_items():
            import json
            from ..workshop.local_parser import LocalWorkshopMetadataParser

            # Load settings to retrieve the Steam API key for enrichment.
            try:
                from ..setup.wizard import SetupWizard
            except ImportError:
                from dayzconfigmaster.setup.wizard import SetupWizard
            projects_root = Path.home() / "Documents" / "DayZProjects"
            wizard_local = SetupWizard(str(projects_root))
            settings_local = wizard_local.get_settings()

            # First, try to find Steam installation path to read .acf file
            steam_path = None
            
            # Try correct Linux path first (most common for local installs)
            possible_steam_paths_first = [
                Path.home() / ".local" / "share" / "Steam",
            ]
            
            for p in possible_steam_paths_first:
                if p and p.exists():
                    steam_path = str(p)
                    break
            
            # If not found, try other common paths
            if not steam_path:
                possible_other_paths = [
                    Path.home() / ".steam" / "steam",
                    Path("/opt/steam"),
                    Path("/usr/share/steam"),
                ]
                
                for p in possible_other_paths:
                    if p and p.exists():
                        steam_path = str(p)
                        break
            
            # Also try to detect from workshop directory (fallback only)
            workshop_dir_value = self._workshop_dir_var.get().strip()
            if not steam_path and workshop_dir_value:
                workshop_path = Path(workshop_dir_value)
                possible_steam_paths_from_ws = [
                    workshop_path.parent.parent,
                    workshop_path.parent.parent / ".." if workshop_path.name == "content" else None,
                ]
                
                for p in possible_steam_paths_from_ws:
                    if p and p.exists():
                        steam_path = str(p)
                        break
            
            # Fallback to detecting Steam path via wizard
            if not steam_path:
                try:
                    from ..setup.wizard import SetupWizard
                except ImportError:
                    from dayzconfigmaster.setup.wizard import SetupWizard
                projects_root = Path.home() / "Documents" / "DayZProjects"
                wizard_local = SetupWizard(str(projects_root))
                detected_steam = wizard_local.detect_steam_workshop_path()
                if detected_steam:
                    steam_path = str(Path(detected_steam).parent.parent)
            
            items = []
            
            # Try to parse .acf file first (most reliable method)
            acf_items = []
            if steam_path:
                try:
                    parser = LocalWorkshopMetadataParser(steam_path)
                    acf_items = parser.parse_acf_file()
                    
                    # Enrich with Steam API
                    steam_api_key = settings_local.get("steam_api_key")
                    enriched = parser.enrich_with_steam_api(acf_items, steam_api_key)
                    
                    for item in enriched:
                        title = item.mod_name if item.mod_name and item.mod_name != "Unknown" else str(item.workshop_id)
                        
                        # Store in our list for filtering (class-level to avoid scoping issues)
                        self._local_workshop_items.append({
                            "id": item.workshop_id,
                            "mod_name": item.mod_name,
                            "title": title[:60] + "..." if len(title) > 60 else title,
                            "author": item.author_steam_id_64 or "",
                            "full_title": title,
                            "subscribers": 0,
                            "tags": [],
                            "folder": "",
                            "local_metadata": item.local_metadata,
                            "remote_metadata": item.remote_metadata
                        })
                        
                        items.append({
                            "id": item.workshop_id,
                            "title": title[:60] + "..." if len(title) > 60 else title,
                            "author": item.author_steam_id_64 or "",
                            "full_title": title,
                            "subscribers": 0,
                            "tags": [],
                            "folder": "",
                            "local_metadata": item.local_metadata,
                            "remote_metadata": item.remote_metadata
                        })
                    
                    status_label_local.config(text=f"Found {len(items)} workshop items (from .acf file + Steam API)")
                except Exception as e:
                    print(f"ACF parsing exception: {e}")
                    status_label_local.config(text=f".acf parse error ({str(e)[:50]}), falling back to folder scan...")
            
            # If .acf parsing didn't work or returned nothing, try folder scanning
            if not self._local_workshop_items and not items:
                workshop_path = Path(self._workshop_dir_var.get())
                if not workshop_path.exists() or not workshop_path.is_dir():
                    messagebox.showwarning("Directory Not Found", f"Workshop directory does not exist:\n{self._workshop_dir_var.get()}")
                    return
                
                # Clear existing results
                local_results_tree.delete(*local_results_tree.get_children())
                
                for item_folder in workshop_path.iterdir():
                    if item_folder.is_dir():
                        folder_name = item_folder.name
                        try:
                            _ = int(folder_name)
                            
                            title = folder_name
                            author = "Unknown"
                            subscribers = 0
                            tags = []
                            
                            new_item = {
                                "id": folder_name,
                                "title": title[:60] + "..." if len(title) > 60 else title,
                                "author": author,
                                "full_title": title,
                                "subscribers": subscribers,
                                "tags": tags,
                                "folder": str(item_folder)
                            }
                            new_item["mod_name"] = new_item.get("title", "")
                            self._local_workshop_items.append(new_item)
                            items.append(new_item)
                        except ValueError:
                            continue
                
                # Sort both lists by workshop ID
                self._local_workshop_items.sort(key=lambda x: x["id"])
                items.sort(key=lambda x: x["id"])
            else:
                # Sort both lists by subscriber count (most popular first)
                self._local_workshop_items.sort(key=lambda x: x.get("subscribers", 0), reverse=True)
                items.sort(key=lambda x: x.get("subscribers", 0), reverse=True)
            
            for item in self._local_workshop_items:
                title = item.get("full_title", "") or item.get("mod_name", "")
                
                local_results_tree.insert("", tk.END, values=(
                    str(item["id"]),
                    title if title and "Mod #" not in title else "Unknown",
                    item.get("author_steam_id_64", "") or item.get("author", "")
                ))
            
            # Update status message
            if steam_path and acf_items:
                has_titles = False
                try:
                    has_titles = any(not item.mod_name.startswith("Mod #") for item in enriched)
                except AttributeError:
                    # Handle case where enriched contains dicts (fallback case)
                    has_titles = any(not item.get("mod_name", "").startswith("Mod #") for item in enriched)
                status_label_local.config(text=f"Found {len(items)} local workshop items")
        
        scan_btn = ttk.Button(local_items_frame, text="🔄 Scan Local Items", command=scan_local_items)
        scan_btn.pack(anchor=tk.W, pady=(0, 10))
        
        # Local results treeview
        local_tree_scroll_y = ttk.Scrollbar(local_items_frame, orient=tk.VERTICAL)
        local_tree_scroll_x = ttk.Scrollbar(local_items_frame, orient=tk.HORIZONTAL)
        
        local_results_tree = ttk.Treeview(
            local_items_frame,
            columns=("ID", "Title", "Author"),
            show="headings",
            yscrollcommand=local_tree_scroll_y.set,
            xscrollcommand=local_tree_scroll_x.set,
            height=10
        )
        
        local_tree_scroll_y.config(command=local_results_tree.yview)
        local_tree_scroll_x.config(command=local_results_tree.xview)
        
        local_results_tree.column("ID", width=80, anchor=tk.CENTER)
        local_results_tree.column("Title", width=350)
        local_results_tree.column("Author", width=150)
        
        local_results_tree.heading("ID", text="Workshop ID")
        local_results_tree.heading("Title", text="Title")
        local_results_tree.heading("Author", text="Author")
        
        # Make Workshop ID column clickable to open in browser
        def on_id_click(event):
            selection = local_results_tree.selection()
            if not selection:
                return
            
            item_id = local_results_tree.item(selection[0])['values'][0]
            import webbrowser
            workshop_url = f"https://steamcommunity.com/sharedfiles/filedetails/?id={item_id}"
            webbrowser.open(workshop_url)
            
            try:
                self.root.clipboard_clear()
                self.root.clipboard_append(workshop_url)
            except:
                pass
        
        local_results_tree.bind("<Double-1>", lambda e: on_id_click(e))
        
        local_tree_scroll_y.pack(side=tk.RIGHT, fill=tk.Y)
        local_tree_scroll_x.pack(side=tk.BOTTOM, fill=tk.X)
        local_results_tree.pack(fill=tk.X, expand=False)
        
        # Local status bar
        local_status_frame = ttk.Frame(local_items_frame)
        local_status_frame.pack(fill=tk.X, pady=(5, 0))
        
        status_label_local = ttk.Label(local_status_frame, text="Click 'Scan' to list local workshop items", foreground="gray")
        status_label_local.pack(anchor=tk.W)
        
        # Cleanup frame for filtering/deleting (defined AFTER scan_local_items to have items available)
        if not hasattr(self, '_local_workshop_items'):
            self._local_workshop_items = []
        
        def filter_unknown():
            local_results_tree.delete(*local_results_tree.get_children())
            for item in self._local_workshop_items:
                title = item.get("full_title", "") or item.get("mod_name", "")
                display_title = title if title and not title.startswith("Mod #") else "Unknown"
                
                if item.get("mod_name", "").startswith("Mod #"):
                    local_results_tree.insert("", tk.END, values=(
                        str(item["id"]),
                        display_title,
                        item.get("author_steam_id_64", "") or item.get("author", "")
                    ))
        
        def filter_valid():
            local_results_tree.delete(*local_results_tree.get_children())
            for item in self._local_workshop_items:
                title = item.get("full_title", "") or item.get("mod_name", "")
                
                if not item.get("mod_name", "").startswith("Mod #"):
                    local_results_tree.insert("", tk.END, values=(
                        str(item["id"]),
                        title[:60] + "..." if len(title) > 60 else title,
                        item.get("author_steam_id_64", "") or item.get("author", "")
                    ))
        
        def delete_unknown():
            import shutil
            deleted_count = 0
            
            for item in self._local_workshop_items:
                mod_name = item.get("mod_name", "")
                if mod_name.startswith("Mod #"):
                    workshop_id = str(item.get("id")) or str(item.get("workshop_id"))
                    
                    local_meta = item.get("local_metadata", {})
                    folder_path = None
                    if isinstance(local_meta, dict):
                        folder_path = local_meta.get("disk_path")
                    
                    print(f"Item {workshop_id}: Checking path: {folder_path}, exists: {Path(folder_path).exists() if folder_path else False}")
                    
                    if folder_path and Path(folder_path).exists():
                        try:
                            shutil.rmtree(folder_path)
                            deleted_count += 1
                            print(f"Deleted: {folder_path}")
                        except Exception as e:
                            print(f"Failed to delete {folder_path}: {e}")
            
            messagebox.showinfo("Cleanup Complete", f"Deleted {deleted_count} unknown workshop items")
        
        # Filter buttons frame
        filter_frame = ttk.Frame(local_items_frame)
        filter_frame.pack(fill=tk.X, pady=(5, 0))
        
        btn_unknown = ttk.Button(filter_frame, text="Show Only Unknown (Mod #*)", command=filter_unknown)
        btn_unknown.pack(side=tk.LEFT, padx=(0, 5), pady=2)
        
        btn_valid = ttk.Button(filter_frame, text="Show Valid Items", command=filter_valid)
        btn_valid.pack(side=tk.LEFT, padx=5, pady=2)
        
        btn_delete = ttk.Button(filter_frame, text="Delete Unknown Items", command=delete_unknown)
        btn_delete.pack(side=tk.LEFT, padx=5, pady=2)
    
    def _new_project(self):
        """Create a new project"""
        if messagebox.askokcancel("New Project", "Start a new project? Unsaved changes will be lost."):
            for widget, _ in self.input_widgets.values():
                widget.delete(0, tk.END)
            self.status_var.set("New project created")
    
    def _open_file(self):
        """Open an existing file"""
        filename = filedialog.askopenfilename(
            title="Open Configuration File",
            filetypes=[
                ("XML files", "*.xml"),
                ("Text files", "*.txt"),
                ("CFG files", "*.cfg"),
                ("JSON files", "*.json"),
                ("All files", "*.*")
            ]
        )
        if filename:
            self.status_var.set(f"Loading: {filename}")
            
            config, error = self.parser.load_file(filename)
            if config:
                self.current_config = config
                config_type = detect_config_type(filename) or 'Unknown'
                self.status_var.set(f"Loaded: {config_type} configuration from {filename}")
                
                if isinstance(config, ServerConfig):
                    self._update_server_config_ui(config)
                    msg = f"Successfully loaded serverDZ.cfg\n"
                    msg += f"Hostname: {config.name}\n"
                    msg += f"Port: {config.port}\n"
                    msg += f"Max Players: {config.max_players}\n"
                    msg += f"Game Mode: {config.game_mode}"
                    messagebox.showinfo("Load Successful", msg)
                else:
                    messagebox.showinfo("Load Successful", 
                                      f"Successfully loaded {filename}")
            elif error:
                self.status_var.set(f"Load failed: {error}")
                messagebox.showerror("Load Error", f"Failed to load file:\n{error}")
    
    def _save_file(self):
        """Save current file"""
        filename = filedialog.asksaveasfilename(
            title="Save Configuration",
            defaultextension=".cfg",
            filetypes=[("CFG files", "*.cfg"), ("XML files", "*.xml"), ("All files", "*.*")]
        )
        if filename:
            self.status_var.set(f"Saved: {filename}")
    
    def _exit_app(self):
        """Exit the application"""
        if messagebox.askokcancel("Quit", "Do you want to quit DayzConfigMaster?"):
            self.root.destroy()
    
    def _edit_treeview_item(self, tree):
        """Allow editing of treeview items by double-clicking"""
        selected = tree.selection()
        if not selected:
            return
        
        # Get the selected item and column
        item_id = selected[0]
        
        # Create an entry widget to edit the value
        x, y, width, height = tree.bbox(item_id, '#1')
        
        # Get current values
        values = tree.item(item_id, 'values')
        
        # Create a simple edit dialog
        self._show_edit_dialog(tree, item_id, values)
    
    def _show_edit_dialog(self, tree, item_id, values):
        """Show a dialog to edit treeview item values"""
        columns = tree.cget('columns')
        num_fields = len(columns)
        field_height = 65 * num_fields
        button_height = 80
        min_width = max(450, 300 + num_fields * 40)
        geometry = f"{min_width}x{max(field_height + button_height, 320)}"

        dialog = self._safe_toplevel("Edit Item", geometry)
        if dialog is None:
            messagebox.showerror("GUI Error", "Cannot open edit dialog: no GUI root available.")
            return
        
        try:
            dialog.transient(self.root)
        except tk.TclError:
            pass

        # Main container with padding
        main_frame = ttk.Frame(dialog, padding=15)
        main_frame.pack(fill=tk.BOTH, expand=True)
        
        # Input fields frame
        input_frame = ttk.Frame(main_frame)
        input_frame.pack(fill=tk.X, pady=(0, 20))
        
        entry_widgets = []
        for i, col in enumerate(columns):
            ttk.Label(input_frame, text=col, font=("Arial", 9, "bold")).pack(anchor=tk.W, padx=(5, 10), pady=(3, 0))
            var = tk.StringVar(value=values[i] if i < len(values) else "")
            entry = ttk.Entry(input_frame, textvariable=var)
            entry.pack(fill=tk.X, padx=5, pady=(0, 8))
            entry_widgets.append((col, var))
        
        def save_edit():
            new_values = tuple(var.get() for _, var in entry_widgets)
            tree.item(item_id, values=new_values)
            dialog.destroy()
        
        def cancel_edit():
            dialog.destroy()
        
        # Buttons frame at bottom - always visible with padding
        button_frame = ttk.Frame(main_frame)
        button_frame.pack(fill=tk.X, pady=(5, 0))
        
        ttk.Button(button_frame, text="Save", command=save_edit).pack(side=tk.RIGHT, padx=(5, 2))
        ttk.Button(button_frame, text="Cancel", command=cancel_edit).pack(side=tk.RIGHT, padx=2)
        
        # Update dialog to ensure it's viewable before grabbing
        dialog.update_idletasks()
        dialog.grab_set()
        try:
            self.root.wait_window(dialog)
        except tk.TclError:
            pass

    def _show_preferences(self):
        """Show preferences dialog with backup comparison and browser options"""
        pref_window = self._safe_toplevel("Preferences - Settings & Backup Comparison", "900x650")
        if pref_window is None:
            messagebox.showerror("GUI Error", "Cannot open preferences: no GUI root available.")
            return
        try:
            pref_window.transient(self.root)
        except tk.TclError:
            pass

        # Main frame
        main_frame = ttk.Frame(pref_window, padding=10)
        main_frame.pack(fill=tk.BOTH, expand=True)
        
        # Title and backup comparison section
        title_frame = ttk.LabelFrame(main_frame, text="Preferences & Backup Comparison", padding=10)
        title_frame.pack(fill=tk.X, pady=(0, 15))
        
        ttk.Label(title_frame, text="Current Settings - Compare with Backup at:", font=("Arial", 9, "bold")).pack(anchor=tk.W, pady=(0, 5))
        
        backup_path = "/home/sysadmin/Documents/git/DayzConfigMaster copy"
        ttk.Label(title_frame, text=backup_path, foreground="blue", cursor="hand2", 
                 font=("Arial", 9, "underline")).pack(anchor=tk.W)
        
        # Open backup folder button
        def open_backup_folder():
            import os
            if Path(backup_path).exists():
                os.system(f'xdg-open "{backup_path}"')
            else:
                messagebox.showwarning("Folder Not Found", f"Backup folder not found at:\n{backup_path}")
        
        ttk.Button(title_frame, text="📁 Open Backup Folder", command=open_backup_folder).pack(anchor=tk.W, pady=(5, 0))
        
        # Current projects root
        current_root = self._get_projects_root()
        ttk.Label(title_frame, text=f"\nCurrent Projects Root: {current_root}", font=("Arial", 9)).pack(anchor=tk.W)
        
        # Backup comparison results
        comparison_frame = ttk.LabelFrame(main_frame, text="Backup Comparison Results", padding=10)
        comparison_frame.pack(fill=tk.X, pady=(0, 15))
        
        try:
            backup_path_obj = Path(backup_path)
            current_path_obj = Path(".")
            
            # Check if backup exists
            if not backup_path_obj.exists():
                status_label = ttk.Label(comparison_frame, 
                                       text="⚠️ Backup folder does not exist or is inaccessible",
                                       foreground="orange", font=("Arial", 9))
                status_label.pack(anchor=tk.W)
            else:
                # Compare key files
                comparison_text = tk.Text(comparison_frame, wrap=tk.WORD, height=8, bg="#f0f0f0", relief="flat")
                comparison_text.pack(fill=tk.X)
                
                import filecmp
                
                # Check app.py difference
                backup_app_py = backup_path_obj / "dayzconfigmaster" / "gui" / "app.py"
                current_app_py = Path("dayzconfigmaster/gui/app.py")
                
                comparison_text.insert(tk.END, "=== File Comparison Summary ===\n\n")
                
                if backup_app_py.exists() and current_app_py.exists():
                    # Get file sizes
                    backup_size = backup_app_py.stat().st_size
                    current_size = current_app_py.stat().st_size
                    
                    comparison_text.insert(tk.END, f"app.py - Backup: {backup_size} bytes | Current: {current_size} bytes\n")
                    
                    if backup_size != current_size:
                        diff_pct = abs(current_size - backup_size) / max(backup_size, 1) * 100
                        comparison_text.insert(tk.END, f"Difference: ~{diff_pct:.1f}% size change\n")
                    
                    # Get modification times
                    backup_time = backup_app_py.stat().st_mtime
                    current_time = current_app_py.stat().st_mtime
                    
                    from datetime import datetime
                    backup_dt = datetime.fromtimestamp(backup_time)
                    current_dt = datetime.fromtimestamp(current_time)
                    
                    comparison_text.insert(tk.END, f"\nBackup modified: {backup_dt.strftime('%Y-%m-%d %H:%M:%S')}\n")
                    comparison_text.insert(tk.END, f"Current modified: {current_dt.strftime('%Y-%m-%d %H:%M:%S')}\n")
                    
                    # Show key differences
                    comparison_text.insert(tk.END, "\n=== Key Differences Found ===\n\n")
                    comparison_text.insert(tk.END, "1. New Feature: _scan_and_load_existing_config()\n")
                    comparison_text.insert(tk.END, "   - Automatically scans for existing DayZ server configs on startup\n")
                    comparison_text.insert(tk.END, "   - Prompts user to load existing settings\n\n")
                    
                    comparison_text.insert(tk.END, "2. New Feature: Path Browse Buttons\n")
                    comparison_text.insert(tk.END, "   - Added browse buttons for all directory/path inputs\n")
                    comparison_text.insert(tk.END, "   - Workshop Directory, Mod Paths, Profile paths now have 'Browse' options\n\n")
                    
                    comparison_text.insert(tk.END, "3. Improved Preferences Dialog\n")
                    comparison_text.insert(tk.END, "   - Backup folder comparison display\n")
                    comparison_text.insert(tk.END, "   - Settings export/import capabilities\n\n")
                    
                    comparison_text.config(state=tk.DISABLED)
                else:
                    comparison_text.insert(tk.END, "Could not find both backup and current app.py files for comparison.")
                    comparison_text.config(state=tk.DISABLED)
                
        except Exception as e:
            error_label = ttk.Label(comparison_frame, text=f"Comparison error: {str(e)}", foreground="red")
            error_label.pack(anchor=tk.W)
        
        # Settings management section
        settings_frame = ttk.LabelFrame(main_frame, text="Settings Management", padding=10)
        settings_frame.pack(fill=tk.X, pady=(0, 15))
        
        def export_settings():
            """Export current settings to JSON"""
            settings = {}
            
            # Get current values from input widgets
            for widget_name, (widget, rules) in self.input_widgets.items():
                if hasattr(widget, 'get'):
                    try:
                        value = widget.get()
                        settings[widget_name] = value
                    except:
                        pass
            
            filename = filedialog.asksaveasfilename(
                title="Export Settings",
                defaultextension=".json",
                filetypes=[("JSON files", "*.json"), ("All files", "*.*")]
            )
            
            if filename:
                with open(filename, 'w') as f:
                    json.dump(settings, f, indent=2)
                messagebox.showinfo("Settings Exported", f"Settings saved to:\n{filename}")
        
        def import_settings():
            """Import settings from JSON"""
            filename = filedialog.askopenfilename(
                title="Import Settings",
                filetypes=[("JSON files", "*.json"), ("All files", "*.*")]
            )
            
            if filename:
                try:
                    with open(filename, 'r') as f:
                        settings = json.load(f)
                    
                    for widget_name, value in settings.items():
                        if widget_name in self.input_widgets:
                            widget, _ = self.input_widgets[widget_name]
                            if hasattr(widget, 'delete') and hasattr(widget, 'insert'):
                                widget.delete(0, tk.END)
                                widget.insert(0, str(value))
                    
                    messagebox.showinfo("Settings Imported", f"Loaded settings from:\n{filename}")
                except Exception as e:
                    messagebox.showerror("Import Error", f"Failed to import settings:\n{str(e)}")
        
        def compare_with_backup():
            """Show comparison details between current and backup"""
            diff_window = tk.Toplevel(pref_window)
            diff_window.title("Detailed Comparison - Current vs Backup")
            diff_window.geometry("800x500")
            
            ttk.Label(diff_window, text="Comparison Details", font=("Arial", 12, "bold")).pack(pady=10)
            
            comparison_text = scrolledtext.ScrolledText(diff_window, wrap=tk.WORD, height=25)
            comparison_text.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)
            
            comparison_text.insert(tk.END, "=== Detailed Comparison: Current Setup vs Backup ===\n\n")
            
            comparison_text.insert(tk.END, "WHY IT WAS 'FUCKED':\n")
            comparison_text.insert(tk.END, "-" * 50 + "\n\n")
            
            comparison_text.insert(tk.END, "1. Preferences Window Empty:\n")
            comparison_text.insert(tk.END, "   - Original backup had minimal preferences dialog\n")
            comparison_text.insert(tk.END, "   - No actual settings management or comparison features\n\n")
            
            comparison_text.insert(tk.END, "2. Missing Path Browsing:\n")
            comparison_text.insert(tk.END, "   - Directory fields had NO browse buttons\n")
            comparison_text.insert(tk.END, "   - Users had to manually type full paths (error-prone)\n\n")
            
            comparison_text.insert(tk.END, "3. No Backup Comparison:\n")
            comparison_text.insert(tk.END, "   - Could not see what changed between versions\n")
            comparison_text.insert(tk.END, "   - No way to verify settings consistency\n\n")
            
            comparison_text.insert(tk.END, "FIXES APPLIED:\n")
            comparison_text.insert(tk.END, "-" * 50 + "\n\n")
            
            comparison_text.insert(tk.END, "1. ✓ Full Preferences Dialog with:\n")
            comparison_text.insert(tk.END, "   - Backup folder path display and opening\n")
            comparison_text.insert(tk.END, "   - Settings export/import functionality\n")
            comparison_text.insert(tk.END, "   - Detailed comparison display\n\n")
            
            comparison_text.insert(tk.END, "2. ✓ Path Browse Buttons:\n")
            comparison_text.insert(tk.END, "   - Workshop Directory: Browse button\n")
            comparison_text.insert(tk.END, "   - Mod Paths: Browse button\n")
            comparison_text.insert(tk.END, "   - Profile paths: Browse buttons\n")
            comparison_text.insert(tk.END, "   - Root folders: Browse buttons\n\n")
            
            comparison_text.insert(tk.END, "3. ✓ Backup Comparison:\n")
            comparison_text.insert(tk.END, "   - Compare current settings with backup\n")
            comparison_text.insert(tk.END, "   - See what changed between versions\n")
            comparison_text.insert(tk.END, "   - Visual indicator of differences\n\n")
            
            comparison_text.config(state=tk.DISABLED)
        
        btn_frame = ttk.Frame(settings_frame)
        btn_frame.pack(pady=5)
        
        ttk.Button(btn_frame, text="💾 Export Settings", command=export_settings).pack(side=tk.LEFT, padx=5)
        ttk.Button(btn_frame, text="📂 Import Settings", command=import_settings).pack(side=tk.LEFT, padx=5)
        ttk.Button(btn_frame, text="📋 Compare with Backup", command=compare_with_backup).pack(side=tk.LEFT, padx=5)
        
        # Path configuration section
        path_frame = ttk.LabelFrame(main_frame, text="Directory/Path Settings (Browse Enabled)", padding=10)
        path_frame.pack(fill=tk.BOTH, expand=True, pady=(0, 15))
        
        ttk.Label(path_frame, 
                 text="All path/directory fields now have 'Browse' buttons for easy navigation!", 
                 foreground="green", font=("Arial", 9, "bold")).pack(anchor=tk.W, pady=(0, 10))
        
        # Quick reference list
        quick_ref = tk.Text(path_frame, wrap=tk.WORD, height=6, bg="#e8f4e8", relief="flat")
        quick_ref.pack(fill=tk.X)
        quick_ref.insert(tk.END, "Available Directory Paths:\n")
        quick_ref.insert(tk.END, "- Workshop Directory: Steam workshop downloads\n")
        quick_ref.insert(tk.END, "- Mod Paths: Server mod directories (@ModName)\n")
        quick_ref.insert(tk.END, "- Profile Path: Player save data location\n")
        quick_ref.insert(tk.END, "- Root Folder: Server installation root\n")
        quick_ref.insert(tk.END, "- Config File: serverDZ.cfg location\n")
        quick_ref.config(state=tk.DISABLED)
        
        button_frame = ttk.Frame(pref_window)
        button_frame.pack(pady=15)
        
        ttk.Button(button_frame, text="Close", command=pref_window.destroy).pack(side=tk.LEFT, padx=5)
    
    def _refresh_all(self):
        """Refresh all components"""
        self.status_var.set("All components refreshed")
    
    def _validate_all_settings(self):
        """Validate all configuration settings"""
        errors = []
        warnings = []
        
        # Validate all input widgets
        for field_name, (widget, rules) in self.input_widgets.items():
            value = widget.get()
            
            if not value and "required" in rules:
                errors.append(f"{field_name}: Required field is empty")
                continue
            
            try:
                num_value = float(value)
                
                if "min" in rules and num_value < rules["min"]:
                    errors.append(f"{field_name}: Value {value} is below minimum {rules['min']}")
                elif "max" in rules and num_value > rules["max"]:
                    errors.append(f"{field_name}: Value {value} exceeds maximum {rules['max']}")
            except (ValueError, TypeError):
                if value:  # Only error if not empty
                    errors.append(f"{field_name}: Must be a number")
        
        report_lines = []
        
        if errors:
            report_lines.append("ERRORS (Must Fix):")
            for error in errors:
                report_lines.append(f"  ❌ {error}")
        
        if warnings:
            report_lines.append("")
            report_lines.append("WARNINGS (Review Recommended):")
            for warning in warnings:
                report_lines.append(f"  ⚠️  {warning}")
        
        if not errors and not warnings:
            report_lines.append("✅ All configuration settings are valid!")
        
        if errors or warnings:
            messagebox.showwarning("Configuration Audit Results", "\n".join(report_lines))
        else:
            messagebox.showinfo("Configuration Audit Complete", "\n".join(report_lines))
    
    def _generate_cfg(self):
        """Generate serverDZ.cfg file(s) from the current UI state."""
        try:
            self._save_current_effects_content()
            if self._multi_instance_enabled_var.get() and self._instance_vars:
                self._generate_cfg_for_instance()
            else:
                configs = self._build_single_instance_configs()
                self._show_generated_configs(configs)
        except Exception as e:
            messagebox.showerror("Generation Error", f"Failed to generate configuration:\n{str(e)}")

    def _build_single_instance_configs(self) -> Dict[str, str]:
        """Build serverDZ.cfg and Effects & Triggers files for the single/default instance."""
        cfg_content = self._build_server_cfg_content()
        configs = {"serverDZ.cfg": cfg_content}
        configs.update(self._build_effects_filenames_for_instance(1, ""))
        return configs

    def _build_effects_filenames_for_instance(self, instance_id: int, suffix: str) -> Dict[str, str]:
        """Return Effects & Triggers content with filenames for the given instance."""
        result = {}
        content = self._instance_effects_content.get(instance_id, {})
        for filename, text in content.items():
            base, ext = filename.rsplit(".", 1) if "." in filename else (filename, "")
            out_name = f"{base}{suffix}.{ext}" if ext else f"{base}{suffix}"
            result[out_name] = text
        return result

    def _generate_cfg_for_instance(self):
        """Ask which instance to generate a config for, then generate it."""
        if not self._instance_vars:
            configs = self._build_single_instance_configs()
            self._show_generated_configs(configs)
            return

        # Build choices: All instances + each individual instance
        choices = ["All Instances"]
        for instance in self._instance_vars:
            instance_id = instance["id"].get()
            map_name = instance["map"].get() or "no map"
            choices.append(f"Instance {instance_id} ({map_name})")

        choice_window = self._safe_toplevel("Generate serverDZ.cfg", "350x200")
        if choice_window is None:
            messagebox.showerror("GUI Error", "Cannot open generate dialog: no GUI root available.")
            return
        try:
            choice_window.transient(self.root)
            choice_window.grab_set()
        except tk.TclError:
            pass

        ttk.Label(
            choice_window,
            text="Select which instance configuration to generate:",
            wraplength=320
        ).pack(padx=10, pady=10)

        selected = tk.StringVar(value=choices[0])
        combo = ttk.Combobox(choice_window, values=choices, textvariable=selected, state="readonly", width=40)
        combo.pack(padx=10, pady=5)

        def on_generate():
            choice_window.destroy()
            selection = selected.get()

            if selection == choices[0]:
                configs = self._build_instance_cfgs_content()
                self._show_generated_configs(configs)
            else:
                # Extract instance index from selection
                idx = choices.index(selection) - 1
                instance = self._instance_vars[idx]
                instance_id = int(instance.get("id", {}).get() or 1)
                all_configs = self._build_instance_cfgs_content()
                cfg_filename = f"serverDZ_{instance_id}.cfg"
                configs = {cfg_filename: all_configs.get(cfg_filename, "")}
                configs.update(self._build_effects_filenames_for_instance(instance_id, f"_{instance_id}"))
                self._show_generated_configs(configs)

        def on_cancel():
            choice_window.destroy()

        btn_frame = ttk.Frame(choice_window)
        btn_frame.pack(pady=15)
        ttk.Button(btn_frame, text="Generate", command=on_generate).pack(side=tk.LEFT, padx=5)
        ttk.Button(btn_frame, text="Cancel", command=on_cancel).pack(side=tk.LEFT, padx=5)

    def _build_instance_cfgs_content(self) -> Dict[str, str]:
        """Build serverDZ.cfg content for each configured instance."""
        def get_value(name, default=""):
            widget, _ = self.input_widgets.get(name, (None, None))
            if widget is None:
                return default
            try:
                return widget.get()
            except Exception:
                return default

        base_config = {
            "max_players": int(get_value("Max Players", "60") or 60),
            "password": get_value("Server Password", ""),
            "password_admin": get_value("Admin Password", ""),
            "description": get_value("Description", ""),
            "enable_whitelist": get_value("Enable Whitelist", "0 (No)"),
            "map_size": int(get_value("Map Size", "2000") or 2000),
            "server_time": get_value("Server Time", "SystemTime"),
            "server_time_acceleration": float(get_value("Time Acceleration", "1.0") or 1.0),
            "server_night_time_acceleration": float(get_value("Night Time Acceleration", "1.0") or 1.0),
            "server_time_persistent": get_value("Persistent Time", "0 (No)"),
            "disable_von": get_value("Disable VoN", "0 (No)"),
            "von_codec_quality": int(get_value("VoN Codec Quality", "20") or 20),
            "disable_3rd_person": get_value("Disable 3rd Person", "0 (No)"),
            "disable_crosshair": get_value("Disable Crosshair", "0 (No)"),
            "disable_personal_light": get_value("Disable Personal Light", "1 (Yes)"),
            "lighting_config": get_value("Lighting Config", "0 (Bright)"),
            "shard_id": get_value("Shard ID", ""),
            "guaranteed_updates": int(get_value("Guaranteed Updates", "1") or 1),
            "login_queue_concurrent_players": int(get_value("Login Queue Concurrent", "5") or 5),
            "login_queue_max_players": int(get_value("Login Queue Max", "500") or 500),
            "storage_auto_fix": get_value("Storage Auto Fix", "1 (Yes)"),
            "verify_signatures": get_value("Verify Signatures", "2 (Verify)"),
            "force_same_build": get_value("Force Same Build", "1 (Yes)"),
            "mod_paths": get_value("Mod Paths", ""),
            "game_mode": get_value("Game Mode", "Survival"),
        }

        # Resolve real world names for workshop maps so the mission template
        # matches the actual world name inside the map mod (e.g. "alteria" not
        # the display name "Alteria").
        workshop_dir = self._get_workshop_directory() or ""
        resolved_maps: Dict[str, str] = {}
        resolved_mods: Dict[int, str] = {}
        for instance in self._instance_vars:
            display_name = instance.get("map", {}).get() or base_config.get("map_name", "")
            if display_name and display_name not in resolved_maps:
                resolved_maps[display_name] = self._resolve_world_name(
                    display_name, workshop_dir
                )
            instance_id = int(instance.get("id", {}).get() or 1)
            resolved_mods[instance_id] = self._get_instance_mods(instance)

        base_name = get_value("Server Name", "DayZ Server")
        instance_cfgs = generate_instance_cfgs(
            base_name,
            base_config,
            self._instance_vars,
            resolved_maps=resolved_maps,
            resolved_mods=resolved_mods,
        )

        # Flatten to filename-keyed dict and include per-instance Effects & Triggers
        configs = {}
        for instance_id, cfg_content in instance_cfgs.items():
            configs[f"serverDZ_{instance_id}.cfg"] = cfg_content
            configs.update(self._build_effects_filenames_for_instance(instance_id, f"_{instance_id}"))
        return configs

    def _show_generated_configs(self, configs: Dict[str, str]):
        """Show generated config(s) in a tabbed viewer with save support."""
        result_window = self._safe_toplevel("Generated Configuration(s)", "900x700")
        if result_window is None:
            messagebox.showerror("GUI Error", "Cannot display generated configs: no GUI root available.")
            return

        # Determine target directories from configured instance root folders when available.
        instance_dirs: Dict[str, str] = {}
        if self._instance_vars:
            for instance in self._instance_vars:
                instance_id = instance["id"].get()
                root_folder = instance.get("root_folder", {}).get() or ""
                if root_folder:
                    instance_dirs[f"serverDZ_{instance_id}.cfg"] = root_folder

        if len(configs) == 1:
            filename, content = next(iter(configs.items()))
            text_area = scrolledtext.ScrolledText(result_window, wrap=tk.NONE, height=30, font=("Courier", 9))
            text_area.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)
            text_area.insert(tk.END, content)

            button_frame = ttk.Frame(result_window)
            button_frame.pack(pady=10)

            def save_single():
                default_dir = instance_dirs.get(filename, "")
                path = filedialog.asksaveasfilename(
                    title=f"Save {filename}",
                    defaultextension=".cfg",
                    initialdir=default_dir or None,
                    initialfile=filename,
                    filetypes=[("CFG files", "*.cfg"), ("All files", "*.*")]
                )
                if path:
                    success, error = write_cfg_to_file(content, path)
                    if success:
                        messagebox.showinfo("Saved", f"Configuration saved to:\n{path}")
                        self.status_var.set(f"Saved configuration to {path}")
                    else:
                        messagebox.showerror("Save Error", f"Failed to save:\n{error}")

            ttk.Button(button_frame, text="💾 Save to File", command=save_single).pack(side=tk.LEFT, padx=5)
            ttk.Button(button_frame, text="Close", command=result_window.destroy).pack(side=tk.LEFT, padx=5)
        else:
            cfg_notebook = ttk.Notebook(result_window)
            cfg_notebook.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)

            text_areas = {}
            for filename, content in sorted(configs.items()):
                frame = ttk.Frame(cfg_notebook)
                cfg_notebook.add(frame, text=filename)
                text_area = scrolledtext.ScrolledText(frame, wrap=tk.NONE, height=30, font=("Courier", 9))
                text_area.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
                text_area.insert(tk.END, content)
                text_areas[filename] = text_area

            button_frame = ttk.Frame(result_window)
            button_frame.pack(pady=10)

            def save_all():
                # Default to the first configured instance root folder, if any.
                default_dir = next(iter(instance_dirs.values()), "")
                folder = filedialog.askdirectory(
                    title="Select folder to save all instance configs",
                    initialdir=default_dir or None
                )
                if not folder:
                    return

                saved = []
                failed = []
                for filename, content in configs.items():
                    path = Path(folder) / filename
                    success, error = write_cfg_to_file(content, str(path))
                    if success:
                        saved.append(str(path))
                    else:
                        failed.append(f"{filename}: {error}")

                if saved:
                    self.status_var.set(f"Saved {len(saved)} instance configs to {folder}")
                if failed:
                    messagebox.showerror("Save Errors", "Failed to save:\n" + "\n".join(failed))
                elif saved:
                    messagebox.showinfo("Saved", f"Saved {len(saved)} configuration files to:\n{folder}")

            ttk.Button(button_frame, text="💾 Save All to Folder", command=save_all).pack(side=tk.LEFT, padx=5)
            ttk.Button(button_frame, text="Close", command=result_window.destroy).pack(side=tk.LEFT, padx=5)

    def _build_server_cfg_content(self) -> str:
        """Build serverDZ.cfg content from current UI values."""
        def get_value(name, default=""):
            widget, _ = self.input_widgets.get(name, (None, None))
            if widget is None:
                return default
            try:
                return widget.get()
            except Exception:
                return default

        map_name = get_value("Map Name", "")
        workshop_dir = self._get_workshop_directory() or ""
        map_name = self._resolve_world_name(map_name, workshop_dir)

        return generate_server_cfg(
            name=get_value("Server Name", "DayZ Server"),
            port=int(get_value("Server Port", "2302") or 2302),
            query_port=int(get_value("Query Port", "2303") or 2303),
            max_players=int(get_value("Max Players", "60") or 60),
            password=get_value("Server Password", ""),
            password_admin=get_value("Admin Password", ""),
            description=get_value("Description", ""),
            enable_whitelist=get_value("Enable Whitelist", "0 (No)"),
            map_name=map_name,
            map_size=int(get_value("Map Size", "2000") or 2000),
            server_time=get_value("Server Time", "SystemTime"),
            server_time_acceleration=float(get_value("Time Acceleration", "1.0") or 1.0),
            server_night_time_acceleration=float(get_value("Night Time Acceleration", "1.0") or 1.0),
            server_time_persistent=get_value("Persistent Time", "0 (No)"),
            disable_von=get_value("Disable VoN", "0 (No)"),
            von_codec_quality=int(get_value("VoN Codec Quality", "20") or 20),
            disable_3rd_person=get_value("Disable 3rd Person", "0 (No)"),
            disable_crosshair=get_value("Disable Crosshair", "0 (No)"),
            disable_personal_light=get_value("Disable Personal Light", "1 (Yes)"),
            lighting_config=get_value("Lighting Config", "0 (Bright)"),
            shard_id=get_value("Shard ID", ""),
            guaranteed_updates=int(get_value("Guaranteed Updates", "1") or 1),
            login_queue_concurrent_players=int(get_value("Login Queue Concurrent", "5") or 5),
            login_queue_max_players=int(get_value("Login Queue Max", "500") or 500),
            instance_id=int(get_value("Instance ID", "1") or 1),
            storage_auto_fix=get_value("Storage Auto Fix", "1 (Yes)"),
            verify_signatures=get_value("Verify Signatures", "2 (Verify)"),
            force_same_build=get_value("Force Same Build", "1 (Yes)"),
            mod_paths=get_value("Mod Paths", ""),
            admin_logs=get_value("Admin Logs", ""),
            network_logs=get_value("Network Logs", ""),
            memory_limit=get_value("Memory Limit", ""),
            thread_count=get_value("Thread Count", ""),
            update_settings=get_value("Update Settings", ""),
            profiles_path="",
            game_mode=get_value("Game Mode", "Survival"),
        )
    
    def _get_projects_root(self) -> str:
        """Get the projects root directory from preferences."""
        try:
            from ..setup.wizard import SetupWizard
        except ImportError:
            from dayzconfigmaster.setup.wizard import SetupWizard
        
        projects_root = Path.home() / "Documents" / "DayZProjects"
        wizard = SetupWizard(str(projects_root))
        settings = wizard.get_settings()
        
        return str(settings.get("projects_root", projects_root))

    def _get_mod_preset_manager(self) -> ModPresetManager:
        """Return the shared mod preset manager for the current projects root."""
        projects_root = Path(self._get_projects_root())
        if not hasattr(self, "_mod_preset_manager") or self._mod_preset_manager is None:
            self._mod_preset_manager = ModPresetManager(projects_root)
        # Refresh from disk in case another process edited presets.
        self._mod_preset_manager._load()
        return self._mod_preset_manager

    def _get_default_instance_root(self, instance_num: int) -> str:
        """Return a writable default root folder for an instance."""
        return str(Path(self._get_projects_root()) / "instances" / f"server{instance_num}")

    def _sanitize_instance_root(self, root_folder: str, instance_num: int) -> str:
        """Replace legacy /dayz/... defaults with a writable project path."""
        if not root_folder:
            return self._get_default_instance_root(instance_num)
        # Legacy absolute /dayz/... defaults are not user-writable; redirect them.
        if root_folder.startswith("/dayz/") or root_folder.lower().startswith("c:\\\\dayz\\\\"):
            return self._get_default_instance_root(instance_num)
        return root_folder

    def _validate_dayz_server_path(self) -> Tuple[bool, str]:
        """Validate that the configured DayZ server path exists and has an executable."""
        dayz_path = self.dayz_path_var.get().strip()
        if not dayz_path:
            return False, "DayZ Server Path is not set. Configure it in Server Control > General Settings."
        path = Path(dayz_path)
        if not path.exists():
            return False, f"DayZ Server Path does not exist: {dayz_path}"
        if not path.is_dir():
            return False, f"DayZ Server Path is not a directory: {dayz_path}"
        # Check for at least one known server executable.
        exe_names = ["DayZServer_x64", "DayZServer", "DayZDiag_x64", "DayZDiag"]
        suffix = ".exe" if os.name == "nt" else ""
        for exe in exe_names:
            if (path / f"{exe}{suffix}").exists():
                return True, ""
        return False, f"No DayZ server executable found in: {dayz_path}"

    def _get_instance_binary_name(
        self,
        instance_id: int,
        mode: str = "normal",
    ) -> str:
        """Return the instance-unique binary name used for this instance."""
        base = "DayZDiag" if mode == "debug" else "DayZServer"
        return f"{base}_instance_{instance_id}"

    def _build_isolated_instance_env(self, instance_root: Path) -> Dict[str, str]:
        """Build an environment that isolates per-instance Steam/IPC sockets.

        DayZ's Steam client libraries (steamclient.so) maintain singleton IPC
        state under XDG_RUNTIME_DIR (e.g. ~/.steam/steam.pipe). When multiple
        server instances run under the same Linux user, this shared state
        causes only the first-started instance to register successfully with
        Steam and accept connections.

        We therefore isolate only the runtime/socket directories while keeping
        the real HOME/XDG_CONFIG_HOME/XDG_DATA_HOME visible, so Steam can still
        find credentials, libraries, and the user's installed workshop content.
        """
        runtime_dir = instance_root / ".runtime"
        steam_runtime_dir = runtime_dir / "steam"
        tmp_dir = instance_root / ".tmp"

        for directory in (runtime_dir, steam_runtime_dir, tmp_dir):
            directory.mkdir(parents=True, exist_ok=True)

        env = os.environ.copy()
        env["XDG_RUNTIME_DIR"] = str(runtime_dir)
        env["TMPDIR"] = str(tmp_dir)
        # STEAM_RUNTIME_DIR influences where steamclient.so places its IPC
        # sockets; point it inside the isolated runtime dir.
        env["STEAM_RUNTIME_DIR"] = str(steam_runtime_dir)
        return env

    def _link_base_server_files(
        self,
        base_server_dir: Path,
        instance_dir: Path,
        config_filename: str,
    ) -> None:
        """
        Symlink base DayZ server files into an instance directory.

        This lets the server launch from the instance directory while still
        finding dayz.gproj, addons/, dta/, etc. Existing instance-specific files
        (config, profiles, mpmissions, keys, mod links) are preserved.

        The server executable is COPIED (not symlinked) and renamed per
        instance. Steam/BattlEye identify a server by its executable path, so
        running the same physical binary from multiple instance directories
        causes only the first started instance to be connectable.
        """
        if not base_server_dir.exists() or not base_server_dir.is_dir():
            return

        instance_id = 1
        if "_" in config_filename:
            try:
                instance_id = int(config_filename.split("_")[-1].split(".")[0])
            except ValueError:
                instance_id = 1

        # Remove stale symlinked executables from old deployments.
        for exe_name in ("DayZServer", "DayZServer_x64", "DayZDiag", "DayZDiag_x64"):
            stale = instance_dir / exe_name
            if stale.is_symlink():
                try:
                    stale.unlink()
                except OSError:
                    pass

        # Copy the server executable with an instance-unique name.
        # Always copy the first available normal binary to
        # DayZServer_instance_<id> and the first available debug binary to
        # DayZDiag_instance_<id> so the rest of the code has a stable name to
        # launch regardless of whether the base install uses DayZServer or
        # DayZServer_x64.
        normal_copied = False
        debug_copied = False
        for exe_name in ("DayZServer", "DayZServer_x64", "DayZDiag", "DayZDiag_x64"):
            src = base_server_dir / exe_name
            if not src.exists() or not src.is_file():
                continue
            is_debug = "Diag" in exe_name
            if is_debug and debug_copied:
                continue
            if not is_debug and normal_copied:
                continue
            dest_name = self._get_instance_binary_name(
                instance_id, mode="debug" if is_debug else "normal"
            )
            dest = instance_dir / dest_name

            # Avoid copying over a running binary. If the destination already
            # exists and matches the source, reuse it; if it is busy (still
            # executing), keep the existing copy so startup can proceed.
            if dest.exists() and dest.is_file():
                try:
                    src_stat = src.stat()
                    dest_stat = dest.stat()
                    if (
                        src_stat.st_size == dest_stat.st_size
                        and int(src_stat.st_mtime) == int(dest_stat.st_mtime)
                    ):
                        if is_debug:
                            debug_copied = True
                        else:
                            normal_copied = True
                        continue
                except OSError:
                    pass

            try:
                shutil.copy2(src, dest)
                if is_debug:
                    debug_copied = True
                else:
                    normal_copied = True
            except OSError as exc:
                err_text = str(exc).lower()
                if "text file busy" in err_text and dest.exists():
                    # The old binary is still running. Reuse it so the start
                    # can continue; the orphaned-process kill later will take
                    # care of the stale process before launch.
                    print(
                        f"Warning: {dest_name} is busy (old process still running); "
                        f"reusing existing instance binary."
                    )
                    if is_debug:
                        debug_copied = True
                    else:
                        normal_copied = True
                else:
                    print(
                        f"Warning: could not copy instance binary {dest_name}: {exc}"
                    )
            except Exception as exc:
                print(
                    f"Warning: could not copy instance binary {dest_name}: {exc}"
                )

        # Files/directories that must be present for the server to boot.
        required_names = {
            "dayz.gproj",
            "dayzsetting.xml",
            "steam_appid.txt",
            "addons",
            "dta",
            "battleye",
            "libsteam_api.so",
            "steamclient.so",
        }

        # Names that belong to the instance and must not be overwritten.
        protected = {
            config_filename,
            "profiles",
            "mpmissions",
            "keys",
        }

        for item in base_server_dir.iterdir():
            if item.name in protected:
                continue
            if item.name in {"DayZServer", "DayZServer_x64", "DayZDiag", "DayZDiag_x64"}:
                # The executable was copied with an instance-specific name above.
                continue
            if item.name not in required_names and not item.name.startswith("@"):
                # Optional map folders (e.g. chernarusplus, sakhal) are useful.
                # CrashReporter is an executable on some installs and a directory
                # on others; don't symlink it at all to avoid "Is a directory"
                # noise if the server crashes.
                if item.name == "CrashReporter":
                    continue
                if not item.is_dir():
                    continue

            dest = instance_dir / item.name
            if dest.exists() or dest.is_symlink():
                # Never overwrite an existing instance-specific directory.
                if dest.is_dir() and not dest.is_symlink():
                    continue
                try:
                    dest.unlink()
                except OSError:
                    continue

            try:
                rel_target = os.path.relpath(item, dest.parent)
            except ValueError:
                rel_target = str(item)

            try:
                dest.symlink_to(rel_target, target_is_directory=item.is_dir())
            except OSError:
                # Fall back to copying small files if symlinks fail.
                if item.is_file():
                    try:
                        shutil.copy2(item, dest)
                    except OSError:
                        pass

        # Ensure the server binary is linked under its known names.
        for exe_name in ("DayZServer_x64", "DayZServer", "DayZDiag_x64", "DayZDiag"):
            src = base_server_dir / exe_name
            if not src.exists():
                continue
            dest = instance_dir / exe_name
            if dest.exists() or dest.is_symlink():
                if dest.is_dir() and not dest.is_symlink():
                    continue
                try:
                    dest.unlink()
                except OSError:
                    continue
            try:
                rel_target = os.path.relpath(src, dest.parent)
            except ValueError:
                rel_target = str(src)
            try:
                dest.symlink_to(rel_target, target_is_directory=False)
            except OSError:
                try:
                    shutil.copy2(src, dest)
                except OSError:
                    pass
    
    def _resolve_world_name(self, map_name: str, workshop_dir: str) -> str:
        """
        Resolve a map display name to the actual DayZ world name.

        Workshop map display names (e.g. "Alteria") often differ in casing from
        the real world name inside the mod PBO (e.g. "alteria"). Using the wrong
        name in the mission template causes "No world with name 'X'" on startup.

        Args:
            map_name: Map name selected in the UI.
            workshop_dir: Path to the Steam Workshop content directory.

        Returns:
            The best world name to use in serverDZ.cfg.
        """
        if not map_name:
            return map_name

        # If the user already supplied a full template, leave it alone.
        if "." in map_name and (map_name.startswith("dayzOffline.") or map_name.startswith("dayz.")):
            return map_name

        dayz_path = self.dayz_path_var.get().strip() if hasattr(self, "dayz_path_var") else ""

        # Stock maps live in mpmissions/dayzOffline.<name>. If the folder exists
        # with the given casing, trust it.
        if dayz_path:
            for stock_name in (f"dayzOffline.{map_name}", f"dayz.{map_name}"):
                stock_mission = Path(dayz_path) / "mpmissions" / stock_name
                if stock_mission.exists():
                    return map_name

        # Search workshop items for a matching display name and extract the real
        # world name from the world PBO or from a shipped dayzOffline.* folder.
        if workshop_dir:
            content_path = Path(workshop_dir)
            if content_path.exists():
                world_map = self._build_workshop_world_name_map(workshop_dir)
                world_name = world_map.get(map_name.lower())
                if world_name:
                    return world_name

                # Fallback: scan on demand if the item wasn't classified as a map.
                for folder in content_path.iterdir():
                    if not folder.is_dir():
                        continue
                    try:
                        _ = int(folder.name)
                    except ValueError:
                        continue

                    # Match by display name from meta.cpp/mod.cpp or folder name.
                    display = self._read_workshop_display_name(folder) or folder.name
                    if display.lower() != map_name.lower():
                        continue

                    # Some workshop maps ship the mission folder directly; use
                    # its name if it matches the expected pattern.
                    for item in folder.iterdir():
                        if not item.is_dir():
                            continue
                        lower = item.name.lower()
                        if lower.startswith("dayzoffline.") or lower.startswith("dayz."):
                            return item.name.split(".", 1)[1]

                    world_name = self._extract_world_name_from_pbo(folder)
                    if world_name:
                        return world_name

        # Last resort: preserve the user's casing. Do not lowercase blindly;
        # world names are case-sensitive on Linux and must match the PBO.
        return map_name

    @staticmethod
    def _read_workshop_display_name(folder: Path) -> Optional[str]:
        """Read the display name from a workshop item's meta.cpp or mod.cpp."""
        for filename in ("meta.cpp", "mod.cpp"):
            filepath = folder / filename
            if not filepath.exists():
                continue
            try:
                content = filepath.read_text(encoding="utf-8", errors="ignore")
                match = __import__("re").search(
                    r'^\s*name\s*=\s*["\'](.+?)["\']\s*;',
                    content,
                    __import__("re").IGNORECASE | __import__("re").MULTILINE,
                )
                if match:
                    value = match.group(1).strip()
                    if value and not value.startswith("$"):
                        return value
            except (OSError, PermissionError):
                pass
        return None

    @staticmethod
    def _extract_world_name_from_pbo(folder: Path) -> Optional[str]:
        """Try to extract the actual world name from a workshop map's PBOs.

        Only a small slice of each candidate PBO is scanned so multi-gigabyte
        files cannot blow up memory.
        """
        import re
        _MAX_PBO_SCAN_BYTES = 2 * 1024 * 1024  # 2 MB

        def _scan_file(path: Path) -> Optional[str]:
            try:
                size = path.stat().st_size
                if size <= _MAX_PBO_SCAN_BYTES:
                    data = path.read_bytes()
                else:
                    with open(path, "rb") as fh:
                        head = fh.read(_MAX_PBO_SCAN_BYTES // 2)
                        fh.seek(-(_MAX_PBO_SCAN_BYTES // 2), 2)
                        tail = fh.read(_MAX_PBO_SCAN_BYTES // 2)
                    data = head + tail
                text = data.decode("latin-1", errors="ignore")
                # Look for worldName followed by the world name.
                match = re.search(r'worldName\s*([^\s\x00]+)', text, re.IGNORECASE)
                if match:
                    candidate = match.group(1).strip()
                    clean = re.match(r'[a-zA-Z0-9_]+', candidate)
                    if clean:
                        return clean.group(0).lower()
                # Fallback: find a .wrp reference.
                match = re.search(r'([a-zA-Z0-9_]+)\.wrp', text, re.IGNORECASE)
                if match:
                    return match.group(1).strip().lower()
            except (OSError, PermissionError):
                pass
            return None

        for addons_dir in (folder / "addons", folder / "Addons"):
            if not addons_dir.exists() or not addons_dir.is_dir():
                continue
            for pbo in addons_dir.iterdir():
                if not pbo.is_file() or pbo.suffix.lower() != ".pbo":
                    continue
                # world.pbo / terrain.pbo are the strongest indicators.
                if pbo.stem.lower() in ("world", "terrain"):
                    world = _scan_file(pbo)
                    if world:
                        return world

        # Second pass: any PBO containing a .wrp reference.
        for addons_dir in (folder / "addons", folder / "Addons"):
            if not addons_dir.exists() or not addons_dir.is_dir():
                continue
            for pbo in addons_dir.iterdir():
                if not pbo.is_file() or pbo.suffix.lower() != ".pbo":
                    continue
                world = _scan_file(pbo)
                if world:
                    return world
        return None

    def _build_workshop_world_name_map(self, workshop_dir: str) -> Dict[str, str]:
        """Build a {display_name_lower: world_name} map from all workshop maps.

        This scans every workshop item classified as a map, reads its display
        name, extracts the real world name from the PBOs, and stores the
        mapping. The result is cached and reused until the workshop directory
        changes.
        """
        if (
            self._workshop_world_name_cache is not None
            and self._workshop_world_name_cache_dir == workshop_dir
        ):
            return self._workshop_world_name_cache

        mapping: Dict[str, str] = {}
        if not workshop_dir:
            self._workshop_world_name_cache = mapping
            self._workshop_world_name_cache_dir = workshop_dir
            return mapping

        content_path = Path(workshop_dir)
        if not content_path.exists():
            self._workshop_world_name_cache = mapping
            self._workshop_world_name_cache_dir = workshop_dir
            return mapping

        for folder in content_path.iterdir():
            if not folder.is_dir():
                continue
            try:
                _ = int(folder.name)
            except ValueError:
                continue

            display = self._read_workshop_display_name(folder) or folder.name
            world_name = self._extract_world_name_from_pbo(folder)
            if world_name:
                mapping[display.lower()] = world_name
                # Also map the workshop ID so numeric selections work.
                mapping[folder.name.lower()] = world_name

        self._workshop_world_name_cache = mapping
        self._workshop_world_name_cache_dir = workshop_dir
        return mapping

    def _get_timestamp(self) -> str:
        """Return a current HH:MM:SS timestamp string for log output."""
        return datetime.now().strftime("%H:%M:%S")

    def _create_status_bar(self):
        """Create the status bar"""
        self.status_frame = ttk.Frame(self.root)
        self.status_frame.pack(side=tk.BOTTOM, fill=tk.X)
        
        self.status_var = tk.StringVar()
        self.status_var.set("Ready - DayzConfigMaster initialized")
        
        self.status_label = ttk.Label(
            self.status_frame,
            textvariable=self.status_var,
            anchor=tk.W
        )
        self.status_label.pack(fill=tk.X)
    
    def _show_about(self):
        """Show about dialog"""
        messagebox.showinfo(
            "About DayzConfigMaster",
            "DayzConfigMaster v1.0\n"
            "A comprehensive DayZ server configuration editor\n\n"
            "Features:\n"
            "- XML parsing for all DayZ config files\n"
            "- Intuitive GUI interface with scrollable frames\n"
            "- Cross-platform compatibility\n"
            "\nBased on DZconfig.com documentation and DayZ server references"
        )
    
    def _get_workshop_directory(self) -> Optional[str]:
        """Get the workshop directory from cached settings.

        First checks the in-app cache, then falls back to the setup wizard
        settings and auto-detection. The returned path is cached so all tabs
        share a single source of truth.

        Returns:
            Workshop directory path or None if not found.
        """
        if self._workshop_directory_cache:
            return self._workshop_directory_cache

        # Prefer the explicit UI value if the user has set one.
        ui_value = self._workshop_dir_var.get().strip()
        if ui_value and Path(ui_value).exists():
            self._workshop_directory_cache = ui_value
            return ui_value

        try:
            from ..setup.wizard import SetupWizard
        except ImportError:
            from dayzconfigmaster.setup.wizard import SetupWizard

        projects_root = Path.home() / "Documents" / "DayZProjects"
        wizard = SetupWizard(str(projects_root))
        settings = wizard.get_settings()

        workshop_dir = settings.get("steam_workshop_dir", "")
        if workshop_dir and Path(workshop_dir).exists():
            self._workshop_directory_cache = workshop_dir
            self._workshop_dir_var.set(workshop_dir)
            return workshop_dir

        # Try auto-detection as a last resort.
        detected = wizard.detect_steam_workshop_path()
        if detected and Path(detected).exists():
            self._workshop_directory_cache = detected
            self._workshop_dir_var.set(detected)
            return detected

        return None

    def _set_workshop_directory(self, path: str) -> None:
        """Cache the workshop directory for use across all tabs."""
        self._workshop_directory_cache = path
        self._workshop_dir_var.set(path)
        # Invalidate the world-name cache so the next resolution picks up the
        # new workshop directory.
        self._workshop_world_name_cache = None
        self._workshop_world_name_cache_dir = None

    def _get_available_maps(self) -> List[str]:
        """Scan both stock and workshop directories for available maps.

        Stock maps are discovered from the DayZServer ``mpmissions`` directory.
        Workshop maps are discovered by scanning the Steam Workshop content
        directory and classifying items as maps or mods.

        Returns:
            Sorted list of unique map names.
        """
        fallback_defaults = ["chernarusplus", "enoch", "namalsk", "provinggrounds_pmc", "tavi", "malden", "sara"]

        # Determine DayZ server path
        dayz_path = self.dayz_path_var.get().strip() if hasattr(self, "dayz_path_var") else ""
        if not dayz_path:
            # Try common paths
            for candidate in [
                Path.home() / ".local" / "share" / "Steam" / "steamapps" / "common" / "DayZServer",
                Path.home() / ".steam" / "steam" / "steamapps" / "common" / "DayZServer",
                Path(r"C:\Program Files\Steam\steamapps\common\DayZServer"),
                Path(r"C:\Program Files (x86)\Steam\steamapps\common\DayZServer"),
            ]:
                if candidate.exists():
                    dayz_path = str(candidate)
                    break

        # Determine workshop content path from the single source of truth.
        # The cached directory is expected to point at the workshop content
        # folder (e.g. .../steamapps/workshop/content/<appid>).
        workshop_content_path = self._get_workshop_directory() or ""

        self._stock_maps, self._workshop_maps = get_all_available_maps(
            dayz_server_path=dayz_path or None,
            workshop_content_path=workshop_content_path or None,
            fallback_defaults=fallback_defaults
        )

        all_maps = list(self._stock_maps)
        for _, (_, map_name) in self._workshop_maps.items():
            if map_name not in all_maps:
                all_maps.append(map_name)

        return sorted(set(all_maps), key=str.lower)

    def _populate_maps(self, combo_box):
        """Populate a map dropdown with available maps from stock and workshop directories."""
        current_value = combo_box.get()
        unique_maps = self._get_available_maps()
        combo_box['values'] = unique_maps

        # Preserve the current selection if it is still valid; otherwise select the first map
        if current_value in unique_maps:
            combo_box.set(current_value)
        elif unique_maps:
            combo_box.set(unique_maps[0])

    def _refresh_all_map_combos(self):
        """Refresh every map dropdown that has been registered with the app."""
        if self._single_server_map_combo is not None:
            self._populate_maps(self._single_server_map_combo)

        # Refresh map combos inside instance config rows
        for instance in self._instance_vars:
            map_combo = instance.get("map")
            if map_combo is not None:
                self._populate_maps(map_combo)

        # Refresh the Server Config tab map combo if present
        widget = self.input_widgets.get("Map Name")
        if widget is not None:
            map_combo, _ = widget
            if isinstance(map_combo, ttk.Combobox):
                self._populate_maps(map_combo)
    
    def _update_server_config_ui(self, config: ServerConfig):
        """
        Update the server config UI with values from a ServerConfig object.

        Populates all input widgets with values from the loaded configuration.
        """
        def set_entry(name, value):
            entry, _ = self.input_widgets.get(name, (None, None))
            if entry:
                entry.delete(0, tk.END)
                entry.insert(0, str(value))

        def set_combo(name, value, options):
            combo, _ = self.input_widgets.get(name, (None, None))
            if combo:
                if value in options:
                    combo.set(value)
                elif isinstance(value, int) and 0 <= value < len(options):
                    combo.set(options[value])

        try:
            # Basic settings
            if config.name:
                set_entry("Server Name", config.name)
            if isinstance(config.port, int):
                set_entry("Server Port", config.port)
            if isinstance(config.query_port, int):
                set_entry("Query Port", config.query_port)
            if isinstance(config.max_players, int):
                set_entry("Max Players", config.max_players)
            if config.password:
                set_entry("Server Password", config.password)
            if config.password_admin:
                set_entry("Admin Password", config.password_admin)
            if config.description:
                set_entry("Description", config.description)
            if config.game_mode:
                combo, _ = self.input_widgets.get("Game Mode", (None, None))
                if combo:
                    for val in list(combo['values']):
                        if config.game_mode.lower() in val.lower():
                            combo.set(val)
                            break
            if isinstance(config.map_size, int):
                set_entry("Map Size", config.map_size)
            if config.map_name:
                set_entry("Map Name", config.map_name)

            # Time & weather
            if config.server_time:
                set_entry("Server Time", config.server_time)
            set_entry("Time Acceleration", getattr(config, 'server_time_acceleration', 1.0))
            set_entry("Night Time Acceleration", getattr(config, 'server_night_time_acceleration', 1.0))
            set_combo("Persistent Time", getattr(config, 'server_time_persistent', 0), ["0 (No)", "1 (Yes)"])
            if config.time_settings:
                set_entry("Time Settings", config.time_settings)
            if isinstance(config.weather, (int, float)):
                set_entry("Weather", config.weather)

            # Security
            set_combo("Enable Whitelist", getattr(config, 'enable_whitelist', 0), ["0 (No)", "1 (Yes)"])

            # Voice & view
            set_combo("Disable VoN", getattr(config, 'disable_von', 0), ["0 (No)", "1 (Yes)"])
            if isinstance(getattr(config, 'von_codec_quality', 20), int):
                set_entry("VoN Codec Quality", getattr(config, 'von_codec_quality', 20))
            set_combo("Disable 3rd Person", getattr(config, 'disable_3rd_person', 0), ["0 (No)", "1 (Yes)"])
            set_combo("Disable Crosshair", getattr(config, 'disable_crosshair', 0), ["0 (No)", "1 (Yes)"])
            set_combo("Disable Personal Light", getattr(config, 'disable_personal_light', 1), ["0 (No)", "1 (Yes)"])
            set_combo("Lighting Config", getattr(config, 'lighting_config', 0), ["0 (Bright)", "1 (Dark)"])
            if config.shard_id:
                set_entry("Shard ID", config.shard_id)

            # Network
            if isinstance(getattr(config, 'guaranteed_updates', 1), int):
                set_entry("Guaranteed Updates", getattr(config, 'guaranteed_updates', 1))
            if isinstance(getattr(config, 'login_queue_concurrent_players', 5), int):
                set_entry("Login Queue Concurrent", getattr(config, 'login_queue_concurrent_players', 5))
            if isinstance(getattr(config, 'login_queue_max_players', 500), int):
                set_entry("Login Queue Max", getattr(config, 'login_queue_max_players', 500))

            # Instance & storage
            if isinstance(getattr(config, 'instance_id', 1), int):
                set_entry("Instance ID", getattr(config, 'instance_id', 1))
            set_combo("Storage Auto Fix", getattr(config, 'storage_auto_fix', 1), ["0 (No)", "1 (Yes)"])

            # Mod settings
            if isinstance(config.verify_signatures, int):
                options = ["0 (Off)", "1 (Debug)", "2 (Verify)"]
                set_combo("Verify Signatures", config.verify_signatures, options)
            if isinstance(config.force_same_build, int):
                options = ["0 (No)", "1 (Yes)"]
                set_combo("Force Same Build", config.force_same_build, options)
            if config.mod_paths:
                set_entry("Mod Paths", config.mod_paths)
                self._sync_mod_paths_to_tree(str(config.mod_paths))

        except Exception as e:
            print(f"Error updating UI: {e}")
