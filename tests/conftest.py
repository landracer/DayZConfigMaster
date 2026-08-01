"""Pytest configuration shared by all tests."""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent))

# Start the self-memory guard before any test runs so a leaking test cannot
# hard-lock the host. 10 GB is the hard ceiling requested by the user.
try:
    from dayzconfigmaster.utils.memory_guard import setup_memory_safety
    setup_memory_safety()
except Exception as exc:
    print(f"Warning: could not start test memory guard: {exc}", file=sys.stderr)
