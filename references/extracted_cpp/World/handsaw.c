// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/tools/handsaw.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HandSaw: Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionUnrestrainTarget);
        AddAction(ActionMineBush);
        AddAction(ActionSawPlanks);
        AddAction(ActionDismantlePart);
        AddAction(ActionSkinning);
        AddAction(ActionMineTreeBark);
