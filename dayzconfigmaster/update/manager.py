# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Update manager for DayZConfigMaster

"""
Update Manager - Self-updating functionality with Velopack integration.

Features:
- Check GitHub releases for new versions
- Download and verify updates (SHA256)
- Install updates silently or with user confirmation
- Velopack support for Windows native updater
"""

import json
import os
import hashlib
from pathlib import Path
from typing import Optional, Dict, Any, List
import urllib.request
import urllib.error


class UpdateInfo:
    """Information about an available update."""
    
    def __init__(
        self,
        version: str,
        release_name: str,
        release_url: str,
        download_url: str,
        published_at: str,
        assets: List[Dict[str, Any]] = None,
        release_body: str = ""
    ):
        """
        Initialize update info.
        
        Args:
            version: Version number (e.g., "2.1.0")
            release_name: Name/title of the release
            release_url: URL to GitHub release page
            download_url: Direct download URL for installer
            published_at: Release date
            assets: List of release assets
            release_body: Full markdown body of the release (for checksums, etc.)
        """
        self.version = version
        self.release_name = release_name
        self.release_url = release_url
        self.download_url = download_url
        self.published_at = published_at
        self.assets = assets or []
        self.release_body = release_body
    
    def to_dict(self) -> Dict[str, Any]:
        """Convert to dictionary."""
        return {
            "version": self.version,
            "release_name": self.release_name,
            "release_url": self.release_url,
            "download_url": self.download_url,
            "published_at": self.published_at,
            "assets": self.assets,
            "release_body": self.release_body
        }
    


