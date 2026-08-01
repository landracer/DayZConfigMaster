// ======================================================================
// File: mp5__railhndgrd_8c_source.html
// Category: other
// ======================================================================

class MP5_RailHndgrd extends Inventory_Base
{
override bool CanDetachAttachment (EntityAI parent)
{
if ( Weapon_Base.Cast(parent) && parent.FindAttachmentBySlotName("weaponFlashlight")/*.IsKindOf("UniversalLight")*/ )
{
return false;
}
return true;
}
}