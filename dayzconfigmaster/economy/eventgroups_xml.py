# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# eventgroups.xml parser/editor for DayZ Central Economy
# Ported from dayz-labs EconomyXml.cs

"""
EventGroupsXml class for parsing and editing eventgroups.xml.

eventgroups.xml defines groups of events that can be enabled/disabled together.
"""

from typing import Optional, List, Dict
from dataclasses import dataclass, field

from .ce_xml import XDoc, XElement, CeXml


@dataclass  
class EventGroup:
    """
    A group of economy events that can be enabled/disabled together.
    
    Each group has:
    - A name (unique identifier)
    - List of event names in the group
    - Whether the group is enabled by default
    """
    name: str
    events: List[str] = field(default_factory=list)
    enabled_by_default: bool = False


class EventGroupsXml:
    """
    Parser and editor for eventgroups.xml (DayZ Central Economy event groups file).
    
    Key features:
    - Parse and save eventgroups.xml
    - Access individual event group entries by name
    - Add, modify, or remove event group entries
    """
    
    def __init__(self):
        self._doc: Optional[XDoc] = None
        self._groups: Dict[str, EventGroup] = {}  # name -> EventGroup
    
    @property
    def doc(self) -> XDoc:
        """Get or create the underlying document."""
        if self._doc is None:
            self._doc = XDoc.create("economy")
        return self._doc
    
    @staticmethod
    def parse(xml_str: str) -> 'EventGroupsXml':
        """
        Parse an eventgroups.xml string.
        
        Args:
            xml_str: The XML content to parse
            
        Returns:
            New EventGroupsXml instance with parsed data
        """
        obj = EventGroupsXml()
        doc = CeXml.parse_doc(xml_str)
        obj._doc = doc
        
        if doc and doc.root:
            obj._parse_groups(doc.root)
        
        return obj
    
    @staticmethod
    def from_file(filepath: str) -> Optional['EventGroupsXml']:
        """
        Load an eventgroups.xml file.
        
        Args:
            filepath: Path to the XML file
            
        Returns:
            New EventGroupsXml instance, or None if loading fails
        """
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read()
            return EventGroupsXml.parse(content)
        except (IOError, Exception):
            return None
    
    def _parse_groups(self, root: XElement) -> None:
        """Parse all event group entries from the XML."""
        self._groups.clear()
        
        # Look for eventgroup elements
        for elem in root.children:
            if elem.tag == "eventgroup":
                name = elem.attr("name")
                if name:
                    group = self._parse_event_group(elem)
                    if group:
                        self._groups[name.lower()] = group
    
    def _parse_event_group(self, elem: XElement) -> Optional[EventGroup]:
        """Parse a single eventgroup element."""
        name = elem.attr("name")
        if not name:
            return None
        
        # Get whether enabled by default
        enabled_str = elem.attr("enabled")
        enabled_by_default = enabled_str.lower() in ("true", "yes", "1") if enabled_str else False
        
        # Parse event names in the group
        events: List[str] = []
        for event_elem in elem.find_all("event"):
            event_name = event_elem.text.strip() if event_elem.text else ""
            if event_name:
                events.append(event_name)
        
        return EventGroup(
            name=name,
            events=events,
            enabled_by_default=enabled_by_default
        )
    
    def get_group(self, name: str) -> Optional[EventGroup]:
        """
        Get an event group by name.
        
        Args:
            name: The event group name (case-insensitive)
            
        Returns:
            EventGroup if found, None otherwise
        """
        return self._groups.get(name.lower())
    
    def set_group(self, group: EventGroup) -> 'EventGroupsXml':
        """
        Add or update an event group entry.
        
        Args:
            group: The EventGroup to add/update
            
        Returns:
            Self for chaining
        """
        self._groups[group.name.lower()] = group
        
        # Update the XML document
        root = self.doc.root
        if not root:
            return self
        
        # Find or create the eventgroup element
        old_elem = None
        for elem in root.children:
            elem_name = elem.attr("name")
            if elem.tag == "eventgroup" and elem_name and elem_name.lower() == group.name.lower():
                old_elem = elem
                break
        
        # Remove old entry if exists
        if old_elem:
            root.remove_child(old_elem)
        
        # Add new element
        group_elem = XElement("eventgroup", name=group.name)
        group_elem.set_attr("enabled", "true" if group.enabled_by_default else "false")
        root.add_child(group_elem)
        
        for event_name in group.events:
            event_elem = XElement("event")
            event_elem.text = event_name
            group_elem.add_child(event_elem)
        
        return self
    
    def remove_group(self, name: str) -> bool:
        """
        Remove an event group by name.
        
        Args:
            name: The event group name to remove
            
        Returns:
            True if found and removed
        """
        if name.lower() not in self._groups:
            return False
        
        del self._groups[name.lower()]
        
        # Update the XML document
        root = self.doc.root
        if not root:
            return True
        
        for elem in root.children:
            elem_name = elem.attr("name")
            if elem.tag == "eventgroup" and elem_name and elem_name.lower() == name.lower():
                root.remove_child(elem)
                break
        
        return True
    
    def get_all_groups(self) -> Dict[str, EventGroup]:
        """Get all event group entries."""
        return dict(self._groups)
    
    def get_group_names(self) -> List[str]:
        """Get all event group names."""
        return list(self._groups.keys())
    
    def to_xml(self) -> str:
        """Convert back to XML string."""
        return CeXml.serialize(self.doc)
    
    def save(self, filepath: str) -> bool:
        """
        Save the eventgroups.xml file.
        
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