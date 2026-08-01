# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Unit tests for economy module
# Tests ported from dayz-labs EconomyXml.cs

"""
Unit tests for DayZConfigMaster economy module.
Tests cover:
- EventGroupsXml parsing and editing
- EventsXml operations
"""

import os
import sys
import tempfile
from pathlib import Path

# Add project root to path
sys.path.insert(0, str(Path(__file__).parent.parent))

from dayzconfigmaster.economy import EventGroupsXml, EventGroup, EventsXml


def test_event_groups_xml_parse():
    """Test parsing eventgroups.xml."""
    xml_content = '''<?xml version="1.0" encoding="UTF-8"?>
<economy>
    <eventgroup name="dayEvents" enabled="true">
        <event>DayEvent1</event>
        <event>DayEvent2</event>
    </eventgroup>
    <eventgroup name="nightEvents" enabled="false">
        <event>NightEvent1</event>
    </eventgroup>
</economy>'''
    
    obj = EventGroupsXml.parse(xml_content)
    assert obj is not None
    assert obj.get_group("dayEvents") is not None
    assert obj.get_group("NightEvents") is not None
    
    day_group = obj.get_group("dayEvents")
    assert day_group.enabled_by_default == True
    assert len(day_group.events) == 2
    
    print("test_event_groups_xml_parse: PASSED")


def test_event_groups_xml_set_group():
    """Test adding/updating a group."""
    xml_content = '''<?xml version="1.0" encoding="UTF-8"?>
<economy>
</economy>'''
    
    obj = EventGroupsXml.parse(xml_content)
    
    # Add new group
    group = EventGroup(
        name="testGroup",
        events=["event1", "event2"],
        enabled_by_default=True
    )
    obj.set_group(group)
    
    retrieved = obj.get_group("testGroup")
    assert retrieved is not None
    assert len(retrieved.events) == 2
    
    print("test_event_groups_xml_set_group: PASSED")


def test_event_groups_xml_remove_group():
    """Test removing a group."""
    xml_content = '''<?xml version="1.0" encoding="UTF-8"?>
<economy>
    <eventgroup name="toRemove" enabled="true">
        <event>someEvent</event>
    </eventgroup>
</economy>'''
    
    obj = EventGroupsXml.parse(xml_content)
    
    assert obj.get_group("toRemove") is not None
    result = obj.remove_group("toRemove")
    assert result == True
    assert obj.get_group("toRemove") is None
    
    print("test_event_groups_xml_remove_group: PASSED")


def test_event_groups_xml_save_load():
    """Test saving and loading from file."""
    with tempfile.TemporaryDirectory() as tmpdir:
        filepath = os.path.join(tmpdir, "eventgroups.xml")
        
        obj = EventGroupsXml()
        group = EventGroup(
            name="testGroup",
            events=["event1"],
            enabled_by_default=False
        )
        obj.set_group(group)
        
        # Save to file
        assert obj.save(filepath) == True
        
        # Load from file
        loaded_obj = EventGroupsXml.from_file(filepath)
        assert loaded_obj is not None
        assert loaded_obj.get_group("testGroup") is not None
        
    print("test_event_groups_xml_save_load: PASSED")


def test_events_xml_parse():
    """Test parsing events.xml."""
    xml_content = '''<?xml version="1.0" encoding="UTF-8"?>
<economy>
    <event name="commerceBuyFood" type="CommerceBuy" chance="1.5">
        <startTime>6</startTime>
        <endTime>22</endTime>
        <cargo name="Bread" countMin="2" countMax="4"/>
    </event>
</economy>'''
    
    obj = EventsXml.parse(xml_content)
    assert obj is not None
    
    event = obj.get_event("commerceBuyFood")
    assert event is not None
    assert event.type == "CommerceBuy"
    assert abs(event.chance - 1.5) < 0.001
    assert len(event.cargos) == 1
    
    print("test_events_xml_parse: PASSED")


if __name__ == "__main__":
    test_event_groups_xml_parse()
    test_event_groups_xml_set_group()
    test_event_groups_xml_remove_group()
    test_event_groups_xml_save_load()
    test_events_xml_parse()
    print("\nAll economy tests passed!")