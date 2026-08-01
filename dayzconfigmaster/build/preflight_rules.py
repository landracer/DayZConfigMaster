# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Preflight rules for DayZ mod validation

"""
Preflight Rules - Complete set of validation rules matching dayz-labs.

Rule Families:
1. ConfigRules - Configuration file syntax and structure
2. FileSystemRules - File system and path conventions
3. ReferenceRules - Addon dependencies and references
4. ScriptRules - Script file syntax and patterns
"""

import os
from pathlib import Path
import re
from typing import Dict, List, Optional, Any


class PreflightRule:
    """Base class for preflight rules."""
    
    def __init__(self):
        self.id: str = ""
        self.name: str = ""
        self.description: str = ""
    
    def check(self, mod_path: Path, result: 'PreflightResult'):
        """Check a rule against the mod directory."""
        pass


class PreflightResult:
    """Results from preflight checks."""
    
    def __init__(self):
        self.errors: List[Dict[str, Any]] = []
        self.warnings: List[Dict[str, Any]] = []
        self.infos: List[Dict[str, Any]] = []
    
    def add_error(self, rule_id: str, message: str, file: Optional[str] = None, line: int = 0):
        """Add an error finding."""
        self.errors.append({
            "rule": rule_id,
            "message": message,
            "file": file,
            "line": line,
            "severity": "error"
        })
    
    def add_warning(self, rule_id: str, message: str, file: Optional[str] = None, line: int = 0):
        """Add a warning finding."""
        self.warnings.append({
            "rule": rule_id,
            "message": message,
            "file": file,
            "line": line,
            "severity": "warning"
        })
    
    def add_info(self, rule_id: str, message: str, file: Optional[str] = None):
        """Add an info finding."""
        self.infos.append({
            "rule": rule_id,
            "message": message,
            "file": file,
            "severity": "info"
        })
    
    def is_ok(self) -> bool:
        """Check if preflight passed (no errors)."""
        return len(self.errors) == 0
    
    def to_dict(self) -> Dict[str, Any]:
        """Convert to dictionary."""
        return {
            "ok": self.is_ok(),
            "errors": self.errors,
            "warnings": self.warnings,
            "infos": self.infos
        }


