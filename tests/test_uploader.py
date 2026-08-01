# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

# Unit tests for workshop/uploader.py request validation

"""
Unit tests for WorkshopUploader._validate_request.

These tests need no Steam client — they exercise the pure validation path that
guards every upload before any Steam call is made.
"""

import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent))

from dayzconfigmaster.workshop.uploader import (
    WorkshopUploader, WorkshopUploadRequest,
)


def _valid_folder() -> str:
    return tempfile.mkdtemp()


def test_valid_request_passes():
    ok, msg = WorkshopUploader()._validate_request(
        WorkshopUploadRequest(content_folder=_valid_folder(), title="My Mod")
    )
    assert ok is True
    assert msg is None


def test_missing_folder_rejected():
    ok, msg = WorkshopUploader()._validate_request(
        WorkshopUploadRequest(content_folder="/nope/does/not/exist", title="X")
    )
    assert ok is False
    assert "does not exist" in msg


def test_folder_that_is_a_file_rejected():
    f = Path(tempfile.mkdtemp()) / "afile.txt"
    f.write_text("x")
    ok, msg = WorkshopUploader()._validate_request(
        WorkshopUploadRequest(content_folder=str(f), title="X")
    )
    assert ok is False
    assert "not a directory" in msg


def test_title_too_long_rejected():
    ok, msg = WorkshopUploader()._validate_request(
        WorkshopUploadRequest(content_folder=_valid_folder(), title="A" * 129)
    )
    assert ok is False
    assert "128" in msg


def test_description_too_long_rejected():
    ok, msg = WorkshopUploader()._validate_request(
        WorkshopUploadRequest(
            content_folder=_valid_folder(), title="X", description="A" * 8001
        )
    )
    assert ok is False
    assert "8000" in msg


def test_bad_visibility_rejected():
    ok, msg = WorkshopUploader()._validate_request(
        WorkshopUploadRequest(content_folder=_valid_folder(), title="X", visibility=5)
    )
    assert ok is False
    assert "Visibility" in msg


def test_missing_preview_rejected():
    ok, msg = WorkshopUploader()._validate_request(
        WorkshopUploadRequest(
            content_folder=_valid_folder(), title="X",
            preview_file="/nope/preview.png",
        )
    )
    assert ok is False
    assert "Preview file does not exist" in msg


def test_oversized_preview_rejected():
    folder = _valid_folder()
    preview = Path(folder) / "preview.png"
    preview.write_bytes(b"0" * (1_048_576 + 1))
    ok, msg = WorkshopUploader()._validate_request(
        WorkshopUploadRequest(
            content_folder=folder, title="X", preview_file=str(preview)
        )
    )
    assert ok is False
    assert "too large" in msg
