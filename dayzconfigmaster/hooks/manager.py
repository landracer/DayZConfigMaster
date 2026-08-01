# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Hook manager for DayZ server management.

Supports executing scripts at various points in the server lifecycle:
- beforeStart - run before server starts
- afterStart - run after server is ready
- missionChanged - run when mission files change
"""

import os
import subprocess
import threading
from datetime import datetime
from enum import Enum
from pathlib import Path
from typing import Callable, Dict, List, Optional


class HookType(Enum):
    """Types of hooks that can be executed."""
    BEFORE_START = 'beforeStart'       # Before server starts
    AFTER_START = 'afterStart'         # After server is ready
    MISSION_CHANGED = 'missionChanged' # When mission files change


class HookResult:
    """Result of a hook execution."""
    
    def __init__(self, success: bool, output: str = "", error: str = "",
                 return_code: int = 0):
        self.success = success
        self.output = output
        self.error = error
        self.return_code = return_code
        self.timestamp = datetime.now()
    
    def __str__(self) -> str:
        status = "SUCCESS" if self.success else f"FAILED (code: {self.return_code})"
        return f"{status} - {self.timestamp.strftime('%Y-%m-%d %H:%M:%S')}"


class Hook:
    """A hook definition."""
    
    def __init__(self, hook_type: HookType, program: str,
                 params: Optional[List[str]] = None):
        """
        Initialize a hook.
        
        Args:
            hook_type: When to execute the hook
            program: Path to script/executable to run
            params: Additional parameters for the script
        """
        self.type = hook_type
        self.program = program
        self.params = params or []
    
    def get_full_command(self) -> List[str]:
        """Get the full command to execute."""
        cmd = [self.program]
        cmd.extend(self.params)
        return cmd


class HookManager:
    """
    Manage and execute hooks for DayZ server lifecycle events.
    
    Hooks allow running custom scripts at specific points in the
    server lifecycle - useful for logging, notifications, or
    preparation tasks.
    """
    
    def __init__(self, projects_root: str):
        """
        Initialize the hook manager.
        
        Args:
            projects_root: Root directory containing project files
        """
        self.projects_root = Path(projects_root)
        self.hooks_dir = self.projects_root / "hooks"
        self.config_file = self.projects_root / "hooks.json"
        
        # Create hooks directory if needed
        self.hooks_dir.mkdir(parents=True, exist_ok=True)
        
        # Loaded hooks by type
        self._hooks: Dict[HookType, List[Hook]] = {
            HookType.BEFORE_START: [],
            HookType.AFTER_START: [],
            HookType.MISSION_CHANGED: []
        }
    
    def load_hooks(self, config_path: Optional[str] = None):
        """
        Load hooks from configuration file.
        
        Args:
            config_path: Path to JSON config file
        """
        self._hooks = {
            HookType.BEFORE_START: [],
            HookType.AFTER_START: [],
            HookType.MISSION_CHANGED: []
        }
        
        if config_path is None:
            config_path = str(self.config_file)
        
        try:
            import json
            
            if not Path(config_path).exists():
                return
            
            with open(config_path, 'r') as f:
                data = json.load(f)
            
            hooks_data = data.get('hooks', [])
            
            for hook_data in hooks_data:
                try:
                    hook_type = HookType(hook_data.get('type', 'beforeStart'))
                    program = hook_data.get('program', '')
                    
                    if not program:
                        continue
                    
                    params = hook_data.get('params', [])
                    if isinstance(params, str):
                        params = [params]
                    
                    hook = Hook(
                        hook_type=hook_type,
                        program=program,
                        params=params
                    )
                    
                    self._hooks[hook_type].append(hook)
                    
                except (ValueError, TypeError) as e:
                    print(f"Invalid hook configuration: {e}")
                    
        except Exception as e:
            print(f"Error loading hooks: {e}")
    
    def save_hooks(self, config_path: Optional[str] = None):
        """
        Save hooks to configuration file.
        
        Args:
            config_path: Path to JSON config file
        """
        if config_path is None:
            config_path = str(self.config_file)
        
        try:
            import json
            
            # Build hook list
            hooks_data = []
            
            for hook_type, hooks in self._hooks.items():
                for hook in hooks:
                    hooks_data.append({
                        'type': hook.type.value,
                        'program': hook.program,
                        'params': hook.params
                    })
            
            with open(config_path, 'w') as f:
                json.dump({'hooks': hooks_data}, f, indent=2)
                
        except Exception as e:
            print(f"Error saving hooks: {e}")
    
    def add_hook(self, hook_type: HookType, program: str,
                 params: Optional[List[str]] = None) -> bool:
        """
        Add a new hook.
        
        Args:
            hook_type: When to execute the hook
            program: Path to script/executable to run
            params: Additional parameters
            
        Returns:
            True if successfully added
        """
        hook = Hook(hook_type=hook_type, program=program, params=params)
        
        # Validate program exists or is a valid command
        prog_path = Path(program)
        if not prog_path.exists() and prog_path.is_absolute():
            print(f"Warning: Program path does not exist: {program}")
        
        self._hooks[hook_type].append(hook)
        return True
    
    def remove_hook(self, hook_type: HookType, index: int) -> bool:
        """
        Remove a hook by index.
        
        Args:
            hook_type: Type of hook
            index: Index in the hooks list
            
        Returns:
            True if removed successfully
        """
        try:
            del self._hooks[hook_type][index]
            return True
        except IndexError:
            return False
    
    def execute_hook(self, hook_type: HookType) -> List[HookResult]:
        """
        Execute all hooks of a specific type.
        
        Args:
            hook_type: Type of hooks to execute
            
        Returns:
            List of results for each executed hook
        """
        results = []
        
        for hook in self._hooks.get(hook_type, []):
            result = self._execute_single_hook(hook)
            results.append(result)
        
        return results
    
    def _execute_single_hook(self, hook: Hook) -> HookResult:
        """Execute a single hook and capture output."""
        cmd = hook.get_full_command()
        
        try:
            # Determine if we're on Windows or Unix
            is_windows = os.name == 'nt'
            
            process = subprocess.Popen(
                cmd,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                shell=is_windows
            )
            
            output, error = process.communicate()
            return_code = process.returncode
            
            result = HookResult(
                success=(return_code == 0),
                output=output,
                error=error,
                return_code=return_code
            )
            
            print(f"Hook executed: {hook.type.value} -> {result}")
            return result
            
        except FileNotFoundError:
            return HookResult(
                success=False,
                output="",
                error=f"Program not found: {hook.program}",
                return_code=-1
            )
        
        except Exception as e:
            return HookResult(
                success=False,
                output="",
                error=str(e),
                return_code=-1
            )
    
    def execute_before_start(self) -> List[HookResult]:
        """Execute all beforeStart hooks."""
        return self.execute_hook(HookType.BEFORE_START)
    
    def execute_after_start(self) -> List[HookResult]:
        """Execute all afterStart hooks."""
        return self.execute_hook(HookType.AFTER_START)
    
    def execute_mission_changed(self) -> List[HookResult]:
        """Execute all missionChanged hooks."""
        return self.execute_hook(HookType.MISSION_CHANGED)
    
    def get_hooks_by_type(self, hook_type: HookType) -> List[Hook]:
        """Get all hooks of a specific type."""
        return self._hooks.get(hook_type, [])
    
    def clear_hooks(self, hook_type: Optional[HookType] = None):
        """
        Clear hooks, optionally for a specific type.
        
        Args:
            hook_type: Type to clear (None clears all)
        """
        if hook_type is None:
            self._hooks = {
                HookType.BEFORE_START: [],
                HookType.AFTER_START: [],
                HookType.MISSION_CHANGED: []
            }
        else:
            self._hooks[hook_type] = []