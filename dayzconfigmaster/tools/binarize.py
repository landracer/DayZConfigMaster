# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
DayZ Binarize wrapper.

Thin subprocess wrapper around DayZ Tools' binarize.exe (folder mode).
It converts the binarizable models (MLOD .p3d, etc.) under source into dest,
mirroring sub-paths; it does NOT copy configs or other files.
"""

import os
from pathlib import Path
from typing import List, Tuple, Optional


def find_binarize(tools_path: str) -> Optional[Path]:
    """
    Locate the binarize.exe executable in DayZ Tools directory.

    Args:
        tools_path: Path to the DayZ Tools root directory

    Returns:
        Path to binarize.exe if found, None otherwise
    """
    bin_dir = Path(tools_path) / "Bin"
    if not bin_dir.exists():
        return None

    exe_name = "binarize.exe" if os.name == 'nt' else "binarize"
    exe_path = bin_dir / exe_name
    return exe_path if exe_path.exists() else None


def make_args(
    source_dir: str,
    dest_dir: str,
    bin_path: str,
    addon_folders: List[str],
    textures_dir: str,
    max_processes: int = 1
) -> List[str]:
    """
    Build command line arguments for binarize.

    Args:
        source_dir: Source directory containing models to binarize
        dest_dir: Destination directory for binarized output
        bin_path: Path to the mounted project drive (e.g., P:\\\\)
        addon_folders: List of addon folders to include
        textures_dir: Directory for scratch texture files
        max_processes: Maximum parallel processes

    Returns:
        List of command line arguments
    """
    bp = bin_path.rstrip('\\/')
    if len(bp) == 2 and bp[1] == ':':
        bp += "\\"

    args = [
        "-targetBonesInterval=56",
        f"-maxProcesses={max(1, max_processes)}",
        "-always",
        "-silent",
    ]

    for folder in addon_folders:
        fd = folder.rstrip('\\/')
        if len(fd) == 2 and fd[1] == ':':
            fd += "\\"
        args.append(f"-addon={fd}")

    args.append(f"-textures={textures_dir}")
    args.append(f"-binpath={bp}")
    args.append(source_dir)
    args.append(dest_dir)

    return args


def run(
    exe_path: str,
    source_dir: str,
    dest_dir: str,
    bin_path: str,
    addon_folders: List[str],
    textures_dir: str,
    max_processes: int = 1
) -> Tuple[bool, str]:
    """
    Run binarize.exe to convert models to binary format.

    Args:
        exe_path: Path to binarize.exe
        source_dir: Source directory containing models to binarize
        dest_dir: Destination directory for binarized output
        bin_path: Path to the mounted project drive (e.g., P:\\\\)
        addon_folders: List of addon folders to include
        textures_dir: Directory for scratch texture files
        max_processes: Maximum parallel processes

    Returns:
        Tuple of (success, output_message)
    """
    import subprocess

    try:
        work_dir = bin_path.rstrip('\\/')
        if len(work_dir) == 2 and work_dir[1] == ':':
            work_dir += "\\"

        args = make_args(
            source_dir=source_dir,
            dest_dir=dest_dir,
            bin_path=bin_path,
            addon_folders=addon_folders,
            textures_dir=textures_dir,
            max_processes=max_processes
        )

        result = subprocess.run(
            [exe_path] + args,
            cwd=work_dir,
            capture_output=True,
            text=True,
            timeout=300  # 5 minute timeout for large models
        )

        if result.returncode != 0:
            return False, f"binarize failed: {result.stderr.strip()}"

        return True, "Binarization complete"

    except subprocess.TimeoutExpired:
        return False, "binarize timed out (exceeded 5 minutes)"
    except OSError as e:
        return False, f"Failed to run binarize: {e}"


def binarize_folder(
    source_dir: str,
    dest_dir: str,
    tools_path: str,
    addon_paths: Optional[List[str]] = None,
    textures_dir: Optional[str] = None,
    max_processes: int = 1
) -> Tuple[bool, str]:
    """
    Convenience wrapper to binarize all models in a folder.

    Args:
        source_dir: Source directory containing models
        dest_dir: Destination directory for output
        tools_path: Path to DayZ Tools root
        addon_paths: List of addon paths (defaults to [tools_path])
        textures_dir: Textures dir (defaults to temp)
        max_processes: Maximum parallel processes

    Returns:
        Tuple of (success, message)
    """
    exe_path = find_binarize(tools_path)
    if exe_path is None:
        return False, (
            "binarize.exe not found. Install DayZ Tools and configure its path. "
            f"Searched in: {Path(tools_path) / 'Bin'}"
        )

    addon_paths = addon_paths or [tools_path]
    textures_dir = textures_dir or str(Path(dest_dir).parent / ".textures")

    return run(
        exe_path=str(exe_path),
        source_dir=source_dir,
        dest_dir=dest_dir,
        bin_path=tools_path,
        addon_folders=addon_paths,
        textures_dir=textures_dir,
        max_processes=max_processes
    )
