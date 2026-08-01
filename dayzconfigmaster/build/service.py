# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Build service for managing mod builds

"""
Build service for managing mod builds.

Coordinates the build pipeline:
1. Preflight checks
2. PBO building
3. Signing (if enabled)
4. Deployment to work drive/server profiles
"""

import os
import subprocess
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple


class DSSignFile:
    """Wrapper for DSSignFile from DayZ Tools."""
    
    def __init__(self, dayz_tools_path: str):
        self.dayz_tools_path = Path(dayz_tools_path)
        self.signfile_path = self._find_signfile()
    
    def _find_signfile(self) -> Optional[Path]:
        """Locate DSSignFile executable with robust path detection."""
        possible_names = ["DSSignFile", "dssignfile", "signfile"]
        
        # Check common locations based on OS
        if os.name == 'nt':
            # Windows - check DayZ Tools locations
            tool_dirs = [
                self.dayz_tools_path / "Bin",
                self.dayz_tools_path,
                Path(r"C:\Program Files\Steam\steamapps\common\DayZ Tools\Bin"),
                Path(r"C:\Program Files (x86)\Steam\steamapps\common\DayZ Tools\Bin"),
            ]
        else:
            # Linux/macOS - check DayZ Tools locations
            tool_dirs = [
                self.dayz_tools_path / "Bin",
                self.dayz_tools_path,
            ]
        
        for tool_dir in tool_dirs:
            if not tool_dir.exists():
                continue
            
            for name in possible_names:
                exe_path = tool_dir / f"{name}.exe"
                if exe_path.exists() and os.access(exe_path, os.X_OK):
                    return exe_path
                
                # Linux/macOS might not have .exe extension
                no_ext_path = tool_dir / name
                if no_ext_path.exists() and os.access(no_ext_path, os.X_OK):
                    return no_ext_path
        
        # Also search recursively in common parent directories
        for dirpath in [self.dayz_tools_path, self.dayz_tools_path.parent]:
            if not dirpath:
                continue
            try:
                for item in dirpath.rglob("*dssignfile*"):
                    if os.access(item, os.X_OK):
                        return item
            except (OSError, PermissionError):
                continue
        
        return None
    
    def sign_pbo(
        self,
        pbo_file: str,
        private_key: str,
        output_dir: Optional[str] = None
    ) -> tuple:
        """
        Sign a PBO file with a private key.
        
        Args:
            pbo_file: Path to the .pbo file
            private_key: Path to the .biprivatekey file
            output_dir: Output directory for signed PBO (optional)
        
        Returns:
            Tuple of (success, message)
        """
        if not self.signfile_path:
            return False, "DSSignFile not found in DayZ Tools"
        
        pbo_file = str(Path(pbo_file).resolve())
        private_key = str(Path(private_key).resolve())
        
        output_dir = Path(output_dir or Path(pbo_file).parent)
        output_pbo = output_dir / Path(pbo_file).name
        
        cmd = [
            str(self.signfile_path),
            "sign",
            "-f", pbo_file,
            "-k", private_key,
            "-o", str(output_pbo)
        ]
        
        try:
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=60
            )
            
            if result.returncode == 0 or "Signed" in result.stdout:
                return True, f"Signed: {output_pbo}"
            
            error_msg = result.stderr.strip() or result.stdout.strip()
            return False, f"Signing failed: {error_msg}"
        
        except subprocess.TimeoutExpired:
            return False, "Signing timed out"
        except Exception as e:
            return False, f"Signing error: {str(e)}"
    
    def verify_signature(self, pbo_file: str) -> tuple:
        """
        Verify a PBO file's signature.
        
        Args:
            pbo_file: Path to the .pbo file
        
        Returns:
            Tuple of (is_valid, message)
        """
        if not self.signfile_path:
            return False, "DSSignFile not found"
        
        cmd = [
            str(self.signfile_path),
            "verify",
            "-f", str(Path(pbo_file).resolve())
        ]
        
        try:
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=60
            )
            
            if result.returncode == 0 or "Valid" in result.stdout:
                return True, "Signature is valid"
            
            error_msg = result.stderr.strip() or result.stdout.strip()
            return False, f"Signature verification failed: {error_msg}"
        
        except subprocess.TimeoutExpired:
            return False, "Verification timed out"
        except Exception as e:
            return False, f"Verification error: {str(e)}"


