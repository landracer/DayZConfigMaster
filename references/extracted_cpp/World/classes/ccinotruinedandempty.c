// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/itemconditioncomponents/ccinotruinedandempty.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CCINotRuinedAndEmpty : CCIBase
{
    override bool Can( PlayerBase player, ItemBase item )
    {
        return ( item && !item.IsDamageDestroyed() && item.GetQuantity() > 0);
