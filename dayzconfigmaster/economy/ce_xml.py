# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Shared XML helpers for DayZ Central Economy
# Ported from dayz-labs CeXml.cs

"""
CeXml class providing shared XML helpers.

This module provides Python equivalents of dayz-labs CeXml functionality:
- parse_doc: Parse XML string into document (like XDocument.Parse)
- serialize: Convert document back to string
- by_name: Case-insensitive lookup by attribute value

Note: This uses ElementTree which has limitations compared to XDocument.
For full comment/order preservation, consider lxml or xml.etree.ElementTree.
"""

import re
from typing import Optional, List, Iterable
from xml.etree import ElementTree as ET


class XNode:
    """Base XML node (abstract base for XDoc and XElement)."""
    pass


class XAttribute:
    """Represents an XML attribute."""
    
    def __init__(self, name: str, value: str):
        self.name = name
        self.value = value
    
    @staticmethod
    def parse(raw: Optional[str]) -> 'XAttribute':
        """Parse a raw attribute string into an XAttribute."""
        if not raw:
            return XAttribute("", "")
        
        # Split on first '='
        eq_pos = raw.find('=')
        if eq_pos == -1:
            return XAttribute(raw.strip(), "")
        
        name = raw[:eq_pos].strip()
        value = raw[eq_pos + 1:].strip().strip('"\'')
        return XAttribute(name, value)
    
    def __str__(self) -> str:
        return f'{self.name}="{self.value}"'


class XElement(XNode):
    """Represents an XML element."""
    
    def __init__(self, tag: str, text: Optional[str] = None, **attributes):
        self.tag = tag
        self.text = text
        self.attributes: dict = attributes
        self.children: List['XElement'] = []
        self._raw_text = text  # Preserve original text formatting
    
    @staticmethod
    def create(tag: str) -> 'XElement':
        """Create a new element with the given tag."""
        return XElement(tag)
    
    @staticmethod
    def parse(xml_str: str) -> Optional['XElement']:
        """
        Parse an XML string into an XElement.
        
        Args:
            xml_str: The XML string to parse
            
        Returns:
            Parsed XElement or None if parsing fails
        """
        try:
            # Wrap single element in a root for parsing
            wrapped = f"<root>{xml_str}</root>"
            root = ET.fromstring(wrapped)
            return XElement._from_etree(root)
        except ET.ParseError:
            return None
    
    @staticmethod
    def _from_etree(elem: ET.Element) -> 'XElement':
        """Convert an ElementTree element to XElement."""
        attributes = dict(elem.attrib)
        
        # Get text content
        text = elem.text.strip() if elem.text and elem.text.strip() else None
        
        xelem = XElement(elem.tag, text, **attributes)
        
        # Add children
        for child in elem:
            xelem.children.append(XElement._from_etree(child))
        
        return xelem
    
    def find(self, tag: str) -> Optional['XElement']:
        """Find first child with matching tag."""
        for child in self.children:
            if child.tag == tag:
                return child
        return None
    
    def find_all(self, tag: str) -> List['XElement']:
        """Find all children with matching tag."""
        return [child for child in self.children if child.tag == tag]
    
    def attr(self, name: str) -> Optional[str]:
        """Get attribute value by name."""
        return self.attributes.get(name)
    
    def attr_int(self, name: str, fallback: int = 0) -> int:
        """Get attribute value as integer with fallback."""
        from .ce_num import CeNum
        return CeNum.int(self.attr(name), fallback)
    
    def attr_float(self, name: str, fallback: float = 0.0) -> float:
        """Get attribute value as float with fallback."""
        from .ce_num import CeNum
        return CeNum.float(self.attr(name), fallback)
    
    def attr_bool(self, name: str, fallback: bool = False) -> bool:
        """Get attribute value as boolean with fallback."""
        from .ce_num import CeNum
        return CeNum.bool(self.attr(name), fallback)
    
    def set_attr(self, name: str, value: str) -> 'XElement':
        """Set an attribute value and return self for chaining."""
        self.attributes[name] = str(value)
        return self
    
    def remove_attr(self, name: str) -> 'XElement':
        """Remove an attribute and return self for chaining."""
        if name in self.attributes:
            del self.attributes[name]
        return self
    
    def add_child(self, child: 'XElement') -> 'XElement':
        """Add a child element and return self for chaining."""
        self.children.append(child)
        return self
    
    def remove_child(self, child: 'XElement') -> bool:
        """Remove a child element. Returns True if found."""
        if child in self.children:
            self.children.remove(child)
            return True
        return False
    
    def text_content(self) -> str:
        """Get all text content from this element and children."""
        parts = []
        if self.text:
            parts.append(self.text)
        for child in self.children:
            parts.append(child.text_content())
        return ''.join(parts)


class XDoc(XNode):
    """Represents an XML document with root element."""
    
    def __init__(self, root: Optional[XElement] = None, version: str = "1.0", encoding: str = "UTF-8"):
        self.root = root
        self.version = version
        self.encoding = encoding
    
    @staticmethod
    def create(root_tag: str) -> 'XDoc':
        """Create a new document with the given root element."""
        return XDoc(XElement.create(root_tag))
    
    @staticmethod
    def parse(xml_str: str) -> Optional['XDoc']:
        """
        Parse an XML string into an XDoc.
        
        Args:
            xml_str: The XML string to parse
            
        Returns:
            Parsed XDoc or None if parsing fails
        """
        try:
            root = ET.fromstring(xml_str)
            return XDoc(XElement._from_etree(root))
        except ET.ParseError:
            return None
    
    @staticmethod
    def from_file(filepath: str) -> Optional['XDoc']:
        """Parse an XML file into an XDoc."""
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read()
            return XDoc.parse(content)
        except (IOError, ET.ParseError):
            return None
    
    def to_file(self, filepath: str) -> bool:
        """Write the document to a file."""
        try:
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(self.serialize())
            return True
        except IOError:
            return False
    
    def save(self) -> bool:
        """Save to the original file path (if known)."""
        # For now, this is a placeholder
        # In a full implementation, we'd track the source filepath
        return self.to_file("output.xml")


