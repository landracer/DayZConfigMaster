// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/weaponattachments/attachments.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ButtstockBase extends Inventory_Base
{
    override void OnWasAttached(EntityAI parent, int slot_id)
    {
        super.OnWasAttached(parent, slot_id);
        
        if( parent.IsWeapon() )
        {
            parent.SetButtstockAttached(true,slot_id);
        }       
