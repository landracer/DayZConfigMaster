# Types and Variables - DayZ Scripting

DayZ uses a type-safe language called **Enforce**. This page documents the core types and variable declarations.

## Core Types

| Type | Description | Example |
|------|-------------|---------|
| `bool` | Boolean (true/false) | `bool isAlive = true;` |
| `int` | 32-bit integer | `int health = 100;` |
| `float` | Floating point number | `float distance = 10.5f;` |
| `string` | Text string | `string playerName = "Survivor";` |
| `vector` | 3D vector (x, y, z) | `vector position = "100 0 200";` |
| `typename` | Class type reference | `typename myType = Man.Type();` |

## Variable Declarations

### Standard Variables
```cpp
// Regular variable with value initialization
float m_Value = 0.0;

// Constant (immutable)
const float GRAVITY = 9.81;
```

### Reference Variables
The `reference` keyword creates a weak reference that doesn't prevent garbage collection:
```cpp
reference float g_GlobalVariable;      // Global variable accessible anywhere
reference Man m_Player;                // Weak reference to player object
```

### Proto Native Functions
Functions declared with `proto native` are implemented in C++:
```cpp
proto native void Print(string text);         // Output to console
proto native bool IsDay();                    // Check if it's daytime
proto native vector GetPosition();            // Get entity position
```

## Access Modifiers

| Modifier | Description |
|----------|-------------|
| `private` | Only accessible within the class |
| `protected` | Accessible in derived classes |
| `public` | Accessible from anywhere |

## Variable Naming Convention

DayZ scripts use **m_ prefix** for member variables:
```cpp
class MyClass : BaseClass {
    float m_MyValue;       // Member variable
    string m_Name;         // String member
    
    void Init() {
        m_MyValue = 10;
        m_Name = "Example";
    }
}
```

## Type Conversion

### Explicit Casting
```cpp
// Cast to specific type
Man player = Man.Cast(someEntity);

// Check if cast is valid
if (player) {
    Print("Successfully cast to Man");
}
```

### Enum Types
Enumerations define named constants:
```cpp
enum MyEnum {
    VALUE1,    // 0
    VALUE2,    // 1
    VALUE3     // 2
}

MyEnum myValue = MyEnum.VALUE1;