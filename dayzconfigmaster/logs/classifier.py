# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Log line classifier for DayZ logs.

Buckets each line into quick-filter categories used by the log viewer:
- Error
- Warning
- Connection
- ModSuccess

A line may match multiple buckets (e.g. an error about a connection).
"""

from enum import IntFlag
from typing import List


class LogCategory(IntFlag):
    """Quick-filter buckets for a log line."""
    NONE = 0
    ERROR = 1
    WARNING = 2
    CONNECTION = 4
    MODSUCCESS = 8


class LogLineClassifier:
    """Heuristic, substring-based classification of DayZ log lines."""

    @staticmethod
    def classify(line: str) -> LogCategory:
        """Return the category bucket(s) for a single log line."""
        lower = line.lower()
        cat = LogCategory.NONE

        if any(n in lower for n in ("(e):", "error", "exception", "can't compile",
                                     "cannot ", "failed", "critical", "fatal")):
            cat |= LogCategory.ERROR

        if any(n in lower for n in ("(w):", "warning", "warn")):
            cat |= LogCategory.WARNING

        if "connect" in lower or "disconnect" in lower or "connection" in lower:
            cat |= LogCategory.CONNECTION

        if any(n in lower for n in ("loading mod", "loaded", "successfully",
                                     "mission read", "initializing", "registered",
                                     "mod initialized")):
            cat |= LogCategory.MODSUCCESS

        return cat

    @staticmethod
    def matches(line: str, category: LogCategory, search: str = "") -> bool:
        """
        Return True if ``line`` belongs to ``category`` and contains ``search``.

        ``category`` may be LogCategory.NONE to disable category filtering.
        """
        if search and search.lower() not in line.lower():
            return False
        if category == LogCategory.NONE:
            return True
        return LogLineClassifier.classify(line).value & category.value != 0

    @staticmethod
    def filter_lines(lines: List[str], category: LogCategory,
                     search: str = "") -> List[str]:
        """Filter a list of log lines by category and optional search text."""
        return [line for line in lines
                if LogLineClassifier.matches(line, category, search)]