class CfgConvert:
    """Wrapper for CfgConvert from DayZ Tools."""
    
    def __init__(self, dayz_tools_path: str):
        self.dayz_tools_path = Path(dayz_tools_path)
        self.cfgconvert_path = self._find_cfgconvert()
    
    def _find_cfgconvert(self) -> Optional[Path]:
        """Locate CfgConvert executable with robust path detection."""
        possible_names = ["CfgConvert", "cfgconvert"]
        
        # Check common locations based on OS
        if os.name == 'nt':
            # Windows - check DayZ Tools locations
            tool_dirs = [
                self.dayz_tools_path / "Bin",
                self.dayz_tools_path,
                Path(r"C:\Program Files\Steam\steamapps\common\DayZ Tools\Bin"),
                Path(r"C:\Program Files (x86)\Steam\steamapps\common\DayZ Tools\Bin"),
            ]
        else:
            # Linux/macOS - check DayZ Tools locations
            tool_dirs = [
                self.dayz_tools_path / "Bin",
                self.dayz_tools_path,
            ]
        
        for tool_dir in tool_dirs:
            if not tool_dir.exists():
                continue
            
            for name in possible_names:
                exe_path = tool_dir / f"{name}.exe"
                if exe_path.exists() and os.access(exe_path, os.X_OK):
                    return exe_path
                
                # Linux/macOS might not have .exe extension
                no_ext_path = tool_dir / name
                if no_ext_path.exists() and os.access(no_ext_path, os.X_OK):
                    return no_ext_path
        
        # Also search recursively in common parent directories
        for dirpath in [self.dayz_tools_path, self.dayz_tools_path.parent]:
            if not dirpath:
                continue
            try:
                for item in dirpath.rglob("*cfgconvert*"):
                    if os.access(item, os.X_OK):
                        return item
            except (OSError, PermissionError):
                continue
        
        return None
    
    def binarize_config(
        self,
        config_cpp: str,
        output_bin: Optional[str] = None
    ) -> tuple:
        """
        Convert config.cpp to config.bin (binarize).
        
        Args:
            config_cpp: Path to the .cpp file
            output_bin: Output path for .bin file
        
        Returns:
            Tuple of (success, message)
        """
        if not self.cfgconvert_path:
            return False, "CfgConvert not found"
        
        config_cpp = str(Path(config_cpp).resolve())
        output_bin = output_bin or str(Path(config_cpp).with_suffix('.bin').resolve())
        
        cmd = [
            str(self.cfgconvert_path),
            "-from=config",
            f"-in={config_cpp}",
            f"-out={output_bin}"
        ]
        
        try:
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=300
            )
            
            if result.returncode == 0 or "Success" in result.stdout:
                return True, f"Binarized: {output_bin}"
            
            error_msg = result.stderr.strip() or result.stdout.strip()
            return False, f"Binarization failed: {error_msg}"
        
        except subprocess.TimeoutExpired:
            return False, "Binarization timed out"
        except Exception as e:
            return False, f"Binarization error: {str(e)}"
    
    def decompile_config(
        self,
        config_bin: str,
        output_cpp: Optional[str] = None
    ) -> tuple:
        """
        Convert config.bin to config.cpp (decompile).
        
        Args:
            config_bin: Path to the .bin file
            output_cpp: Output path for .cpp file
        
        Returns:
            Tuple of (success, message)
        """
        if not self.cfgconvert_path:
            return False, "CfgConvert not found"
        
        config_bin = str(Path(config_bin).resolve())
        output_cpp = output_cpp or str(Path(config_bin).with_suffix('.cpp').resolve())
        
        cmd = [
            str(self.cfgconvert_path),
            "-from=archive",
            f"-in={config_bin}",
            f"-out={output_cpp}"
        ]
        
        try:
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=300
            )
            
            if result.returncode == 0 or "Success" in result.stdout:
                return True, f"Decompiled: {output_cpp}"
            
            error_msg = result.stderr.strip() or result.stdout.strip()
            return False, f"Decompilation failed: {error_msg}"
        
        except subprocess.TimeoutExpired:
            return False, "Decompilation timed out"
        except Exception as e:
            return False, f"Decompilation error: {str(e)}"


