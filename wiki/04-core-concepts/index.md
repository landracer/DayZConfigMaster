# Core Concepts - DayZ Scripting Fundamentals

This section covers the fundamental concepts of DayZ scripting.

## The Enforce Language

DayZ uses a C++-like language called **Enforce**. Key characteristics:

### Syntax Overview

```cpp
// Comments start with //
class ClassName : ParentClass {
    // Member variables (m_ prefix convention)
    float m_MyValue;
    
    // Methods
    void MyMethod() {
        Print("Hello");
    }
}
```

### Type System

| Type | Description |
|------|-------------|
| `bool` | True/false value |
| `int`, `float` | Numeric types |
| `string` | Text string |
| `vector` | 3D vector (x, y, z) |
| `typename` | Class type reference |

## Script Structure

DayZ scripts follow a specific organization:

```
/scripts/
├── 1_core/          # Core engine functions
├── 2_gamelib/       # Game library classes
├── 3_game/          # Game entities and systems
├── 4_world/         # World-specific implementations
└── 5_mission/       # Mission-specific scripts
```

## Key Concepts

### 1. Classes

All DayZ objects are classes that inherit from BaseClass:

```cpp
class MyClass : BaseClass {
    void Init() {
        // Constructor logic
    }
}
```

### 2. Native Functions

Functions marked with `proto native` are implemented in C++:

```cpp
proto native void Print(string text);
proto native bool IsDay();
```

### 3. References

The `reference` keyword creates a weak reference (doesn't prevent garbage collection):

```cpp
reference float g_GlobalVar;  // Global variable accessible from anywhere
```

### 4. Enums

Enumerations define named constants:

```cpp
enum MyEnum {
    VALUE1,
    VALUE2,
    VALUE3
}
```

## Constants Reference

Common constant categories:

| Category | Example |
|----------|---------|
| Input Actions | `INPUT_ACTION_TYPE_DOWN_EVENT` |
| Materials | `MATERIAL_METAL`, `MATERIAL_WOOD` |
| Colors | `COLOR_WHITE`, `COLOR_RED` |
| Game States | `EGameState::PLAYING` |

## See Also

- [Script Reference](../05-script-reference/) - C++ source code with annotations