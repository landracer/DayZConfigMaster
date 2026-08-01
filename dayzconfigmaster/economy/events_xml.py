# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# events.xml parser/editor for DayZ Central Economy
# Ported from dayz-labs EconomyXml.cs

"""
EventsXml class for parsing and editing events.xml.

events.xml defines economy events with settings:
- Event names and types
- Probabilities/chances
- Start/End times
- Cargo definitions
"""

import re
from typing import Optional, List, Dict
from dataclasses import dataclass, field

from .ce_xml import XDoc, XElement, CeXml
from .ce_num import CeNum


@dataclass
class EventCargo:
    """A cargo definition for an economy event."""
    name: str
    count_min: int = 1
    count_max: int = 1
    attachments_min: int = 0
    attachments_max: int = 0


@dataclass  
class EventEntry:
    """
    A single entry in events.xml representing a commerce event.
    
    Each event has:
    - A name (unique identifier)
    - Type (e.g., "CommerceBuy", "CommerceSell")
    - Probability/chance
    - Cargo definitions
    """
    name: str
    type: str = "CommerceBuy"
    chance: float = 1.0
    start_time: int = 8   # Hour of day
    end_time: int = 20    # Hour of day
    cargos: List[EventCargo] = field(default_factory=list)


class EventsXml:
    """
    Parser and editor for events.xml (DayZ Central Economy commerce events file).
    
    Key features:
    - Parse and save events.xml
    - Access individual event entries by name
    - Add, modify, or remove event entries
    """
    
    def __init__(self):
        self._doc: Optional[XDoc] = None
        self._events: Dict[str, EventEntry] = {}  # name -> EventEntry
    
    @property
    def doc(self) -> XDoc:
        """Get or create the underlying document."""
        if self._doc is None:
            self._doc = XDoc.create("economy")
        return self._doc
    
    @staticmethod
    def parse(xml_str: str) -> 'EventsXml':
        """
        Parse an events.xml string.
        
        Args:
            xml_str: The XML content to parse
            
        Returns:
            New EventsXml instance with parsed data
        """
        obj = EventsXml()
        doc = CeXml.parse_doc(xml_str)
        obj._doc = doc
        
        if doc and doc.root:
            obj._parse_events(doc.root)
        
        return obj
    
    @staticmethod
    def from_file(filepath: str) -> Optional['EventsXml']:
        """
        Load an events.xml file.
        
        Args:
            filepath: Path to the XML file
            
        Returns:
            New EventsXml instance, or None if loading fails
        """
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read()
            return EventsXml.parse(content)
        except (IOError, Exception):
            return None
    
    def _parse_events(self, root: XElement) -> None:
        """Parse all event entries from the XML."""
        self._events.clear()
        
        # Look for commerce events or event-like elements
        for elem in root.children:
            if elem.tag == "event" or elem.tag == "commerce":
                name = elem.attr("name")
                if name:
                    entry = self._parse_event_entry(elem)
                    if entry:
                        self._events[name.lower()] = entry
    
    def _parse_event_entry(self, elem: XElement) -> Optional[EventEntry]:
        """Parse a single event element."""
        name = elem.attr("name")
        if not name:
            return None
        
        # Get event type
        event_type = elem.attr("type") or "CommerceBuy"
        
        # Get chance/probability (default 1.0)
        chance_str = elem.attr("chance")
        chance = CeNum.float(chance_str, 1.0)
        
        # Get start/end times
        start_time = elem.attr_int("startTime", 8)
        end_time = elem.attr_int("endTime", 20)
        
        # Parse cargo definitions
        cargos: List[EventCargo] = []
        for cargo_elem in elem.find_all("cargo"):
            cargo_name = cargo_elem.attr("name")
            if not cargo_name:
                continue
            
            count_min = cargo_elem.attr_int("countMin", 1)
            count_max = cargo_elem.attr_int("countMax", 1)
            attachments_min = cargo_elem.attr_int("attachmentsMin", 0)
            attachments_max = cargo_elem.attr_int("attachmentsMax", 0)
            
            cargos.append(EventCargo(
                name=cargo_name,
                count_min=count_min,
                count_max=count_max,
                attachments_min=attachments_min,
                attachments_max=attachments_max
            ))
        
        return EventEntry(
            name=name,
            type=event_type,
            chance=chance,
            start_time=start_time,
            end_time=end_time,
            cargos=cargos
        )
    
    def get_event(self, name: str) -> Optional[EventEntry]:
        """
        Get an event entry by name.
        
        Args:
            name: The event name (case-insensitive)
            
        Returns:
            EventEntry if found, None otherwise
        """
        return self._events.get(name.lower())
    
    def set_event(self, entry: EventEntry) -> 'EventsXml':
        """
        Add or update an event entry.
        
        Args:
            entry: The EventEntry to add/update
            
        Returns:
            Self for chaining
        """
        self._events[entry.name.lower()] = entry
        
        # Update the XML document
        root = self.doc.root
        if not root:
            return self
        
        # Find or create the event element
        old_elem = None
        for elem in root.children:
            if elem.tag in ("event", "commerce") and elem.attr("name", "").lower() == entry.name.lower():
                old_elem = elem
                break
        
        # Remove old entry if exists
        if old_elem:
            root.remove_child(old_elem)
        
        # Add new element
        event_elem = XElement("event", name=entry.name, type=entry.type)
        event_elem.set_attr("chance", str(entry.chance))
        event_elem.set_attr("startTime", str(entry.start_time))
        event_elem.set_attr("endTime", str(entry.end_time))
        root.add_child(event_elem)
        
        for cargo in entry.cargos:
            cargo_elem = XElement("cargo", name=cargo.name)
            cargo_elem.set_attr("countMin", str(cargo.count_min))
            cargo_elem.set_attr("countMax", str(cargo.count_max))
            cargo_elem.set_attr("attachmentsMin", str(cargo.attachments_min))
            cargo_elem.set_attr("attachmentsMax", str(cargo.attachments_max))
            event_elem.add_child(cargo_elem)
        
        return self
    
    def remove_event(self, name: str) -> bool:
        """
        Remove an event entry by name.
        
        Args:
            name: The event name to remove
            
        Returns:
            True if found and removed
        """
        if name.lower() not in self._events:
            return False
        
        del self._events[name.lower()]
        
        # Update the XML document
        root = self.doc.root
        if not root:
            return True
        
        for elem in root.children:
            if elem.tag in ("event", "commerce") and elem.attr("name", "").lower() == name.lower():
                root.remove_child(elem)
                break
        
        return True
    
    def get_all_events(self) -> Dict[str, EventEntry]:
        """Get all event entries."""
        return dict(self._events)
    
    def get_event_names(self) -> List[str]:
        """Get all event names."""
        return list(self._events.keys())
    
    def to_xml(self) -> str:
        """Convert back to XML string."""
        return CeXml.serialize(self.doc)
    
    def save(self, filepath: str) -> bool:
        """
        Save the events.xml file.
        
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