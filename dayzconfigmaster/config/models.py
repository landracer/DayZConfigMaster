# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Data models for DayZ configuration files

"""
Data models providing structured representation of DayZ server configuration types.

Each model provides:
- Field definitions with validation rules
- Serialization/deserialization to/from config file formats
- Default values and type hints
"""

from dataclasses import dataclass, field, asdict
from typing import Dict, List, Any, Optional, Union
import json


@dataclass
class ConfigField:
    """Represents a single configuration field with validation rules"""
    name: str
    value: Any = None
    default: Any = None
    min_val: Optional[Union[int, float]] = None
    max_val: Optional[Union[int, float]] = None
    valid_values: Optional[List[str]] = None
    format: Optional[str] = None
    description: str = ""
    
    def validate(self) -> tuple:
        """Validate field value and return (is_valid, message)"""
        if self.value is None or self.value == "":
            return True, None
        
        # Check valid values
        if self.valid_values and isinstance(self.value, str):
            if self.value not in self.valid_values:
                return False, f"Value must be one of: {', '.join(self.valid_values)}"
        
        # Check numeric ranges
        try:
            num_val = float(self.value)
            if self.min_val is not None and num_val < self.min_val:
                return False, f"Below minimum ({self.min_val})"
            if self.max_val is not None and num_val > self.max_val:
                return False, f"Above maximum ({self.max_val})"
        except (ValueError, TypeError):
            if self.value:  # Only error if not empty
                return False, "Must be a number"
        
        return True, None


@dataclass
class EconomyConfig:
    """economycore.xml - Core economy configuration"""
    classes: Dict[str, int] = field(default_factory=lambda: {
        "Item": 1,
        "Weapon": 2,
        "Vehicle": 3,
        "Clothing": 4,
        "Food": 5,
        "Tool": 6
    })
    
    defaults: Dict[str, str] = field(default_factory=lambda: {
        "max_items": "1000",
        "max_weapons": "500",
        "max_vehicles": "100"
    })
    
    def to_xml(self) -> str:
        """Convert to XML format"""
        xml_lines = ['<?xml version="1.0" encoding="UTF-8"?>']
        xml_lines.append('<economy>')
        
        if self.classes:
            xml_lines.append('  <classes>')
            for name, type_id in self.classes.items():
                xml_lines.append(f'    <class name="{name}" value="{type_id}"/>')
            xml_lines.append('  </classes>')
        
        if self.defaults:
            xml_lines.append('  <defaults>')
            for key, val in self.defaults.items():
                xml_lines.append(f'    <default name="{key}" value="{val}"/>')
            xml_lines.append('  </defaults>')
        
        xml_lines.append('</economy>')
        return '\n'.join(xml_lines)
    
    @classmethod
    def from_xml(cls, xml_content: str) -> 'EconomyConfig':
        """Parse XML content into EconomyConfig"""
        import re
        
        classes = {}
        defaults = {}
        
        # Parse classes
        class_pattern = r'<class\s+name="([^"]+)"\s+value="([^"]+)"/>'
        for match in re.finditer(class_pattern, xml_content):
            name, value = match.groups()
            try:
                classes[name] = int(value)
            except ValueError:
                classes[name] = 0
        
        # Parse defaults
        default_pattern = r'<default\s+name="([^"]+)"\s+value="([^"]+)"/>'
        for match in re.finditer(default_pattern, xml_content):
            name, value = match.groups()
            defaults[name] = value
        
        return cls(classes=classes, defaults=defaults)


@dataclass
class Category:
    """A limit category (e.g., weapons, tools, clothes)"""
    name: str
    description: str = ""
    limits: Dict[str, int] = field(default_factory=dict)


