# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# File I/O utilities for DayZ configuration

"""
File I/O utilities providing common file operations.
"""

from pathlib import Path
from typing import Optional, List


def read_file(filepath: str) -> Optional[str]:
    """
    Read a text file and return its contents.
    
    Args:
        filepath: Path to the file
        
    Returns:
        File contents as string or None if error
    """
    path = Path(filepath)
    
    try:
        return path.read_text(encoding='utf-8')
    except Exception:
        return None


def write_file(filepath: str, content: str) -> bool:
    """
    Write content to a text file.
    
    Args:
        filepath: Path to the file
        content: Content to write
        
    Returns:
        True if successful, False otherwise
    """
    try:
        path = Path(filepath)
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(content, encoding='utf-8')
        return True
    except Exception:
        return False


def list_files(directory: str, pattern: Optional[str] = None) -> List[Path]:
    """
    List files in a directory.
    
    Args:
        directory: Directory to search
        pattern: Glob pattern (e.g., "*.xml") or None for all files
        
    Returns:
        List of Path objects
    """
    path = Path(directory)
    
    if not path.exists() or not path.is_dir():
        return []
    
    if pattern:
        return list(path.glob(pattern))
    else:
        return [p for p in path.iterdir() if p.is_file()]


def get_file_size(filepath: str) -> int:
    """
    Get file size in bytes.
    
    Args:
        filepath: Path to the file
        
    Returns:
        File size in bytes or 0 if error
    """
    try:
        return Path(filepath).stat().st_size
    except Exception:
        return 0


def ensure_directory(directory: str) -> bool:
    """
    Ensure a directory exists, creating it if necessary.
    
    Args:
        directory: Directory path
        
    Returns:
        True if successful, False otherwise
    """
    try:
        Path(directory).mkdir(parents=True, exist_ok=True)
        return True
    except Exception:
        return False