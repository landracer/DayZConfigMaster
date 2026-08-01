# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Centralized dialog helper for DayZConfigMaster GUI.

Every messagebox call in the application goes through these functions so that:
1. The message is always printed to stdout/stderr (visible in CLI/logs).
2. A native dialog is shown *only* when a live Tk root exists.
3. Callers never need to sprinkle ``if self.root else print(...)`` everywhere.

Usage – replace direct imports with::

    from ..gui.dialog_helper import messagebox

Then call ``messagebox.showerror(...)`` exactly as before — the wrapper
handles the rest.
"""

from __future__ import annotations

import sys
import tkinter as tk
from typing import Any, Callable, Optional
from tkinter import messagebox as _tk_messagebox

# ── global Tk root reference (set once by DayzConfigMasterApp.__init__) ──
_root_ref: Optional[tk.Tk] = None


def set_tk_root(root: Optional[tk.Tk]) -> None:
    """Call this ONCE during app startup so dialogs know whether a GUI exists."""
    global _root_ref
    _root_ref = root


def _gui_available() -> bool:
    """Return True if a live Tk root is registered and still alive."""
    if _root_ref is None:
        return False
    try:
        return _root_ref.winfo_exists()
    except tk.TclError:
        return False


# ── Pretty-print helpers ───────────────────────────────────────────────────

_PREFIX = "[DayzConfigMaster]"


def _print_msg(level: str, title: str, message: str) -> None:
    """Always print the dialog text to stdout (stderr for errors)."""
    header = f"{_PREFIX} {level}: [{title}]"
    print(header, file=sys.stdout, flush=True)
    if message:
        # Collapse multiple newlines for compact CLI output.
        print("  " + message.strip().replace("\n", " | "), file=sys.stdout, flush=True)


def _print_error(title: str, message: str) -> None:
    """Print error messages to stderr (plus stdout for capture)."""
    header = f"{_PREFIX} ERROR: [{title}]"
    body = message.strip().replace("\n", " | ") if message else ""
    print(header, file=sys.stderr, flush=True)
    if body:
        print(f"  {body}", file=sys.stderr, flush=True)
    # Also print to stdout so a single pipe captures everything.
    print(header, file=sys.stdout, flush=True)
    if body:
        print(f"  {body}", file=sys.stdout, flush=True)


# ── Wrapper functions (drop-in replacements for messagebox *) ─────────────

def showerror(title: str = "Error", message: str = "", icon=None, parent=None) -> None:
    """Show an error dialog (CLI + GUI)."""
    _print_error(title, message)
    if _gui_available():
        try:
            _tk_messagebox.showerror(title=title, message=message, icon=icon, parent=parent)
        except tk.TclError:
            pass


def showwarning(title: str = "Warning", message: str = "", icon=None, parent=None) -> None:
    """Show a warning dialog (CLI + GUI)."""
    header = f"{_PREFIX} WARNING: [{title}]"
    body = message.strip().replace("\n", " | ") if message else ""
    print(header, file=sys.stdout, flush=True)
    if body:
        print(f"  {body}", file=sys.stdout, flush=True)
    if _gui_available():
        try:
            _tk_messagebox.showwarning(title=title, message=message, icon=icon, parent=parent)
        except tk.TclError:
            pass


def showinfo(title: str = "Info", message: str = "", icon=None, parent=None) -> None:
    """Show an info dialog (CLI + GUI)."""
    header = f"{_PREFIX} INFO: [{title}]"
    body = message.strip().replace("\n", " | ") if message else ""
    print(header, file=sys.stdout, flush=True)
    if body:
        print(f"  {body}", file=sys.stdout, flush=True)
    if _gui_available():
        try:
            _tk_messagebox.showinfo(title=title, message=message, icon=icon, parent=parent)
        except tk.TclError:
            pass


def askyesno(title: str = "Confirm", message: str = "", icon=None, parent=None) -> bool:
    """Ask yes/no — returns True/False (defaults to True when no GUI)."""
    header = f"{_PREFIX} QUESTION: [{title}]"
    body = message.strip().replace("\n", " | ") if message else ""
    print(header, file=sys.stdout, flush=True)
    if body:
        print(f"  {body}", file=sys.stdout, flush=True)
    if _gui_available():
        try:
            return _tk_messagebox.askyesno(title=title, message=message, icon=icon, parent=parent)
        except tk.TclError:
            pass
    return True  # safe default when running headless


def askokcancel(title: str = "Confirm", message: str = "", icon=None, parent=None) -> bool:
    """Ask ok/cancel — returns True/False (defaults to True when no GUI)."""
    header = f"{_PREFIX} QUESTION: [{title}]"
    body = message.strip().replace("\n", " | ") if message else ""
    print(header, file=sys.stdout, flush=True)
    if body:
        print(f"  {body}", file=sys.stdout, flush=True)
    if _gui_available():
        try:
            return _tk_messagebox.askokcancel(title=title, message=message, icon=icon, parent=parent)
        except tk.TclError:
            pass
    return True


def askretrycancel(title: str = "Retry", message: str = "", icon=None, parent=None) -> bool:
    """Ask retry/cancel — returns True/False (defaults to False when no GUI)."""
    header = f"{_PREFIX} QUESTION: [{title}]"
    body = message.strip().replace("\n", " | ") if message else ""
    print(header, file=sys.stdout, flush=True)
    if body:
        print(f"  {body}", file=sys.stdout, flush=True)
    if _gui_available():
        try:
            return _tk_messagebox.askretrycancel(title=title, message=message, icon=icon, parent=parent)
        except tk.TclError:
            pass
    return False


def askquestion(title: str = "Question", message: str = "", icon=None, parent=None) -> str:
    """Ask a question ('yes'/'no') — defaults to 'yes' when no GUI."""
    header = f"{_PREFIX} QUESTION: [{title}]"
    body = message.strip().replace("\n", " | ") if message else ""
    print(header, file=sys.stdout, flush=True)
    if body:
        print(f"  {body}", file=sys.stdout, flush=True)
    if _gui_available():
        try:
            return _tk_messagebox.askquestion(title=title, message=message, icon=icon, parent=parent)
        except tk.TclError:
            pass
    return "yes"


def askcancel(title: str = "Cancel", message: str = "", icon=None, parent=None) -> bool:
    """Ask cancel/ok — returns True (cancelled) or False."""
    header = f"{_PREFIX} QUESTION: [{title}]"
    body = message.strip().replace("\n", " | ") if message else ""
    print(header, file=sys.stdout, flush=True)
    if body:
        print(f"  {body}", file=sys.stdout, flush=True)
    if _gui_available():
        try:
            return _tk_messagebox.askokcancel(title=title, message=message, icon=icon or tk.ERROR, parent=parent)
        except tk.TclError:
            pass
    return False


# ── Namespace object so callers can use ``messagebox.showerror(...)``, etc. ─

def _generic_fallback(
    title: str = "",
    message: str = "",
    icon: str = "error",
    parent=None,
) -> None:
    """Dispatches to the right function based on *icon*."""
    dispatch = {
        "error": showerror,
        "warning": showwarning,
        "info": showinfo,
        "question": lambda t=title, m=message: askquestion(title=t, message=m),
        "askyesno": lambda t=title, m=message: askyesno(title=t, message=m),
        "askokcancel": lambda t=title, m=message: askokcancel(title=t, message=m),
        "askretrycancel": lambda t=title, m=message: askretrycancel(
            title=t, message=m
        ),
        "askcancel": lambda t=title, m=message: askcancel(title=t, message=m),
    }
    fn = dispatch.get(icon, showerror)
    fn(title, message)


class _MessageBoxNamespace:
    """A lightweight namespace that exposes every dialog function as an attribute.

    This lets code written like::

        from gui.dialog_helper import messagebox
        messagebox.showerror("Error", "something broke")

    work unchanged — the old ``from tkinter import messagebox`` style is
    preserved without needing to touch thousands of call sites.
    """

    showerror = staticmethod(showerror)
    showwarning = staticmethod(showwarning)
    showinfo = staticmethod(showinfo)
    askyesno = staticmethod(askyesno)
    askokcancel = staticmethod(askokcancel)
    askretrycancel = staticmethod(askretrycancel)
    askquestion = staticmethod(askquestion)
    askcancel = staticmethod(askcancel)

    def __call__(
        self,
        title: str = "",
        message: str = "",
        icon: str = "error",
        parent=None,
    ) -> None:
        """Generic fallback — dispatches to the right function based on *icon*."""
        _generic_fallback(title=title, message=message, icon=icon, parent=parent)

    # Aliases that some callers use
    warning = staticmethod(showwarning)
    error = staticmethod(showerror)
    info = staticmethod(showinfo)


# Module-level name so ``from dialog_helper import messagebox`` gives a namespace.
messagebox = _MessageBoxNamespace()


__all__ = [
    "showerror",
    "showwarning",
    "showinfo",
    "askyesno",
    "askokcancel",
    "askretrycancel",
    "askquestion",
    "askcancel",
    "messagebox",
    "set_tk_root",
    "_gui_available",
]