class CeXml:
    """
    XML parsing and manipulation helpers for DayZ Central Economy.
    
    Provides Python equivalents of dayz-labs CeXml functionality:
    - ParseDoc: Parse XML string into document (like XDocument.Parse)
    - Serialize: Convert document back to string
    - ByName: Case-insensitive lookup by attribute value
    """
    
    # ElementTree doesn't preserve comments natively
    # For full comment preservation, use lxml or a custom solution
    
    @staticmethod
    def parse_doc(xml_str: str) -> XDoc:
        """
        Parse XML string into an XDoc (like dayz-labs XDocument.Parse).
        
        Args:
            xml_str: The XML string to parse
            
        Returns:
            Parsed XDoc object
            
        Raises:
            ET.ParseError: If the XML is invalid
        """
        return XDoc.parse(xml_str)
    
    @staticmethod
    def serialize(doc: XDoc) -> str:
        """
        Convert an XDoc back to an XML string.
        
        Args:
            doc: The document to serialize
            
        Returns:
            XML string representation
        """
        lines = []
        
        # XML declaration
        if doc.version and doc.encoding:
            lines.append(f'<?xml version="{doc.version}" encoding="{doc.encoding}"?>')
        
        # Serialize root element
        if doc.root:
            lines.append(CeXml._serialize_element(doc.root, 0))
        
        return '\n'.join(lines)
    
    @staticmethod
    def _serialize_element(elem: XElement, indent_level: int) -> str:
        """Serialize an XElement to a string with proper indentation."""
        indent = '    ' * indent_level
        
        # Build attributes string
        attr_parts = []
        for name, value in elem.attributes.items():
            if value is None:
                attr_parts.append(f'{name}')
            else:
                escaped_value = str(value).replace('&', '&').replace('<', '<').replace('>', '>')
                attr_parts.append(f'{name}="{escaped_value}"')
        
        attrs_str = ' '.join(attr_parts)
        
        # Check if element has content
        has_text = elem.text is not None and elem.text.strip()
        has_children = len(elem.children) > 0
        
        if not has_text and not has_children:
            # Empty element - self-closing
            if attrs_str:
                return f'{indent}<{elem.tag} {attrs_str}/>'
            else:
                return f'{indent}<{elem.tag}/>'
        
        # Start tag with attributes
        start_tag = f'<{elem.tag}'
        if attrs_str:
            start_tag += f' {attrs_str}'
        start_tag += '>'
        
        # Build content
        parts = [start_tag]
        
        if has_text and not has_children:
            # Text-only element
            escaped_text = elem.text.replace('&', '&').replace('<', '<')
            parts.append(escaped_text)
        elif has_children:
            # Has children - serialize each child
            for child in elem.children:
                parts.append(CeXml._serialize_element(child, indent_level + 1))
            
            if has_text and elem.text.strip():
                escaped_text = elem.text.replace('&', '&').replace('<', '<')
                parts.insert(1, f'{indent}    {escaped_text}')
        
        # End tag
        parts.append(f'{indent}</{elem.tag}>')
        
        return '\n'.join(parts)
    
    @staticmethod
    def by_name(
        elements: Iterable[XElement], 
        name: str, 
        excluding: Optional[XElement] = None,
        attr: str = "name"
    ) -> Optional[XElement]:
        """
        Find an element by attribute value (case-insensitive).
        
        Args:
            elements: Collection of elements to search
            name: The name to look for in the specified attribute
            excluding: Element to exclude from search (optional)
            attr: The attribute name to match against (default: "name")
            
        Returns:
            First matching element, or None if not found
        """
        if elements is None:
            return None
        
        # Handle both list-like and generator iterables
        elements_list = list(elements) if not isinstance(elements, list) else elements
        
        for elem in elements_list:
            if elem == excluding:
                continue
            
            attr_value = elem.attributes.get(attr)
            if attr_value and attr_value.strip().lower() == name.strip().lower():
                return elem
        
        return None
    
    @staticmethod
    def by_name_all(
        elements: Iterable[XElement],
        name: str,
        attr: str = "name"
    ) -> List[XElement]:
        """
        Find all elements with matching attribute value (case-insensitive).
        
        Args:
            elements: Collection of elements to search
            name: The name to look for in the specified attribute
            attr: The attribute name to match against (default: "name")
            
        Returns:
            List of matching elements
        """
        if elements is None:
            return []
        
        results = []
        elements_list = list(elements) if not isinstance(elements, list) else elements
        
        for elem in elements_list:
            attr_value = elem.attributes.get(attr)
            if attr_value and attr_value.strip().lower() == name.strip().lower():
                results.append(elem)
        
        return results
    
    @staticmethod
    def load_file(filepath: str) -> Optional[XDoc]:
        """
        Load an XML file into an XDoc.
        
        Args:
            filepath: Path to the XML file
            
        Returns:
            Parsed XDoc or None if loading fails
        """
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read()
            return CeXml.parse_doc(content)
        except (IOError, ET.ParseError):
            return None
    
    @staticmethod
    def save_file(doc: XDoc, filepath: str) -> bool:
        """
        Save an XDoc to a file.
        
        Args:
            doc: The document to save
            filepath: Path to the output file
            
        Returns:
            True if successful, False otherwise
        """
        try:
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(CeXml.serialize(doc))
            return True
        except IOError:
            return False