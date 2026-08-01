// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionlockattachment.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//Transfer locktypes from BasicDefines.hpp here. This is mostly for readability
//NONE must be 0 as it is actively used below
enum eLockTypes
{
    NONE = 0,
    LOCK_SCREW = 1,
    LOCK_BOLT = 2,
    LOCK_NUT = 3,
    LOCK_WIRE = 4
