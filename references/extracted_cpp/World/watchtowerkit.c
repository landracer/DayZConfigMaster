// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/watchtowerkit.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class WatchtowerKit extends KitBase
{
    override bool CanReceiveAttachment(EntityAI attachment, int slotId)
    {
        ItemBase att = ItemBase.Cast(GetInventory().FindAttachment(slotId));
        if (att)
            return false;
        
        return super.CanReceiveAttachment(attachment, slotId);
