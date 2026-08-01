# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
DayZ CfgConvert wrapper.

Thin subprocess wrapper around DayZ Tools' CfgConvert.exe (DeRap).
Used to convert between config.bin and config.cpp formats.
"""

import os
from pathlib import Path
from typing import List, Tuple, Optional


def find_cfgconvert(tools_path: str) -> Optional[Path]:
    """
    Locate the CfgConvert.exe executable in DayZ Tools directory.

    Args:
        tools_path: Path to the DayZ Tools root directory

    Returns:
        Path to CfgConvert.exe if found, None otherwise
    """
    bin_dir = Path(tools_path) / "Bin"
    if not bin_dir.exists():
        return None

    exe_name = "CfgConvert.exe" if os.name == 'nt' else "CfgConvert"
    exe_path = bin_dir / exe_name
    return exe_path if exe_path.exists() else None


def unbinarize_args(bin_path: str, out_cpp: str) -> List[str]:
    """
    Build arguments for converting config.bin to config.cpp.

    Args:
        bin_path: Path to the input .bin file
        out_cpp: Path to the output .cpp file

    Returns:
        List of command line arguments
    """
    return ["-txt", "-dst", out_cpp, bin_path]


def to_bin_args(cpp_path: str, out_bin: str) -> List[str]:
    """
    Build arguments for converting config.cpp to binary.

    Args:
        cpp_path: Path to the input .cpp file
        out_bin: Path to the output .bin file

    Returns:
        List of command line arguments
    """
    return ["-bin", "-dst", out_bin, cpp_path]


def run(
    exe_path: str,
    args: List[str],
    cwd: Optional[str] = None
) -> Tuple[bool, str]:
    """
    Run CfgConvert.exe with the given arguments.

    Args:
        exe_path: Path to CfgConvert.exe
        args: Command line arguments
        cwd: Working directory (defaults to bin folder)

    Returns:
        Tuple of (success, output_message)
    """
    import subprocess

    try:
        if cwd is None:
            cwd = str(Path(exe_path).parent)

        result = subprocess.run(
            [exe_path] + args,
            cwd=cwd,
            capture_output=True,
            text=True,
            timeout=60  # 60 second timeout for config conversion
        )

        if result.returncode != 0:
            return False, f"CfgConvert failed: {result.stderr.strip()}"

        return True, "Conversion complete"

    except subprocess.TimeoutExpired:
        return False, "CfgConvert timed out (exceeded 60 seconds)"
    except OSError as e:
        return False, f"Failed to run CfgConvert: {e}"


def unbinarize(
    exe_path: str,
    bin_path: str,
    out_cpp: str
) -> Tuple[bool, str]:
    """
    Convert a config.bin file to config.cpp.

    Args:
        exe_path: Path to CfgConvert.exe
        bin_path: Path to the input .bin file
        out_cpp: Path to the output .cpp file

    Returns:
        Tuple of (success, message)
    """
    args = unbinarize_args(bin_path, out_cpp)
    cwd = str(Path(bin_path).parent)
    return run(exe_path, args, cwd)


def to_bin(
    exe_path: str,
    cpp_path: str,
    out_bin: str
) -> Tuple[bool, str]:
    """
    Convert a config.cpp file to binary format.

    Args:
        exe_path: Path to CfgConvert.exe
        cpp_path: Path to the input .cpp file
        out_bin: Path to the output .bin file

    Returns:
        Tuple of (success, message)
    """
    args = to_bin_args(cpp_path, out_bin)
    cwd = str(Path(cpp_path).parent)
    return run(exe_path, args, cwd)


def convert_to_cpp(
    bin_path: str,
    out_cpp: str,
    tools_path: str
) -> Tuple[bool, str]:
    """
    Convenience wrapper to unbinarize a config.bin file.

    Args:
        bin_path: Path to the input .bin file
        out_cpp: Path to the output .cpp file
        tools_path: Path to DayZ Tools root

    Returns:
        Tuple of (success, message)
    """
    exe_path = find_cfgconvert(tools_path)
    if exe_path is None:
        return False, (
            "CfgConvert.exe not found. Install DayZ Tools and configure its path. "
            f"Searched in: {Path(tools_path) / 'Bin'}"
        )

    cwd = str(Path(bin_path).parent)
    args = unbinarize_args(bin_path, out_cpp)

    return run(str(exe_path), args, cwd)


def convert_to_bin(
    cpp_path: str,
    out_bin: str,
    tools_path: str
) -> Tuple[bool, str]:
    """
    Convenience wrapper to convert a config.cpp file to binary.

    Args:
        cpp_path: Path to the input .cpp file
        out_bin: Path to the output .bin file
        tools_path: Path to DayZ Tools root

    Returns:
        Tuple of (success, message)
    """
    exe_path = find_cfgconvert(tools_path)
    if exe_path is None:
        return False, (
            "CfgConvert.exe not found. Install DayZ Tools and configure its path. "
            f"Searched in: {Path(tools_path) / 'Bin'}"
        )

    cwd = str(Path(cpp_path).parent)
    args = to_bin_args(cpp_path, out_bin)

    return run(str(exe_path), args, cwd)
