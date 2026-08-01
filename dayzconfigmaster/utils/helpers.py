# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Helper functions for DayZ configuration

"""
Helper functions providing utility operations for configuration management.
"""

from typing import Union


def format_size(size_bytes: int) -> str:
    """
    Format file size in human-readable format.
    
    Args:
        size_bytes: Size in bytes
        
    Returns:
        Human-readable string (e.g., "1.5 MB")
    """
    if size_bytes < 0:
        return "Invalid"
    
    for unit in ['B', 'KB', 'MB', 'GB']:
        if abs(size_bytes) < 1024.0:
            return f"{size_bytes:.2f} {unit}"
        size_bytes /= 1024.0
    
    return f"{size_bytes:.2f} TB"


def validate_port(port: Union[int, str], min_val: int = 1024, max_val: int = 65535) -> tuple:
    """
    Validate a port number.
    
    Args:
        port: Port number to validate
        min_val: Minimum valid port (default 1024)
        max_val: Maximum valid port (default 65535)
        
    Returns:
        Tuple of (is_valid, message)
    """
    try:
        port_num = int(port)
        
        if port_num < min_val or port_num > max_val:
            return False, f"Port must be between {min_val} and {max_val}"
        
        return True, None
        
    except (ValueError, TypeError):
        return False, "Port must be a valid number"


def calculate_multiplier_range(base: float = 1.0, min_mult: float = 0.5, max_mult: float = 2.0) -> tuple:
    """
    Calculate a reasonable range around a base multiplier.
    
    Args:
        base: Base multiplier value
        min_mult: Minimum multiplier (as fraction of base)
        max_mult: Maximum multiplier (as multiple of base)
        
    Returns:
        Tuple of (min_range, max_range)
    """
    return (base * min_mult, base * max_mult)


def format_seconds(seconds: int) -> str:
    """
    Format seconds in human-readable format.
    
    Args:
        seconds: Number of seconds
        
    Returns:
        Human-readable string (e.g., "1h 30m 45s")
    """
    if seconds < 0:
        return "Invalid"
    
    hours = seconds // 3600
    minutes = (seconds % 3600) // 60
    secs = seconds % 60
    
    parts = []
    if hours > 0:
        parts.append(f"{hours}h")
    if minutes > 0 or hours > 0:
        parts.append(f"{minutes}m")
    parts.append(f"{secs}s")
    
    return " ".join(parts)


def safe_float(value: str, default: float = 0.0) -> float:
    """
    Safely convert a string to float.
    
    Args:
        value: String to convert
        default: Default value if conversion fails
        
    Returns:
        Float value or default
    """
    try:
        return float(value)
    except (ValueError, TypeError):
        return default


def safe_int(value: str, default: int = 0) -> int:
    """
    Safely convert a string to integer.
    
    Args:
        value: String to convert
        default: Default value if conversion fails
        
    Returns:
        Integer value or default
    """
    try:
        return int(float(value))
    except (ValueError, TypeError):
        return default


def clamp(value: float, min_val: float, max_val: float) -> float:
    """
    Clamp a value between min and max.
    
    Args:
        value: Value to clamp
        min_val: Minimum allowed value
        max_val: Maximum allowed value
        
    Returns:
        Clamped value
    """
    return max(min_val, min(max_val, value))