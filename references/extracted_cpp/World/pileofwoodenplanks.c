// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/pileofwoodenplanks.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PileOfWoodenPlanks extends ItemBase
{   
    void PileOfWoodenPlanks()
    {
        if ( g_Game.IsServer() )
        {
            SetAllowDamage(false);
        }
