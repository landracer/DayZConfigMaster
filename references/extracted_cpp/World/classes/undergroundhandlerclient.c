// Source: F:/Games/Dayz/scripts/4_world/classes/undergroundhandlerclient.c Source File
// Extracted from DayZ Code Explorer by Zeroy

enum EUndergroundPresence
{
    NONE,//player is not interacting with underground at any level
    OUTER,//player is on the outskirts of the underdound, some effects are already in effect, while others might not be
    TRANSITIONING,//player has entered underground and is in the process of screen darkening transition
    FULL//the player is now fully entered underground
