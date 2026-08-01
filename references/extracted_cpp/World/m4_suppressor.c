// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/suppressorbase/m4_suppressor.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class M4_Suppressor extends SuppressorBase
{
    override bool CanPutAsAttachment( EntityAI parent )
    {
        if (!super.CanPutAsAttachment(parent)) 
            return false;
        
        if (parent.IsSlotReserved(InventorySlots.GetSlotIdFromString(&quot;suppressorImpro&quot;)) || parent.IsSlotReserved(InventorySlots.GetSlotIdFromString(&quot;weaponBayonet&quot;)))
            return false;
        
        if ( parent.FindAttachmentBySlotName(&quot;suppressorImpro&quot;) == NULL && parent.FindAttachmentBySlotName(&quot;weaponBayonet&quot;) == NULL )
        {
            return true;
        }
        return false;
