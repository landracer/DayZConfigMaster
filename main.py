

# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

#!/usr/bin/env python3
"""
DayzConfigMaster - Main Application Entry Point (Refactored)
Uses DayzConfigMasterApp class for GUI with setup wizard integration
"""

import sys
from pathlib import Path

# Add current directory to path for imports
sys.path.insert(0, str(Path(__file__).parent))

# Hard safety ceiling: this process must never be allowed to consume enough
# RAM to hard-lock the host. Applied before any heavy imports.
try:
    from dayzconfigmaster.utils.memory_guard import setup_memory_safety
    setup_memory_safety()
except Exception as exc:
    print(f"Warning: could not start memory safety guard: {exc}", file=sys.stderr)

try:
    from dayzconfigmaster.setup.wizard import SetupWizard
    from dayzconfigmaster.gui.app import DayzConfigMasterApp
    import tkinter as tk
except ImportError as e:
    print(f"Error importing modules: {e}")
    sys.exit(1)


def check_and_run_setup() -> bool:
    """
    Ensure configuration exists, applying OS-based defaults automatically.

    This never prompts on the command line: on first run it writes sensible
    default locations for the current OS so the GUI opens immediately. Users can
    change any location later from the Preferences dialog.

    Returns:
        Always True (startup is never blocked by setup).
    """
    projects_root = Path.home() / "Documents" / "DayZProjects"
    wizard = SetupWizard(str(projects_root))

    if not wizard.is_setup_complete():
        print("First run: applying default locations for this OS...")
        wizard.run_automatic()
        print("Defaults applied. You can change locations in Edit > Preferences.")

    return True


def main():
    """Main application entry point"""
    # Check and run setup if needed
    if not check_and_run_setup():
        print("Setup failed. Exiting.")
        sys.exit(1)
    
    print("\nStarting DayzConfigMaster...")
    
    root = tk.Tk()
    
    app = DayzConfigMasterApp(root)
    
    print("DayzConfigMaster running...")
    root.mainloop()


if __name__ == "__main__":
    main()
