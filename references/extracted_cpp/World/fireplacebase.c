// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/fireplacebase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

enum FireplaceFireState
{
    NO_FIRE             = 1,
    START_FIRE          = 2,
    SMALL_FIRE          = 3,    
    NORMAL_FIRE         = 4,
    END_FIRE            = 5,
    EXTINGUISHING_FIRE  = 6,
    EXTINGUISHED_FIRE   = 7,
    REIGNITED_FIRE      = 8,
    
    COUNT               = 9         //for net sync purposes