@dataclass  
class LimitsConfig:
    """limitsdefinitions.xml - Item and spawn limits configuration"""
    categories: List[Category] = field(default_factory=lambda: [
        Category("weapons", "Firearms, melee weapons"),
        Category("tools", "Axes, shovels, saws, knives"),
        Category("clothes", "Clothing, hats, gloves, shoes"),
        Category("containers", "Backpacks, cases, pouches"),
        Category("food", "Canned food, drinks, raw food"),
        Category("vehiclesparts", "Tires, spark plugs, batteries"),
        Category("explosives", "Grenades, mines, landmines")
    ])
    
    usages: Dict[str, str] = field(default_factory=lambda: {
        "Military": "Military bases, tents, barracks, checkpoints",
        "Police": "Police stations",
        "Hunting": "Hunting stands, lodges, deer stands",
        "Town": "Residential houses, apartments in towns",
        "Village": "Small rural settlements",
        "Farm": "Barns, farm buildings, silos",
        "Industrial": "Factories, warehouses, workshops",
        "Medic": "Hospitals, medical buildings",
        "Firefighter": "Fire stations",
        "School": "Schools, piano houses",
        "Office": "Office buildings",
        "Coast": "Coastal buildings, boats, lighthouses"
    })
    
    values: Dict[str, str] = field(default_factory=lambda: {
        "Tier1": "Coastal areas - Basic civilian loot",
        "Tier2": "Inland towns - Mid-range civilian and some military",
        "Tier3": "Deep inland / NWAF area - High-value military loot",
        "Tier4": "Contaminated / Tisy - Top-tier military, rare items"
    })
    
    def to_xml(self) -> str:
        """Convert to XML format"""
        xml_lines = ['<?xml version="1.0" encoding="UTF-8"?>']
        xml_lines.append('<limitsdefinitions>')
        
        # Categories
        xml_lines.append('  <categories>')
        for cat in self.categories:
            xml_lines.append(f'    <category name="{cat.name}" description="{cat.description}"/>')
        xml_lines.append('  </categories>')
        
        # Usages
        xml_lines.append('  <usages>')
        for usage, desc in self.usages.items():
            xml_lines.append(f'    <usage name="{usage}" description="{desc}"/>')
        xml_lines.append('  </usages>')
        
        # Values/Tiers
        xml_lines.append('  <values>')
        for value, desc in self.values.items():
            xml_lines.append(f'    <value name="{value}" description="{desc}"/>')
        xml_lines.append('  </values>')
        
        xml_lines.append('</limitsdefinitions>')
        return '\n'.join(xml_lines)
    
    @classmethod
    def from_xml(cls, xml_content: str) -> 'LimitsConfig':
        """Parse XML content into LimitsConfig"""
        import re
        
        categories = []
        usages = {}
        values = {}
        
        # Parse categories
        cat_pattern = r'<category\s+name="([^"]+)"\s+description="([^"]*)"/>'
        for match in re.finditer(cat_pattern, xml_content):
            name, desc = match.groups()
            categories.append(Category(name=name, description=desc))
        
        # Parse usages
        usage_pattern = r'<usage\s+name="([^"]+)"\s+description="([^"]*)"/>'
        for match in re.finditer(usage_pattern, xml_content):
            name, desc = match.groups()
            usages[name] = desc
        
        # Parse values
        value_pattern = r'<value\s+name="([^"]+)"\s+description="([^"]*)"/>'
        for match in re.finditer(value_pattern, xml_content):
            name, desc = match.groups()
            values[name] = desc
        
        return cls(categories=categories, usages=usages, values=values)


@dataclass
class RandomPreset:
    """A random preset with cargo items"""
    name: str
    cargo_count: int = 10
    attachments: int = 5
    min_items: int = 1
    max_items: int = 3


@dataclass
class RandomPresetConfig:
    """cfgrandompresets.xml - Random item presets configuration"""
    presets: List[RandomPreset] = field(default_factory=lambda: [
        RandomPreset("DefaultWeapon", cargo_count=10, attachments=5, min_items=1, max_items=3),
        RandomPreset("DefaultAmmo", cargo_count=20, attachments=0, min_items=1, max_items=5),
        RandomPreset("DefaultFood", cargo_count=15, attachments=3, min_items=1, max_items=4),
        RandomPreset("MilitaryPreset", cargo_count=25, attachments=8, min_items=2, max_items=6),
        RandomPreset("CivilianPreset", cargo_count=12, attachments=4, min_items=1, max_items=3)
    ])
    
    def to_xml(self) -> str:
        """Convert to XML format"""
        xml_lines = ['<?xml version="1.0" encoding="UTF-8"?>']
        xml_lines.append('<randompresets>')
        
        for preset in self.presets:
            xml_lines.append(f'  <preset name="{preset.name}">')
            xml_lines.append(f'    <cargo count="{preset.cargo_count}" attachments="{preset.attachments}"/>')
            xml_lines.append(f'    <range min="{preset.min_items}" max="{preset.max_items}"/>')
            xml_lines.append('  </preset>')
        
        xml_lines.append('</randompresets>')
        return '\n'.join(xml_lines)
    
    @classmethod
    def from_xml(cls, xml_content: str) -> 'RandomPresetConfig':
        """Parse XML content into RandomPresetConfig"""
        import re
        
        presets = []
        
        preset_pattern = r'<preset\s+name="([^"]+)">.*?<cargo\s+count="(\d+)"\s+attachments="(\d+)".*?<range\s+min="(\d+)"\s+max="(\d+)"/>'
        for match in re.finditer(preset_pattern, xml_content, re.DOTALL):
            name, cargo_count, attachments, min_items, max_items = match.groups()
            presets.append(RandomPreset(
                name=name,
                cargo_count=int(cargo_count),
                attachments=int(attachments),
                min_items=int(min_items),
                max_items=int(max_items)
            ))
        
        return cls(presets=presets)


