// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/suppressorbase/ak_suppressor.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AK_Suppressor extends ItemSuppressor
{
    override bool CanPutAsAttachment( EntityAI parent )
    {       
        if (!super.CanPutAsAttachment(parent)) 
            return false;
        
        if (parent.IsSlotReserved(InventorySlots.GetSlotIdFromString(&quot;suppressorImpro&quot;)) || parent.IsSlotReserved(InventorySlots.GetSlotIdFromString(&quot;weaponBayonetAK&quot;)))
            return false;
        
        if ( parent.FindAttachmentBySlotName(&quot;suppressorImpro&quot;) == NULL && parent.FindAttachmentBySlotName(&quot;weaponBayonetAK&quot;) == NULL )
        {
            return true;
        }
        return false;
