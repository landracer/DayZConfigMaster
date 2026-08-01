// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/woodaxe.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class WoodAxe extends ToolBase
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionMineTree);
        AddAction(ActionMineTreeBark);
        AddAction(ActionMineBush);
        //AddAction(ActionRepairPart);
        AddAction(ActionDismantlePart);
        //AddAction(ActionBuildPart);
        AddAction(ActionUnrestrainTarget);
        AddAction(ActionSkinning);
        AddAction(ActionCraftBolts);
