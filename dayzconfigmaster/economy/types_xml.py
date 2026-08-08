# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# types.xml parser/editor for DayZ Central Economy
# Ported from dayz-labs TypesXml.cs

"""
TypesXml class for parsing and editing types.xml.

types.xml defines item types with their properties:
- Categories (e.g., "Container", "Weapon")
- Usages (e.g., "Town", "Military")
- Values (e.g., "Tier1", "Tier3")
"""

import re
import tempfile
from typing import Optional, List, Dict, Set
from pathlib import Path
from dataclasses import dataclass, field

from .ce_xml import XDoc, XElement, CeXml


@dataclass
class Category:
    """A category for an item type."""
    name: str
    value: str = ""


@dataclass
class Usage:
    """A usage context for an item type."""
    name: str
    value: str = ""


@dataclass
class Value:
    """A value tier for an item type."""
    name: str
    value: str = ""


@dataclass
class TypeEntry:
    """
    A single entry in types.xml representing an item type.
    
    Each type has:
    - A name (unique identifier)
    - Category, Usage, Value tags with optional values
    - Economic properties (nominal, min, lifetime, restock)
    - Quant/cost flags (quantmin, quantmax, cost, tags)
    """
    name: str
    categories: List[Category] = field(default_factory=list)
    usages: List[Usage] = field(default_factory=list)
    values: List[Value] = field(default_factory=list)
    
    # Economic properties from XML element attributes/children
    nominal: int = 0
    min: int = 0
    lifetime: int = 0
    restock: int = 0
    
    # Additional DayZ item properties
    quantmin: int = -1
    quantmax: int = -1
    cost: int = 100
    tags: List[str] = field(default_factory=list)
    
    # Flags from XML <flags .../> element
    flags: dict = field(default_factory=lambda: {
        "count_in_map": True,
        "count_in_player": False,
        "count_in_cargo": False,
        "count_in_hoarder": False,
        "crafted": False,
        "deloot": False,
    })