@dataclass
class SpawnableType:
    """A spawnable item type with categories"""
    name: str
    category: str = ""
    usage: str = ""
    value: str = ""


@dataclass
class SpawnableTypeConfig:
    """cfgspawnabletypes.xml - Spawnable item types configuration"""
    spawnable_types: List[SpawnableType] = field(default_factory=lambda: [
        SpawnableType("AKM", category="weapons", usage="Military", value="Tier3"),
        SpawnableType("AmmoBox", category="tools", usage="Town", value="Tier2"),
        SpawnableType("Food", category="food", usage="Village", value="Tier1"),
        SpawnableType("Backpack", category="containers", usage="Town", value="Tier2"),
        SpawnableType("Medkit", category="tools", usage="Medic", value="Tier3")
    ])
    
    def to_xml(self) -> str:
        """Convert to XML format"""
        xml_lines = ['<?xml version="1.0" encoding="UTF-8"?>']
        xml_lines.append('<spawnabletypes>')
        
        for stype in self.spawnable_types:
            xml_lines.append(f'  <type name="{stype.name}">')
            if stype.category:
                xml_lines.append(f'    <category value="{stype.category}"/>')
            if stype.usage:
                xml_lines.append(f'    <usage name="{stype.usage}"/>')
            if stype.value:
                xml_lines.append(f'    <value name="{stype.value}"/>')
            xml_lines.append('  </type>')
        
        xml_lines.append('</spawnabletypes>')
        return '\n'.join(xml_lines)
    
    @classmethod
    def from_xml(cls, xml_content: str) -> 'SpawnableTypeConfig':
        """Parse XML content into SpawnableTypeConfig"""
        import re
        
        spawnable_types = []
        
        # Parse type blocks
        type_pattern = r'<type\s+name="([^"]+)">(.*?)</type>'
        for match in re.finditer(type_pattern, xml_content, re.DOTALL):
            name = match.group(1)
            block = match.group(2)
            
            category_match = re.search(r'<category\s+value="([^"]*)"/>', block)
            usage_match = re.search(r'<usage\s+name="([^"]*)"/>', block)
            value_match = re.search(r'<value\s+name="([^"]*)"/>', block)
            
            spawnable_types.append(SpawnableType(
                name=name,
                category=category_match.group(1) if category_match else "",
                usage=usage_match.group(1) if usage_match else "",
                value=value_match.group(1) if value_match else ""
            ))
        
        return cls(spawnable_types=spawnable_types)


@dataclass
class Trader:
    """A trader with items and settings"""
    name: str
    category: str = "General"
    items: List[str] = field(default_factory=list)
    min_distance: float = 50.0
    max_distance: float = 200.0
    can_be_robbed: bool = True
    price_multiplier: float = 1.0
    buy_price_modifier: float = -0.1
    sell_price_modifier: float = 0.1


