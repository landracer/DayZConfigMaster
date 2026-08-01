// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/inventory_base/mp5_compensator.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MP5_Compensator extends ItemSuppressor
{
    override bool CanPutAsAttachment( EntityAI parent )
    {
        if (!super.CanPutAsAttachment(parent)) 
            return false;
        
        if (parent.IsSlotReserved(InventorySlots.GetSlotIdFromString(&quot;pistolMuzzle&quot;)))
            return false;
        
        if ( !parent.FindAttachmentBySlotName(&quot;pistolMuzzle&quot;) )
        {
            return true;
        }
        return false;
