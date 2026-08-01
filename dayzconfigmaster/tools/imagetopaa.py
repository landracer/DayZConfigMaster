# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
DayZ ImageToPAA wrapper.

Thin subprocess wrapper around DayZ Tools' ImageToPAA.exe.
Converts PNG and TGA images to PAA format with optional suffix validation.
"""

import os
from pathlib import Path
from typing import List, Tuple, Optional


# Valid suffixes for PAA textures (based on DayZ conventions)
VALID_SUFFIXES = [
    "_co", "_ca", "_nohq", "_smdi", "_as", "_dt", "_mc",
    "_nofhq", "_sky", "_detail"
]


def find_imagetopaa(tools_path: str) -> Optional[Path]:
    """
    Locate the ImageToPAA.exe executable in DayZ Tools directory.

    Args:
        tools_path: Path to the DayZ Tools root directory

    Returns:
        Path to ImageToPAA.exe if found, None otherwise
    """
    bin_dir = Path(tools_path) / "Bin"
    if not bin_dir.exists():
        return None

    exe_name = "ImageToPAA.exe" if os.name == 'nt' else "ImageToPAA"
    exe_path = bin_dir / exe_name
    return exe_path if exe_path.exists() else None


def has_valid_suffix(file_name: str) -> bool:
    """
    Check if an image file has a valid suffix for PAA conversion.

    Args:
        file_name: The filename to check

    Returns:
        True if the file has a valid suffix, False otherwise
    """
    stem = Path(file_name).stem
    return any(stem.endswith(suffix) for suffix in VALID_SUFFIXES)


def convert_args(input_path: str, output_path: Optional[str] = None) -> Tuple[str, str]:
    """
    Generate input/output paths for conversion.

    Args:
        input_path: Path to the source image (PNG/TGA)
        output_path: Path for output PAA (defaults to changing extension)

    Returns:
        Tuple of (input_path, output_path)
    """
    if output_path is None:
        output_path = str(Path(input_path).with_suffix('.paa'))
    return input_path, output_path


def plan_folder(
    dir_path: str,
    recursive: bool = False
) -> List[Tuple[str, str, bool]]:
    """
    Plan PAA conversion jobs for all images in a folder.

    Args:
        dir_path: Directory to scan for images
        recursive: Whether to search subdirectories

    Returns:
        List of (input_path, output_path, has_valid_suffix) tuples
    """
    if not os.path.isdir(dir_path):
        return []

    opt = os.walk if recursive else lambda d, f: [(d, [], [f for f in os.listdir(d) if f == f])]

    jobs = []
    if recursive:
        for root, dirs, files in os.walk(dir_path):
            for file in files:
                if file.lower().endswith(('.png', '.tga')):
                    input_path = os.path.join(root, file)
                    output_path = str(Path(input_path).with_suffix('.paa'))
                    has_suffix = has_valid_suffix(file)
                    jobs.append((input_path, output_path, has_suffix))
    else:
        for file in os.listdir(dir_path):
            if file.lower().endswith(('.png', '.tga')):
                input_path = os.path.join(dir_path, file)
                output_path = str(Path(input_path).with_suffix('.paa'))
                has_suffix = has_valid_suffix(file)
                jobs.append((input_path, output_path, has_suffix))

    return jobs


def run(exe_path: str, input_path: str, output_path: str) -> Tuple[bool, str]:
    """
    Run ImageToPAA.exe to convert an image to PAA format.

    Args:
        exe_path: Path to ImageToPAA.exe
        input_path: Path to the source image
        output_path: Path for the output PAA file

    Returns:
        Tuple of (success, message)
    """
    import subprocess

    try:
        result = subprocess.run(
            [exe_path, input_path, output_path],
            capture_output=True,
            text=True,
            timeout=120  # 2 minute timeout for large images
        )

        if result.returncode != 0:
            return False, f"ImageToPAA failed: {result.stderr.strip()}"

        if not os.path.exists(output_path):
            return False, "ImageToPAA ran but output file was not created"

        return True, f"Converted: {input_path} -> {output_path}"

    except subprocess.TimeoutExpired:
        return False, "ImageToPAA timed out (exceeded 120 seconds)"
    except OSError as e:
        return False, f"Failed to run ImageToPAA: {e}"


def convert_folder(
    dir_path: str,
    tools_path: str,
    recursive: bool = False
) -> List[Tuple[str, str, bool, str]]:
    """
    Convert all images in a folder to PAA format.

    Args:
        dir_path: Directory containing source images
        tools_path: Path to DayZ Tools root
        recursive: Whether to search subdirectories

    Returns:
        List of (input_path, output_path, success, message) tuples
    """
    exe_path = find_imagetopaa(tools_path)
    if exe_path is None:
        return [
            ("", "", False,
             "ImageToPAA.exe not found. Install DayZ Tools and configure its path.")
        ]

    jobs = plan_folder(dir_path, recursive)

    results = []
    for input_path, output_path, has_suffix in jobs:
        success, message = run(str(exe_path), input_path, output_path)
        results.append((input_path, output_path, success, message))

    return results


def convert_image(
    input_path: str,
    tools_path: str,
    output_path: Optional[str] = None
) -> Tuple[bool, str]:
    """
    Convert a single image to PAA format.

    Args:
        input_path: Path to the source image
        tools_path: Path to DayZ Tools root
        output_path: Path for output (defaults to changing extension)

    Returns:
        Tuple of (success, message)
    """
    exe_path = find_imagetopaa(tools_path)
    if exe_path is None:
        return False, (
            "ImageToPAA.exe not found. Install DayZ Tools and configure its path. "
            f"Searched in: {Path(tools_path) / 'Bin'}"
        )

    input_p, output_p = convert_args(input_path, output_path)
    return run(str(exe_path), input_p, output_p)


def check_suffix(file_name: str) -> bool:
    """
    Check if a file has a valid suffix for DayZ PAA textures.

    Args:
        file_name: The filename to check

    Returns:
        True if valid suffix, False otherwise
    """
    return has_valid_suffix(file_name)
