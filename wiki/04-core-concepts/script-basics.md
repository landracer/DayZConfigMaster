# Script Basics - DayZ Scripting Fundamentals

This page covers the fundamental structure and syntax of DayZ scripts.

## File Organization

DayZ scripts are organized in a hierarchical directory structure:

```
/scripts/
├── 1_core/          # Core engine functions (script.c, enmath.c)
├── 2_gamelib/       # Game library classes (gamelib.c, tools.c)
├── 3_game/          # Main game code
│   ├── entities/    # Entity classes (Man, Building, etc.)
│   ├── systems/     # System implementations (Inventory, AI)
│   ├── effects/     # Visual/sound effects
│   └── gui/         # GUI components
├── 4_world/         # World-specific implementations
└── 5_mission/       # Mission-specific scripts
```

## Script Structure

A typical DayZ script follows this structure:

```cpp
// ============================================
// File Header - Metadata
// ============================================
// Source: F:/Games/Dayz/scripts/X_category/scriptname.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// ============================================
// Include Statements - Import dependencies
// ============================================
#include "script.c"
#include "gamelib.c"

// ============================================
// Type Declarations - Enums, typedefs
// ============================================
enum MyEnum {
    VALUE1,
    VALUE2,
    VALUE3
}

typedef Param1<int> MyParams;

// ============================================
// Class Definitions
// ============================================
class MyClass : BaseClass
{
    // Member variables (m_ prefix convention)
    float m_MyValue;
    string m_Name;
    
    // Constructor/Init method
    void Init()
    {
        m_MyValue = 0.0;
        m_Name = "Default";
    }
    
    // Class methods
    void DoSomething()
    {
        Print("Performing action");
        m_MyValue += 1.0;
    }
    
    // Native function declaration (implemented in C++)
    proto native void NativeFunction();
}

// ============================================
// Global Functions (if needed)
// ============================================
void GlobalFunction()
{
    Print("This is a global function");
}
```

## Class Declaration Syntax

```cpp
class ClassName : ParentClass {
    // 1. Member variables (private by default, m_ prefix)
    float m_Value;
    
    // 2. Constructor (same name as class)
    void ClassName()
    {
        // Initialization code
    }
    
    // 3. Destructor
    void ~ClassName()
    {
        // Cleanup code
    }
    
    // 4. Methods
    void MyMethod() {
        // Method body
    }
}
```

## Native vs Script Functions

| Type | Syntax | Description |
|------|--------|-------------|
| **Script** | `void Func()` | Implemented in Enforce script |
| **Native** | `proto native void Func()` | Implemented in C++ |

## Common Built-in Functions

```cpp
Print(string text);           // Output to console
GetGame();                    // Get global game instance
SpawnEntity(typename type);   // Spawn entity by type name
GetPosition();                // Get entity position (vector)
SetPosition(vector pos);      // Set entity position
```

## Script Lifecycle

1. **Load** - Scripts are loaded from .c files
2. **Parse** - Enforce parser compiles scripts
3. **Initialize** - Classes instantiated, Init() called
4. **Update** - Game loop calls Update() methods
5. **Cleanup** - Desctructors called on shutdown

## Best Practices

1. Always use `m_` prefix for member variables
2. Override parent class methods with `override` keyword
3. Use `proto native` only when implementing C++ functions
4. Add comments to explain complex logic
5. Follow the existing directory structure