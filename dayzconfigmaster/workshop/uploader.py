# Steam Workshop upload/publish via SteamworksPy

"""
Steam Workshop Uploader for DayZ mods.

Provides tools to publish and update workshop items using SteamworksPy.
Requires:
- Steam client running and logged in
- steam_appid.txt containing DayZ app id (221100) in working directory
"""

from __future__ import annotations
import json
import os
import subprocess
import time
from pathlib import Path
from dataclasses import dataclass, field
from typing import Optional, List


DAYZ_APP_ID = 221100


@dataclass
class WorkshopUploadRequest:
    """Represents a workshop upload request."""
    content_folder: str
    title: str
    description: str = ""
    preview_file: str = ""  # path to png/jpg (< 1 MB)
    tags: List[str] = field(default_factory=list)
    visibility: int = 0  # 0=public, 1=friends, 2=private
    change_note: str = ""
    published_file_id: Optional[int] = None  # None = create new


class WorkshopUploader:
    """Upload and update workshop items via SteamworksPy."""

    def __init__(self, app_id: int = DAYZ_APP_ID):
        self.app_id = app_id
        self._steam = None

    def _ensure_steam(self):
        """Initialize Steamworks if not already loaded."""
        if self._steam is None:
            try:
                from steamworks import STEAMWORKS
                self._steam = STEAMWORKS()
                self._steam.initialize()
            except ImportError:
                raise RuntimeError(
                    "SteamworksPy not installed. Install with: pip install SteamworksPy"
                )
        return self._steam

    def _validate_request(self, req: WorkshopUploadRequest) -> tuple:
        """Validate upload request before any Steam calls. Returns (is_valid, message)."""
        # 1. Validate content folder exists
        content_path = Path(req.content_folder)
        if not content_path.exists():
            return False, f"Content folder does not exist: {req.content_folder}"
        if not content_path.is_dir():
            return False, f"Content path is not a directory: {req.content_folder}"

        # 2. Validate preview file (if set)
        if req.preview_file:
            preview_path = Path(req.preview_file)
            if not preview_path.exists():
                return False, f"Preview file does not exist: {req.preview_file}"
            # Check size < 1 MB
            size = preview_path.stat().st_size
            if size > 1_048_576:
                return False, f"Preview file too large ({size} bytes). Max is 1 MB."

        # 3. Validate title length (max 128 chars)
        if len(req.title) > 128:
            return False, f"Title exceeds 128 characters ({len(req.title)} chars)"

        # 4. Validate description length (max 8000 chars)
        if len(req.description) > 8000:
            return False, f"Description exceeds 8000 characters ({len(req.description)} chars)"

        # 5. Validate visibility
        if req.visibility not in [0, 1, 2]:
            return False, "Visibility must be 0 (public), 1 (friends), or 2 (private)"

        return True, None

    def _save_metadata(self, content_folder: str, published_file_id: int) -> Path:
        """Save workshop metadata to a .workshop.json file."""
        content_path = Path(content_folder)
        metadata_path = content_path / f"{content_path.name}.workshop.json"

        data = {
            "publishedfileid": str(published_file_id),
            "contentfolder": content_path.name,
            "title": "",
            "description": "",
            "previewfile": "",
            "visibility": 0,
            "tags": [],
        }

        # Try to load existing metadata if available
        existing = content_path / f"{content_path.name}.workshop.json"
        if existing.exists():
            try:
                with open(existing, 'r') as f:
                    data.update(json.load(f))
            except (json.JSONDecodeError, OSError):
                pass

        with open(metadata_path, 'w') as f:
            json.dump(data, f, indent=2)

        return metadata_path

    def _load_metadata(self, content_folder: str) -> Optional[int]:
        """Load published file ID from .workshop.json if exists."""
        content_path = Path(content_folder)
        metadata_path = content_path / f"{content_path.name}.workshop.json"

        if not metadata_path.exists():
            return None

        try:
            with open(metadata_path, 'r') as f:
                data = json.load(f)
                pfid = data.get("publishedfileid")
                return int(pfid) if pfid else None
        except (json.JSONDecodeError, OSError, ValueError):
            return None

    def _find_steam_app_id_file(self) -> Optional[Path]:
        """Find or create steam_appid.txt in working directory."""
        # Check current directory first
        current = Path.cwd() / "steam_appid.txt"
        if current.exists():
            return current

        # Check parent directories (walk up to find DayZ project root)
        for parent in Path.cwd().parents:
            candidate = parent / "steam_appid.txt"
            if candidate.exists():
                return candidate

        # Create one with DayZ app id
        current.write_text(f"{DAYZ_APP_ID}\n")
        return current

    def _create_new_item(self, steam: object, workshop: object, timeout: int = 30) -> tuple:
        """Create a new workshop item; return (published_file_id or None, error)."""
        result = {"id": None, "error": None, "done": False}

        def _on_created(created):
            # created is CreateItemResult_t with publishedFileId and result fields
            result["id"] = getattr(created, "publishedFileId", None)
            if not result["id"]:
                # Try dictionary access as fallback
                result["id"] = created.get("publishedFileId") if isinstance(created, dict) else None
            result["done"] = True

        workshop.SetItemCreatedCallback(_on_created)
        workshop.CreateItem(self.app_id, 0)  # 0 = k_EWorkshopFileTypeCommunity

        start = time.time()
        while not result["done"] and time.time() - start < timeout:
            steam.run_callbacks()   # PUMP callbacks — required!
            time.sleep(0.1)

        if not result["done"] or not result["id"]:
            return None, "Timed out waiting for CreateItem callback"
        return int(result["id"]), None

    def publish(self, req: WorkshopUploadRequest, on_progress=None) -> tuple:
        """
        Create or update a workshop item.

        Args:
            req: WorkshopUploadRequest with all parameters
            on_progress: Optional callback(percent: float) to report upload progress

        Returns:
            Tuple of (success, message, published_file_id)
        """
        # 1. Validate inputs first (no Steam call needed)
        is_valid, error_msg = self._validate_request(req)
        if not is_valid:
            return False, error_msg, None

        # 2. Check if we have an existing published file ID
        existing_id = self._load_metadata(req.content_folder)

        # 3. Ensure Steam is loaded
        steam = self._ensure_steam()
        workshop = steam.Workshop()

        try:
            # If updating existing item, use StartItemUpdate with published_file_id
            if existing_id and req.published_file_id is None:
                target_id = existing_id
            elif req.published_file_id is not None:
                target_id = req.published_file_id
            else:
                # Create new item
                target_id, err = self._create_new_item(steam, workshop)
                if err:
                    return False, err, None

            update_handle = workshop.StartItemUpdate(self.app_id, target_id)

            if update_handle == 0:
                return False, "Failed to start item update", None

            # 4. Set item properties
            try:
                workshop.SetItemTitle(update_handle, req.title)
            except Exception as e:
                return False, f"Failed to set title: {e}", None

            try:
                workshop.SetItemDescription(update_handle, req.description)
            except Exception as e:
                return False, f"Failed to set description: {e}", None

            # Set content directory
            try:
                workshop.SetItemContent(update_handle, req.content_folder)
            except Exception as e:
                return False, f"Failed to set content directory: {e}", None

            # Set preview image if provided
            if req.preview_file:
                try:
                    workshop.SetItemPreview(update_handle, req.preview_file)
                except Exception as e:
                    return False, f"Failed to set preview: {e}", None

            # Set tags
            try:
                workshop.SetItemTags(update_handle, req.tags)
            except Exception as e:
                return False, f"Failed to set tags: {e}", None

            # Set visibility
            from steamworks.enums import ERemoteStoragePublishedFileVisibility
            vis_map = {
                0: ERemoteStoragePublishedFileVisibility.k_ERemoteStoragePublishedFileVisibilityPublic,
                1: ERemoteStoragePublishedFileVisibility.k_ERemoteStoragePublishedFileVisibilityFriendsOnly,
                2: ERemoteStoragePublishedFileVisibility.k_ERemoteStoragePublishedFileVisibilityPrivate,
            }
            try:
                workshop.SetItemVisibility(update_handle, vis_map[req.visibility])
            except Exception as e:
                return False, f"Failed to set visibility: {e}", None

            # 5. Submit the update
            change_note = req.change_note or ""
            workshop.SubmitItemUpdate(update_handle, change_note)

            # 6. Poll for progress with callback pumping
            max_wait = 300  # 5 minutes max wait
            start_time = time.time()

            while time.time() - start_time < max_wait:
                steam.run_callbacks()  # Pump callbacks during submit poll

                status = workshop.GetItemUpdateProgress(update_handle)
                progress = status.get('progress', 0)

                if on_progress:
                    on_progress(progress * 100)  # Convert to percentage

                # Check if update is complete
                from steamworks.enums import EItemUpdateStatus
                item_status = status.get('status')
                if item_status == EItemUpdateStatus.k_EItemUpdateStatusCommitFinished:
                    break
                elif item_status in [
                    EItemUpdateStatus.k_EItemUpdateStatusInvalid,
                    EItemUpdateStatus.k_EItemUpdateStatusNotEnoughContent,
                    EItemUpdateStatus.k_EItemUpdateStatusMissingPreviewImage,
                    EItemUpdateStatus.k_EItemUpdateStatusGenericError
                ]:
                    return False, f"Upload failed with status: {item_status}", None

                # Wait before polling again
                time.sleep(1)

            # Use target_id directly - no GetSubscribedItems hack!
            new_id = target_id

            # Save metadata with published file ID
            self._save_metadata(req.content_folder, new_id)

            verb = "updated" if (existing_id or req.published_file_id) else "created"
            return True, f"Workshop item {verb}: {new_id}", new_id

        except Exception as e:
            return False, f"Upload error: {e}", None
        finally:
            # Clean up Steam resources
            try:
                if self._steam:
                    self._steam.shutdown()
                    self._steam = None
            except Exception:
                pass