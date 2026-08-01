# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""
Log management package providing:
- Real-time log file watching
- Line-level log classification
- Diagnostics engine for error analysis
"""

from .tailer import LogTailer, LogResolver
from .classifier import LogLineClassifier, LogCategory
from .diagnostics import DiagnosticsEngine, DiagnosticRule

__all__ = [
    "LogTailer",
    "LogResolver",
    "LogLineClassifier",
    "LogCategory",
    "DiagnosticsEngine",
    "DiagnosticRule",
]