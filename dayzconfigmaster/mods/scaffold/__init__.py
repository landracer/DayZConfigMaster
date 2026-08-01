# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Mod project scaffolding and management.

Functions for creating new mod projects with proper DayZ structure.
"""

import os
from pathlib import Path
from typing import Tuple, Optional


def is_valid_mod_name(name: str) -> bool:
    """
    Check if a mod name is valid (alphanumeric + underscore only).

    Args:
        name: Mod name to validate

    Returns:
        True if valid, False otherwise
    """
    if not name:
        return False
    # Valid characters: letters, digits, underscores
    for char in name:
        if not (char.isalnum() or char == '_'):
            return False
    return True


def scaffold_mod(
    projects_root: str,
    mod_name: str,
    author: str = "Unknown"
) -> Tuple[bool, str]:
    """
    Create a new mod project with standard DayZ structure.

    Args:
        projects_root: Root directory containing mods
        mod_name: Name of the mod to create
        author: Author name for the mod

    Returns:
        Tuple of (success, message)
    """
    if not is_valid_mod_name(mod_name):
        return False, f"Invalid mod name: {mod_name}"

    mod_dir = Path(projects_root) / "mods" / mod_name
    meta_dir = mod_dir / ".dzl"

    try:
        # Create directory structure
        mod_dir.mkdir(parents=True, exist_ok=True)
        (mod_dir / "scripts").mkdir(exist_ok=True)
        (mod_dir / "scripts" / "3_Game").mkdir(exist_ok=True)
        (mod_dir / "scripts" / "4_World").mkdir(exist_ok=True)
        (mod_dir / "scripts" / "5_Mission").mkdir(exist_ok=True)
        (mod_dir / "data").mkdir(exist_ok=True)
        (mod_dir / "gui").mkdir(exist_ok=True)
        meta_dir.mkdir(parents=True, exist_ok=True)

        # Create config.cpp
        config_cpp = f'''class CfgPatches
{{
    class {mod_name}
    {{
        units[] = {{}};
        weapons[] = {{}};
        requiredVersion = 0.1;
        requiredAddons[] = {{ "DZ_Data" }};
        author = "{author}";
    }};
}};

class CfgMods
{{
    class {mod_name}
    {{
        dir = "{mod_name}";
        name = "{mod_name}";
        author = "{author}";
        type = "mod";
        dependencies[] = {{ "Game", "World", "Mission" }};
        class defs
        {{
            class gameScriptModule {{ value = ""; files[] = {{ "{mod_name}/scripts/3_Game" }}; }};
            class worldScriptModule {{ value = ""; files[] = {{ "{mod_name}/scripts/4_World" }}; }};
            class missionScriptModule {{ value = ""; files[] = {{ "{mod_name}/scripts/5_Mission" }}; }};
        }};
    }};
}};
'''
        (mod_dir / "config.cpp").write_text(config_cpp)

        # Create $PBOPREFIX$
        (mod_dir / "$PBOPREFIX$").write_text(mod_name)

        # Create README.md
        readme = f'''# {mod_name}

DayZ mod scaffolded by DayZConfigMaster.

- Source: this folder (under your ProjectsRoot)
- Built PBO target: `P:\\Mods\\@{mod_name}\\Addons\\`
- Scripts: `scripts/3_Game`, `4_World`, `5_Mission`
'''
        (mod_dir / "README.md").write_text(readme)

        # Create .gitignore
        gitignore = '''# DayZ mod

# Signing keys — NEVER commit your private key
*.biprivatekey

# Build output
*.pbo
*.pbo.*

# Logs & temporary files
*.log
*.tmp
*.bak

# OS / editor cruft
Thumbs.db
desktop.ini
.vs/
.idea/
*.user
'''
        (mod_dir / ".gitignore").write_text(gitignore)

        # Create .dzl/mod.json metadata
        import json
        meta = {
            "name": mod_name,
            "author": author,
            "created": __import__('datetime').datetime.utcnow().isoformat() + "Z"
        }
        (meta_dir / "mod.json").write_text(json.dumps(meta, indent=2))

        # Create .gitkeep files
        for subdir in ["scripts/3_Game", "scripts/4_World", "scripts/5_Mission", "data", "gui"]:
            (mod_dir / subdir / ".gitkeep").write_text("")

        return True, f"Mod '{mod_name}' scaffolded successfully"

    except Exception as e:
        return False, f"Failed to create mod: {e}"


def fix_junction(link_path: str, target_path: str) -> Tuple[bool, str]:
    """
    Create or repair a junction/symlink for DayZ mod development.

    On Windows, uses mklink /J (junction). On Linux/macOS, uses ln -s (symlink).

    Args:
        link_path: Path where the junction should be created
        target_path: Target directory the junction should point to

    Returns:
        Tuple of (success, message)
    """
    import subprocess
    from pathlib import Path

    link = Path(link_path)
    target = Path(target_path)

    # Ensure target exists
    if not target.exists():
        try:
            target.mkdir(parents=True, exist_ok=True)
        except Exception as e:
            return False, f"Target directory does not exist and cannot be created: {e}"

    # Check if link already exists and points to correct target
    if link.exists():
        if os.name == 'nt':
            import ctypes
            # On Windows, check if it's a reparse point
            attrs = ctypes.windll.kernel32.GetFileAttributesW(str(link))
            if attrs != -1:  # FILE_ATTRIBUTE_REPARSE_POINT = 0x400
                # Check target
                current_target = link.resolve()
                if current_target == target.resolve():
                    return True, "Link already exists and points to correct target"
        else:
            # On Unix-like systems
            if os.path.islink(str(link)):
                current_target = Path(os.readlink(str(link)))
                if current_target.resolve() == target.resolve():
                    return True, "Symlink already exists and points to correct target"

    # Remove existing link if it's broken or pointing elsewhere
    try:
        if link.exists() and (os.name == 'nt' or os.path.islink(str(link))):
            if os.name == 'nt':
                subprocess.run(['cmd', '/c', 'rmdir', str(link)], check=True)
            else:
                link.unlink()
    except Exception as e:
        return False, f"Failed to remove existing link: {e}"

    # Create the new junction/symlink
    try:
        if os.name == 'nt':
            result = subprocess.run(
                ['cmd', '/c', 'mklink', '/J', str(link), str(target)],
                capture_output=True,
                text=True
            )
            if result.returncode != 0:
                return False, f"Failed to create junction: {result.stderr}"
        else:
            link.symlink_to(target)

        return True, f"Link created: {link} -> {target}"

    except Exception as e:
        return False, f"Failed to create link: {e}"


def get_mod_presets_dir(projects_root: str) -> Path:
    """
    Get the directory containing mod presets.

    Args:
        projects_root: Root directory containing mods

    Returns:
        Path to mod-presets directory
    """
    return Path(projects_root) / "mod-presets"


def save_mod_preset(
    projects_root: str,
    name: str,
    active_mods: list
) -> Tuple[bool, str]:
    """
    Save a named preset of enabled mods.

    Args:
        projects_root: Root directory containing mods
        name: Name for the preset
        active_mods: List of mod paths that should be enabled

    Returns:
        Tuple of (success, message)
    """
    presets_dir = get_mod_presets_dir(projects_root)

    try:
        presets_dir.mkdir(parents=True, exist_ok=True)

        import json
        preset_data = {
            "name": name,
            "mods": active_mods,
            "saved_at": __import__('datetime').datetime.utcnow().isoformat() + "Z"
        }

        preset_file = presets_dir / f"{name}.json"
        preset_file.write_text(json.dumps(preset_data, indent=2))

        return True, f"Mod preset '{name}' saved with {len(active_mods)} mods"

    except Exception as e:
        return False, f"Failed to save mod preset: {e}"


def load_mod_preset(
    projects_root: str,
    name: str
) -> Tuple[bool, dict]:
    """
    Load a named mod preset.

    Args:
        projects_root: Root directory containing mods
        name: Name of the preset to load

    Returns:
        Tuple of (success, data_dict)
    """
    presets_dir = get_mod_presets_dir(projects_root)
    preset_file = presets_dir / f"{name}.json"

    try:
        if not preset_file.exists():
            return False, {}

        import json
        with open(preset_file) as f:
            data = json.load(f)

        return True, data

    except Exception as e:
        return False, {}


def list_mod_presets(projects_root: str) -> list:
    """
    List all available mod presets.

    Args:
        projects_root: Root directory containing mods

    Returns:
        List of preset names
    """
    presets_dir = get_mod_presets_dir(projects_root)

    if not presets_dir.exists():
        return []

    import glob
    files = glob.glob(str(presets_dir / "*.json"))
    return [Path(f).stem for f in files]


def delete_mod_preset(
    projects_root: str,
    name: str
) -> Tuple[bool, str]:
    """
    Delete a named mod preset.

    Args:
        projects_root: Root directory containing mods
        name: Name of the preset to delete

    Returns:
        Tuple of (success, message)
    """
    presets_dir = get_mod_presets_dir(projects_root)
    preset_file = presets_dir / f"{name}.json"

    try:
        if not preset_file.exists():
            return False, f"Preset '{name}' does not exist"

        preset_file.unlink()

        return True, f"Mod preset '{name}' deleted"

    except Exception as e:
        return False, f"Failed to delete mod preset: {e}"
