// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/fencekit.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FenceKit extends KitBase
{
    override bool CanReceiveAttachment(EntityAI attachment, int slotId)
    {
        if ( !super.CanReceiveAttachment(attachment, slotId) )
            return false;
        
        ItemBase att = ItemBase.Cast(GetInventory().FindAttachment(slotId));
        if (att)
            return false;
        
        return true;
