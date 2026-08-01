# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Preflight engine for mod validation

"""
Preflight engine for validating mod projects before building.

Checks:
- Config file syntax (CfgPatches, CfgMods)
- Required addons dependencies
- Missing asset references
- Path hygiene (lowercase, no absolute paths)

Rule families matching dayz-labs:
1. ConfigRules - Configuration file syntax and structure
2. FileSystemRules - File system and path conventions  
3. ReferenceRules - Addon dependencies and references
4. ScriptRules - Script file syntax and patterns
"""

import os
from pathlib import Path
import re
from typing import Dict, List, Optional, Any

# Import from the separate rules module
from dayzconfigmaster.build.preflight_rules import (
    PreflightResult,
    ConfigRules,
    FileSystemRules,
    ReferenceRules,
    ScriptRules
)


class PreflightEngine:
    """Main preflight engine that runs all checks."""
    
    def __init__(self):
        self.config_rules = ConfigRules()
        self.filesystem_rules = FileSystemRules()
        self.reference_rules = ReferenceRules()
        self.script_rules = ScriptRules()
    
    def run(self, mod_dir: str) -> Dict[str, Any]:
        """
        Run all preflight checks on a mod directory.
        
        Args:
            mod_dir: Path to mod directory
        
        Returns:
            Preflight report dict
        """
        result = PreflightResult()
        
        mod_path = Path(mod_dir)
        
        if not mod_path.exists():
            result.add_error("mod-missing", f"Mod directory does not exist: {mod_dir}")
            return result.to_dict()
        
        # Run all rule families (matching dayz-labs)
        self._run_config_rules(mod_path, result)
        self._run_filesystem_rules(mod_path, result)
        self._run_reference_rules(mod_path, result)
        self._run_script_rules(mod_path, result)
        
        return result.to_dict()
    
    def _run_config_rules(self, mod_path: Path, result: PreflightResult):
        """Run configuration rules."""
        self.config_rules.check_prefix_file(mod_path, result)
        self.config_rules.check_configs(mod_path, result)
        self.config_rules.check_cfgmods(mod_path, result)
    
    def _run_filesystem_rules(self, mod_path: Path, result: PreflightResult):
        """Run filesystem rules."""
        self.filesystem_rules.check(mod_path, result)
    
    def _run_reference_rules(self, mod_path: Path, result: PreflightResult):
        """Run reference rules."""
        self.reference_rules.check(mod_path, result)
        self.reference_rules.check_external_references(mod_path, result)
    
    def _run_script_rules(self, mod_path: Path, result: PreflightResult):
        """Run script rules."""
        self.script_rules.check(mod_path, result)


def run_preflight(mod_dir: str) -> Dict[str, Any]:
    """
    Convenience function to run preflight checks.
    
    Args:
        mod_dir: Path to mod directory
        
    Returns:
        Preflight report dict
    """
    engine = PreflightEngine()
    return engine.run(mod_dir)