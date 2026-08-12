# Script Reference - C++ Code Documentation

This section contains DayZ C++ source code organized by functional category, with detailed annotations.

## Categories

### Entities

Classes representing in-game entities (players, NPCs, buildings):

| File | Description |
|------|-------------|
| [building.c](./entities/building/) | Building/structure classes |
| [human.c](./entities/human/) | Player/Human character class |
| [entityai.c](./entities/entityai/) | Base AI entity class |

### Systems

Game systems that manage mechanics:

| File | Description |
|------|-------------|
| [inventory.c](./systems/inventory/) | Item inventory management system |
| [universaltemperaturesource.c](./systems/temperature/) | Temperature calculations and effects |
| [arrowmanagerbase.c](./systems/arrows/) | Projectile tracking system |

### Tools & Components

Utility classes and interfaces:

| File | Description |
|------|-------------|
| [component.c](./tools/component/) | Component base class |
| [tools.c](./tools/tools/) | Utility tools and helpers |
| [debug.c](./tools/debug/) | Debug utilities |

### Actions

Player interaction actions (built from ActionBase):

| Category | Examples |
|----------|----------|
| Eating/Drinking | Drink, Eat, Consume |
| Medical | Bandage, CPR, Blood transfusion |
| Crafting | Craft, Dismantle, Build |
| Interaction | Open doors, Close containers |

### Entities

Classes representing in-game entities (players, NPCs, buildings):

| File | Description |
|------|-------------|
| [man.c](./entities/man/) | Player character class |
| [human.c](./entities/human/) | Human base class with input controller |
| [building.c](./entities/building/) | Building/structure classes |

See also:
- [Entities Overview](./entities/entities.md) - Entity system overview
- [Inventory System](./systems/inventory.md) - Inventory documentation

## Script Organization

DayZ scripts are organized in a hierarchical structure:

```
/scripts/
├── 1_core/          # Core engine (script.c, enmath.c, physics.c)
├── 2_gamelib/       # Game library (gamelib.c, tools.c)
├── 3_game/          # Main game code
│   ├── entities/    # Entity classes
│   ├── systems/     # System implementations
│   ├── tools/       # Utilities
│   └── effects/     # Visual/sound effects
└── 4_world/         # World-specific overrides
```

## See Also

- [Core Concepts](../04-core-concepts/) - Foundational scripting concepts