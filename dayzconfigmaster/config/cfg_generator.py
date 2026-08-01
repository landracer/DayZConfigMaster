# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
serverDZ.cfg generator for DayZConfigMaster.

Provides helpers to generate complete, DayZ-compliant serverDZ.cfg content
from the UI state, including support for multi-instance configurations.

The generated file includes every standard serverDZ.cfg option used by the
DayZ dedicated server executable. Optional values are only emitted when the
caller supplies a non-empty value so the output stays clean for simple setups.
"""

from datetime import datetime
from pathlib import Path
from typing import Dict, List, Any, Optional


def _normalize_verify_signatures(value: str) -> str:
    """Extract numeric verifySignatures value from UI string."""
    if value is None:
        return "2"
    value = str(value).strip()
    if value.startswith("0"):
        return "0"
    if value.startswith("1"):
        return "1"
    if value.startswith("2"):
        return "2"
    return value or "2"


def _normalize_force_same_build(value: str) -> str:
    """Extract numeric forceSameBuild value from UI string."""
    if value is None:
        return "1"
    value = str(value).strip()
    if value.startswith("0"):
        return "0"
    if value.startswith("1"):
        return "1"
    return value or "1"


def _normalize_binary(value: Any, default: str = "0") -> str:
    """Normalize a 0/1 toggle to a string."""
    if value is None:
        return default
    value = str(value).strip()
    if not value:
        return default
    if value[0] == "1" or value.lower() in ("true", "yes", "on"):
        return "1"
    if value[0] == "0" or value.lower() in ("false", "no", "off"):
        return "0"
    return default


def _normalize_int(value: Any, default: int = 0) -> int:
    """Normalize an integer value that may come from a UI combo string."""
    if value is None:
        return default
    value = str(value).strip()
    if not value:
        return default
    # UI combos often prefix the number, e.g. '0 (Bright)', '1 (Dark)'
    first = value.split()[0]
    try:
        return int(first)
    except ValueError:
        return default


def _get_ui_value(value: Any, default: str = "") -> str:
    """Return a string from a Tkinter variable or a plain string."""
    if value is None:
        return default
    if callable(getattr(value, "get", None)):
        result = value.get()
        return str(result) if result is not None else default
    return str(value) if value != "" else default


def _get_ui_value_optional(value: Any, default: Optional[str] = None) -> Optional[str]:
    """Return a string or None from a Tkinter variable or plain string."""
    if value is None:
        return default
    if callable(getattr(value, "get", None)):
        result = value.get()
        return str(result) if result else default
    result = str(value)
    return result if result else default


def _format_mod_paths(mod_paths: str) -> str:
    """Normalize mod path string to semicolon-separated entries."""
    if not mod_paths:
        return ""
    # Support both semicolon and comma separators
    raw = mod_paths.replace(",", ";")
    parts = [p.strip() for p in raw.split(";") if p.strip()]
    normalized = []
    for part in parts:
        # Preserve full absolute paths and steam:// URLs as-is.
        if Path(part).is_absolute() or part.startswith("steam://"):
            normalized.append(part)
        elif not part.startswith("@"):
            normalized.append("@" + part)
        else:
            normalized.append(part)
    return ";".join(normalized)


def _mission_template(map_name: str) -> str:
    """Build the Missions class block for the selected map."""
    if not map_name:
        map_name = "chernarusplus"
    # If the user already supplied dayzOffline.*, use it directly.
    if "." in map_name and map_name.startswith("dayzOffline."):
        template = map_name
    else:
        template = f"dayzOffline.{map_name}"
    return (
        "\nclass Missions\n"
        "{\n"
        "    class DayZ\n"
        "    {\n"
        f'        template="{template}"; // Mission to load on server startup\n'
        "    };\n"
        "};\n"
    )


def generate_server_cfg(
    name: str = "DayZ Server",
    port: int = 2302,
    query_port: int = 2303,
    max_players: int = 60,
    password: str = "",
    password_admin: str = "",
    description: str = "",
    enable_whitelist: str = "0",
    map_name: str = "",
    map_size: int = 2000,
    server_time: str = "SystemTime",
    server_time_acceleration: float = 1.0,
    server_night_time_acceleration: float = 1.0,
    server_time_persistent: str = "0",
    disable_von: str = "0",
    von_codec_quality: int = 20,
    disable_3rd_person: str = "0",
    disable_crosshair: str = "0",
    disable_personal_light: str = "1",
    lighting_config: int = 0,
    shard_id: str = "",
    guaranteed_updates: int = 1,
    login_queue_concurrent_players: int = 5,
    login_queue_max_players: int = 500,
    instance_id: int = 1,
    storage_auto_fix: str = "1",
    verify_signatures: str = "2 (Verify)",
    force_same_build: str = "1 (Yes)",
    mod_paths: str = "",
    admin_logs: str = "",
    network_logs: str = "",
    memory_limit: str = "",
    thread_count: str = "",
    update_settings: str = "",
    profiles_path: str = "",
    game_mode: str = "Survival",
    time_settings: str = "",
    weather: str = "",
    fog_density: float = 0.5,
    rain_intensity: float = 0.8,
    wind_speed: float = 5.0,
    temperature: float = 20.0,
    base_damage_multiplier: float = 1.0,
    hit_point_multiplier: float = 1.0,
    loot_spawn_multiplier: float = 1.0,
    zombie_spawn_multiplier: float = 1.0,
    vehicle_spawn_multiplier: float = 1.0,
    item_decay_time: int = 3600,
) -> str:
    """
    Generate a complete serverDZ.cfg content string.

    All parameters mirror the standard DayZ dedicated server configuration
    options. Optional values are only written when provided.
    """
    lines = [
        "// DayZ Server Configuration (serverDZ.cfg)",
        "// Generated by DayzConfigMaster",
        f"// Generated: {datetime.now().isoformat()}",
        "",
        "// Basic Server Settings",
        f'hostname = "{name}";        // Server name',
    ]

    if password:
        lines.append(f'password = "{password}";        // Password to connect to the server')
    if password_admin:
        lines.append(f'passwordAdmin = "{password_admin}";        // Password to become a server admin')
    if description:
        lines.append(f'description = "{description}";        // Description displayed in the server browser')

    lines.append(f"enableWhitelist = {_normalize_binary(enable_whitelist, '0')};        // Enable/disable whitelist (value 0-1)")
    lines.append(f"maxPlayers = {max_players};            // Maximum amount of players")
    lines.append("")
    lines.append("// Security & Build")
    lines.append(f"verifySignatures = {_normalize_verify_signatures(verify_signatures)};       // Verifies .pbos against .bisign files (only 2 is supported)")
    lines.append(f"forceSameBuild = {_normalize_force_same_build(force_same_build)};         // Allow only clients with the same .exe revision (value 0-1)")
    lines.append("")
    lines.append("// Voice & View")
    lines.append(f"disableVoN = {_normalize_binary(disable_von, '0')};             // Enable/disable voice over network (value 0-1)")
    lines.append(f"vonCodecQuality = {von_codec_quality};       // Voice over network codec quality, the higher the better (values 0-30)")

    if shard_id:
        lines.append(f'shardId = "{shard_id}";        // Six alphanumeric characters for private server')

    lines.append(f"disable3rdPerson = {_normalize_binary(disable_3rd_person, '0')};         // Toggles 3rd person view for players (value 0-1)")
    lines.append(f"disableCrosshair = {_normalize_binary(disable_crosshair, '0')};         // Toggles the cross-hair (value 0-1)")
    lines.append(f"disablePersonalLight = {_normalize_binary(disable_personal_light, '1')};   // Disables personal light for all clients connected to server")
    lines.append(f"lightingConfig = {_normalize_int(lighting_config, 0)};         // 0 for brighter night setup, 1 for darker night setup")
    lines.append("")
    lines.append("// Time & Weather")
    lines.append(f'serverTime = "{server_time}";    // Initial in-game time. "SystemTime" uses local machine time, or use "YYYY/MM/DD/HH/MM"')
    lines.append(f"serverTimeAcceleration = {server_time_acceleration};  // Accelerated time multiplier (value 0-24)")
    lines.append(f"serverNightTimeAcceleration = {server_night_time_acceleration};  // Night time multiplier (0.1-64), also multiplied by serverTimeAcceleration")
    lines.append(f"serverTimePersistent = {_normalize_binary(server_time_persistent, '0')};     // Save server time to storage (value 0-1)")
    lines.append("")
    lines.append("// Network")
    lines.append(f"port = {port};")
    if query_port:
        lines.append(f"queryPort = {query_port};")
    lines.append(f"guaranteedUpdates = {guaranteed_updates};        // Communication protocol used with game server (use only number 1)")
    lines.append(f"loginQueueConcurrentPlayers = {login_queue_concurrent_players};  // Players concurrently processed during login")
    lines.append(f"loginQueueMaxPlayers = {login_queue_max_players};       // Maximum players waiting in login queue")
    lines.append("")
    lines.append("// Instance & Storage")
    lines.append(f"instanceId = {instance_id};             // DayZ server instance id, identifies instances and their storage folders")
    lines.append(f"storageAutoFix = {_normalize_binary(storage_auto_fix, '1')};         // Checks/replaces corrupted persistence files (value 0-1)")

    # Map size is a DayZConfigMaster helper value, not a real serverDZ.cfg key,
    # but we keep it as a comment for reference.
    if map_size:
        lines.append(f"// mapSize = {map_size};        // DayZConfigMaster reference value (not used by serverDZ.cfg)")

    # Mods are intentionally NOT written to serverDZ.cfg. DayZ loads mods via
    # the -mod=... command-line flag. Writing mod = "..." here is ignored and
    # can confuse server operators.
    if profiles_path:
        lines.append("")
        lines.append("// Paths")
        lines.append(f'profiles = "{profiles_path}";   // Base profile directory')

    # Logging
    lines.append("")
    lines.append("// Logging")
    if admin_logs:
        lines.append(f'adminLog = "{admin_logs}";')
    else:
        lines.append("adminLog = true;")
    if network_logs:
        lines.append(f'netLog = "{network_logs}";')
    lines.append("logMemory = true;")

    # Optional performance / advanced settings
    if memory_limit:
        lines.append(f"memoryLimit = {memory_limit};")
    if thread_count:
        lines.append(f"threadCount = {thread_count};")
    if update_settings:
        lines.append(f'updateSettings = "{update_settings}";')
    if game_mode:
        lines.append(f'gameMode = "{game_mode}";')

    # Legacy / custom settings kept for backward compatibility
    if time_settings:
        lines.append(f'// timeSettings = "{time_settings}";')
    if weather:
        lines.append(f"// weather = {weather};")

    # Mission template block
    lines.append("")
    lines.append("// Mission to load on server startup")
    lines.append(_mission_template(map_name))

    return "\n".join(lines)


def generate_instance_cfgs(
    base_name: str,
    base_config: Dict[str, Any],
    instances: List[Dict[str, Any]],
    resolved_maps: Optional[Dict[str, str]] = None,
    resolved_mods: Optional[Dict[int, str]] = None,
) -> Dict[int, str]:
    """
    Generate serverDZ.cfg content for each configured instance.

    Args:
        base_name: Base server name
        base_config: Global server config values (max_players, password, etc.)
        instances: List of instance var dicts from the UI
        resolved_maps: Optional {display_name: world_name} lookup for workshop maps
        resolved_mods: Optional {instance_id: full_mod_string} lookup that
            already includes the workshop map mod. Falls back to the instance's
            configured mod_paths when not provided.

    Returns:
        {instance_id: cfg_content}
    """
    configs = {}
    resolved_maps = resolved_maps or {}
    resolved_mods = resolved_mods or {}

    for instance in instances:
        instance_id = int(_get_ui_value(instance.get("id", {}), "1") or 1)

        display_map = _get_ui_value(
            instance.get("map", {}),
            base_config.get("map_name", ""),
        )
        map_name = resolved_maps.get(display_map, display_map)

        # Show the selected map in the server browser instead of an instance number.
        instance_name = base_name
        if display_map:
            instance_name = f"{base_name} - {display_map}"

        cfg_content = generate_server_cfg(
            name=instance_name,
            port=int(_get_ui_value(instance.get("game_port", {}), "2302") or 2302),
            query_port=int(_get_ui_value(instance.get("query_port", {}), "2303") or 2303),
            max_players=int(base_config.get("max_players", 60)),
            password=base_config.get("password", ""),
            password_admin=base_config.get("password_admin", ""),
            description=base_config.get("description", ""),
            enable_whitelist=base_config.get("enable_whitelist", "0"),
            map_name=map_name,
            map_size=int(base_config.get("map_size", 2000)),
            server_time=base_config.get("server_time", "SystemTime"),
            server_time_acceleration=float(base_config.get("server_time_acceleration", 1.0) or 1.0),
            server_night_time_acceleration=float(base_config.get("server_night_time_acceleration", 1.0) or 1.0),
            server_time_persistent=base_config.get("server_time_persistent", "0"),
            disable_von=base_config.get("disable_von", "0"),
            von_codec_quality=int(base_config.get("von_codec_quality", 20) or 20),
            disable_3rd_person=base_config.get("disable_3rd_person", "0"),
            disable_crosshair=base_config.get("disable_crosshair", "0"),
            disable_personal_light=base_config.get("disable_personal_light", "1"),
            lighting_config=_normalize_int(base_config.get("lighting_config", 0), 0),
            shard_id=base_config.get("shard_id", ""),
            guaranteed_updates=int(base_config.get("guaranteed_updates", 1) or 1),
            login_queue_concurrent_players=int(base_config.get("login_queue_concurrent_players", 5) or 5),
            login_queue_max_players=int(base_config.get("login_queue_max_players", 500) or 500),
            instance_id=instance_id,
            storage_auto_fix=base_config.get("storage_auto_fix", "1"),
            verify_signatures=base_config.get("verify_signatures", "2 (Verify)"),
            force_same_build=base_config.get("force_same_build", "1 (Yes)"),
            mod_paths=resolved_mods.get(
                instance_id,
                _get_ui_value(
                    instance.get("mod_paths", {}),
                    base_config.get("mod_paths", ""),
                ),
            ),
            profiles_path=_get_ui_value(
                instance.get("profile", {}),
                f"./profile{instance_id}",
            ),
            game_mode=base_config.get("game_mode", "Survival"),
        )

        configs[instance_id] = cfg_content

    return configs


def write_cfg_to_file(cfg_content: str, filepath: str) -> tuple:
    """
    Write generated cfg content to a file.

    Args:
        cfg_content: CFG content string
        filepath: Destination file path

    Returns:
        Tuple of (success, error_message)
    """
    try:
        path = Path(filepath)
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(cfg_content, encoding='utf-8')
        return True, None
    except Exception as e:
        return False, str(e)
