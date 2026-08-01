// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/hacksaw.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hacksaw extends ToolBase
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
