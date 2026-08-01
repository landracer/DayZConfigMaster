# Hooks Module

Hook execution manager for DayZ server lifecycle events.

## Features
- Execute scripts at specific points in server lifecycle
- Hook types: beforeStart, afterStart, missionChanged
- Configurable hooks via JSON configuration
- Result tracking and logging

## Usage

```python
from dayzconfigmaster.hooks import HookManager, HookType

# Create hook manager
hook_mgr = HookManager("/path/to/projects")

# Load hooks from config (creates default if not exists)
hook_mgr.load_hooks()

# Add a beforeStart hook
hook_mgr.add_hook(
    HookType.BEFORE_START,
    "/path/to/backup.sh",
    ["--pre-restart"]
)

# Add an afterStart hook
hook_mgr.add_hook(
    HookType.AFTER_START,
    "/path/to/notify.py"
)

# Execute hooks
results = hook_mgr.execute_before_start()
for result in results:
    print(f"Result: {result}")
```

## Hook Types

| Type | When Executed |
|------|---------------|
| `beforeStart` | Before the server starts |
| `afterStart` | After the server is ready |
| `missionChanged` | When mission files change |

## Script Examples

### Bash (Linux/Mac)
```bash
#!/bin/bash
# before_start.sh
echo "$(date): Starting server..."
# Your pre-start logic here
```

### Python
```python
#!/usr/bin/env python3
import datetime
print(f"{datetime.datetime.now()}: Server starting...")
# Your pre-start logic here
```

## Configuration Format

```json
{
  "hooks": [
    {
      "type": "beforeStart",
      "program": "/path/to/script.sh",
      "params": ["arg1", "arg2"]
    },
    {
      "type": "afterStart",
      "program": "/path/to/notify.py"
    }
  ]
}