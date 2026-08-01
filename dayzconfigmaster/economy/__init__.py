# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Economy package for DayZ Central Economy XML parsing and editing
# Ported from dayz-labs C# implementation

"""
Economy package providing tools for parsing, editing, and generating Central Economy (CE) XML files.

This module ports the dayz-labs CeCoreXml and related classes to Python:

Key Classes:
    CeNum - Invariant culture numeric parsing functions
    CeXml - Shared XML helpers (parse_doc, serialize, by_name)
    CeCoreXml - cfgeconomycore.xml parser/editor
    TypesXml - types.xml parser/editor
    EventsXml - events.xml parser/editor  
    GlobalsXml - globals.xml parser/editor
    EnvironmentXml - environment/weather settings

XML Parsing Notes:
    Unlike simple regex parsing, this uses ElementTree for accurate XML manipulation
    that preserves comments and element order. The API is designed to match the
    dayz-labs patterns with Pythonic conventions.
"""

from .ce_num import CeNum
from .ce_xml import CeXml, XNode, XDoc, XElement, XAttribute
from .ce_core_xml import CeCoreXml
from .types_xml import TypesXml, TypeEntry, Category, Usage, Value
from .events_xml import EventsXml
from .globals_xml import GlobalsXml
from .environment_xml import EnvironmentXml
from .eventgroups_xml import EventGroupsXml, EventGroup

__all__ = [
    'CeNum',
    'CeXml', 
    'XNode', 'XDoc', 'XElement', 'XAttribute',
    'CeCoreXml',
    'TypesXml', 'TypeEntry', 'Category', 'Usage', 'Value',
    'EventsXml',
    'GlobalsXml',
    'EnvironmentXml',
    'EventGroupsXml',
    'EventGroup'
]
