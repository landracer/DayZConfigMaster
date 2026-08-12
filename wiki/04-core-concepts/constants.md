# Constants Reference

DayZ uses constant values throughout its scripting system for configuration and behavior control.

## Input Constants

Defined in `1_core/constants.c`:

| Constant | Value | Description |
|----------|-------|-------------|
| `INPUT_MODULE_TYPE_MASK` | 0x00700000 | Mask for input module type |
| `INPUT_KEY_MASK` | 0x000000ff | Mask for key identifier |
| `INPUT_ACTION_TYPE_MASK` | 0x00000f00 | Mask for action types |
| `INPUT_AXIS` | 0x00010000 | Axis input flag |
| `INPUT_POV` | 0x00020000 | Point-of-view input flag |

## Input Device Types

| Constant | Value | Description |
|----------|-------|-------------|
| `INPUT_DEVICE_KEYBOARD` | 0x00000000 | Keyboard device |
| `INPUT_DEVICE_MOUSE` | 0x00100000 | Mouse button |
| `INPUT_DEVICE_STICK` | 0x00200000 | Joystick/stick |
| `INPUT_DEVICE_XINPUT` | 0x00300000 | XInput device (Xbox controller) |
| `INPUT_DEVICE_TRACKIR` | 0x00400000 | TrackIR head tracking |
| `INPUT_DEVICE_GAMEPAD` | 0x00500000 | Gamepad |
| `INPUT_DEVICE_CHEAT` | 0x00600000 | Cheat input device |

## Action Types

| Constant | Value | Description |
|----------|-------|-------------|
| `INPUT_ACTION_TYPE_NONE` | 0x00000000 | No action |
| `INPUT_ACTION_TYPE_STATE` | 0x00000100 | State-based (held) action |
| `INPUT_ACTION_TYPE_DOWN_EVENT` | 0x00000200 | Key press event |
| `INPUT_ACTION_TYPE_UP_EVENT` | 0x00000300 | Key release event |
| `INPUT_ACTION_TYPE_SHORTCLICK_EVENT` | 0x00000400 | Short click event |
| `INPUT_ACTION_TYPE_HOLD_EVENT` | 0x00000500 | Hold event |

## Material Types

| Constant | Value | Description |
|----------|-------|-------------|
| `MATERIAL_DEFAULT` | 0 | Default material type |
| `MATERIAL_METAL` | 1 | Full steel |
| `MATERIAL_IRON` | 2 | Iron |
| `MATERIAL_GLASS` | 3 | Glass pane |
| `MATERIAL_PLASTIC` | 4 | Plastic objects |
| `MATERIAL_LIQUID` | 5 | Liquids (water) |
| `MATERIAL_SLIME` | 6 | Slime, oil |
| `MATERIAL_BETON` | 7 | Concrete |
| `MATERIAL_RUBBER` | 8 | Rubber, linoleum |
| `MATERIAL_FLESH` | 9 | Flesh (humanoids) |
| `MATERIAL_GRASS` | 10 | Grass |
| `MATERIAL_WOOD` | 11 | Wood |
| `MATERIAL_SNOW` | 12 | Snow |
| `MATERIAL_SAND` | 13 | Soft sand |
| `MATERIAL_DIRT` | 14 | Super-soft dirt |
| `MATERIAL_GRAVEL` | 15 | Gravel |
| `MATERIAL_STONE` | 16 | Rocks, cliffs |

## Color Constants

Colors are stored as ARGB values (Alpha, Red, Green, Blue):

| Constant | Value | Description |
|----------|-------|-------------|
| `COLOR_WHITE` | 0xFFFFFFFF | White (full opacity) |
| `COLOR_RED` | 0xFFF22613 | Red |
| `COLOR_GREEN` | 0xFF2ECC71 | Green |
| `COLOR_BLUE` | 0xFF4B77BE | Blue |
| `COLOR_YELLOW` | 0xFFF7CA18 | Yellow |

## String Constants

| Constant | Value | Description |
|----------|-------|-------------|
| `STRING_EMPTY` | "" | Empty string reference |

## Script Files with Constants

- `1_core/constants.c` - Core engine constants
- `3_game/constants.c` - Game-specific constants
- `4_world/classes/_constants.c` - World/mission constants

## Usage Example

```cpp
// Check if an entity is made of metal
if (entity.GetMaterial() == MATERIAL_METAL) {
    Print("This object is made of steel!");
}