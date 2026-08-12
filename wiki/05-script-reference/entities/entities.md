# Entities - DayZ Script Reference

Entities are the fundamental objects in DayZ. This document covers key entity classes.

## Base Classes

### EntityAI

Base class for all interactive entities in the game.

```cpp
class EntityAI : WorldObject {
    // Core properties
    float GetHealth();                    // Current health (0-100)
    string GetTypeName();                 // Type name (e.g., "SurvivorMale_DZ")
    
    // Inventory operations
    Inventory GetInventory();             // Get inventory component
    
    // Position and transform
    vector GetPosition();                 // World position
    vector GetOrientation();              // Orientation angles
    
    // Damage system
    void SetHealth(string zone, float value);
    void TakeDamage(int damageType, vector pos, float amount);
}
```

**Key Features:**
- Health management with damage zones
- Inventory integration
- Position and transform operations

### Building

Base class for all structures and buildings.

```cpp
class Building : EntityAI {
    // Door system
    int GetDoorCount();                   // Number of doors
    float GetDoorAngle(int index);        // Current door angle (0-1)
    void SetDoorAngle(int index, float angle);
    
    // Lock system
    bool IsLocked();                      // Check if locked
    void Lock();
    void Unlock();
}
```

### Man

Base class for all player characters.

```cpp
class Man : Human {
    // Input controller
    HumanInputController GetInputController();
    
    // Actions and state
    void SetActionMode(string mode);
    bool IsInVehicle();                   // Check if in vehicle
    
    // State queries
    bool IsAlive();                       // Check if alive
    bool IsHandsEmpty();                  // Check if no items held
}
```

## Entity Creation

### Spawning Entities

```cpp
// Spawn by type name
EntityAI entity = GetGame().SpawnEntity("ItemName");

// Spawn at position
vector pos = "100 0 200";
GetGame().SnapshotInsert(entity, pos);

// Spawn from template
vobject templateRes = vobject.LoadFromFile("path/to/template.vorgen");
GetGame().SpawnEntityTemplate(templateRes);
```

## Entity Lifecycle

1. **Creation** - Spawning entity with GetGame().SpawnEntity()
2. **Initialization** - Init() method called automatically
3. **Active** - Game loop calls Update() methods
4. **Destruction** - Destroy() or RemoveFromWorld()

## Common Operations

### Moving Entities

```cpp
// Set position
entity.SetPosition("100 0 200");

// Get position
vector pos = entity.GetPosition();
Print("Entity at: " + pos);
```

### Checking Entity Type

```cpp
if (entity.IsInherited(Man)) {
    Print("This is a player character");
}

typename entityType = entity.GetType();
if (entityType == ItemBase.Type()) {
    Print("This is an item");
}
```

## See Also

- [Core Concepts](../04-core-concepts/) - Scripting fundamentals