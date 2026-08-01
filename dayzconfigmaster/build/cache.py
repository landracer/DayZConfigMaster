# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Build cache with content-hash for skipping unchanged mods

"""
Build Cache - Content-hash based caching to skip unchanged mods during builds.

Features:
- Calculate file content hashes (SHA256)
- Store hash metadata in cache.json
- Skip rebuild if content hasn't changed
- Automatic cache invalidation on source changes
"""

import hashlib
import json
from pathlib import Path
from typing import Dict, Any, List, Optional, Set


class BuildCache:
    """
    Content-hash based build cache for PBO builds.
    
    Cache file format (build_cache.json):
    {
        "cache_version": 1,
        "last_updated": 1700000000.0,
        "entries": {
            "mod_name.pbo": {
                "content_hash": "...",
                "source_files": [...],
                "file_hashes": { ... },
                "timestamp": 1700000000.0
            }
        }
    }
    """
    
    CACHE_VERSION = 1
    
    def __init__(self, cache_dir: str):
        """
        Initialize build cache.
        
        Args:
            cache_dir: Directory to store cache files
        """
        self.cache_dir = Path(cache_dir)
        self.cache_file = self.cache_dir / "build_cache.json"
        self.entries: Dict[str, Any] = {}
        self._load_cache()
    
    def _load_cache(self):
        """Load existing cache from disk."""
        if not self.cache_file.exists():
            self.entries = {
                "cache_version": self.CACHE_VERSION,
                "last_updated": 0.0,
                "entries": {}
            }
            return
        
        try:
            with open(self.cache_file, 'r') as f:
                data = json.load(f)
            
            # Validate cache version
            if data.get("cache_version") != self.CACHE_VERSION:
                print(f"Cache version mismatch: expected {self.CACHE_VERSION}, got {data.get('cache_version')}")
                self.entries = {
                    "cache_version": self.CACHE_VERSION,
                    "last_updated": 0.0,
                    "entries": {}
                }
                return
            
            self.entries = data
        except (json.JSONDecodeError, IOError) as e:
            print(f"Warning: Could not load build cache: {e}")
            self.entries = {
                "cache_version": self.CACHE_VERSION,
                "last_updated": 0.0,
                "entries": {}
            }
    
    def _save_cache(self):
        """Save cache to disk."""
        try:
            if not self.cache_dir.exists():
                self.cache_dir.mkdir(parents=True, exist_ok=True)
            
            with open(self.cache_file, 'w') as f:
                json.dump(self.entries, f, indent=2)
        except IOError as e:
            print(f"Error saving build cache: {e}")
    
    def _calculate_file_hash(self, file_path: Path) -> str:
        """
        Calculate SHA256 hash of a file.
        
        Args:
            file_path: Path to file
            
        Returns:
            Hexadecimal hash string
        """
        sha256 = hashlib.sha256()
        
        try:
            with open(file_path, 'rb') as f:
                for chunk in iter(lambda: f.read(8192), b''):
                    sha256.update(chunk)
            
            return sha256.hexdigest()
        except (IOError, OSError) as e:
            print(f"Warning: Could not hash file {file_path}: {e}")
            return ""
    
    def _calculate_dir_hash(self, dir_path: Path) -> Dict[str, str]:
        """
        Calculate hashes for all files in a directory.
        
        Args:
            dir_path: Source directory path
            
        Returns:
            Dictionary mapping relative paths to their hashes
        """
        file_hashes = {}
        
        if not dir_path.exists():
            return file_hashes
        
        # Get all relevant source files (excluding .git, build artifacts)
        skip_patterns = ['.git', '__pycache__', '.pyc', '.pyo']
        
        for file_path in dir_path.rglob("*"):
            if file_path.is_file():
                rel_path = str(file_path.relative_to(dir_path))
                
                # Skip patterns
                should_skip = False
                for pattern in skip_patterns:
                    if pattern in rel_path:
                        should_skip = True
                        break
                
                if should_skip:
                    continue
                
                hash_value = self._calculate_file_hash(file_path)
                if hash_value:
                    file_hashes[rel_path] = hash_value
        
        return file_hashes
    
    def _get_source_hash(self, source_dir: Path) -> str:
        """
        Get combined hash of all source files.
        
        Args:
            source_dir: Source directory
            
        Returns:
            Combined SHA256 hash
        """
        file_hashes = self._calculate_dir_hash(source_dir)
        
        # Create a single hash from all file hashes
        combined = json.dumps(file_hashes, sort_keys=True)
        return hashlib.sha256(combined.encode()).hexdigest()
    
    def get_source_files(self, source_dir: Path) -> List[str]:
        """
        Get list of all source files in directory.
        
        Args:
            source_dir: Source directory
            
        Returns:
            List of relative file paths
        """
        files = []
        skip_patterns = ['.git', '__pycache__', '.pyc', '.pyo']
        
        for file_path in source_dir.rglob("*"):
            if file_path.is_file():
                rel_path = str(file_path.relative_to(source_dir))
                
                should_skip = False
                for pattern in skip_patterns:
                    if pattern in rel_path:
                        should_skip = True
                        break
                
                if not should_skip:
                    files.append(rel_path)
        
        return sorted(files)
    
    def is_up_to_date(self, mod_name: str, source_dir: Path) -> bool:
        """
        Check if a mod's build is up to date.
        
        Args:
            mod_name: Name of the mod
            source_dir: Source directory
            
        Returns:
            True if cache hit (no changes)
        """
        source_hash = self._get_source_hash(source_dir)
        
        entry = self.entries.get("entries", {}).get(mod_name)
        
        if not entry:
            return False
        
        # Check if hash matches
        if entry.get("content_hash") != source_hash:
            return False
        
        # Check if any source file is newer than cache timestamp
        for rel_path, cached_hash in entry.get("file_hashes", {}).items():
            file_path = source_dir / rel_path
            
            if not file_path.exists():
                return False
            
            current_hash = self._calculate_file_hash(file_path)
            
            if current_hash != cached_hash:
                return False
        
        return True
    
    def get_cache_info(self, mod_name: str) -> Optional[Dict[str, Any]]:
        """
        Get cache info for a mod.
        
        Args:
            mod_name: Name of the mod
            
        Returns:
            Cache entry or None
        """
        return self.entries.get("entries", {}).get(mod_name)
    
    def invalidate(self, mod_name: str) -> bool:
        """
        Invalidate cache entry for a mod.
        
        Args:
            mod_name: Name of the mod
            
        Returns:
            True if invalidated
        """
        entries = self.entries.get("entries", {})
        
        if mod_name in entries:
            del entries[mod_name]
            self._save_cache()
            return True
        
        return False
    
    def invalidate_all(self) -> int:
        """
        Invalidate all cache entries.
        
        Returns:
            Number of entries invalidated
        """
        entries = self.entries.get("entries", {})
        count = len(entries)
        entries.clear()
        self._save_cache()
        return count
    
    def update(
        self,
        mod_name: str,
        source_dir: Path,
        output_pbo: Path,
        prefix: str = "",
        timestamp: float = None
    ) -> bool:
        """
        Update cache entry after successful build.
        
        Args:
            mod_name: Name of the mod
            source_dir: Source directory
            output_pbo: Output PBO path
            prefix: PBO prefix
            timestamp: Build timestamp (default: now)
            
        Returns:
            True if update successful
        """
        source_hash = self._get_source_hash(source_dir)
        file_hashes = self._calculate_dir_hash(source_dir)
        
        entries = self.entries.get("entries", {})
        
        entries[mod_name] = {
            "content_hash": source_hash,
            "source_files": list(file_hashes.keys()),
            "file_hashes": file_hashes,
            "output_pbo": str(output_pbo),
            "prefix": prefix,
            "timestamp": timestamp or __import__('time').time()
        }
        
        self.entries["last_updated"] = __import__('time').time()
        self._save_cache()
        
        return True
    
    def get_all_entries(self) -> Dict[str, Any]:
        """Get all cache entries."""
        return self.entries.get("entries", {})
    
    def clear(self):
        """Clear all cache entries."""
        self.entries["entries"] = {}
        self.entries["last_updated"] = 0.0
        self._save_cache()
    
    def get_size(self) -> int:
        """
        Get number of cached entries.
        
        Returns:
            Number of cache entries
        """
        return len(self.entries.get("entries", {}))