# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Parser module for DayZ mod configuration files.

Supports parsing:
- config.cpp (DayZ mod format)
- JSON files
- XML files

Extracts settings from config classes and generates SettingField objects
for interactive editing.
"""

import re
import json
import xml.etree.ElementTree as ET
from typing import Dict, List, Optional, Any, Tuple
from enum import Enum


# Maximum visible length for truncated string displays (chars)
_JSON_TRUNCATE_MAX = 100


def _truncate_json(raw: str, max_len: int = _JSON_TRUNCATE_MAX) -> str:
    """Return a short display prefix of *raw* JSON with an ellipsis.

    Examples::

        >>> _truncate_json('["a","b","c"]')
        '["a", "b", ... (500 more chars)]'
    """
    if len(raw) <= max_len:
        return raw
    # Show the first chunk and how many chars were cut.
    show = raw[:max_len]
    # Break at a convenient boundary (comma or whitespace) so it looks tidy.
    last_comma = show.rfind(",")
    if last_comma > max_len * 3 // 4:
        show = show[:last_comma + 1]
    remaining = len(raw) - len(show)
    return f"{show.strip()} ... ({remaining} more chars)"


class SettingType(Enum):
    """Types of configuration settings."""
    INTEGER = "integer"
    FLOAT = "float"
    BOOLEAN = "boolean"
    STRING = "string"
    SELECT = "select"  # Dropdown from defined values
    COLOR = "color"
    ARRAY = "array"
    LONG_STRING = "long_string"  # Long string needing a wider / expandable box
    ARRAY_SUMMARY = "array_summary"  # Holds the full JSON blob for arrays-of-dicts


class SettingField:
    """
    Represents a single configuration setting field.
    
    Attributes:
        name: The setting key/name
        value: Current value (will be converted to appropriate type)
        default_value: Default value if not set
        min_val: Minimum numeric value
        max_val: Maximum numeric value
        step: Step size for increments (for sliders)
        setting_type: Type of the setting
        description: Human-readable description
        valid_values: List of valid string values (for select fields)
        depends_on: Optional field name this setting depends on
    """

    def __init__(
        self,
        name: str,
        value: Any = None,
        default_value: Any = None,
        min_val: Optional[float] = None,
        max_val: Optional[float] = None,
        step: float = 1.0,
        setting_type: SettingType = SettingType.STRING,
        description: str = "",
        valid_values: Optional[List[str]] = None,
        depends_on: Optional[str] = None,
        display_value: Optional[str] = None,
    ):
        self.name = name
        self.value = value
        self.default_value = default_value
        self.min_val = min_val
        self.max_val = max_val
        self.min_value = min_val  # alias for app.py compatibility
        self.max_value = max_val  # alias for app.py compatibility
        self.step = step
        self.setting_type = setting_type
        self.description = description
        self.valid_values = valid_values or []
        self.depends_on = depends_on
        # display_value is used for truncated previews of long strings
        self.display_value = display_value

    def to_dict(self) -> Dict[str, Any]:
        """Convert field to dictionary representation."""
        return {
            "name": self.name,
            "value": self.value,
            "default_value": self.default_value,
            "min_val": self.min_val,
            "max_val": self.max_val,
            "step": self.step,
            "setting_type": self.setting_type.value,
            "description": self.description,
            "valid_values": self.valid_values,
        }
    
    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> 'SettingField':
        """Create field from dictionary representation."""
        return cls(
            name=data["name"],
            value=data.get("value"),
            default_value=data.get("default_value"),
            min_val=data.get("min_val"),
            max_val=data.get("max_val"),
            step=data.get("step", 1.0),
            setting_type=SettingType(data.get("setting_type", "string")),
            description=data.get("description", ""),
            valid_values=data.get("valid_values", []),
        )


class ConfigParser:
    """
    Parser for DayZ configuration files.
    
    Supports multiple formats and automatically detects the type
    of each setting for interactive editing.
    """
    
    def __init__(self):
        self.settings: List[SettingField] = []
        self.raw_data: Dict[str, Any] = {}
    
    def parse_file(self, filepath: str) -> List[SettingField]:
        """
        Parse a configuration file and return settings list.
        
        Args:
            filepath: Path to the config file
            
        Returns:
            List of SettingField objects
        """
        self.settings = []
        
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read()
            
            # Detect format based on extension and content
            ext = filepath.lower().split('.')[-1]
            
            if ext in ('cpp',):
                return self._parse_cpp(content)
            elif ext == 'json':
                return self._parse_json(content, filepath)
            elif ext == 'xml':
                return self._parse_xml(content, filepath)
            else:
                # Try to auto-detect based on content
                if '{' in content or '[' in content:
                    return self._try_parse_as_config_cpp(content)
                return [SettingField("content", content, setting_type=SettingType.STRING)]
                
        except Exception as e:
            return [SettingField("_error", str(e), setting_type=SettingType.STRING)]
    
    def _parse_cpp(self, content: str) -> List[SettingField]:
        """Parse config.cpp format."""
        settings = []
        
        # Find class definitions like "class ClassName { ... }"
        class_pattern = r'class\s+(\w+)\s*\{([^}]+)\}'
        
        for match in re.finditer(class_pattern, content, re.DOTALL):
            class_name = match.group(1)
            class_content = match.group(2)
            
            # Extract settings from within the class
            setting_settings = self._extract_cpp_settings(class_content, class_name)
            settings.extend(setting_settings)
        
        return settings
    
    def _extract_cpp_settings(self, content: str, context: str = "") -> List[SettingField]:
        """Extract individual settings from config.cpp content."""
        settings = []
        
        # Patterns for different types of settings
        int_pattern = r'(\w+)\s*=\s*(-?\d+);'
        float_pattern = r'(\w+)\s*=\s*(-?\d+\.\d+);'
        bool_pattern = r'(\w+)\s*=\s*(0|1);'
        string_pattern = r'(\w+)\s*=\s*"([^"]*)";'
        
        # Extract integers
        for match in re.finditer(int_pattern, content):
            name = match.group(1)
            value = int(match.group(2))
            settings.append(SettingField(
                name=name,
                value=value,
                min_val=-10000 if "time" not in name.lower() else 0,
                max_val=10000 if "time" not in name.lower() else 86400,
                setting_type=SettingType.INTEGER
            ))
        
        # Extract floats
        for match in re.finditer(float_pattern, content):
            name = match.group(1)
            value = float(match.group(2))
            settings.append(SettingField(
                name=name,
                value=value,
                min_val=0.0,
                max_val=100.0,
                step=0.1,
                setting_type=SettingType.FLOAT
            ))
        
        # Extract booleans (0/1)
        for match in re.finditer(bool_pattern, content):
            name = match.group(1)
            value = bool(int(match.group(2)))
            settings.append(SettingField(
                name=name,
                value=value,
                setting_type=SettingType.BOOLEAN
            ))
        
        # Extract strings
        for match in re.finditer(string_pattern, content):
            name = match.group(1)
            value = match.group(2)
            
            # Check if this might be an enum/selection
            valid_values = self._detect_valid_values(content, name)
            if valid_values:
                settings.append(SettingField(
                    name=name,
                    value=value,
                    valid_values=valid_values,
                    setting_type=SettingType.SELECT
                ))
            else:
                settings.append(SettingField(
                    name=name,
                    value=value,
                    setting_type=SettingType.STRING
                ))
        
        # Process arrays like "items[] = { ... }"
        array_pattern = r'(\w+)\s*\[\]\s*=\s*\{([^}]*)\};'
        for match in re.finditer(array_pattern, content):
            name = match.group(1)
            items_str = match.group(2)
            
            # Parse array items
            items = []
            for item_match in re.finditer(r'"([^"]*)"', items_str):
                items.append(item_match.group(1))
            
            if not items:
                for item_match in re.finditer(r'(\w+)', items_str):
                    if item_match.group(1) not in ('items', '=', '{', '}', ';'):
                        items.append(item_match.group(1))
            
            settings.append(SettingField(
                name=name,
                value=items if items else "",
                valid_values=items if items else [],
                setting_type=SettingType.ARRAY
            ))
        
        # Process nested classes recursively
        nested_class_pattern = r'class\s+(\w+)\s*\{([^}]+(?:\{[^{}]*\}[^{}]*)*)\}'
        for match in re.finditer(nested_class_pattern, content):
            nested_name = match.group(1)
            nested_content = match.group(2)
            
            # Generate description from class name
            nested_settings = self._extract_cpp_settings(nested_content, f"{context}.{nested_name}")
            settings.extend(nested_settings)
        
        return settings
    
    def _parse_json(self, content: str, filepath: str) -> List[SettingField]:
        """Parse JSON configuration file."""
        settings = []
        
        try:
            data = json.loads(content)
            self.raw_data = data
            self._extract_json_settings(data, "", settings)
        except json.JSONDecodeError as e:
            settings.append(SettingField("_error", f"JSON parse error: {e}", setting_type=SettingType.STRING))
        
        return settings
    
    def _extract_json_settings(
        self,
        data: Any,
        prefix: str,
        settings: List[SettingField]
    ) -> None:
        """Recursively extract settings from JSON data."""
        if isinstance(data, dict):
            for key, value in data.items():
                full_name = f"{prefix}.{key}" if prefix else key
                
                if isinstance(value, bool):
                    settings.append(SettingField(
                        name=full_name,
                        value=value,
                        setting_type=SettingType.BOOLEAN
                    ))
                elif isinstance(value, int):
                    settings.append(SettingField(
                        name=full_name,
                        value=value,
                        min_val=-2147483648,
                        max_val=2147483647,
                        setting_type=SettingType.INTEGER
                    ))
                elif isinstance(value, float):
                    settings.append(SettingField(
                        name=full_name,
                        value=value,
                        min_val=0.0,
                        max_val=1000000.0,
                        step=0.1,
                        setting_type=SettingType.FLOAT
                    ))
                elif isinstance(value, str):
                    # Check if this might be a color (hex format)
                    if re.match(r'^#?[0-9A-Fa-f]{6}$', value) or re.match(r'^#?[0-9A-Fa-f]{8}$', value):
                        settings.append(SettingField(
                            name=full_name,
                            value=value,
                            setting_type=SettingType.COLOR
                        ))
                    else:
                        settings.append(SettingField(
                            name=full_name,
                            value=value,
                            setting_type=SettingType.STRING
                        ))
                elif isinstance(value, list):
                    # Handle arrays
                    if value and all(isinstance(x, str) for x in value):
                        settings.append(SettingField(
                            name=full_name,
                            value=value,
                            valid_values=value,
                            setting_type=SettingType.ARRAY
                        ))
                    elif value and all(isinstance(x, dict) for x in value):
                        # Arrays of objects: expand each object as flattened sub-fields
                        self._extract_array_of_dicts(value, full_name, settings)
                    else:
                        # Mixed / nested arrays: store as truncated JSON string
                        raw = json.dumps(value, ensure_ascii=False)
                        settings.append(SettingField(
                            name=full_name,
                            value=value,  # keep original list for saving
                            display_value=_truncate_json(raw),
                            setting_type=SettingType.LONG_STRING,
                        ))
                elif isinstance(value, dict):
                    self._extract_json_settings(value, full_name, settings)

    def _extract_array_of_dicts(
        self,
        items: List[Dict[str, Any]],
        prefix: str,
        settings: List[SettingField],
    ) -> None:
        """Expand an array of dicts into flattened dotted sub-fields.

        Example for ``RoamingLocations`` with two entries:
            RoamingLocations[0].Name       = "Bielawa"
            RoamingLocations[0].Position   = [1573, 0, 9677]
            RoamingLocations[0].Radius     = 200.0
            RoamingLocations[0].Type       = "Village"
            RoamingLocations[0].Enabled    = 1
            RoamingLocations[1].Name       = "Lukow"
            ...

        Each primitive leaf becomes its own SettingField so the UI can edit
        individual values without showing a hundreds-of-characters string.
        """
        for idx, item in enumerate(items):
            for key, val in item.items():
                name = f"{prefix}[{idx}].{key}"
                if isinstance(val, bool):
                    settings.append(SettingField(name=name, value=val, setting_type=SettingType.BOOLEAN))
                elif isinstance(val, int):
                    settings.append(SettingField(name=name, value=val, setting_type=SettingType.INTEGER))
                elif isinstance(val, float):
                    settings.append(SettingField(name=name, value=val, setting_type=SettingType.FLOAT))
                elif isinstance(val, str):
                    settings.append(SettingField(name=name, value=val, setting_type=SettingType.STRING))
                elif isinstance(val, list):
                    # Position arrays or other sub-arrays stay as a compact list
                    raw = json.dumps(val, ensure_ascii=False)
                    if len(raw) > 120:
                        settings.append(SettingField(name=name, value=val, display_value=_truncate_json(raw), setting_type=SettingType.LONG_STRING))
                    else:
                        settings.append(SettingField(name=name, value=val, setting_type=SettingType.ARRAY))
                elif isinstance(val, dict):
                    self._extract_array_of_dicts([val], name, settings)

        # Add a "Summary" field that holds the full raw JSON for export,
        # marked so the UI knows to render it as an expandable textarea.
        raw = json.dumps(items, ensure_ascii=False, indent=2)
        settings.append(SettingField(
            name=f"{prefix}",  # top-level key (e.g. RoamingLocations)
            value=items,
            display_value=_truncate_json(raw),
            setting_type=SettingType.ARRAY_SUMMARY,
            description=f"Array of {len(items)} item(s). Each sub-field is editable above.",
        ))
        
    def _parse_xml(self, content: str, filepath: str) -> List[SettingField]:
        """Parse XML configuration file."""
        settings = []
        
        try:
            root = ET.fromstring(content)
            self.raw_data = ET.tostring(root, encoding='unicode')
            self._extract_xml_settings(root, "", settings)
        except ET.ParseError as e:
            settings.append(SettingField("_error", f"XML parse error: {e}", setting_type=SettingType.STRING))
        
        return settings
    
    def _extract_xml_settings(
        self,
        element: ET.Element,
        prefix: str,
        settings: List[SettingField]
    ) -> None:
        """Recursively extract settings from XML elements."""
        # Get attributes as settings
        for attr_name, attr_value in element.attrib.items():
            full_name = f"{prefix}.{attr_name}" if prefix else attr_name
            
            # Try to infer type from value
            if attr_value.lower() in ('true', 'false'):
                settings.append(SettingField(
                    name=full_name,
                    value=attr_value.lower() == 'true',
                    setting_type=SettingType.BOOLEAN
                ))
            elif re.match(r'^-?\d+$', attr_value):
                settings.append(SettingField(
                    name=full_name,
                    value=int(attr_value),
                    min_val=-2147483648,
                    max_val=2147483647,
                    setting_type=SettingType.INTEGER
                ))
            elif re.match(r'^-?\d+\.\d+$', attr_value):
                settings.append(SettingField(
                    name=full_name,
                    value=float(attr_value),
                    min_val=0.0,
                    max_val=1000000.0,
                    step=0.1,
                    setting_type=SettingType.FLOAT
                ))
            elif re.match(r'^#?[0-9A-Fa-f]{6}$', attr_value):
                settings.append(SettingField(
                    name=full_name,
                    value=attr_value,
                    setting_type=SettingType.COLOR
                ))
            else:
                settings.append(SettingField(
                    name=full_name,
                    value=attr_value,
                    setting_type=SettingType.STRING
                ))
        
        # Process child elements
        for child in element:
            self._extract_xml_settings(child, f"{prefix}.{child.tag}", settings)
    
    def _try_parse_as_config_cpp(self, content: str) -> List[SettingField]:
        """Try to parse as config.cpp even if extension suggests otherwise."""
        return self._parse_cpp(content)
    
    def _detect_valid_values(self, content: str, field_name: str) -> Optional[List[str]]:
        """
        Try to detect valid string values for a field (enum-like detection).
        
        Looks for common patterns like:
        - Multiple quoted strings in nearby context
        - Field names with "Mode", "Type", "State" etc.
        """
        # Common enum indicators in DayZ configs
        enum_indicators = [
            'mode', 'type', 'state', 'class', 'level',
            'category', 'action', 'behavior', 'style'
        ]
        
        if not any(indicator in field_name.lower() for indicator in enum_indicators):
            return None
        
        # Look for quoted strings near this field
        pattern = rf'(\w+)\s*=\s*"([^"]*)";'
        candidates = []
        
        for match in re.finditer(pattern, content):
            if "mode" not in match.group(1).lower():
                continue
            candidates.append(match.group(2))
        
        # If we found multiple candidates with similar pattern, treat as valid values
        if len(candidates) >= 2:
            return list(dict.fromkeys(candidates))  # Remove duplicates
        
        return None
    
    def get_settings_by_category(self) -> Dict[str, List[SettingField]]:
        """
        Group settings by inferred category based on name.
        
        Returns:
            Dictionary mapping category names to lists of settings
        """
        categories: Dict[str, List[SettingField]] = {}
        
        for setting in self.settings:
            # Infer category from field name
            parts = setting.name.lower().split('.')
            if len(parts) > 1:
                category = parts[-2].replace('_', ' ').title()
            elif '_' in setting.name:
                category = setting.name.split('_')[0].title()
            else:
                category = "General"
            
            if category not in categories:
                categories[category] = []
            categories[category].append(setting)
        
        return categories