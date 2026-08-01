// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/shelterkit.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ShelterKit extends KitBase
{
    override bool CanReceiveAttachment(EntityAI attachment, int slotId)
    {
        if ( !super.CanReceiveAttachment(attachment, slotId) )
            return false;
        
        ItemBase att = ItemBase.Cast(GetInventory().FindAttachment(slotId));
        if (att)
            return false;
        
        return true;
