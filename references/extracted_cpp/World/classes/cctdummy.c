// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/targetconditionscomponents/cctdummy.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CCTDummy : CCTBase
{
    override bool Can( PlayerBase player, ActionTarget target )
    {
        return ( target && target.GetObject() );
