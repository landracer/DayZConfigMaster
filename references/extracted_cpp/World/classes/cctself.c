// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/targetconditionscomponents/cctself.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CCTSelf : CCTBase
{
    override bool Can( PlayerBase player, ActionTarget target )
    {
        return ( player && !player.IsDamageDestroyed());
