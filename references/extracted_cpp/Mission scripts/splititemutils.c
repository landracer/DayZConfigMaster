// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/splititemutils.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SplitItemUtils
{
    static void TakeOrSplitToInventory ( notnull PlayerBase player, notnull EntityAI target, notnull EntityAI item)
    {
        ItemBase item_base = ItemBase.Cast( item );
        
        if( !item.GetInventory().CanRemoveEntity() || !player.CanManipulateInventory() )
            return;
        
        InventoryLocation il = new InventoryLocation;
        if( target.GetInventory().FindFreeLocationFor( item, FindInventoryLocationType.ANY, il) )
        {
            if( item_base.GetTargetQuantityMax(il.GetSlot()) >= item_base.GetQuantity() )
            {
                if( il.GetType() == InventoryLocationType.ATTACHMENT )
                {
                    player.PredictiveTakeEntityToTargetAttachmentEx(il.GetParent(), item, il.GetSlot());
                }
                else
                {
                    InventoryLocation src = new InventoryLocation;
                    if (item.GetInventory().GetCurrentInventoryLocation(src))
                        player.PredictiveTakeToDst(src, il);
                
                }
            }
            else
            {
                item_base.SplitIntoStackMaxClient( il.GetParent(), il.GetSlot() );
            }
        }
