# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# System tray icon and hidden window support for DayZConfigMaster

"""
System Tray Support - Cross-platform system tray icon with hidden window.

Features:
- Windows: Uses ctypes + win32gui for full system tray integration
- Linux: Uses AppIndicator3 for Unity/GNOME system tray
- macOS: Uses notify2 for notification center support
"""

import logging
import os
import sys
import threading
import time
from pathlib import Path

logger = logging.getLogger(__name__)

try:
    import tkinter as tk
    from tkinter import ttk, messagebox, Menu
except ImportError:
    tk = None


class SystemTrayIcon:
    """
    Cross-platform system tray icon for DayZConfigMaster.
    
    Provides:
    - Hidden window support (GUI runs without visible main window)
    - System tray icon with context menu
    - Quick actions (launch GUI, restart server, check status)
    """
    
    def __init__(self, app_name: str = "DayZConfigMaster"):
        self.app_name = app_name
        self.running = False
        self.thread = None
        self.hidden_root = None
        self.menu_items = []
        
        # Platform-specific setup
        self.is_windows = os.name == 'nt'
        self.is_linux = not self.is_windows and sys.platform.startswith('linux')
        self.is_mac = sys.platform.startswith('darwin')
        
        # Platform-specific tray implementations
        self._tray_impl = None
    
    def start(self, on_exit=None, on_show=None, menu_actions: dict = None):
        """
        Start the system tray icon in a separate thread.
        
        Args:
            on_exit: Callback when exit is selected
            on_show: Callback to show the main window
            menu_actions: Dict of action_name -> callback
        """
        if not tk:
            print("Tkinter not available - system tray disabled")
            return
        
        self.running = True
        self.on_exit_callback = on_exit
        self.on_show_callback = on_show
        
        # Default menu actions
        default_actions = {
            "Show GUI": lambda: print("Show GUI pressed"),
            "Exit": lambda: print("Exit pressed")
        }
        
        if menu_actions:
            default_actions.update(menu_actions)
        
        self.menu_actions = default_actions
        
        # Create hidden root window (no visible window)
        self.hidden_root = tk.Tk()
        self.hidden_root.withdraw()  # Hide the window
        self.hidden_root.title(self.app_name)
        
        # Platform-specific system tray setup
        if self.is_windows:
            self._setup_windows_tray()
        elif self.is_linux:
            self._setup_linux_tray()
        else:
            # macOS - use basic tkinter approach with notify2
            self._setup_mac_tray()
    
    def _setup_windows_tray(self):
        """Setup Windows system tray using ctypes + win32gui."""
        try:
            import win32gui
            import win32con
            import win32api
            
            # Register window class
            wc = win32gui.WNDCLASS()
            wc.hInstance = win32api.GetModuleHandle(None)
            wc.lpszClassName = "DayZConfigMasterTray"
            wc.lpfnWndProc = {win32con.WM_DESTROY: self._on_destroy}
            
            try:
                class_atom = win32gui.RegisterClass(wc)
            except Exception:
                # Class already registered
                class_atom = 0
            
            # Create hidden window
            self.tray_hwnd = win32gui.CreateWindow(
                "DayZConfigMasterTray",
                "DayZConfigMaster Tray",
                0,
                0, 0, 0, 0,
                None, None, wc.hInstance, None
            )
            
            # Add tray icon
            self._add_tray_icon()
            
        except ImportError:
            print("pywin32 not available - using notify2 fallback")
    
    def _setup_linux_tray(self):
        """Setup Linux system tray with AppIndicator3."""
        try:
            import gi
            gi.require_version('AppIndicator3', '0.1')
            from gi.repository import AppIndicator3, Gtk
            
            # Create indicator
            self.indicator = AppIndicator3.Indicator.new(
                "dayzconfigmaster",
                "application-x-python",
                AppIndicator3.IndicatorCategory.APPLICATION_STATUS
            )
            
            menu = self._create_menu()
            self.indicator.set_menu(menu)
            self.indicator.set_status(AppIndicator3.IndicatorStatus.ACTIVE)
            
            # Run GTK main loop in a thread
            def gtk_main():
                Gtk.main()
            
            self._gtk_thread = threading.Thread(target=gtk_main, daemon=True)
            self._gtk_thread.start()
            
        except ImportError:
            # No app indicator - try notify2 for notifications
            print("AppIndicator not available - trying notify2")
    
    def _setup_mac_tray(self):
        """Setup macOS system tray with notify2."""
        import threading
        
        try:
            import notify2
            
            if not notify2.init(self.app_name):
                raise Exception("Failed to initialize notify2")
            
            # Store notify2 for later use
            self._notify = notify2
            
            print("macOS: Notification center integrated via notify2")
            
        except ImportError:
            print("macOS: No tray support available (install notify2)")
    
    def _create_menu(self) -> tk.Menu:
        """Create context menu for the tray icon."""
        menu = Menu(self.hidden_root, tearoff=0)
        
        if self.on_show_callback:
            menu.add_command(label="Show GUI", command=self.on_show_callback)
        
        # Add server status if available
        try:
            from ..server.instance_manager import InstanceManager
            manager = InstanceManager(".")
            instances = manager.list_instances()
            
            if instances:
                server_menu = Menu(menu, tearoff=0)
                for inst in instances:
                    server_menu.add_command(
                        label=f"Status: {inst.name}",
                        command=lambda i=inst: self._show_server_status(i)
                    )
                menu.add_cascade(label="Server Status", menu=server_menu)
        except Exception:
            pass
        
        menu.add_separator()
        
        for name, callback in self.menu_actions.items():
            if name not in ["Show GUI", "Exit"]:
                menu.add_command(label=name, command=callback)
        
        menu.add_separator()
        menu.add_command(label="Exit", command=self._exit_app)
        
        return menu
    
    def _add_tray_icon(self):
        """Add icon to system tray."""
        try:
            import win32gui
            import win32con
            
            # Load an icon (or use default)
            icon_path = self._get_icon_path()
            
            if os.path.exists(icon_path):
                hicon = win32gui.LoadImage(
                    0, icon_path,
                    win32con.IMAGE_ICON,
                    16, 16,
                    win32con.LR_LOADFROMFILE
                )
            else:
                hicon = 0
            
            # Add to tray
            self.tray_nid = win32gui.NIM_ADD
            self.tray_data = (
                self.tray_hwnd,
                0,  # ID
                win32con.NIF_ICON | win32con.NIF_MESSAGE | win32con.NIF_TIP,
                win32con.WM_USER + 100,
                hicon,
                "DayZConfigMaster"
            )
            
            win32gui.Shell_NotifyIcon(win32con.NIM_ADD, self.tray_data)
            
        except ImportError:
            pass
    
    def _show_notification(self, title: str, message: str):
        """Show system notification."""
        try:
            if self.is_windows and hasattr(self, 'tray_hwnd'):
                import win32gui
                import win32con
                
                # Flash the window to show it's running
                win32gui.FlashWindow(self.tray_hwnd, True)
            
            elif self.is_linux and hasattr(self, '_notify'):
                try:
                    import notify2
                    n = notify2.Notification(title, message, "dialog-information")
                    n.show()
                except ImportError:
                    pass
            
        except Exception as e:
            print(f"Notification error: {e}")
    
    def _show_server_status(self, instance):
        """Show server status for an instance."""
        try:
            from ..server.instance_manager import InstanceManager
            manager = InstanceManager(".")
            status = manager.get_instance(instance.name)
            
            if status:
                msg = f"Instance: {status.name}\nStatus: {'Running' if status.enabled else 'Stopped'}\nPort: {status.port}"
                self.show_notification("Server Status", msg)
        except Exception as e:
            print(f"Status error: {e}")
    
    def _get_icon_path(self) -> str:
        """Get path to application icon."""
        # Try various common locations
        possible_paths = [
            Path(__file__).parent.parent / "icons" / "app.ico",
            Path(__file__).parent.parent / "resources" / "icon.png",
            os.environ.get("DZL_ICON_PATH"),
        ]
        
        for path in possible_paths:
            if path and os.path.exists(path):
                return str(path)
        
        # Return empty string as fallback
        return ""
    
    def _on_destroy(self, hwnd, msg, wparam, lparam):
        """Handle window destroy."""
        self.running = False
    
    def _exit_app(self):
        """Exit the application."""
        if hasattr(self, 'hidden_root') and self.hidden_root:
            try:
                self.hidden_root.quit()
            except Exception as exc:
                logger.debug("Failed to quit hidden root: %s", exc)
        
        # Clean up tray icon
        if hasattr(self, '_cleanup_tray'):
            self._cleanup_tray()
        
        if self.on_exit_callback:
            self.on_exit_callback()
        
        self.running = False
        sys.exit(0)
    
    def _cleanup_tray(self):
        """Clean up system tray resources."""
        try:
            import win32gui
            import win32con
            
            if hasattr(self, 'tray_hwnd') and hasattr(self, 'tray_nid'):
                # Remove icon from tray
                win32gui.Shell_NotifyIcon(win32con.NIM_DELETE, (self.tray_hwnd, 0))
        except ImportError:
            pass
    
    def stop(self):
        """Stop the system tray icon."""
        self.running = False
        
        if hasattr(self, 'hidden_root') and self.hidden_root:
            try:
                self.hidden_root.quit()
            except Exception as exc:
                logger.debug("Failed to quit hidden root during stop: %s", exc)
        
        # Clean up tray resources
        if hasattr(self, '_cleanup_tray'):
            self._cleanup_tray()
    
    def update_icon_state(self, active: bool):
        """Update tray icon to show active/inactive state."""
        if not self.is_windows or not hasattr(self, 'tray_hwnd'):
            return
        
        try:
            import win32gui
            import win32con
            
            # Update notification text based on state
            status = "Running" if active else "Stopped"
            new_data = list(self.tray_data)
            new_data[-1] = f"DayZConfigMaster [{status}]"
            self.tray_data = tuple(new_data)
            
            win32gui.Shell_NotifyIcon(win32con.NIM_MODIFY, self.tray_data)
        except ImportError:
            pass
    
    def show_notification(self, title: str, message: str, priority: int = 0):
        """
        Show a notification.
        
        Args:
            title: Notification title
            message: Notification message
            priority: 0=normal, 1=high, 2=critical
        """
        # Use platform-specific notification system
        if self.is_windows and hasattr(self, 'tray_hwnd'):
            import win32gui
            import win32con
            win32gui.FlashWindow(getattr(self, 'tray_hwnd', 0), True)
        elif self.is_linux:
            try:
                import notify2
                n = notify2.Notification(title, message, "dialog-information")
                n.show()
            except ImportError:
                pass


