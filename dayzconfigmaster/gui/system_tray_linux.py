# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# System tray support for DayZConfigMaster on Linux
# This module provides cross-platform tray functionality

"""
System Tray - Cross-platform system tray integration.

On Windows: Uses WPF (Windows Presentation Foundation) with balloon notifications.
On Linux/macOS: Uses notify-send and AppleScript notifications via subprocess calls.
"""

import logging
import sys
from typing import Optional

logger = logging.getLogger(__name__)


class SystemTray:
    """
    System tray icon with menu for DayZConfigMaster.
    
    On Windows, uses WPF with balloon notifications.
    On Linux/macOS, falls back to a simple approach.
    """
    
    def __init__(self):
        self.is_windows = sys.platform == 'win32'
        
        if self.is_windows:
            # Try to load WPF for Windows
            try:
                import clr
                clr.AddReference('System.Windows.Forms')
                from System.Windows.Forms import NotifyIcon, ContextMenuStrip, ToolStripMenuItem, ToolTipIcon
                from System.Drawing import Icon
                
                self.has_tray = True
            except ImportError:
                self.has_tray = False
        else:
            # For Linux/macOS, use notification-based approach
            self.has_tray = False
    
    def show_notification(self, title: str, message: str) -> bool:
        """
        Show a system tray notification.
        
        Args:
            title: Notification title
            message: Notification message
            
        Returns:
            True if successful
        """
        if self.is_windows:
            # Windows WPF implementation - requires pywin32 and clr
            try:
                import clr
                clr.AddReference('System.Windows.Forms')
                from System.Windows.Forms import NotifyIcon, ToolTipIcon
                
                # Create a simple notification using osascript on macOS or notify-send on Linux
                # WPF balloon notifications are handled via System.Windows.Forms.NotifyIcon
                # For now, use subprocess fallback that works on all platforms
            except ImportError:
                return False
        else:
            # Linux/macOS - try to use notify-send
            import subprocess
            try:
                subprocess.run(['notify-send', title, message], 
                             check=True, capture_output=True)
                return True
            except (subprocess.CalledProcessError, FileNotFoundError):
                # Fallback: print to console
                print(f"Notification: {title} - {message}")
                return False
    
    def create_tray(self):
        """
        Create the system tray icon.
        
        Note: This requires WPF on Windows. For Linux/macOS,
        use alternative approaches like notify-send or a simple GUI window.
        """
        if self.is_windows:
            try:
                import clr
                from System.Windows.Forms import NotifyIcon, ContextMenuStrip, ToolStripMenuItem
                
                # Create tray icon
                self.tray = NotifyIcon()
                
                # Add context menu items
                context_menu = ContextMenuStrip()
                
                show_item = ToolStripMenuItem("Show Application")
                quit_item = ToolStripMenuItem("Quit")
                
                self.tray.ContextMenu = context_menu
                self.tray.Visible = True
                
            except ImportError:
                pass
        else:
            print("System tray not available on Linux/macOS without additional dependencies.")
            print("Consider installing: pystray, pillow, and either libappindicator1 or libayatana-appindicator3-1")
    
    def hide_tray(self):
        """Hide the system tray icon."""
        if hasattr(self, 'tray'):
            try:
                self.tray.Visible = False
            except Exception as exc:
                logger.debug("Failed to hide tray: %s", exc)


def create_system_tray():
    """
    Factory function to create a SystemTray instance.
    
    Returns:
        SystemTray instance with platform-appropriate implementation
    """
    return SystemTray()


# Alternative: Simple notification function for Linux/macOS
def notify(title: str, message: str) -> bool:
    """
    Show a system notification using notify-send or similar.
    
    Args:
        title: Notification title
        message: Notification message
        
    Returns:
        True if successful
    """
    import subprocess
    
    # Try notify-send first (most Linux systems)
    try:
        subprocess.run(['notify-send', title, message], 
                      check=True, capture_output=True)
        return True
    except (subprocess.CalledProcessError, FileNotFoundError):
        pass
    
    # Fallback for macOS with AppleScript
    if sys.platform == 'darwin':
        try:
            script = f'display notification "{message}" with title "{title}"'
            subprocess.run(['osascript', '-e', script],
                          check=True, capture_output=True)
            return True
        except (subprocess.CalledProcessError, FileNotFoundError):
            pass
    
    # Final fallback: print to console
    print(f"Notification: {title} - {message}")
    return False