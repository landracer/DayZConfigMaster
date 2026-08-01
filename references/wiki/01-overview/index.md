# Overview - DayZ Server Reference

This section provides introductions to DayZ server scripting and configuration concepts.

## What is DayZ Server Scripting?

DayZ uses a C++-like scripting language (called Enforce) for its game logic. This scripting system handles:

1. **Entities** - Players, NPCs, buildings, items
2. **Systems** - Inventory, AI, temperature, combat
3. **Actions** - Player interactions with the world
4. **GUI** - User interface elements

## Script File Structure

DayZ scripts follow a specific structure:

```cpp
// 1. Include statements (import dependencies)
#include "script.c"
#include "gamelib.c"

// 2. Class definitions
class MyClass : BaseClass
{
    // Member variables
    float m_Value;
    
    // Constructor/Init
    void Init()
    {
        m_Value = 0;
    }
    
    // Methods
    void DoSomething()
    {
        // Method body
    }
}
```

## Key Features

- **Object-oriented**: Classes inherit from BaseClass or other classes
- **Type-safe**: Uses Enforce language syntax
- **Component-based**: Many entities have components (UserActionsComponent, Inventory)
- **Event-driven**: Uses timers and callbacks for async operations

## Where to Find Documentation

- [Command Line Tools](../02-command-line-tools) - PBO management, server launch
- [Python API](../03-python-api) - dayz-dev-tools programmatic access
- [Core Concepts](../04-core-concepts) - Scripting fundamentals
- [Script Reference](../05-script-reference) - C++ source code with annotations

## Related Resources

- [DayZ Dev Tools Documentation](https://dayz-dev-tools.readthedocs.io/)
- [DayZ Explorer](https://dayzexplorer.zeroy.com/)