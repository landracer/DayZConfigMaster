// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/inventory_base/m4_rishndgrd.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class M4_RISHndgrd extends Inventory_Base
{
    override bool CanDetachAttachment (EntityAI parent)
    {
        if ( Weapon_Base.Cast(parent) && parent.FindAttachmentBySlotName(&quot;weaponFlashlight&quot;) )
        {
            return false;
        }
        return true;
