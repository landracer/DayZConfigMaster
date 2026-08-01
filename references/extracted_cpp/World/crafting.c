// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/crafting/crafting.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BurlapSack: Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionCoverHeadTarget);
        AddAction(ActionCoverHeadSelf);