def create_hidden_window(app_name: str = "DayZConfigMaster") -> tk.Tk:
    """
    Create a hidden tkinter window for background operations.
    
    Args:
        app_name: Window title
    
    Returns:
        Hidden Tk root window
    """
    if not tk:
        raise RuntimeError("Tkinter not available")
    
    root = tk.Tk()
    root.withdraw()  # Hide the window
    root.title(app_name)
    
    return root


def run_with_system_tray(
    app_class,
    app_name: str = "DayZConfigMaster",
    menu_actions: dict = None
):
    """
    Run an application with system tray support.
    
    Args:
        app_class: Application class to instantiate when shown
        app_name: Application name
        menu_actions: Dict of menu actions -> callbacks
    
    Returns:
        None (blocks until exit)
    """
    if not tk:
        raise RuntimeError("Tkinter required for GUI")
    
    tray = SystemTrayIcon(app_name)
    
    # Show function
    def show_app():
        root = tk.Tk()
        root.deiconify()  # Show the window
        app_class(root)
        root.mainloop()
    
    def exit_app():
        if hasattr(tray, 'hidden_root') and tray.hidden_root:
            try:
                tray.hidden_root.quit()
            except Exception as exc:
                logger.debug("Failed to quit hidden root during exit: %s", exc)
        sys.exit(0)
    
    # Start tray in background thread
    def tray_thread():
        tray.start(
            on_exit=exit_app,
            on_show=show_app,
            menu_actions=menu_actions
        )
    
    t = threading.Thread(target=tray_thread, daemon=True)
    t.start()
    
    # Keep main thread alive
    while tray.running:
        time.sleep(0.1)


if __name__ == "__main__":
    # Test the system tray
    import sys
    
    def test_exit():
        print("Exit callback called")
    
    run_with_system_tray(
        app_class=None,
        menu_actions={
            "Test": lambda: print("Test action"),
            "Status": lambda: print("Status check"),
        }
    )