class TypesXml:
    """
    Parser and editor for types.xml (DayZ Central Economy types definition file).
    
    Key features:
    - Parse and save types.xml
    - Access individual type entries by name
    - Add, modify, or remove type entries
    """
    
    def __init__(self):
        self._doc: Optional[XDoc] = None
        self._types: Dict[str, TypeEntry] = {}  # name -> TypeEntry
    
    @property
    def doc(self) -> XDoc:
        """Get or create the underlying document."""
        if self._doc is None:
            self._doc = XDoc.create("types")
        return self._doc
    
    @staticmethod
    def parse(xml_str: str) -> 'TypesXml':
        """
        Parse a types.xml string.
        
        Args:
            xml_str: The XML content to parse
            
        Returns:
            New TypesXml instance with parsed data
        """
        obj = TypesXml()
        doc = CeXml.parse_doc(xml_str)
        obj._doc = doc
        
        if doc and doc.root:
            obj._parse_types(doc.root)
        
        return obj
    
    @staticmethod
    def from_file(filepath: str) -> Optional['TypesXml']:
        """
        Load a types.xml file.
        
        Args:
            filepath: Path to the XML file
            
        Returns:
            New TypesXml instance, or None if loading fails
        """
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read()
            return TypesXml.parse(content)
        except (IOError, OSError, ValueError):
            return None

    @staticmethod
    def from_text(content: str) -> Optional['TypesXml']:
        """
        Load a types.xml document from a string.
        
        Args:
            content: XML content as a string.
            
        Returns:
            New TypesXml instance, or None if parsing fails.
        """
        try:
            return TypesXml.parse(content)
        except (ValueError, Exception):
            return None
    
    def _parse_types(self, root: XElement) -> None:
        """Parse all type entries from the XML."""
        self._types.clear()
        
        for type_elem in root.find_all("type"):
            entry = self._parse_type_entry(type_elem)
            if entry:
                # Store with lowercase key for case-insensitive lookup
                self._types[entry.name.lower()] = entry
    
    def _parse_type_entry(self, elem: XElement) -> Optional[TypeEntry]:
        """Parse a single type element."""
        name = elem.attr("name")
        if not name:
            return None
        
        categories: List[Category] = []
        usages: List[Usage] = []
        values: List[Value] = []
        
        # Create a temporary TypeEntry to hold the parsed data
        entry_data = TypeEntry(name=name)
        
        for child in elem.children:
            if child.tag == "category":
                # Real DayZ format: <category name="weapons" value="Tier3"/>
                cat_name = child.attr("name") or child.text_content() or ""
                cat_val = child.attr("value") or ""
                categories.append(Category(name=cat_name, value=cat_val))
            elif child.tag == "usage":
                # Real DayZ format: <usage name="Military" value=""/>
                use_name = child.attr("name") or child.text_content() or ""
                use_val = child.attr("value") or ""
                usages.append(Usage(name=use_name, value=use_val))
            elif child.tag == "value":
                # Real DayZ format: <value name="Tier3" value=""/>
                val_name = child.attr("name") or child.text_content() or ""
                val_val = child.attr("value") or ""
                values.append(Value(name=val_name, value=val_val))
            elif child.tag in ("nominal", "min", "lifetime", "restock"):
                # Parse economic property as integer
                try:
                    val = int(child.text_content() or "0")
                    setattr(entry_data, child.tag, val)
                except ValueError:
                    pass
            elif child.tag == "quantmin":
                entry_data.quantmin = int(child.text_content() or "-1")
            elif child.tag == "quantmax":
                entry_data.quantmax = int(child.text_content() or "-1")
            elif child.tag == "cost":
                entry_data.cost = int(child.text_content() or "0")
            elif child.tag == "tag" and child.attr("name"):
                entry_data.tags.append(child.attr("name"))
            elif child.tag == "flags":
                # Parse flags attributes
                for flag_name in ("count_in_map", "count_in_player", "count_in_cargo",
                                  "count_in_hoarder", "crafted", "deloot"):
                    val = child.attr(flag_name)
                    if val is not None:
                        entry_data.flags[flag_name] = val.lower() in ("1", "true")
        
        entry_data.categories = categories
        entry_data.usages = usages
        entry_data.values = values
        
        return TypeEntry(
            name=name,
            categories=entry_data.categories,
            usages=entry_data.usages,
            values=entry_data.values,
            nominal=entry_data.nominal,
            min=entry_data.min,
            lifetime=entry_data.lifetime,
            restock=entry_data.restock,
            quantmin=entry_data.quantmin,
            quantmax=entry_data.quantmax,
            cost=entry_data.cost,
            tags=entry_data.tags,
            flags=entry_data.flags
        )
    
    def get_type(self, name: str) -> Optional[TypeEntry]:
        """
        Get a type entry by name.
        
        Args:
            name: The type name (case-insensitive)
            
        Returns:
            TypeEntry if found, None otherwise
        """
        return self._types.get(name.lower())
    
    def get_type_all(self, name: str) -> List[TypeEntry]:
        """
        Get all type entries matching a name pattern.
        
        Args:
            name: The type name (case-insensitive)
            
        Returns:
            List of matching TypeEntry objects
        """
        results = []
        for k, v in self._types.items():
            if name.lower() in k.lower():
                results.append(v)
        return results
    
    def set_type(self, entry: TypeEntry) -> 'TypesXml':
        """
        Add or update a type entry.
        
        Args:
            entry: The TypeEntry to add/update
            
        Returns:
            Self for chaining
        """
        self._types[entry.name.lower()] = entry
        
        # Update the XML document
        root = self.doc.root
        if not root:
            return self
        
        # Find or create the type element
        old_elem = None
        for elem in root.children:
            name_attr = elem.attr("name")
            if elem.tag == "type" and name_attr and name_attr.lower() == entry.name.lower():
                old_elem = elem
                break
        
        # Remove old entry if exists
        if old_elem:
            root.remove_child(old_elem)
        
        # Add new element
        type_elem = XElement("type", name=entry.name)
        root.add_child(type_elem)
        
        # Economic properties are CHILD ELEMENTS with text (mirror the parser).
        for field_name in ("nominal", "min", "lifetime", "restock"):
            value = getattr(entry, field_name, 0)
            child = XElement(field_name)
            child.text = str(int(value))
            type_elem.add_child(child)

        # category/usage/value: name goes in attribute, value also as attribute (DayZ format).
        # Format: <category name="weapons" value="Tier3"/> for DayZ
        for cat in entry.categories:
            elem = XElement("category", name=cat.name or "", value=cat.value or "")
            type_elem.add_child(elem)
        for use in entry.usages:
            elem = XElement("usage", name=use.name or "", value=use.value or "")
            type_elem.add_child(elem)
        for val in entry.values:
            elem = XElement("value", name=val.name or "", value=val.value or "")
            type_elem.add_child(elem)

        # Additional DayZ properties (all values including -1 are valid in DayZ)
        child = XElement("quantmin")
        child.text = str(entry.quantmin)
        type_elem.add_child(child)
        child = XElement("quantmax")
        child.text = str(entry.quantmax)
        type_elem.add_child(child)
        child = XElement("cost")
        child.text = str(entry.cost)
        type_elem.add_child(child)

        # Tags as <tag name=""> elements
        for tag in entry.tags:
            elem = XElement("tag", name=tag)
            type_elem.add_child(elem)

        # Flags as <flags .../> element
        flags_elem = XElement("flags")
        for flag_name, flag_val in entry.flags.items():
            flags_elem.attributes[flag_name] = "1" if flag_val else "0"
        if flags_elem.attributes:
            type_elem.add_child(flags_elem)

        return self
    
    def remove_type(self, name: str) -> bool:
        """
        Remove a type entry by name.
        
        Args:
            name: The type name to remove
            
        Returns:
            True if found and removed
        """
        if name.lower() not in self._types:
            return False
        
        del self._types[name.lower()]
        
        # Update the XML document
        root = self.doc.root
        if not root:
            return True
        
        for elem in root.children:
            name_attr = elem.attr("name")
            if elem.tag == "type" and name_attr and name_attr.lower() == name.lower():
                root.remove_child(elem)
                break
        
        return True
    
    def get_all_types(self) -> Dict[str, TypeEntry]:
        """Get all type entries."""
        return dict(self._types)
    
    def get_type_names(self) -> List[str]:
        """Get all type names."""
        return list(self._types.keys())
    
    def get_categories(self) -> List[str]:
        """Get all category names."""
        # Collect from type entries
        categories = set()
        for entry in self._types.values():
            for cat in entry.categories:
                categories.add(cat.name)
        return list(categories)
    
    def get_usages(self) -> List[str]:
        """Get all usage names."""
        usages = set()
        for entry in self._types.values():
            for use in entry.usages:
                usages.add(use.name)
        return list(usages)
    
    def get_values(self) -> List[str]:
        """Get all value tier names."""
        values = set()
        for entry in self._types.values():
            for val in entry.values:
                values.add(val.name)
        return list(values)

    # ========== BATCH OPERATIONS ==========

    def batch_scale(self, class_names: List[str], field: str, percent: float) -> Dict[str, bool]:
        """
        Scale an integer field by a percentage for given classes.

        Args:
            class_names: List of type names to modify
            field: Field name ('nominal', 'min', 'lifetime', 'restock')
            percent: Percentage factor (e.g., 120 = +20%)

        Returns:
            Dict mapping class name to success status
        """
        results = {}
        caps = {"lifetime": 3_888_000, "restock": 3_888_000}
        factor = percent / 100.0

        for name in class_names:
            entry = self.get_type(name)
            if entry is None:
                results[name] = False
                continue

            # Get current value from TypeEntry attributes
            current_val = getattr(entry, field, 0) or 0
            new_val = int(round(current_val * factor))

            # Apply caps
            if field in caps:
                new_val = min(new_val, caps[field])
            new_val = max(0, new_val)

            setattr(entry, field, new_val)
            
            # Update the XML element with the new value
            root = self.doc.root
            if root:
                for elem in root.children:
                    type_name = elem.attr("name")
                    if elem.tag == "type" and type_name and type_name.lower() == name.lower():
                        # Replace or add the element
                        found = False
                        for existing in list(elem.children):
                            if existing.tag == field:
                                existing.text = str(new_val)
                                found = True
                                break
                        if not found:
                            elem.add_child(XElement(field, text=str(new_val)))

            self.set_type(entry)  # Update the XML and internal state
            results[name] = True

        return results

    def batch_set_category(self, class_names: List[str], category: str) -> Dict[str, bool]:
        """
        Set a category for given classes.

        Args:
            class_names: List of type names to modify
            category: Category name to set

        Returns:
            Dict mapping class name to success status
        """
        results = {}

        for name in class_names:
            entry = self.get_type(name)
            if entry is None:
                results[name] = False
                continue

            # Remove existing categories and add new one
            entry.categories = [Category(name=category)]
            
            # Update XML (use DayZ attribute format)
            root = self.doc.root
            if root:
                for elem in root.children:
                    type_name = elem.attr("name")
                    if elem.tag == "type" and type_name and type_name.lower() == name.lower():
                        # Remove old category elements
                        for child in list(elem.children):
                            if child.tag == "category":
                                elem.remove_child(child)
                        # Add new category with DayZ format: <category name="..."/>
                        cat_elem = XElement("category", name=category, value="")
                        elem.add_child(cat_elem)

            self.set_type(entry)  # Update the XML and internal state
            results[name] = True

        return results

    def _parse_classnames(self, text: str) -> List[str]:
        """
        Parse comma/space/newline separated class names.
        
        Args:
            text: Text containing class names (comma, space, or newline separated)
            
        Returns:
            List of unique class names in order of first appearance
        """
        # Extract all word characters as potential class names
        matches = re.findall(r'\w+', text)
        # Deduplicate while preserving order
        seen: Set[str] = set()
        result: List[str] = []
        for name in matches:
            if name and name not in seen:  # Skip empty strings
                seen.add(name)
                result.append(name)
        return result

    def import_classnames(self, text: str, defaults: Optional[dict] = None) -> List[str]:
        """
        Create type entries from a blob of class names. Skips names that already exist.
        
        Args:
            text: Text containing class names (comma, space, or newline separated)
            defaults: Optional default economic values for new entries
            
        Returns:
            List of names that were ADDED (existing names are skipped)
        """
        defaults = defaults or {}
        added: List[str] = []
        for name in self._parse_classnames(text):
            if self.get_type(name) is not None:
                continue
            entry = TypeEntry(
                name=name,
                nominal=defaults.get("nominal", 0),
                min=defaults.get("min", 0),
                lifetime=defaults.get("lifetime", 0),
                restock=defaults.get("restock", 0),
            )
            self.set_type(entry)
            added.append(name)
        return added
    
    def backup_types(self) -> Optional[Path]:
        """
        Create a timestamped backup of the current types.xml.
        
        Backups are stored in the same directory with suffix .backup_<timestamp>.
        Prunes to keep only the newest 20 backups.
        
        Returns:
            Path to backup file, or None if backup failed
        """
        import time
        
        if not self._doc or not self.doc.root:
            return None
        
        try:
            timestamp = time.strftime("%Y%m%d_%H%M%S")
            original_path = getattr(self, '_last_loaded_path', None)
            
            # Use dedicated hidden backup folder for unambiguous pruning
            orig_path_obj = Path(original_path) if original_path else None
            backup_dir = (orig_path_obj.parent / ".dcm-types-backups") if orig_path_obj else Path(tempfile.gettempdir()) / "dayz_types_backups"
            backup_dir.mkdir(exist_ok=True)
            
            # Ensure a unique filename even for rapid successive backups
            # (second-resolution timestamps would otherwise collide/overwrite).
            backup_path = backup_dir / f"types_{timestamp}.xml"
            counter = 1
            while backup_path.exists():
                backup_path = backup_dir / f"types_{timestamp}_{counter:03d}.xml"
                counter += 1
            
            xml_str = self.to_xml()
            with open(backup_path, 'w', encoding='utf-8') as f:
                f.write(xml_str)
            
            # Prune to newest 20 backups
            all_backups = sorted(backup_dir.glob("types_*.xml"))
            for old in all_backups[:-20]:
                try:
                    old.unlink()
                except OSError:
                    pass
            
            return backup_path
        except (IOError, OSError, ValueError):
            return None
    
    def list_backups(self) -> List[Path]:
        """Return backup paths, newest first."""
        
        original_path = getattr(self, '_last_loaded_path', None)
        
        # Use dedicated hidden backup folder
        if original_path:
            orig_path_obj = Path(original_path)
            backup_dir = orig_path_obj.parent / ".dcm-types-backups"
        else:
            backup_dir = Path(tempfile.gettempdir()) / "dayz_types_backups"
        
        if not backup_dir.exists():
            return []
        
        backups = sorted(backup_dir.glob("types_*.xml"), reverse=True)
        return backups
    
    def restore_backup(self, backup_path: Path) -> bool:
        """
        Restore types.xml from a backup file.
        
        Snapshots the current state before overwriting so restore is undoable.
        
        Args:
            backup_path: Path to the backup .xml file
            
        Returns:
            True if successful, False otherwise
        """
        try:
            self.backup_types()  # Snapshot current so restore is undoable
            
            with open(backup_path, 'r', encoding='utf-8') as f:
                content = f.read()
            new_types = TypesXml.parse(content)
            
            # Update our document
            self._doc = new_types._doc
            self._types.clear()
            for k, v in new_types.get_all_types().items():
                self._types[k] = v
            
            return True
        except OSError:
            return False
        except ValueError:
            return False
    
    def batch_add_flag(self, class_names: List[str], flag_kind: str, flag_value: str) -> Dict[str, bool]:
        """
        Add a usage/value/tag to existing flags for given classes.

        Args:
            class_names: List of type names to modify
            flag_kind: One of 'usage', 'value'
            flag_value: The flag value to add

        Returns:
            Dict mapping class name to success status
        """
        results = {}

        for name in class_names:
            entry = self.get_type(name)
            if entry is None:
                results[name] = False
                continue

            # Add flag if not already present (deduplication)
            if flag_kind == 'usage':
                existing = [u.name for u in entry.usages]
                if flag_value not in existing:
                    entry.usages.append(Usage(name=flag_value))
                    # Update XML with DayZ format: <usage name="..."/>
                    root = self.doc.root
                    if root:
                        for elem in root.children:
                            type_name = elem.attr("name")
                            if elem.tag == "type" and type_name and type_name.lower() == name.lower():
                                usage_elem = XElement("usage", name=flag_value, value="")
                                elem.add_child(usage_elem)
            elif flag_kind == 'value':
                existing = [v.name for v in entry.values]
                if flag_value not in existing:
                    entry.values.append(Value(name=flag_value))
                    # Update XML with DayZ format: <value name="..."/>
                    root = self.doc.root
                    if root:
                        for elem in root.children:
                            type_name = elem.attr("name")
                            if elem.tag == "type" and type_name and type_name.lower() == name.lower():
                                value_elem = XElement("value", name=flag_value, value="")
                                elem.add_child(value_elem)

            self.set_type(entry)
            results[name] = True

        return results
    
    def to_xml(self) -> str:
        """Convert back to XML string."""
        return CeXml.serialize(self.doc)
    
    def save(self, filepath: str) -> bool:
        """
        Save the types.xml file.
        
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