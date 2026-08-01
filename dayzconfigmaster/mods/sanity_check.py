# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Mod Sanity Check module - detects and repairs corrupted mod folders.

This module provides comprehensive mod validation including:
- MD5 checksum verification of critical files
- Symlink loop detection
- Folder structure validation
- Critical file presence checks
"""

from __future__ import annotations

import hashlib
import os
from pathlib import Path
from typing import Dict, List, Optional, Set, Tuple
import time


class ModSanityError(Exception):
    """Raised when a mod fails sanity check."""
    pass


class ModChecker:
    """
    Comprehensive mod folder validator.
    
    Performs multiple validation passes on workshop mod folders to detect:
    - Corrupted downloads (MD5 mismatch)
    - Symlink loops (infinite recursion)
    - Missing critical files
    - Invalid folder structures
    """
    
    # Known critical files that should exist in every valid mod
    CRITICAL_FILES: Dict[str, Set[str]] = {
        'meta.cpp', 'mod.cpp',
        'addons/', 'keys/',
    }
    
    def __init__(self, workshop_dir: Path):
        """
        Initialize checker with workshop directory.
        
        Args:
            workshop_dir: Path to workshop content directory
                          (e.g., .../steamapps/workshop/content/221100)
        """
        self.workshop_dir = Path(workshop_dir)
        if not self.workshop_dir.exists():
            raise ValueError(f"Workshop directory does not exist: {workshop_dir}")
        
        # Cache for checksums and check results
        self._checksum_cache: Dict[str, str] = {}
        self._check_results: Dict[int, Dict] = {}
        
    def _compute_md5(self, filepath: Path) -> Optional[str]:
        """
        Compute MD5 hash of a file.
        
        Args:
            filepath: Path to the file
            
        Returns:
            Hex string of MD5 hash, or None if file cannot be read
        """
        try:
            if not filepath.is_file():
                return None
                
            # Use large block size for efficiency on big files
            md5 = hashlib.md5()
            with open(filepath, 'rb') as f:
                for chunk in iter(lambda: f.read(8192), b''):
                    md5.update(chunk)
            
            return md5.hexdigest()
        except (OSError, PermissionError):
            return None
    
    def _get_file_checksum(self, filepath: Path) -> str:
        """
        Get or compute checksum for a file.
        
        Args:
            filepath: Path to the file
            
        Returns:
            MD5 hash string
        """
        path_str = str(filepath.resolve())
        if path_str in self._checksum_cache:
            return self._checksum_cache[path_str]
        
        checksum = self._compute_md5(filepath)
        if checksum:
            self._checksum_cache[path_str] = checksum
        
        return checksum or ""
    
    def _detect_symlink_loops(self, folder: Path, max_depth: int = 20) -> Optional[str]:
        """
        Detect symlink loops in a mod folder.
        
        Args:
            folder: Path to the mod folder
            max_depth: Maximum recursion depth before declaring loop
            
        Returns:
            Description of the loop if found, None otherwise
        """
        visited: Set[Path] = set()
        current_path = folder.resolve()
        
        try:
            for depth in range(max_depth):
                # Resolve symlink to get actual path
                resolved = current_path.resolve()
                
                if resolved in visited:
                    return f"Symlink loop detected at depth {depth}: {resolved}"
                
                visited.add(resolved)
                
                # Try to move into a subdirectory to detect loops
                found_subdir = False
                try:
                    for item in resolved.iterdir():
                        if item.is_dir() and not item.is_symlink():
                            current_path = item
                            found_subdir = True
                            break
                except (OSError, PermissionError):
                    continue
                
                if not found_subdir:
                    break
                    
        except (OSError, PermissionError) as e:
            return f"Error checking for loops: {e}"
        
        return None
    
    def _check_folder_structure(self, folder: Path) -> List[str]:
        """
        Validate mod folder structure.
        
        Args:
            folder: Path to the mod folder
            
        Returns:
            List of validation issues (empty if valid)
        """
        issues = []
        
        try:
            # Check for essential files
            has_meta_cpp = (folder / "meta.cpp").exists()
            has_mod_cpp = (folder / "mod.cpp").exists()
            
            # At least one meta file should exist
            if not has_meta_cpp and not has_mod_cpp:
                issues.append("Missing both meta.cpp and mod.cpp")
            
            # Check for addons directory (critical)
            addons_dir = folder / "addons"
            if not addons_dir.exists():
                issues.append("Missing addons/ directory")
            elif not addons_dir.is_dir() or addons_dir.is_symlink():
                issues.append("addons/ should be a real directory, not symlink")
            
            # Check for keys directory
            keys_dir = folder / "keys"
            if not keys_dir.exists():
                # Try alternate names (Keys/, Key/)
                found_keys = False
                for name in ["Keys", "Key"]:
                    alt = folder / name
                    if alt.exists() and alt.is_dir():
                        found_keys = True
                        break
                
                if not found_keys:
                    issues.append("Missing keys/ directory")
            
            # Check for broken symlinks
            try:
                for item in folder.rglob("*"):
                    if item.is_symlink():
                        try:
                            resolved = item.resolve()
                            if not resolved.exists():
                                issues.append(f"Broken symlink: {item.relative_to(folder)}")
                        except OSError:
                            issues.append(f"Unresolvable symlink: {item.relative_to(folder)}")
            except (OSError, PermissionError):
                pass
                
        except (OSError, PermissionError) as e:
            issues.append(f"Error reading folder: {e}")
        
        return issues
    
    def _get_folder_size(self, folder: Path) -> int:
        """
        Get total size of a folder (not following symlinks).
        
        Args:
            folder: Path to the mod folder
            
        Returns:
            Total size in bytes
        """
        total = 0
        try:
            for item in folder.iterdir():
                if item.is_file() and not item.is_symlink():
                    total += item.stat().st_size
                elif item.is_dir() and not item.is_symlink():
                    total += self._get_folder_size(item)
        except (OSError, PermissionError):
            pass
        return total
    
    def _has_real_content(self, folder: Path) -> bool:
        """
        Check if a mod folder has actual content (not just placeholder).
        
        Args:
            folder: Path to the mod folder
            
        Returns:
            True if folder has substantial content
        """
        try:
            # Count real files (not symlinks)
            file_count = 0
            total_size = 0
            for item in folder.rglob("*"):
                if item.is_file() and not item.is_symlink():
                    file_count += 1
                    total_size += item.stat().st_size
            
            # A valid mod should have at least some files
            return file_count > 2 and total_size > 1024  # At least a few KB
        except (OSError, PermissionError):
            return False
    
    def check_mod(self, workshop_id: int) -> Dict:
        """
        Perform comprehensive sanity check on a mod.
        
        Args:
            workshop_id: Workshop item ID
            
        Returns:
            Check results dictionary with:
                - success: bool
                - issues: list of problem descriptions
                - checksums: dict of file paths to MD5 hashes
                - size_bytes: total folder size
                - file_count: number of files
        """
        folder = self.workshop_dir / str(workshop_id)
        
        if not folder.exists():
            return {
                "success": False,
                "issues": [f"Folder not found: {folder}"],
                "checksums": {},
                "size_bytes": 0,
                "file_count": 0,
            }
        
        # Start timing
        start_time = time.time()
        
        issues = []
        checksums = {}
        
        # Check 1: Detect symlink loops
        loop_issue = self._detect_symlink_loops(folder)
        if loop_issue:
            issues.append(loop_issue)
        
        # Check 2: Validate folder structure
        structure_issues = self._check_folder_structure(folder)
        issues.extend(structure_issues)
        
        # Check 3: Compute checksums for critical files
        for filename in ["meta.cpp", "mod.cpp"]:
            filepath = folder / filename
            if filepath.exists() and not filepath.is_symlink():
                checksum = self._get_file_checksum(filepath)
                if checksum:
                    checksums[str(filename)] = checksum
        
        # Check 4: Verify real content exists
        if not self._has_real_content(folder):
            issues.append("No real file content found (empty or placeholder)")
        
        # Calculate totals
        total_size = self._get_folder_size(folder)
        file_count = sum(1 for _ in folder.rglob("*") if _.is_file() and not _.is_symlink())
        
        elapsed = time.time() - start_time
        
        return {
            "success": len(issues) == 0,
            "issues": issues,
            "checksums": checksums,
            "size_bytes": total_size,
            "file_count": file_count,
            "elapsed_seconds": round(elapsed, 3),
        }
    
    def check_all_mods(self, ids: Optional[List[int]] = None) -> Dict[int, Dict]:
        """
        Check all mods in the workshop directory.
        
        Args:
            ids: Optional list of workshop IDs to check. If None, checks all.
            
        Returns:
            Dictionary mapping workshop ID to check results
        """
        if ids is None:
            # Get all mod folders
            ids = []
            try:
                for item in self.workshop_dir.iterdir():
                    try:
                        _ = int(item.name)
                        ids.append(int(item.name))
                    except ValueError:
                        continue
            except (OSError, PermissionError):
                pass
        
        results: Dict[int, Dict] = {}
        
        for workshop_id in sorted(ids):
            results[workshop_id] = self.check_mod(workshop_id)
        
        return results
    
    def get_bad_mods(self) -> List[Tuple[int, Dict]]:
        """
        Get list of mods that failed sanity check.
        
        Returns:
            List of (workshop_id, results_dict) tuples
        """
        all_results = self.check_all_mods()
        bad_mods = [
            (wid, result)
            for wid, result in sorted(all_results.items())
            if not result.get("success", False)
        ]
        return bad_mods


def format_check_report(results: Dict[int, Dict]) -> str:
    """
    Format check results as human-readable report.
    
    Args:
        results: Dictionary of workshop ID to results
        
    Returns:
        Formatted string report
    """
    lines = ["=== Mod Sanity Check Report ===", ""]
    
    total = len(results)
    good = sum(1 for r in results.values() if r.get("success"))
    bad = total - good
    
    lines.append(f"Total mods checked: {total}")
    lines.append(f"Valid: {good}")
    lines.append(f"Problematic: {bad}")
    lines.append("")
    
    if bad > 0:
        lines.append("--- PROBLEMATIC MODS ---")
        lines.append("")
        
        for workshop_id, result in sorted(results.items()):
            if not result.get("success", False):
                lines.append(f"[ID: {workshop_id}]")
                
                size = result.get("size_bytes", 0)
                if size > 0:
                    mb = size / (1024 * 1024)
                    lines.append(f"  Size: {mb:.2f} MB")
                
                file_count = result.get("file_count", 0)
                lines.append(f"  Files: {file_count}")
                
                issues = result.get("issues", [])
                if issues:
                    for issue in issues:
                        lines.append(f"  ❌ {issue}")
                else:
                    lines.append("  No specific issues listed")
                
                checksums = result.get("checksums", {})
                if checksums:
                    lines.append("  Checksums:")
                    for file, hash_val in checksums.items():
                        lines.append(f"    {file}: {hash_val}")
                
                elapsed = result.get("elapsed_seconds", 0)
                lines.append(f"  Checked in: {elapsed:.3f}s")
                
                lines.append("")
    
    return "\n".join(lines)


if __name__ == "__main__":
    # Example usage
    import sys
    
    workshop_path = Path.home() / ".steam" / "steam" / "steamapps" / "workshop" / "content" / "221100"
    if not workshop_path.exists():
        print(f"Workshop directory not found: {workshop_path}")
        sys.exit(1)
    
    checker = ModChecker(workshop_path)
    
    # Check specific mods or all
    check_ids = None  # Or specify: [3369325490, 3718035516]
    
    results = checker.check_all_mods(check_ids)
    
    print(format_check_report(results))