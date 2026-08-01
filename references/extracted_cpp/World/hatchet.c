// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/hatchet.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hatchet extends ToolBase
{
    override void SetActions()
    {
        super.SetActions();
 
        AddAction(ActionMineTree);
        AddAction(ActionMineTreeBark);
        AddAction(ActionMineBush);
        AddAction(ActionRepairPart);
        AddAction(ActionDismantlePart);
        AddAction(ActionBuildPart);
        //AddAction(ActionSawPlanks);
        AddAction(ActionUnrestrainTarget);
        AddAction(ActionSkinning);
        AddAction(ActionCraftBolts);
        AddAction(ActionDigWorms);