class BuildService:
    """Manages the complete mod build process."""
    
    def __init__(
        self,
        projects_root: str,
        dayz_tools_path: str,
        key_path: Optional[str] = None
    ):
        """
        Initialize build service.
        
        Args:
            projects_root: Root directory for all projects
            dayz_tools_path: Path to DayZ Tools installation
            key_path: Path to signing key directory (optional)
        """
        self.projects_root = Path(projects_root)
        self.dayz_tools_path = Path(dayz_tools_path)
        self.key_path = Path(key_path) if key_path else None
        
        # Import modules - use pure-Python PBO writer by default
        try:
            from dayzconfigmaster.build.pbo_writer import SimplePboWriter
            self.builder = SimplePboWriter()  # Pure Python, no AddonBuilder needed
        except ImportError:
            # Fallback to AddonBuilder
            from dayzconfigmaster.mods.pbo_builder import PboBuilder
            self.builder = PboBuilder(str(dayz_tools_path))
        
        from dayzconfigmaster.mods.discovery import ModLoader, ModDeployer
        from dayzconfigmaster.build.preflight import PreflightEngine
        from dayzconfigmaster.build.cache import BuildCache
        self.loader = ModLoader(projects_root)
        self.deployer = ModDeployer("", projects_root)
        self.preflight_engine = PreflightEngine()
        self.cache = BuildCache(str(self.projects_root / ".build_cache"))
    
    def preflight(
        self,
        mod_name: str,
        mod_dir: Optional[str] = None
    ) -> Dict[str, Any]:
        """
        Run preflight checks on a mod project.
        
        Args:
            mod_name: Name of the mod to check
            mod_dir: Path to mod directory (optional)
        
        Returns:
            Preflight report dict with findings and severity
        """
        if not mod_dir:
            mod_dir = self.projects_root / "mods" / mod_name
        
        return self.preflight_engine.run(str(mod_dir))
    
    def build(
        self,
        mod_name: str,
        clean: bool = False,
        binarize: bool = True,
        sign: bool = False,
        force: bool = False
    ) -> Dict[str, Any]:
        """
        Build a mod project.
        
        Args:
            mod_name: Name of the mod to build
            clean: Clear output before building?
            binarize: Binarize configs/models?
            sign: Sign with creator key?
            force: Force rebuild even if unchanged?
        
        Returns:
            Build result dict with success/failure and messages
        """
        mod_dir = self.projects_root / "mods" / mod_name
        
        if not mod_dir.exists():
            return {
                "success": False,
                "error": f"Mod directory not found: {mod_dir}"
            }
        
        # Run preflight (errors block build)
        preflight_report = self.preflight(mod_name, str(mod_dir))
        
        if not preflight_report.get("ok", True):
            return {
                "success": False,
                "error": "Preflight check failed",
                "findings": preflight_report.get("findings", [])
            }
        
        # Determine output path
        build_output = self.projects_root / "build" / f"{mod_name}"
        pbo_file = str(build_output) + ".pbo"

        # Content-hash cache: skip rebuild if source hasn't changed
        if not force and not clean:
            cached = self.cache.is_up_to_date(mod_name, mod_dir)
            if cached:
                return {
                    "success": True,
                    "message": f"Build skipped (unchanged): {pbo_file}",
                    "cached": True
                }

        # Determine prefix from $PBOPREFIX$ file
        prefix_file = mod_dir / "$PBOPREFIX$"
        prefix = ""
        if prefix_file.exists():
            prefix = prefix_file.read_text().strip()

        # Build the PBO
        success, message = self.builder.pack(
            source_dir=str(mod_dir),
            output_pbo=str(build_output),
            prefix=prefix,
            sign_key=str(self.key_path) if sign and self.key_path else None,
            clean=clean,
            binarize=binarize
        )

        if success:
            self.cache.update(mod_name, mod_dir, Path(pbo_file))
        
        result = {
            "success": success,
            "message": message
        }
        
        if success:
            # Deploy to work drive
            pbo_file = str(build_output) + ".pbo"
            self.deployer.deploy_pbo(pbo_file)
            
            result["deployed"] = True
        
        return result
    
    def generate_key(self, key_name: Optional[str] = None) -> tuple:
        """
        Generate a signing key pair.
        
        Args:
            key_name: Name for the key (defaults to creator name)
        
        Returns:
            Tuple of (success, message)
        """
        if not self.key_path:
            return False, "No key path configured"
        
        key_name = key_name or "dayzconfigmaster"
        private_key = self.key_path / f"{key_name}.biprivatekey"
        public_key = self.key_path / f"{key_name}.bikey"
        
        if not self.key_path.exists():
            self.key_path.mkdir(parents=True, exist_ok=True)
        
        # Locate DSCreateKey (part of DayZ Tools, Windows only).
        dscreatekey = self._find_dscreatekey()
        if dscreatekey is None:
            return False, (
                "DSCreateKey not found. Install DayZ Tools and configure its path, "
                "or generate keys manually. No keys were created."
            )
        
        try:
            result = subprocess.run(
                [str(dscreatekey), key_name],
                cwd=str(self.key_path),
                capture_output=True,
                text=True,
                timeout=60,
            )
        except (OSError, subprocess.SubprocessError) as exc:
            return False, f"Failed to run DSCreateKey: {exc}"
        
        if result.returncode != 0:
            return False, f"DSCreateKey failed: {result.stderr.strip()}"
        
        if not private_key.exists() or not public_key.exists():
            return False, "DSCreateKey ran but key files were not produced"
        
        return True, f"Keys generated: {public_key.name}, {private_key.name}"

    def _find_dscreatekey(self):
        """Return Path to DSCreateKey executable, or None if unavailable."""
        import shutil
        # 1) explicit config attribute if the class has one
        configured = getattr(self, "dscreatekey_path", None)
        if configured and Path(configured).exists():
            return Path(configured)
        # 2) on PATH
        exe = "DSCreateKey.exe" if os.name == "nt" else "DSCreateKey"
        found = shutil.which(exe)
        return Path(found) if found else None
