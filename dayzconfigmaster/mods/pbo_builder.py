# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# PBO builder module for DayZ mod building

"""
PBO Builder module for DayZConfigMaster.

Uses AddonBuilder CLI from DayZ Tools to pack mods into PBO files.
This is cross-platform compatible and handles all the complex packing logic.

Features:
- Pack folders into PBO with prefix
- Binarize configs/models
- Sign PBOs with creator key
- Clean build option
"""

import subprocess
import os
from pathlib import Path
from typing import Optional, List, Tuple


class PboBuilder:
    """
    Wrapper for AddonBuilder from DayZ Tools.
    
    Cross-platform wrapper that handles:
    - Path normalization (Windows vs Unix)
    - Command construction
    - Error handling
    - Progress reporting
    """
    
    def __init__(self, dayz_tools_path: str):
        """
        Initialize with DayZ Tools installation path.
        
        Args:
            dayz_tools_path: Path to DayZ Tools directory
        """
        self.dayz_tools_path = Path(dayz_tools_path)
        self.addon_builder_path = self._find_addon_builder()
    
    def _find_addon_builder(self) -> Optional[Path]:
        """Locate AddonBuilder executable."""
        # Check common locations
        possible_names = ["AddonsBuilder", "AddonBuilder", "addonbuilder"]
        
        for name in possible_names:
            if os.name == 'nt':
                exe_path = self.dayz_tools_path / "Bin" / f"{name}.exe"
            else:
                exe_path = self.dayz_tools_path / "Bin" / name
            
            if exe_path.exists():
                return exe_path
        
        # Return None if not found
        return None
    
    def pack(
        self,
        source_dir: str,
        output_pbo: str,
        prefix: Optional[str] = None,
        sign_key: Optional[str] = None,
        clean: bool = False,
        binarize: bool = True,
        verbose: bool = False
    ) -> tuple:
        """
        Pack a folder into a PBO file using AddonBuilder.
        
        Args:
            source_dir: Source directory to pack
            output_pbo: Output PBO path (without .pbo extension)
            prefix: PBO prefix string
            sign_key: Path to private key for signing
            clean: Clear output folder before packing?
            binarize: Binarize configs/models? (False = -packonly)
            verbose: Show detailed output?
        
        Returns:
            Tuple of (success, message)
        """
        if not self.addon_builder_path:
            return False, "AddonBuilder not found in DayZ Tools"
        
        # Convert to absolute paths
        source_dir = str(Path(source_dir).resolve())
        output_pbo = str(Path(output_pbo).resolve())
        
        cmd = [str(self.addon_builder_path), source_dir]
        
        # Output PBO
        if os.name == 'nt':
            # Windows uses quotes for paths
            cmd.append(f'"{output_pbo}.pbo"')
        else:
            cmd.append(f"{output_pbo}.pbo")
        
        # Prefix (wrapped in quotes)
        if prefix:
            cmd.append(f'"{prefix}"')
        
        # Signing key
        if sign_key:
            cmd.append(str(Path(sign_key).resolve()))
        
        # Clean flag (-clear)
        if clean:
            cmd.append("-clear")
        
        # Pack only vs full build
        if not binarize:
            cmd.append("-packonly")
        
        # Verbose mode
        if verbose:
            cmd.append("-dologs")
        
        try:
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=300  # 5 minute timeout
            )
            
            # Check for errors in output
            if "SUCCESS" in result.stdout or result.returncode == 0:
                return True, f"PBO created: {output_pbo}.pbo"
            
            error_msg = result.stderr.strip() or result.stdout.strip()
            return False, f"Build failed: {error_msg}"
        
        except subprocess.TimeoutExpired:
            return False, "Build timed out after 5 minutes"
        except Exception as e:
            return False, f"Build error: {str(e)}"
    
    def unbinarize(self, input_pbo: str, output_path: Optional[str] = None) -> tuple:
        """
        Unbinarize a config.bin to .cpp using DeRap (from CfgConvert).
        
        Args:
            input_pbo: Path to the PBO or config.bin
            output_path: Output path for .cpp file
        
        Returns:
            Tuple of (success, message)
        """
        if not self.addon_builder_path:
            return False, "AddonBuilder not found in DayZ Tools"
        
        # Use CfgConvert from DayZ Tools for DeRap functionality
        cfgconvert_path = self._find_cfgconvert()
        if not cfgconvert_path:
            return False, "CfgConvert (DeRap) not found - required for unbinarization"
        
        input_pbo = str(Path(input_pbo).resolve())
        output_dir = str(Path(output_path or Path(input_pbo).parent / "decompressed").resolve())
        
        # Create output directory if needed
        Path(output_dir).mkdir(parents=True, exist_ok=True)
        
        cmd = [
            str(cfgconvert_path),
            "-from=archive",
            f"-in={input_pbo}",
            f"-out={output_dir}"
        ]
        
        try:
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=300  # 5 minute timeout
            )
            
            if result.returncode == 0 or "Success" in result.stdout:
                return True, f"Unbinarized to: {output_dir}"
            
            error_msg = result.stderr.strip() or result.stdout.strip()
            return False, f"Unbinarize failed: {error_msg}"
        
        except subprocess.TimeoutExpired:
            return False, "Unbinarize timed out after 5 minutes"
        except Exception as e:
            return False, f"Unbinarize error: {str(e)}"
    
    def _find_cfgconvert(self) -> Optional[Path]:
        """Locate CfgConvert executable (DeRap)."""
        possible_names = ["CfgConvert", "cfgconvert", "derap"]
        
        for name in possible_names:
            if os.name == 'nt':
                exe_path = self.dayz_tools_path / "Bin" / f"{name}.exe"
            else:
                exe_path = self.dayz_tools_path / "Bin" / name
            
            if exe_path.exists():
                return exe_path
        
        # Also check common parent directories
        for dirpath in [self.dayz_tools_path, self.dayz_tools_path.parent]:
            if not dirpath:
                continue
            for item in dirpath.iterdir() if dirpath.exists() else []:
                if item.is_file() and any(n.lower() in item.name.lower() for n in possible_names):
                    return item
        
        return None
    
    def binarize(
        self,
        source_dir: str,
        output_pbo: Optional[str] = None,
        convert_mlod: bool = False
    ) -> tuple:
        """
        Binarize configs and optionally convert MLOD to ODOL using AddonBuilder.
        
        Args:
            source_dir: Source directory with files to binarize
            output_pbo: Output PBO path (optional, uses AddonBuilder if not specified)
            convert_mlod: Convert MLOD models to ODOL format
        
        Returns:
            Tuple of (success, message)
        """
        # For full binarization with model conversion, use AddonBuilder
        return self.pack(
            source_dir=source_dir,
            output_pbo=output_pbo or str(Path(source_dir).parent / "binarized"),
            binarize=True
        )
    
    def convert_mlod_to_odol(self, input_file: str, output_path: Optional[str] = None) -> tuple:
        """
        Convert MLOD model to ODOL format using DeRap.
        
        Args:
            input_file: Path to .mlo file (MLOD)
            output_path: Output path for .odol file
        
        Returns:
            Tuple of (success, message)
        """
        cfgconvert_path = self._find_cfgconvert()
        if not cfgconvert_path:
            return False, "CfgConvert (DeRap) not found"
        
        input_file = str(Path(input_file).resolve())
        output_path = output_path or str(Path(input_file).with_suffix('.odol').resolve())
        
        cmd = [
            str(cfgconvert_path),
            "-from=model",
            f"-in={input_file}",
            f"-out={output_path}"
        ]
        
        try:
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=300
            )
            
            if result.returncode == 0 or "Success" in result.stdout:
                return True, f"Converted to: {output_path}"
            
            error_msg = result.stderr.strip() or result.stdout.strip()
            return False, f"MLOD->ODOL conversion failed: {error_msg}"
        
        except subprocess.TimeoutExpired:
            return False, "Conversion timed out"
        except Exception as e:
            return False, f"Conversion error: {str(e)}"
