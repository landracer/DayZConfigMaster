# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Invariant culture numeric parsing for DayZ Central Economy
# Ported from dayz-labs CeNum.cs

"""
CeNum class providing invariant culture numeric parsing functions.

This module provides the same functionality as dayz-labs CeNum:
- Int, Float, Bool parsing with invariant culture
- Fallback values when parsing fails
"""

import re
from typing import Optional


class CeNum:
    """
    Numeric parsing helper using invariant culture.
    
    Invariant culture ensures numbers like "1.5" are parsed correctly 
    regardless of the system's locale settings (which might expect "1,5").
    """
    
    @staticmethod
    def int(raw: Optional[str], fallback: int = 0) -> int:
        """
        Parse a string as an integer using invariant culture.
        
        Args:
            raw: The string to parse, or None
            fallback: Value to return if parsing fails
            
        Returns:
            Parsed integer, or fallback if parsing fails
        """
        if raw is None:
            return fallback
        
        try:
            # Strip whitespace and parse with invariant culture
            val = raw.strip()
            if not val:
                return fallback
            return int(val)
        except (ValueError, TypeError):
            return fallback
    
    @staticmethod
    def float(raw: Optional[str], fallback: float = 0.0) -> float:
        """
        Parse a string as a float using invariant culture.
        
        Args:
            raw: The string to parse, or None
            fallback: Value to return if parsing fails
            
        Returns:
            Parsed float, or fallback if parsing fails
        """
        if raw is None:
            return fallback
        
        try:
            val = raw.strip()
            if not val:
                return fallback
            # Use '.' as decimal separator (invariant culture)
            return float(val.replace(',', '.'))
        except (ValueError, TypeError):
            return fallback
    
    @staticmethod
    def bool(raw: Optional[str], fallback: bool = False) -> bool:
        """
        Parse a string as a boolean.
        
        Accepts: "true", "1", "yes" for True; "false", "0", "no" for False
        
        Args:
            raw: The string to parse, or None
            fallback: Value to return if parsing fails
            
        Returns:
            Parsed boolean, or fallback if parsing fails
        """
        if raw is None:
            return fallback
        
        val = raw.strip().lower()
        
        if val in ('true', '1', 'yes'):
            return True
        elif val in ('false', '0', 'no'):
            return False
        
        return fallback
    
    @staticmethod
    def int_range(raw: Optional[str], min_val: int, max_val: int, fallback: int = 0) -> int:
        """
        Parse a string as an integer within a range.
        
        Args:
            raw: The string to parse, or None
            min_val: Minimum allowed value (inclusive)
            max_val: Maximum allowed value (inclusive)
            fallback: Value to return if parsing fails or out of range
            
        Returns:
            Parsed integer clamped to range, or fallback if parsing fails
        """
        val = CeNum.int(raw, fallback)
        return max(min_val, min(max_val, val))
    
    @staticmethod
    def float_range(raw: Optional[str], min_val: float, max_val: float, fallback: float = 0.0) -> float:
        """
        Parse a string as a float within a range.
        
        Args:
            raw: The string to parse, or None
            min_val: Minimum allowed value (inclusive)
            max_val: Maximum allowed value (inclusive)
            fallback: Value to return if parsing fails or out of range
            
        Returns:
            Parsed float clamped to range, or fallback if parsing fails
        """
        val = CeNum.float(raw, fallback)
        return max(min_val, min(max_val, val))
    
    @staticmethod
    def percentage(raw: Optional[str], fallback: float = 0.0) -> float:
        """
        Parse a string as a percentage (0-1 range).
        
        Accepts both "0.5" and "50%" formats.
        
        Args:
            raw: The string to parse, or None
            fallback: Value to return if parsing fails
            
        Returns:
            Parsed percentage (0-1), or fallback if parsing fails
        """
        if raw is None:
            return fallback
        
        val = raw.strip().lower()
        
        # Handle percentage format
        if val.endswith('%'):
            num = CeNum.float(val[:-1].strip(), fallback)
            return max(0.0, min(1.0, num / 100.0))
        
        # Direct decimal
        return CeNum.float_range(raw, 0.0, 1.0, fallback)
    
    @staticmethod
    def parse_size(raw: Optional[str], fallback: int = 0) -> int:
        """
        Parse size strings with unit suffixes (e.g., "5KB", "2MB").
        
        Args:
            raw: The string to parse, or None
            fallback: Value to return if parsing fails
            
        Returns:
            Parsed size in bytes, or fallback if parsing fails
        """
        if raw is None:
            return fallback
        
        val = raw.strip()
        if not val:
            return fallback
        
        # Match number with optional unit
        match = re.match(r'^(\d+(?:\.\d+)?)\s*(KB|MB|GB)?$', val, re.IGNORECASE)
        
        if not match:
            try:
                return int(val)
            except ValueError:
                return fallback
        
        num_str, unit = match.groups()
        try:
            num = float(num_str)
        except ValueError:
            return fallback
        
        if unit:
            unit = unit.upper()
            if unit == 'KB':
                return int(num * 1024)
            elif unit == 'MB':
                return int(num * 1024 * 1024)
            elif unit == 'GB':
                return int(num * 1024 * 1024 * 1024)
        
        return int(num)