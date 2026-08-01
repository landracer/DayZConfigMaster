// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/itemconditioncomponents/ccinotruinedanddry.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CCINotRuinedAndDry extends CCIBase
{
    override bool Can( PlayerBase player, ItemBase item )
    {
        return ( item && !item.IsDamageDestroyed() && item.GetWet() <= 0 ) ;
