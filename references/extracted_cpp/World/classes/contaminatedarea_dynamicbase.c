// Source: F:/Games/Dayz/scripts/4_world/classes/contaminatedarea/contaminatedarea_dynamicbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

enum eAreaDecayStage
{
    INIT            = 1, // The dynamic area is initializing
    START           = 2, // The dynamic area is starting
    LIVE            = 3, // The dynamic area is live
    DECAY_START     = 4, // The dynamic area decay has started
    DECAY_END       = 5, // The dynamic area will soon be deleted
