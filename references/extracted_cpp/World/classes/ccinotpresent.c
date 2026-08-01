// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/itemconditioncomponents/ccinotpresent.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CCINotPresent : CCIBase
{
    override bool Can( PlayerBase player, ItemBase item )
    {   
        return !item;
