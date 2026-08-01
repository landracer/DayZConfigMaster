// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/inventory.c Source File
// Extracted from DayZ Code Explorer by Zeroy

#ifdef PLATFORM_CONSOLE
enum ConsoleActionToolbarMask
{
    EMPTY                   = 0,
    TO_HANDS_SWAP_VICINITY  = 1,
    TO_HANDS_SWAP_INVENTORY = 2,
    DROP                    = 4,
    EQUIP                   = 8,
    SPLIT                   = 16,
    TO_INVENTORY            = 32,
    OPEN_CLOSE_CONTAINER    = 64,
    MICROMANAGMENT          = 128,
    QUICKSLOT               = 256,
    COMBINE                 = 512,
}
#endif
 
enum Direction
{
    RIGHT,
    LEFT,
    UP,
    DOWN
