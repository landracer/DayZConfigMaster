# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# cfgeconomycore.xml parser/editor for DayZ Central Economy
# Ported from dayz-labs CeCoreXml.cs

"""
CeCoreXml class for parsing and editing cfgeconomycore.xml.

This module ports the dayz-labs CeCoreXml functionality to Python:
- Parse and edit cfgeconomycore.xml files
- Manage file routing entries (folder paths + type mappings)
- Access child XML documents via routed types
"""

from typing import Optional, List, Dict
import os

from .ce_xml import XDoc, XElement, CeXml
from .types_xml import TypesXml


class CeCoreXml:
    """
    Parser and editor for cfgeconomycore.xml (DayZ Central Economy core file).
    
    The cfgeconomycore.xml routes CE files via <folder><file name="type"/> entries.
    For example: <folder path="data"> types.xml </folder>
    
    Key features:
    - Parse and save cfgeconomycore.xml
    - Route lookup by type name (case-insensitive)
    - Load routed XML documents (types.xml, etc.)
    """
    
    def __init__(self):
        self._doc: Optional[XDoc] = None
        self._routed_files: Dict[str, str] = {}  # type_name -> file_path
        self._folders: List[Dict] = []  # [{path: str, files: [str]}]
    
    @property
    def doc(self) -> XDoc:
        """Get or create the underlying document."""
        if self._doc is None:
            self._doc = XDoc.create("economy")
        return self._doc
    
    @staticmethod
    def parse(xml_str: str) -> 'CeCoreXml':
        """
        Parse a cfgeconomycore.xml string.
        
        Args:
            xml_str: The XML content to parse
            
        Returns:
            New CeCoreXml instance with parsed data
        """
        obj = CeCoreXml()
        doc = CeXml.parse_doc(xml_str)
        obj._doc = doc
        
        if doc and doc.root:
            obj._parse_routed_files(doc.root)
        
        return obj
    
    @staticmethod
    def from_file(filepath: str) -> Optional['CeCoreXml']:
        """
        Load a cfgeconomycore.xml file.
        
        Args:
            filepath: Path to the XML file
            
        Returns:
            New CeCoreXml instance, or None if loading fails
        """
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read()
            return CeCoreXml.parse(content)
        except (IOError, Exception):
            return None
    
    def _parse_routed_files(self, root: XElement) -> None:
        """Parse routed file entries from the XML."""
        self._routed_files.clear()
        self._folders.clear()
        
        # Find all <folder> elements
        for folder_elem in root.find_all("folder"):
            path = folder_elem.attr("path") or ""
            
            # Process child files
            folder_data = {"path": path, "files": []}
            
            for file_elem in folder_elem.children:
                if file_elem.tag == "file":
                    type_name = file_elem.attr("name")
                    if type_name:
                        self._routed_files[type_name.lower()] = file_elem.text or ""
                        folder_data["files"].append(type_name)
            
            self._folders.append(folder_data)
    
    def get_routed_file(self, type_name: str) -> Optional[str]:
        """
        Get the file path for a routed type.
        
        Args:
            type_name: The type name (e.g., "types", "globals")
            
        Returns:
            File path relative to the folder's path, or None if not found
        """
        return self._routed_files.get(type_name.lower())
    
    def set_routed_file(self, type_name: str, file_path: str) -> 'CeCoreXml':
        """
        Set or update a routed file entry.
        
        Args:
            type_name: The type name (e.g., "types", "globals")
            file_path: File path relative to the folder's path
            
        Returns:
            Self for chaining
        """
        self._routed_files[type_name.lower()] = file_path
        
        # Find or create a folder element
        root = self.doc.root
        if not root:
            return self
        
        # Look for existing entry
        found_elem = None
        for folder in root.children:
            if folder.tag == "folder":
                for child in folder.children:
                    if child.tag == "file" and child.attr("name", "").lower() == type_name.lower():
                        child.text = file_path
                        found_elem = child
                        break
        
        # Create new entry if not found
        if not found_elem:
            folder_elem = root.find("folder")
            if not folder_elem:
                # Create a default folder
                folder_elem = XElement("folder", path="data")
                root.add_child(folder_elem)
            
            file_elem = XElement("file", text=file_path, name=type_name)
            folder_elem.add_child(file_elem)
        
        return self
    
    def get_folder_paths(self) -> List[str]:
        """Get all configured folder paths."""
        return [f["path"] for f in self._folders if f.get("path")]
    
    def add_folder(self, path: str) -> 'CeCoreXml':
        """
        Add a new folder entry.
        
        Args:
            path: Folder path to add
            
        Returns:
            Self for chaining
        """
        # Check if already exists
        for f in self._folders:
            if f.get("path") == path:
                return self
        
        self._folders.append({"path": path})
        
        root = self.doc.root
        folder_elem = XElement("folder", path=path)
        root.add_child(folder_elem)
        
        return self
    
    def remove_folder(self, path: str) -> bool:
        """
        Remove a folder entry.
        
        Args:
            path: Folder path to remove
            
        Returns:
            True if found and removed
        """
        for i, f in enumerate(self._folders):
            if f.get("path") == path:
                self._folders.pop(i)
                
                root = self.doc.root
                if root:
                    folder_elem = None
                    for elem in root.children:
                        if elem.tag == "folder" and elem.attr("path") == path:
                            folder_elem = elem
                            break
                    
                    if folder_elem:
                        root.remove_child(folder_elem)
                
                return True
        
        return False
    
    def get_all_routed_types(self) -> Dict[str, str]:
        """Get all routed type mappings."""
        return dict(self._routed_files)
    
    def get_routing_entries(self) -> List[Dict[str, str]]:
        """
        Get all routing entries as a list of {type, file} dicts.
        
        Returns:
            List of routing entry dictionaries
        """
        return [
            {"type": k, "file": v}
            for k, v in self._routed_files.items()
        ]
    
    def load_types_xml(self, base_path: Optional[str] = None) -> Optional[TypesXml]:
        """
        Load the types.xml file that's routed by cfgeconomycore.xml.
        
        Args:
            base_path: Base directory to resolve relative paths
            
        Returns:
            Parsed TypesXml object, or None if not found
        """
        type_file = self.get_routed_file("types")
        if not type_file:
            return None
        
        # Resolve the full path
        full_path = type_file
        if base_path and not os.path.isabs(type_file):
            full_path = os.path.join(base_path, type_file)
        
        content = TypesXml.from_file(full_path)
        return content
    
    def to_xml(self) -> str:
        """Convert back to XML string."""
        return CeXml.serialize(self.doc)
    
    def save(self, filepath: str) -> bool:
        """
        Save the cfgeconomycore.xml file.
        
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