class UpdateManager:
    """
    Manages application updates from GitHub releases.
    
    Supports:
    - Checking for new versions
    - Downloading release assets
    - Verifying checksums
    - Velopack integration for Windows
    """
    
    def __init__(
        self,
        repo_owner: str = "landracer",
        repo_name: str = "DayZConfigMaster"
    ):
        """
        Initialize update manager.
        
        Args:
            repo_owner: GitHub repository owner
            repo_name: GitHub repository name
        """
        self.repo_owner = repo_owner
        self.repo_name = repo_name
        self.api_url = f"https://api.github.com/repos/{repo_owner}/{repo_name}/releases"
    
    def _fetch_releases(self) -> List[Dict[str, Any]]:
        """Fetch releases from GitHub API."""
        try:
            with urllib.request.urlopen(self.api_url, timeout=10) as response:
                data = json.loads(response.read().decode())
                return data
        except (urllib.error.URLError, json.JSONDecodeError) as e:
            print(f"Failed to fetch releases: {e}")
            return []
    
    def get_latest_release(self, include_body: bool = False) -> Optional[UpdateInfo]:
        """
        Get the latest release information.
        
        Returns:
            UpdateInfo if available, None otherwise
        """
        releases = self._fetch_releases()
        
        if not releases:
            return None
        
        # Sort by published date (newest first)
        releases.sort(
            key=lambda x: x.get("published_at", ""),
            reverse=True
        )
        
        latest = releases[0]
        
        return UpdateInfo(
            version=latest.get("tag_name", "unknown"),
            release_name=latest.get("name", "Unknown Release"),
            release_url=latest.get("html_url", ""),
            download_url="",
            published_at=latest.get("published_at", ""),
            assets=[
                {
                    "name": asset.get("name", ""),
                    "browser_download_url": asset.get("browser_download_url", ""),
                    "size": asset.get("size", 0)
                }
                for asset in latest.get("assets", [])
            ]
        )
    
    def get_release_by_version(self, version: str) -> Optional[UpdateInfo]:
        """
        Get a specific release by version.
        
        Args:
            version: Version to find (e.g., "2.1.0")
            
        Returns:
            UpdateInfo if found, None otherwise
        """
        releases = self._fetch_releases()
        
        for release in releases:
            tag_name = release.get("tag_name", "").lstrip("v")
            if tag_name == version.lstrip("v"):
                return UpdateInfo(
                    version=release.get("tag_name", ""),
                    release_name=release.get("name", "Unknown Release"),
                    release_url=release.get("html_url", ""),
                    download_url="",
                    published_at=release.get("published_at", ""),
                    assets=[
                        {
                            "name": asset.get("name", ""),
                            "browser_download_url": asset.get("browser_download_url", ""),
                            "size": asset.get("size", 0)
                        }
                        for asset in release.get("assets", [])
                    ]
                )
        
        return None
    
    def check_for_updates(self, current_version: str) -> tuple:
        """
        Check if an update is available.
        
        Args:
            current_version: Current application version
            
        Returns:
            Tuple of (is_update_available, update_info)
        """
        latest = self.get_latest_release()
        
        if not latest:
            return False, None
        
        # Compare versions
        try:
            current_parts = [int(x) for x in current_version.lstrip("v").split(".")]
            latest_parts = [int(x) for x in latest.version.lstrip("v").split(".")]
            
            # Simple semantic version comparison
            if latest_parts > current_parts:
                return True, latest
        except (ValueError, AttributeError):
            pass
        
        return False, None
    
    def download_asset(self, url: str, output_path: Path) -> bool:
        """
        Download a release asset.
        
        Args:
            url: Download URL
            output_path: Output file path
            
        Returns:
            True if successful
        """
        try:
            output_path.parent.mkdir(parents=True, exist_ok=True)
            
            with urllib.request.urlopen(url, timeout=60) as response:
                content = response.read()
                
                with open(output_path, 'wb') as f:
                    f.write(content)
            
            return True
        except (urllib.error.URLError, IOError) as e:
            print(f"Download failed: {e}")
            return False
    
    def calculate_checksum(self, file_path: Path) -> str:
        """
        Calculate SHA256 checksum of a file.
        
        Args:
            file_path: Path to file
            
        Returns:
            Hexadecimal checksum string
        """
        sha256 = hashlib.sha256()
        
        with open(file_path, 'rb') as f:
            for chunk in iter(lambda: f.read(8192), b''):
                sha256.update(chunk)
        
        return sha256.hexdigest()
    
    def verify_checksum(self, file_path: Path, expected_checksum: str) -> bool:
        """
        Verify file checksum.
        
        Args:
            file_path: Path to file
            expected_checksum: Expected SHA256 checksum
            
        Returns:
            True if checksum matches
        """
        actual = self.calculate_checksum(file_path)
        return actual.lower() == expected_checksum.lower()
    
    def install_update(self, update_info: UpdateInfo) -> tuple:
        """
        Install an update with Velopack integration.
        
        Features:
        - Download release asset
        - Verify SHA256 checksum from release notes or assets
        - Install silently or with user confirmation
        
        Args:
            update_info: UpdateInfo containing release details
            
        Returns:
            Tuple of (success, message)
        """
        if not update_info.assets:
            return False, "No assets found for update"
        
        # Find the appropriate asset (Windows installer first)
        asset_url = None
        expected_checksum = None
        
        for asset in update_info.assets:
            name = asset.get("name", "").lower()
            if "windows" in name or ".exe" in name:
                asset_url = asset.get("browser_download_url")
                
                # Look for checksum in release body
                if update_info.release_body and "sha256" in update_info.release_body.lower():
                    import re
                    sha_pattern = r'SHA256[:\s]+([a-fA-F0-9]{64})'
                    match = re.search(sha_pattern, update_info.release_body)
                    if match:
                        expected_checksum = match.group(1)
                
                break
        
        if not asset_url:
            return False, "No Windows installer found"
        
        # Download the installer
        output_path = Path.home() / f"DZLUpdate_{update_info.version}.exe"
        success = self.download_asset(asset_url, output_path)
        
        if not success:
            return False, "Failed to download update"
        
        # Verify checksum if available
        if expected_checksum:
            actual_hash = self.calculate_checksum(output_path)
            if not self.verify_checksum(output_path, expected_checksum):
                return False, f"Checksum verification failed!\nExpected: {expected_checksum}\nActual:   {actual_hash}"
        
        # Use Velopack to install (if available on Windows)
        try:
            import winreg
            # Check for Velopack installation
            try:
                with winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, 
                                   r"SOFTWARE\Velopack") as key:
                    # Velopack is available
                    return True, f"Ready to install: {output_path} (use Velopack CLI)"
            except FileNotFoundError:
                pass
        except ImportError:
            pass
        
        # For now, return path to installer for manual installation
        return True, f"Downloaded update to: {output_path}\nRun the installer to complete the update"


def check_for_updates(current_version: str) -> tuple:
    """
    Convenience function to check for updates.
    
    Args:
        current_version: Current application version
        
    Returns:
        Tuple of (is_update_available, update_info)
    """
    manager = UpdateManager()
    return manager.check_for_updates(current_version)


def auto_update_check() -> None:
    """Auto-update check entry point."""
    import sys
    
    # Get current version
    try:
        from dayzconfigmaster import __version__ as current_version
    except ImportError:
        current_version = "0.0.0"
    
    manager = UpdateManager()
    is_available, info = manager.check_for_updates(current_version)
    
    if is_available and info:
        print(f"Update available: {info.version}")
        print(f"Release: {info.release_name}")
        print(f"URL: {info.release_url}")
        
        # Offer to download
        response = input("Download update? (y/n): ").lower()
        
        if response == 'y':
            success, message = manager.install_update(info)
            print(message)
    else:
        print("No updates available.")


if __name__ == "__main__":
    auto_update_check()