// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/inventory_base/red9bttstck.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Red9Bttstck extends Inventory_Base
{
    override bool CanPutAsAttachment( EntityAI parent )
    {
        if(!super.CanPutAsAttachment(parent)) {return false;}
        if ( !parent.IsKindOf(&quot;PlateCarrierHolster&quot;) && !parent.IsKindOf(&quot;PlateCarrierComplete&quot;) && !parent.IsKindOf(&quot;CarrierHolsterSolo&quot;) && !parent.IsKindOf(&quot;ChestHolster&quot;) )
        {
            return true;
        }
        return false;
