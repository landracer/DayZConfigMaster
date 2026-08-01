# SOVEREIGN INDIVIDUAL LICENSE – Version 1.0
# Copyright (c) 2026 DayzConfigMaster & landracer. All Rights Reserved.
# https://github.com/landracer/DayZConfigMaster/LICENSE

"""Self-memory guard.

Enforces a hard RAM ceiling on the current Python process. If the process
exceeds the limit it kills itself immediately so it cannot hard-lock the host.
"""

import logging
import os
import sys
import threading
import time
from pathlib import Path
from typing import Optional

logger = logging.getLogger(__name__)

DEFAULT_LIMIT_GB = 10.0
CHECK_INTERVAL_SECONDS = 5.0

_guard_thread: Optional[threading.Thread] = None
_guard_stop: Optional[threading.Event] = None


def _get_self_rss_bytes() -> int:
    """Return this process RSS in bytes (Linux /proc fallback)."""
    try:
        for line in Path("/proc/self/status").read_text().splitlines():
            if line.startswith("VmRSS:"):
                parts = line.split()
                if len(parts) >= 2:
                    return int(parts[1]) * 1024  # kB -> bytes
    except Exception:
        pass
    return 0


def _self_destruct(message: str, exit_code: int = 99) -> None:
    """Log the fatal condition and terminate the process immediately."""
    logger.critical("MEMORY GUARD: %s", message)
    try:
        sys.stderr.write(f"FATAL: {message}\n")
        sys.stderr.flush()
    except Exception:
        pass
    os._exit(exit_code)


def start_memory_guard(limit_gb: float = DEFAULT_LIMIT_GB, interval_seconds: float = CHECK_INTERVAL_SECONDS) -> None:
    """Start a daemon thread that kills this process if RSS exceeds ``limit_gb``."""
    global _guard_thread, _guard_stop

    stop_existing_guard()

    limit_bytes = int(limit_gb * 1024 * 1024 * 1024)
    if limit_bytes <= 0:
        logger.warning("Memory guard limit %.2f GB is invalid; not started.", limit_gb)
        return

    stop_event = threading.Event()
    _guard_stop = stop_event

    def _guard_loop() -> None:
        while not stop_event.is_set():
            try:
                rss = _get_self_rss_bytes()
                if rss > limit_bytes:
                    _self_destruct(
                        f"Process RSS {rss / (1024 ** 3):.2f} GB exceeds "
                        f"limit {limit_gb:.2f} GB. Self-terminating."
                    )
            except Exception as exc:
                logger.debug("Self-memory guard check error: %s", exc)
            stop_event.wait(interval_seconds)

    _guard_thread = threading.Thread(target=_guard_loop, name="self-memory-guard", daemon=True)
    _guard_thread.start()
    logger.info("Self-memory guard started (limit %.2f GB).", limit_gb)


def stop_existing_guard() -> None:
    """Stop any running self-memory guard thread."""
    global _guard_thread, _guard_stop
    if _guard_stop is not None:
        _guard_stop.set()
    _guard_thread = None
    _guard_stop = None


def apply_hard_address_space_limit(limit_gb: float = DEFAULT_LIMIT_GB) -> None:
    """Set the kernel RLIMIT_AS ceiling on the current process (best-effort)."""
    try:
        import resource
    except ImportError:
        logger.debug("resource module unavailable; RLIMIT_AS not set.")
        return

    limit_bytes = int(limit_gb * 1024 * 1024 * 1024)
    if limit_bytes <= 0:
        return

    try:
        soft, hard = resource.getrlimit(resource.RLIMIT_AS)
        # Never raise the existing hard limit; only lower or keep it.
        new_hard = min(limit_bytes, hard) if hard != resource.RLIM_INFINITY else limit_bytes
        resource.setrlimit(resource.RLIMIT_AS, (limit_bytes, new_hard))
        logger.info("Set RLIMIT_AS to %.2f GB (hard %.2f GB).", limit_gb, new_hard / (1024 ** 3))
    except Exception as exc:
        logger.warning("Could not set RLIMIT_AS: %s", exc)


def setup_memory_safety(limit_gb: float = DEFAULT_LIMIT_GB, interval_seconds: float = CHECK_INTERVAL_SECONDS) -> None:
    """Apply both the kernel address-space limit and the self-kill watchdog."""
    apply_hard_address_space_limit(limit_gb)
    start_memory_guard(limit_gb, interval_seconds)
