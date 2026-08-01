// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/itemconditioncomponents/ccidummy.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CCIDummy : CCIBase
{
    override bool Can( PlayerBase player, ItemBase item )
    {
        return item != null;
