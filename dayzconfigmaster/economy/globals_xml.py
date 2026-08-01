# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# globals.xml parser/editor for DayZ Central Economy
# Ported from dayz-labs GlobalsXml.cs

"""
GlobalsXml class for parsing and editing globals.xml.

globals.xml contains global economy settings:
- Global multipliers
- Time-based rate adjustments
- Item-specific overrides
"""

from typing import Optional, List, Dict
from dataclasses import dataclass, field

from .ce_xml import XDoc, XElement, CeXml
from .ce_num import CeNum


@dataclass
class RateEntry:
    """A time-based rate entry."""
    start_hour: int = 0
    end_hour: int = 24
    multiplier: float = 1.0


@dataclass
class GlobalSetting:
    """A global economy setting."""
    name: str
    value: float = 1.0
    description: str = ""


@dataclass
class GlobalsEntry:
    """
    A single entry in globals.xml representing a global economy setting.
    
    Each entry has:
    - A name (setting identifier)
    - Multiplier value(s)
    - Rate entries for time-based adjustments
    """
    name: str
    multiplier: float = 1.0
    rates: List[RateEntry] = field(default_factory=list)


class GlobalsXml:
    """
    Parser and editor for globals.xml (DayZ Central Economy global settings file).
    
    Key features:
    - Parse and save globals.xml
    - Access individual setting entries by name
    - Add, modify, or remove entries
    - Manage time-based rate adjustments
    """
    
    def __init__(self):
        self._doc: Optional[XDoc] = None
        self._globals: Dict[str, GlobalsEntry] = {}  # name -> GlobalsEntry
    
    @property
    def doc(self) -> XDoc:
        """Get or create the underlying document."""
        if self._doc is None:
            self._doc = XDoc.create("economy")
        return self._doc
    
    @staticmethod
    def parse(xml_str: str) -> 'GlobalsXml':
        """
        Parse a globals.xml string.
        
        Args:
            xml_str: The XML content to parse
            
        Returns:
            New GlobalsXml instance with parsed data
        """
        obj = GlobalsXml()
        doc = CeXml.parse_doc(xml_str)
        obj._doc = doc
        
        if doc and doc.root:
            obj._parse_globals(doc.root)
        
        return obj
    
    @staticmethod
    def from_file(filepath: str) -> Optional['GlobalsXml']:
        """
        Load a globals.xml file.
        
        Args:
            filepath: Path to the XML file
            
        Returns:
            New GlobalsXml instance, or None if loading fails
        """
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read()
            return GlobalsXml.parse(content)
        except (IOError, Exception):
            return None
    
    def _parse_globals(self, root: XElement) -> None:
        """Parse all global settings from the XML."""
        self._globals.clear()
        
        # Look for globals, global, multiplier elements
        for elem in root.children:
            if elem.tag == "globals":
                # Nested content - look for child entries
                for child in elem.children:
                    entry = self._parse_global_entry(child)
                    if entry:
                        self._globals[entry.name.lower()] = entry
            elif elem.tag == "global" or elem.tag == "multiplier":
                # Direct entry with attributes on the element itself
                entry = self._parse_global_entry(elem)
                if entry:
                    self._globals[entry.name.lower()] = entry
    
    def _parse_global_entry(self, elem: XElement) -> Optional[GlobalsEntry]:
        """Parse a single global setting element."""
        name = elem.attr("name")
        if not name:
            return None
        
        # Get base multiplier
        mult_str = elem.attr("multiplier") or elem.attr("value")
        multiplier = CeNum.float(mult_str, 1.0)
        
        # Parse rate entries for time-based adjustments
        rates: List[RateEntry] = []
        for rate_elem in elem.find_all("rate"):
            start_hour = rate_elem.attr_int("start", 0)
            end_hour = rate_elem.attr_int("end", 24)
            mult_val = CeNum.float(rate_elem.attr("multiplier"), 1.0)
            
            rates.append(RateEntry(
                start_hour=start_hour,
                end_hour=end_hour,
                multiplier=mult_val
            ))
        
        return GlobalsEntry(
            name=name,
            multiplier=multiplier,
            rates=rates
        )
    
    def get_global(self, name: str) -> Optional[GlobalsEntry]:
        """
        Get a global setting by name.
        
        Args:
            name: The setting name (case-insensitive)
            
        Returns:
            GlobalsEntry if found, None otherwise
        """
        return self._globals.get(name.lower())
    
    def set_global(self, entry: GlobalsEntry) -> 'GlobalsXml':
        """
        Add or update a global setting.
        
        Args:
            entry: The GlobalsEntry to add/update
            
        Returns:
            Self for chaining
        """
        self._globals[entry.name.lower()] = entry
        
        # Update the XML document
        root = self.doc.root
        if not root:
            return self
        
        # Find or create the global element
        old_elem = None
        for elem in root.children:
            if elem.attr("name", "").lower() == entry.name.lower():
                old_elem = elem
                break
        
        # Remove old entry if exists
        if old_elem:
            root.remove_child(old_elem)
        
        # Add new element
        global_elem = XElement("global", name=entry.name)
        global_elem.set_attr("multiplier", str(entry.multiplier))
        root.add_child(global_elem)
        
        for rate in entry.rates:
            rate_elem = XElement("rate")
            rate_elem.set_attr("start", str(rate.start_hour))
            rate_elem.set_attr("end", str(rate.end_hour))
            rate_elem.set_attr("multiplier", str(rate.multiplier))
            global_elem.add_child(rate_elem)
        
        return self
    
    def remove_global(self, name: str) -> bool:
        """
        Remove a global setting by name.
        
        Args:
            name: The setting name to remove
            
        Returns:
            True if found and removed
        """
        if name.lower() not in self._globals:
            return False
        
        del self._globals[name.lower()]
        
        # Update the XML document
        root = self.doc.root
        if not root:
            return True
        
        for elem in root.children:
            if elem.attr("name", "").lower() == name.lower():
                root.remove_child(elem)
                break
        
        return True
    
    def get_all_globals(self) -> Dict[str, GlobalsEntry]:
        """Get all global settings."""
        return dict(self._globals)
    
    def get_global_names(self) -> List[str]:
        """Get all setting names."""
        return list(self._globals.keys())
    
    def to_xml(self) -> str:
        """Convert back to XML string."""
        return CeXml.serialize(self.doc)
    
    def save(self, filepath: str) -> bool:
        """
        Save the globals.xml file.
        
        Args:
            filepath: Path to save to
            
        Returns:
            True if successful
        """
        try:
            xml_str = self.to_xml()
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(xml_str)
            return True
        except IOError:
            return False