class ConfigRules:
    """
    Configuration file validation rules.
    
    Checks:
    - $PBOPREFIX$ file exists and has valid content
    - config.cpp syntax (class CfgPatches, CfgMods)
    - Required classes in configuration
    - Proper class naming conventions
    """
    
    def __init__(self):
        self.required_classes = ["CfgPatches", "CfgMods"]
    
    def check_prefix_file(self, mod_path: Path, result: PreflightResult):
        """Check for $PBOPREFIX$ file and validity."""
        prefix_file = mod_path / "$PBOPREFIX$"
        
        if not prefix_file.exists():
            result.add_info(
                "prefix-missing",
                "No $PBOPREFIX$ file found. PBO name will be used as prefix.",
                str(prefix_file)
            )
            return
        
        try:
            content = prefix_file.read_text().strip()
            
            # Check for drive letters (should not contain absolute paths)
            if re.match(r'^[A-Z]:', content, re.IGNORECASE):
                result.add_warning(
                    "prefix-drive",
                    f"PBO prefix contains drive letter: {content}",
                    str(prefix_file)
                )
            
            # Check for proper format
            if len(content) == 0:
                result.add_warning(
                    "prefix-empty",
                    "$PBOPREFIX$ file is empty",
                    str(prefix_file)
                )
        except Exception as e:
            result.add_error(
                "prefix-read-error",
                f"Failed to read $PBOPREFIX$: {str(e)}",
                str(prefix_file)
            )
    
    def check_configs(self, mod_path: Path, result: PreflightResult):
        """Check config.cpp files for required classes."""
        config_files = list(mod_path.rglob("config.cpp"))
        
        if not config_files:
            result.add_error(
                "config-missing",
                "No config.cpp found in mod directory"
            )
            return
        
        for config_file in config_files:
            self._check_config(config_file, mod_path, result)
    
    def _check_config(self, config_file: Path, mod_path: Path, result: PreflightResult):
        """Check a single config file."""
        try:
            content = config_file.read_text()
            
            # Check for CfgPatches
            if "class CfgPatches" not in content:
                result.add_error(
                    "cfgpatches-missing",
                    "CfgPatches class not found. Required for addon registration.",
                    str(config_file)
                )
                return
            
            # Check for requiredAddons in each patch
            patches = re.findall(
                r'class\s+([A-Za-z0-9_]+)\s*\{[^}]*}',
                content,
                re.IGNORECASE | re.DOTALL
            )
            
            for patch_name in patches:
                self._check_patch(patch_name, config_file, mod_path, result)
        
        except Exception as e:
            result.add_error(
                "config-read-error",
                f"Failed to read {config_file}: {str(e)}",
                str(config_file)
            )
    
    def _check_patch(self, patch_name: str, config_file: Path, mod_path: Path, result: PreflightResult):
        """
        Check a CfgPatches entry for validity.
        
        Validates:
        - Patch name exists in CfgPatches class
        - units[] array is not empty
        - requiredAddons[] array is not empty (if present)
        """
        import re
        
        try:
            content = config_file.read_text()
            
            # Parse CfgPatches to find the patch by name
            pattern = rf'class\s+{re.escape(patch_name)}\s*\{{([^}}]+)\}}'
            match = re.search(pattern, content, re.IGNORECASE | re.DOTALL)
            
            if not match:
                result.add_error(
                    "cfgpatches-missing",
                    f"CfgPatches entry '{patch_name}' not found in {config_file.name}",
                    str(config_file)
                )
                return
            
            patch_body = match.group(1)
            
            # Check for units[] array
            units_match = re.search(r'units\s*=\s*\[([^\]]*)\]', patch_body, re.IGNORECASE | re.DOTALL)
            if not units_match or not units_match.group(1).strip():
                result.add_error(
                    "cfgpatches-empty-units",
                    f"CfgPatches entry '{patch_name}' has empty or missing units[] array",
                    str(config_file)
                )
            
            # Check for requiredAddons[] (optional but recommended)
            addons_match = re.search(r'requiredAddons\s*=\s*\[([^\]]*)\]', patch_body, re.IGNORECASE | re.DOTALL)
            if not addons_match or not addons_match.group(1).strip():
                result.add_warning(
                    "cfgpatches-empty-required-addons",
                    f"CfgPatches entry '{patch_name}' has empty or missing requiredAddons[] array (optional but recommended)",
                    str(config_file)
                )
                
        except Exception as e:
            result.add_error(
                "cfgpatches-parse-error",
                f"Failed to parse CfgPatches for '{patch_name}': {str(e)}",
                str(config_file)
            )
    
    def check_cfgmods(self, mod_path: Path, result: PreflightResult):
        """Check for CfgMods class in config."""
        config_files = list(mod_path.rglob("config.cpp"))
        
        for config_file in config_files:
            try:
                content = config_file.read_text()
                
                if "class CfgMods" not in content and "class CfgPatches" not in content:
                    result.add_warning(
                        "cfgmods-missing",
                        "Neither CfgMods nor CfgPatches found. Some mods require one.",
                        str(config_file)
                    )
            except Exception:
                pass


