// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/itemconditioncomponents/ccinotempty.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CCINotEmpty : CCIBase
{
    override bool Can( PlayerBase player, ItemBase item )
    {
        return ( item && item.GetQuantity() > 0 );
