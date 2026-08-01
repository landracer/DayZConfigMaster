// Source: F:/Games/Dayz/scripts/3_game/ai/aiworld.c Source File
// Extracted from DayZ Code Explorer by Zeroy

enum PGPolyFlags
{
    NONE,
 
    WALK,           // Ability to walk (ground, grass, road)
    DISABLED,       // Disabled polygon
    DOOR,           // Ability to move through doors
    INSIDE,         // Ability to move inside buildings
 
    SWIM,           // Ability to swim (water)
    SWIM_SEA,       // Ability to swim (sea water)
 
    LADDER,         // Ability to climb on ladders
    JUMP_OVER,      // Ability to do jumps overs
    JUMP_DOWN,      // Ability to jump down
    CLIMB,          // Ability to climb up
    CRAWL,          // Ability to crawl
    CROUCH,         // Ability to crouch
 
    UNREACHABLE,
 
    ALL,
 
    JUMP,           // JUMP_OVER | JUMP_DOWN
    SPECIAL         // JUMP | CLIMB | CRAWL | CROUCH
