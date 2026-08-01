// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/inventory_base/mosin_compensator.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Mosin_Compensator extends ItemSuppressor
{
    override bool CanPutAsAttachment( EntityAI parent )
    {
        if (!super.CanPutAsAttachment(parent))
            return false;
        
        if (parent.IsSlotReserved(InventorySlots.GetSlotIdFromString(&quot;suppressorImpro&quot;)) || parent.IsSlotReserved(InventorySlots.GetSlotIdFromString(&quot;pistolMuzzle&quot;)))
            return false;
        
        if ( parent.FindAttachmentBySlotName(&quot;suppressorImpro&quot;) == NULL && parent.FindAttachmentBySlotName(&quot;pistolMuzzle&quot;) == NULL )
        {
            return true;
        }
        return false;
