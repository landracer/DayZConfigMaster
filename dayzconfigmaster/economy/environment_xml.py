# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# environment.xml parser/editor for DayZ Central Economy
# Ported from dayz-labs EnvironmentXml.cs

"""
EnvironmentXml class for parsing and editing environment/weather settings.

environment.xml contains weather and environmental settings:
- Global multipliers (damage, loot, zombie spawn rates)
- Weather parameters
- Time-based rate adjustments
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
class EnvironmentSetting:
    """An environment/weather setting."""
    name: str
    value: float = 1.0
    description: str = ""


@dataclass
class EnvironmentEntry:
    """
    A single entry in environment.xml representing an environmental setting.
    
    Each entry has:
    - A name (setting identifier)
    - Multiplier value(s)
    - Rate entries for time-based adjustments
    """
    name: str
    multiplier: float = 1.0
    rates: List[RateEntry] = field(default_factory=list)


class EnvironmentXml:
    """
    Parser and editor for environment.xml (DayZ Central Economy environmental settings file).
    
    Key features:
    - Parse and save environment.xml
    - Access individual setting entries by name
    - Add, modify, or remove entries
    - Manage time-based rate adjustments
    
    This corresponds to dayz-labs EnvironmentXml for weather/environment settings.
    """
    
    def __init__(self):
        self._doc: Optional[XDoc] = None
        self._settings: Dict[str, EnvironmentEntry] = {}  # name -> EnvironmentEntry
    
    @property
    def doc(self) -> XDoc:
        """Get or create the underlying document."""
        if self._doc is None:
            self._doc = XDoc.create("environment")
        return self._doc
    
    @staticmethod
    def parse(xml_str: str) -> 'EnvironmentXml':
        """
        Parse an environment.xml string.
        
        Args:
            xml_str: The XML content to parse
            
        Returns:
            New EnvironmentXml instance with parsed data
        """
        obj = EnvironmentXml()
        doc = CeXml.parse_doc(xml_str)
        obj._doc = doc
        
        if doc and doc.root:
            obj._parse_settings(doc.root)
        
        return obj
    
    @staticmethod
    def from_file(filepath: str) -> Optional['EnvironmentXml']:
        """
        Load an environment.xml file.
        
        Args:
            filepath: Path to the XML file
            
        Returns:
            New EnvironmentXml instance, or None if loading fails
        """
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read()
            return EnvironmentXml.parse(content)
        except (IOError, Exception):
            return None
    
    def _parse_settings(self, root: XElement) -> None:
        """Parse all environment settings from the XML."""
        self._settings.clear()
        
        # Look for environment elements
        for elem in root.children:
            if elem.tag == "environment":
                # Nested content
                for child in elem.children:
                    entry = self._parse_setting_entry(child)
                    if entry:
                        self._settings[entry.name.lower()] = entry
            else:
                entry = self._parse_setting_entry(elem)
                if entry:
                    self._settings[entry.name.lower()] = entry
    
    def _parse_setting_entry(self, elem: XElement) -> Optional[EnvironmentEntry]:
        """Parse a single environment setting element."""
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
        
        return EnvironmentEntry(
            name=name,
            multiplier=multiplier,
            rates=rates
        )
    
    def get_setting(self, name: str) -> Optional[EnvironmentEntry]:
        """
        Get an environment setting by name.
        
        Args:
            name: The setting name (case-insensitive)
            
        Returns:
            EnvironmentEntry if found, None otherwise
        """
        return self._settings.get(name.lower())
    
    def set_setting(self, entry: EnvironmentEntry) -> 'EnvironmentXml':
        """
        Add or update an environment setting.
        
        Args:
            entry: The EnvironmentEntry to add/update
            
        Returns:
            Self for chaining
        """
        self._settings[entry.name.lower()] = entry
        
        # Update the XML document
        root = self.doc.root
        if not root:
            return self
        
        # Find or create the setting element
        old_elem = None
        for elem in root.children:
            if elem.attr("name", "").lower() == entry.name.lower():
                old_elem = elem
                break
        
        # Remove old entry if exists
        if old_elem:
            root.remove_child(old_elem)
        
        # Add new element
        setting_elem = XElement("setting", name=entry.name)
        setting_elem.set_attr("multiplier", str(entry.multiplier))
        root.add_child(setting_elem)
        
        for rate in entry.rates:
            rate_elem = XElement("rate")
            rate_elem.set_attr("start", str(rate.start_hour))
            rate_elem.set_attr("end", str(rate.end_hour))
            rate_elem.set_attr("multiplier", str(rate.multiplier))
            setting_elem.add_child(rate_elem)
        
        return self
    
    def remove_setting(self, name: str) -> bool:
        """
        Remove an environment setting by name.
        
        Args:
            name: The setting name to remove
            
        Returns:
            True if found and removed
        """
        if name.lower() not in self._settings:
            return False
        
        del self._settings[name.lower()]
        
        # Update the XML document
        root = self.doc.root
        if not root:
            return True
        
        for elem in root.children:
            if elem.attr("name", "").lower() == name.lower():
                root.remove_child(elem)
                break
        
        return True
    
    def get_all_settings(self) -> Dict[str, EnvironmentEntry]:
        """Get all environment settings."""
        return dict(self._settings)
    
    def get_setting_names(self) -> List[str]:
        """Get all setting names."""
        return list(self._settings.keys())
    
    # Convenience methods for common environment settings
    def get_damage_multiplier(self) -> float:
        """Get the damage multiplier setting."""
        entry = self.get_setting("damageMultiplier")
        return entry.multiplier if entry else 1.0
    
    def set_damage_multiplier(self, value: float) -> 'EnvironmentXml':
        """Set the damage multiplier setting."""
        entry = EnvironmentEntry(name="damageMultiplier", multiplier=value)
        return self.set_setting(entry)
    
    def get_loot_multiplier(self) -> float:
        """Get the loot multiplier setting."""
        entry = self.get_setting("lootMultiplier")
        return entry.multiplier if entry else 1.0
    
    def set_loot_multiplier(self, value: float) -> 'EnvironmentXml':
        """Set the loot multiplier setting."""
        entry = EnvironmentEntry(name="lootMultiplier", multiplier=value)
        return self.set_setting(entry)
    
    def get_zombie_multiplier(self) -> float:
        """Get the zombie spawn multiplier setting."""
        entry = self.get_setting("zombieMultiplier")
        return entry.multiplier if entry else 1.0
    
    def set_zombie_multiplier(self, value: float) -> 'EnvironmentXml':
        """Set the zombie spawn multiplier setting."""
        entry = EnvironmentEntry(name="zombieMultiplier", multiplier=value)
        return self.set_setting(entry)
    
    def get_vehicle_multiplier(self) -> float:
        """Get the vehicle spawn multiplier setting."""
        entry = self.get_setting("vehicleMultiplier")
        return entry.multiplier if entry else 1.0
    
    def set_vehicle_multiplier(self, value: float) -> 'EnvironmentXml':
        """Set the vehicle spawn multiplier setting."""
        entry = EnvironmentEntry(name="vehicleMultiplier", multiplier=value)
        return self.set_setting(entry)
    
    def to_xml(self) -> str:
        """Convert back to XML string."""
        return CeXml.serialize(self.doc)
    
    def save(self, filepath: str) -> bool:
        """
        Save the environment.xml file.
        
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