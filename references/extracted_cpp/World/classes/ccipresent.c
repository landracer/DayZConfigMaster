// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/itemconditioncomponents/ccipresent.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CCIPresent : CCIBase
{
    override bool Can( PlayerBase player, ItemBase item )
    {   
        return item != null;
