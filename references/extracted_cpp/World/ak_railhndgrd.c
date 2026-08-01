// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/inventory_base/ak_railhndgrd.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AK_RailHndgrd extends Inventory_Base
{
    override bool CanDetachAttachment (EntityAI parent)
    {
        if ( Weapon_Base.Cast(parent) && parent.FindAttachmentBySlotName(&quot;weaponFlashlight&quot;)/*.IsKindOf(&quot;UniversalLight&quot;)*/ )
        {
            return false;
        }
        return true;
