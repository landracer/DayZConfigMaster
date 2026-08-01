// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/inventory_base/candle.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Candle extends Inventory_Base
{
    override bool CanPutInCargo( EntityAI parent )
    {
        if( !super.CanPutInCargo(parent) ) {return false;}
        if (parent && !parent.HasSelection(&quot;flame&quot;))
        {
            return true;
        }
        return false;