@dataclass
class TraderConfig:
    """traderconfig.txt - Trader configurations"""
    traders: List[Trader] = field(default_factory=lambda: [
        Trader(
            name="General Store",
            category="General",
            items=["Food", "Weapons", "Ammo"],
            min_distance=50.0,
            max_distance=200.0
        ),
        Trader(
            name="Medical Supplies",
            category="Medical",
            items=["Medkit", "Painkillers", "Bandages"],
            min_distance=30.0,
            max_distance=150.0,
            price_multiplier=1.2
        ),
        Trader(
            name="Military Equipment",
            category="Military",
            items=["M4A1", "AKM", "Grenade"],
            min_distance=100.0,
            max_distance=500.0,
            can_be_robbed=False,
            price_multiplier=1.5
        )
    ])
    
    def to_txt(self) -> str:
        """Convert to TXT format (DayZ trader config style)"""
        lines = ["// Trader Configuration File"]
        lines.append(f"// Generated by DayzConfigMaster")
        lines.append("")
        
        for i, trader in enumerate(self.traders, 1):
            lines.append(f"Trader{i} {{")
            lines.append(f'    Name = "{trader.name}"')
            lines.append(f'    Category = "{trader.category}"')
            
            if trader.items:
                lines.append('    Items = [')
                for item in trader.items:
                    lines.append(f'        "{item}",')
                lines[-1] = lines[-1].rstrip(',')  # Remove trailing comma
                lines.append('    ]')
            
            lines.append(f'    MinDistance = {trader.min_distance}')
            lines.append(f'    MaxDistance = {trader.max_distance}')
            lines.append(f'    CanBeRobbed = {"true" if trader.can_be_robbed else "false"}')
            lines.append("")
            lines.append('    // DZconfig integration: Custom pricing multipliers')
            lines.append(f'    PriceMultiplier = {trader.price_multiplier}')
            lines.append(f'    BuyPriceModifier = {trader.buy_price_modifier}')
            lines.append(f'    SellPriceModifier = {trader.sell_price_modifier}')
            lines.append("}")
            lines.append("")
        
        return '\n'.join(lines)
    
    @classmethod
    def from_txt(cls, txt_content: str) -> 'TraderConfig':
        """Parse TXT content into TraderConfig"""
        import re
        
        traders = []
        
        # Parse trader blocks
        trader_pattern = r'Trader\d+\s*\{([^}]+)\}'
        for match in re.finditer(trader_pattern, txt_content, re.DOTALL):
            block = match.group(1)
            
            name_match = re.search(r'Name\s*=\s*"([^"]*)"', block)
            category_match = re.search(r'Category\s*=\s*"([^"]*)"', block)
            
            items = []
            items_match = re.search(r'Items\s*=\s*\[(.*?)\]', block, re.DOTALL)
            if items_match:
                item_text = items_match.group(1)
                items = [item.strip().strip('"') for item in item_text.split(',') if item.strip()]
            
            min_dist = float(re.search(r'MinDistance\s*=\s*(\d+\.?\d*)', block).group(1))
            max_dist = float(re.search(r'MaxDistance\s*=\s*(\d+\.?\d*)', block).group(1))
            can_robbed = r'CanBeRobbed\s*=\s*true' in block
            price_mult = float(re.search(r'PriceMultiplier\s*=\s*(\d+\.?\d*)', block).group(1))
            
            traders.append(Trader(
                name=name_match.group(1) if name_match else "Unknown",
                category=category_match.group(1) if category_match else "General",
                items=items,
                min_distance=min_dist,
                max_distance=max_dist,
                can_be_robbed=can_robbed,
                price_multiplier=price_mult
            ))
        
        return cls(traders=traders)


