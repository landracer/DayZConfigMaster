// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/suppressorbase/pistolsuppressor.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PistolSuppressor extends ItemSuppressor
{
    override bool CanPutAsAttachment( EntityAI parent )
    {
        if (!super.CanPutAsAttachment(parent)) 
            return false;
        
        if (parent.IsSlotReserved(InventorySlots.GetSlotIdFromString(&quot;suppressorImpro&quot;)) || parent.IsSlotReserved(InventorySlots.GetSlotIdFromString(&quot;weaponMuzzleMP5&quot;)))
            return false;
        
        if ( parent.FindAttachmentBySlotName(&quot;suppressorImpro&quot;) == NULL && parent.FindAttachmentBySlotName(&quot;weaponMuzzleMP5&quot;) == NULL )
        {
            return true;
        }
        return false;
