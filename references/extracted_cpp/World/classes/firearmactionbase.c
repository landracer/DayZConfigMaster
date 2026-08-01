// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/weapons/firearmactionbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FirearmActionBase : ActionBase
{   
    void FirearmActionBase();
    
    override int GetStanceMask(PlayerBase player)
    {
        return DayZPlayerConstants.STANCEMASK_ALL;