@dataclass
class ServerConfig:
    """serverDZ.cfg - Main server configuration"""
    # Basic settings
    name: str = "DayZ Server"
    port: int = 2302
    query_port: int = 2303
    max_players: int = 60
    password: str = ""
    password_admin: str = ""
    description: str = ""
    game_mode: str = "Survival"

    # Map settings
    map_size: int = 2000
    map_name: str = ""

    # Time and weather
    time_settings: str = "2020/4/1/08/00"  # YYYY/M/D/Hour/Minute
    server_time: str = "SystemTime"
    server_time_acceleration: float = 1.0
    server_night_time_acceleration: float = 1.0
    server_time_persistent: int = 0
    weather: float = 0.5  # 0-1
    fog_density: float = 0.5
    rain_intensity: float = 0.8
    wind_speed: float = 5.0
    temperature: float = 20.0

    # Voice & view
    disable_von: int = 0
    von_codec_quality: int = 20
    disable_3rd_person: int = 0
    disable_crosshair: int = 0
    disable_personal_light: int = 1
    lighting_config: int = 0
    shard_id: str = ""

    # Network
    guaranteed_updates: int = 1
    login_queue_concurrent_players: int = 5
    login_queue_max_players: int = 500

    # Instance & storage
    instance_id: int = 1
    storage_auto_fix: int = 1

    # Multipliers
    base_damage_multiplier: float = 1.0
    hit_point_multiplier: float = 1.0
    loot_spawn_multiplier: float = 1.0
    zombie_spawn_multiplier: float = 1.0
    vehicle_spawn_multiplier: float = 1.0

    # Item decay
    item_decay_time: int = 3600  # seconds

    # Whitelist
    enable_whitelist: int = 0

    # Mod settings
    verify_signatures: int = 2  # 0=Off, 1=Debug, 2=Verify
    force_same_build: int = 1   # 0=No, 1=Yes
    mod_paths: str = "@CF,@MasPuertas"
    
    def to_cfg(self) -> str:
        """Convert to serverDZ.cfg format using the canonical generator."""
        from .cfg_generator import generate_server_cfg
        return generate_server_cfg(
            hostname=self.name,
            port=self.port,
            query_port=self.query_port,
            max_players=self.max_players,
            password=self.password,
            password_admin=self.password_admin,
            description=self.description,
            enable_whitelist=self.enable_whitelist,
            server_time=self.server_time,
            server_time_acceleration=self.server_time_acceleration,
            server_night_time_acceleration=self.server_night_time_acceleration,
            server_time_persistent=self.server_time_persistent,
            disable_von=self.disable_von,
            von_codec_quality=self.von_codec_quality,
            disable_3rd_person=self.disable_3rd_person,
            disable_crosshair=self.disable_crosshair,
            disable_personal_light=self.disable_personal_light,
            lighting_config=self.lighting_config,
            shard_id=self.shard_id,
            guaranteed_updates=self.guaranteed_updates,
            login_queue_concurrent_players=self.login_queue_concurrent_players,
            login_queue_max_players=self.login_queue_max_players,
            instance_id=self.instance_id,
            storage_auto_fix=self.storage_auto_fix,
            verify_signatures=self.verify_signatures,
            force_same_build=self.force_same_build,
            mod_paths=self.mod_paths,
            map_name=self.map_name,
        )
    
    @classmethod
    def from_cfg(cls, cfg_content: str) -> 'ServerConfig':
        """Parse serverDZ.cfg content into ServerConfig"""
        import re
        
        config = cls()
        
        # Map CFG field names to Python attribute names
        field_mapping = {
            'hostname': 'name',
            'queryport': 'query_port',
            'maxplayers': 'max_players',
            'passwordadmin': 'password_admin',
            'enablewhitelist': 'enable_whitelist',
            'servertime': 'server_time',
            'servertimeacceleration': 'server_time_acceleration',
            'servernighttimeacceleration': 'server_night_time_acceleration',
            'servertimepersistent': 'server_time_persistent',
            'disablevon': 'disable_von',
            'voncodecquality': 'von_codec_quality',
            'disable3rdperson': 'disable_3rd_person',
            'disablecrosshair': 'disable_crosshair',
            'disablepersonallight': 'disable_personal_light',
            'lightingconfig': 'lighting_config',
            'shardid': 'shard_id',
            'guaranteedupdates': 'guaranteed_updates',
            'loginqueueconcurrentplayers': 'login_queue_concurrent_players',
            'loginqueuemaxplayers': 'login_queue_max_players',
            'instanceid': 'instance_id',
            'storageautofix': 'storage_auto_fix',
            'verifysignatures': 'verify_signatures',
            'forcesamebuild': 'force_same_build',
            'mod': 'mod_paths',
            'template': 'map_name',
        }

        def _set_attr(key, value):
            python_key = field_mapping.get(key.lower(), key.lower())
            if not hasattr(config, python_key):
                return
            current_val = getattr(config, python_key)
            if isinstance(current_val, bool):
                setattr(config, python_key, bool(int(value)))
            elif isinstance(current_val, int):
                setattr(config, python_key, int(value))
            elif isinstance(current_val, float):
                setattr(config, python_key, float(value))
            else:
                setattr(config, python_key, value)
        
        # Parse string values with quotes (hostname = "value";)
        str_pattern = r'(\w+)\s*=\s*"([^"]*)";'
        for match in re.finditer(str_pattern, cfg_content):
            key, value = match.groups()
            _set_attr(key, value)

        # Parse numeric values (port = 2302; weather = 0.5;)
        num_pattern = r'(\w+)\s*=\s*([\d.]+);'
        for match in re.finditer(num_pattern, cfg_content):
            key, value = match.groups()
            _set_attr(key, value)

        # Extract map name from class Missions block if present
        mission_match = re.search(r'class\s+Missions\s*\{[^}]*class\s+DayZ\s*\{\s*template\s*=\s*"([^"]+)"', cfg_content, re.DOTALL)
        if mission_match:
            template = mission_match.group(1)
            if template.startswith("dayzOffline."):
                config.map_name = template.split("dayzOffline.", 1)[1]
            else:
                config.map_name = template

        return config