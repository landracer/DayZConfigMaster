# Inventory System

The DayZ inventory system manages player and item storage.

## Core Classes

### HumanInventory

Extends `GameInventory` and provides human-specific inventory functionality.

```cpp
class HumanInventory : GameInventory {
    proto native EntityAI GetEntityInHands();
    proto native bool CanAddEntityInHands(EntityAI e);
    proto native EntityAI CreateInHands(string typeName);
    proto native bool CanOpenInventory();
    
    // User reservation system for preventing conflicts
    proto native void SetUserReservedLocation(notnull EntityAI eai, notnull InventoryLocation dst);
    proto native void ClearUserReservedLocation(notnull EntityAI eai);
}
```

**Key Features:**
- Two-handed item holding (GetEntityInHands)
- User reservation system to prevent item conflicts
- Item creation in hands or inventory

### GameInventory

Base class for all inventory systems.

```cpp
class GameInventory : ContainerBase {
    // Inventory operations
    proto native bool AddEntityToInventory(EntityAI e, InventoryLocation loc);
    proto native bool RemoveEntityFromInventory(EntityAI e);
    
    // Query methods
    proto native int GetInventoryContentCount();
    proto native bool HasItemInInventory(typename type);
}
```

## Inventory Locations

Inventory locations define where items are stored:

```cpp
class InventoryLocation {
    EntityAI m_Container;           // Container entity
    int m_SlotId;                   // Slot identifier
    vector m_Position;              // Position offset
    vector m_Angles;                // Orientation angles
}
```

## Common Operations

### Adding Items

```cpp
// Create item in inventory
EntityAI newItem = humanInventory.CreateInInventory("ItemName");

// Add existing entity to inventory
humanInventory.AddEntityToInventory(entity, location);

// Try to add (returns true if successful)
bool canAdd = humanInventory.CanAddEntityToInventory(entity, location);
```

### Removing Items

```cpp
// Remove from specific location
humanInventory.RemoveEntityFromInventory(entity);

// Get item in hands
EntityAI handsItem = humanInventory.GetEntityInHands();
```

## Inventory Slots

Slots are identified by string names:
- "Hands" - Item currently held
- "MainBody" - Main inventory container
- Specific slot IDs for containers

## User Action Integration

Inventory actions use the user action system:

```cpp
class ActionDropItem : ActionBase {
    // Check if player can drop item
    override bool ActionCondition(ParamsActionData actionData) {
        return actionData.m_Man.GetInventory().CanRemoveEntityFromInventory(...);
    }
}
```

## See Also

- [Core Concepts](../04-core-concepts/) - Scripting fundamentals