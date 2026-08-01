# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Log tailing module for real-time log monitoring.

Provides:
- Glob-based discovery of the newest log file per type
- Multiple log type support (script, rpt, adm, console, client)
- Auto-scroll and display management

Memory safety:
- ``tail()`` never reads more than the requested number of lines into memory.
- ``watch()`` tracks file position with ``seek()`` and only reads newly written
  bytes. It also defends against runaway growth caused by a single enormous
  line or a flooded log file.
"""

import os
import time
from collections import deque
from pathlib import Path
from typing import Callable, Dict, List, Optional


class LogResolver:
    """Discover the newest DayZ log files by glob pattern."""

    # Patterns used to resolve each log type. The newest matching file is returned.
    PATTERNS: Dict[str, List[str]] = {
        "script": ["script_*.log", "DayZDiag_x64_script.log"],
        "rpt": ["*.RPT", "DayZDiag_x64_rpt.log"],
        "adm": ["*.ADM", "admin.log"],
        "console": ["server_console.log", "*console*.log"],
        "client": ["script_*.log", "DayZDiag_x64_client.log"],
    }

    def __init__(self, server_profiles: Optional[Path] = None,
                 client_profiles: Optional[Path] = None):
        self.server_profiles = Path(server_profiles) if server_profiles else None
        self.client_profiles = Path(client_profiles) if client_profiles else None

    @staticmethod
    def _newest(directory: Path, patterns: List[str]) -> Optional[Path]:
        """Return the most recently modified file matching any pattern."""
        if not directory or not directory.exists():
            return None

        candidates: List[Path] = []
        for pattern in patterns:
            try:
                candidates.extend(directory.glob(pattern))
            except Exception:
                continue

        if not candidates:
            return None

        return max(candidates, key=lambda p: p.stat().st_mtime)

    def resolve(self, log_type: str) -> Optional[Path]:
        """Resolve the newest log file for ``log_type``."""
        patterns = self.PATTERNS.get(log_type)
        if not patterns:
            return None

        if log_type == "client":
            profiles = self.client_profiles or self.server_profiles
        else:
            profiles = self.server_profiles

        return self._newest(profiles, patterns) if profiles else None

    def resolve_all(self) -> Dict[str, Optional[Path]]:
        """Resolve the newest file for every known log type."""
        return {log_type: self.resolve(log_type) for log_type in self.PATTERNS}


class LogTailer:
    """Tails DayZ server logs in real-time without loading whole files."""

    # Read at most this many bytes per watch iteration to avoid spikes.
    WATCH_MAX_CHUNK_BYTES = 2 * 1024 * 1024  # 2 MB
    # If a single line exceeds this length it is truncated.
    MAX_LINE_LENGTH = 256 * 1024  # 256 KB

    def __init__(self, profiles_path: str, client_profiles_path: Optional[str] = None):
        """
        Initialize with profiles directory path(s).

        Args:
            profiles_path: Path to DayZ server profiles directory.
            client_profiles_path: Optional path to client profiles directory.
        """
        self.resolver = LogResolver(
            server_profiles=Path(profiles_path) if profiles_path else None,
            client_profiles=Path(client_profiles_path) if client_profiles_path else None,
        )

    def get_log_path(self, log_type: str) -> Optional[Path]:
        """Get path to the newest log file for a given type."""
        return self.resolver.resolve(log_type)

    def tail(self, log_type: str, lines: int = 50) -> List[str]:
        """
        Get the last ``lines`` lines of the newest log file for ``log_type``.

        This method is memory-safe: it only keeps ``lines`` entries in memory
        regardless of how large the source file is.
        """
        path = self.get_log_path(log_type)

        if not path or not path.exists():
            return [f"Log file not found for {log_type}"]

        try:
            # Rolling window: never materialise the whole file.
            buffer: deque[str] = deque(maxlen=max(1, lines))
            with open(path, "r", encoding="utf-8", errors="ignore") as f:
                for raw_line in f:
                    if len(raw_line) > self.MAX_LINE_LENGTH:
                        raw_line = raw_line[:self.MAX_LINE_LENGTH] + "... [truncated]\n"
                    buffer.append(raw_line.rstrip())
            return list(buffer)
        except Exception as e:
            return [f"Error reading log: {str(e)}"]

    def watch(
        self,
        log_type: str,
        callback: Callable[[List[str]], None],
        update_interval: float = 1.0,
    ) -> None:
        """
        Watch the newest log file for ``log_type`` and call ``callback`` on new lines.

        Intended to run in a background thread. Uses ``seek()`` so the entire
        file is never re-read on each poll.
        """
        path = self.get_log_path(log_type)

        if not path or not path.exists():
            callback([f"Log file not found for {log_type}"])
            return

        def _read_new_lines(file_obj, last_pos: int) -> tuple[List[str], int]:
            file_obj.seek(0, os.SEEK_END)
            size = file_obj.tell()

            if size < last_pos:
                # Log was truncated/rotated; start from the beginning.
                file_obj.seek(0)
                last_pos = 0

            if size == last_pos:
                return [], last_pos

            read_size = min(size - last_pos, self.WATCH_MAX_CHUNK_BYTES)
            file_obj.seek(last_pos)
            data = file_obj.read(read_size)
            new_pos = last_pos + len(data)

            # Decode safely and split into lines, preserving any trailing
            # partial line for the next iteration.
            text = data.decode("utf-8", errors="ignore")
            raw_lines = text.split("\n")
            if text.endswith("\n"):
                raw_lines = raw_lines[:-1]

            output: List[str] = []
            for idx, line in enumerate(raw_lines):
                if idx == len(raw_lines) - 1 and new_pos < size:
                    # Last chunk may be an incomplete line; rewind position.
                    new_pos -= len(line.encode("utf-8", errors="ignore"))
                    break
                if len(line) > self.MAX_LINE_LENGTH:
                    line = line[:self.MAX_LINE_LENGTH] + "... [truncated]"
                output.append(line.rstrip())

            return output, new_pos

        try:
            with open(path, "rb") as f:
                f.seek(0, os.SEEK_END)
                position = f.tell()

            while True:
                time.sleep(update_interval)

                # Log rotation: if the resolved path changed, restart from the
                # beginning of the new file.
                current_path = self.get_log_path(log_type)
                if current_path != path:
                    path = current_path
                    if not path or not path.exists():
                        callback([f"Log file not found for {log_type}"])
                        return
                    with open(path, "rb") as f:
                        f.seek(0, os.SEEK_END)
                        position = f.tell()
                    continue

                with open(path, "rb") as f:
                    new_lines, position = _read_new_lines(f, position)

                if new_lines:
                    callback(new_lines)

        except Exception as e:
            callback([f"Error watching log: {str(e)}"])