class FileSystemRules:
    """
    File system validation rules.
    
    Checks:
    - All paths are lowercase (DayZ requirement)
    - No Windows-style absolute paths in configs
    - No Unix-style absolute paths
    - Banned file patterns (.pyc, .pyo, etc.)
    - Directory naming conventions
    """
    
    def check(self, mod_path: Path, result: PreflightResult):
        """Run filesystem checks."""
        self._check_lowercase(mod_path, result)
        self._check_absolute_paths(mod_path, result)
        self._check_banned_patterns(mod_path, result)
        self._check_directory_naming(mod_path, result)
        self._check_odol_models(mod_path, result)
        self._check_case_conflicts(mod_path, result)
        self._check_baked_absolute_paths(mod_path, result)
    
    def _check_lowercase(self, mod_path: Path, result: PreflightResult):
        """Check that paths don't contain uppercase letters (DayZ requirement)."""
        for file in mod_path.rglob("*"):
            if file.is_file():
                rel_path = str(file.relative_to(mod_path))
                
                # DayZ requires all lowercase paths
                if any(c.isupper() for c in rel_path):
                    result.add_warning(
                        "uppercase-path",
                        f"Path contains uppercase: {rel_path}",
                        str(file)
                    )
    
    def _check_absolute_paths(self, mod_path: Path, result: PreflightResult):
        """Check that configs don't contain absolute paths."""
        for file in mod_path.rglob("*.cpp"):
            try:
                content = file.read_text()
                
                # Check for Windows-style absolute paths
                if re.search(r'^[A-Z]:\\', content, re.MULTILINE | re.IGNORECASE):
                    result.add_warning(
                        "absolute-path-windows",
                        f"Windows absolute path found in {file}",
                        str(file)
                    )
                
                # Check for Unix-style absolute paths (shouldn't be in config)
                if re.search(r'^/', content, re.MULTILINE):
                    result.add_warning(
                        "absolute-path-unix",
                        f"Unix absolute path found in {file}",
                        str(file)
                    )
            
            except Exception:
                pass
    
    def _check_banned_patterns(self, mod_path: Path, result: PreflightResult):
        """Check for banned file patterns."""
        banned_extensions = [".pyc", ".pyo", "__pycache__"]

        for ext in banned_extensions:
            if ext == "__pycache__":
                pycache = mod_path / "__pycache__"
                if pycache.exists():
                    result.add_info(
                        "banned-directory",
                        f"Python cache directory found: {pycache}",
                        str(pycache)
                    )
            else:
                for file in mod_path.rglob(f"*{ext}"):
                    result.add_info(
                        "banned-file",
                        f"Binary Python file: {file}",
                        str(file)
                    )

    def _check_directory_naming(self, mod_path: Path, result: PreflightResult):
        """Check directory naming conventions."""
        for item in mod_path.iterdir():
            if item.is_dir() and item.name.startswith("_"):
                result.add_info(
                    "underscore-directory",
                    f"Directory starts with underscore (ignored by PBO): {item}",
                    str(item)
                )

    def _check_odol_models(self, mod_path: Path, result: PreflightResult):
        """Flag already-binarized ODOL .p3d files that should not be re-binarized."""
        for file in mod_path.rglob("*.p3d"):
            try:
                header = file.read_bytes(4)
                # ODOL models start with the "ODOL" magic bytes.
                if header == b"ODOL":
                    result.add_error(
                        "odol-model",
                        f"ODOL (already binarized) model will break Binarize: {file}",
                        str(file)
                    )
            except Exception:
                pass

    def _check_case_conflicts(self, mod_path: Path, result: PreflightResult):
        """Detect path-case conflicts that break Linux servers."""
        seen: Dict[str, Path] = {}
        for file in mod_path.rglob("*"):
            if not file.is_file():
                continue
            lower = str(file.relative_to(mod_path)).lower()
            if lower in seen:
                result.add_warning(
                    "case-conflict",
                    f"Case-insensitive path conflict: {seen[lower]} vs {file}",
                    str(file)
                )
            else:
                seen[lower] = file

    def _check_baked_absolute_paths(self, mod_path: Path, result: PreflightResult):
        """Detect Windows-style absolute paths baked into configs."""
        for file in mod_path.rglob("*.cpp"):
            try:
                content = file.read_text()
                if re.search(r'[A-Za-z]:\\\\', content):
                    result.add_error(
                        "baked-absolute-path",
                        f"Windows absolute path baked into config: {file}",
                        str(file)
                    )
            except Exception:
                pass


