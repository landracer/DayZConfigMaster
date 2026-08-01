// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/switchable_base.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Switchable_Base extends ItemBase
{
    override void OnWasAttached( EntityAI parent, int slot_id )
    {
        super.OnWasAttached( parent, slot_id );
        
        ItemBase parent_item;
        if ( Class.CastTo(parent_item,parent) )
        {
            parent_item.AddLightSourceItem(this);
        }