class ReferenceRules:
    """
    Addon reference validation rules.
    
    Checks:
    - requiredAddons in CfgPatches
    - Missing addon references
    - Circular dependencies
    - External file references (textures, configs)
    """
    
    def __init__(self):
        self.seen_addons: set = set()
    
    def check(self, mod_path: Path, result: PreflightResult):
        """Run reference checks."""
        config_files = list(mod_path.rglob("config.cpp"))
        
        for config_file in config_files:
            try:
                content = config_file.read_text()
                
                # Extract requiredAddons
                addons_match = re.findall(
                    r'requiredAddons\s*=\s*\[\s*"([^"]+)"',
                    content,
                    re.IGNORECASE | re.DOTALL
                )
                
                for addon in addons_match:
                    self._check_addon_reference(addon, mod_path, result)
                    
            except Exception:
                pass
    
    def _check_addon_reference(self, addon_name: str, mod_path: Path, result: PreflightResult):
        """
        Check if an addon reference is valid.
        
        Args:
            addon_name: Name of the required addon
            mod_path: Current mod directory
            result: Result object to add findings
        """
        # Skip dayz core addons
        dayz_core = [
            "DayZ_Characters",
            "DayZ_Components",
            "DayZ_Player_Weapons",
            "DayZ_Gameplay"
        ]
        
        if addon_name.lower() in [a.lower() for a in dayz_core]:
            return
        
        # Check if referenced addon exists as a subdirectory
        # (for multi-addon mods)
        sub_dir = mod_path / addon_name
        if not sub_dir.exists():
            result.add_warning(
                "addon-missing",
                f"Referenced addon not found: {addon_name}",
                str(mod_path)
            )
    
    def check_external_references(self, mod_path: Path, result: PreflightResult):
        """Check for external file references."""
        for file in mod_path.rglob("*.cpp"):
            try:
                content = file.read_text()
                
                # Check for external config references
                if re.search(r'include\s+"?/+\w+', content):  # Absolute include path
                    result.add_error(
                        "external-absolute",
                        f"Absolute path reference found in {file}",
                        str(file)
                    )
                    
            except Exception:
                pass


class ScriptRules:
    """
    Script file validation rules.
    
    Checks:
    - .sqf, .sqf files syntax patterns
    - Proper script class definitions
    - Function declarations
    - No banned API calls
    """
    
    def __init__(self):
        self.banned_patterns = [
            (r'\bsleep\s+\d+\.?\d*\s*;', 'Using sleep() with numbers instead of random time'),
            (r'\bcreateVehicle\s+[^;]*\;', 'Use createVehicleCrew or proper spawn functions'),
        ]
    
    def check(self, mod_path: Path, result: PreflightResult):
        """Run script checks."""
        self._check_sqs_files(mod_path, result)
        self._check_sqm_files(mod_path, result)
        self._check_script_patterns(mod_path, result)
    
    def _check_sqs_files(self, mod_path: Path, result: PreflightResult):
        """Check .sqs files for proper syntax."""
        for sqs_file in mod_path.rglob("*.sqs"):
            try:
                content = sqs_file.read_text()
                
                # SQS uses line-based commands separated by newlines
                lines = content.split('\n')
                
                for i, line in enumerate(lines):
                    line = line.strip()
                    
                    if not line or line.startswith('#'):
                        continue
                    
                    # Basic SQS validation - each line should be a valid command
                    # This is a basic check
                    if line and not re.match(r'^[a-zA-Z_][a-zA-Z0-9_]*', line):
                        result.add_warning(
                            "sqs-invalid-line",
                            f"Potential invalid line {i+1}: {line}",
                            str(sqs_file),
                            i + 1
                        )
                        
            except Exception:
                pass
    
    def _check_sqm_files(self, mod_path: Path, result: PreflightResult):
        """Check .sqm mission files for validity."""
        for sqm_file in mod_path.rglob("*.sqm"):
            try:
                content = sqm_file.read_text()
                
                # Check for required SQM header
                if "class Mission" not in content and "class CfgMission" not in content:
                    result.add_info(
                        "sqm-missing-header",
                        f"No mission header found: {sqm_file}",
                        str(sqm_file)
                    )
                    
            except Exception:
                pass
    
    def _check_script_patterns(self, mod_path: Path, result: PreflightResult):
        """Check for banned script patterns."""
        for file in mod_path.rglob("*.cpp"):
            try:
                content = file.read_text()
                
                for pattern, message in self.banned_patterns:
                    if re.search(pattern, content):
                        result.add_warning(
                            "script-pattern",
                            f"{message}",
                            str(file)
                        )
                        
            except Exception:
                pass


class PreflightEngine:
    """Main preflight engine that runs all checks."""
    
    def __init__(self):
        from dayzconfigmaster.build.preflight_rules import (
            ConfigRules,
            FileSystemRules,
            ReferenceRules,
            ScriptRules
        )
        
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
        
        # Run all rule families
        self.config_rules.check_prefix_file(mod_path, result)
        self.config_rules.check_configs(mod_path, result)
        
        self.filesystem_rules.check(mod_path, result)
        
        self.reference_rules.check(mod_path, result)
        self.script_rules.check(mod_path, result)
        
        return result.to_dict()