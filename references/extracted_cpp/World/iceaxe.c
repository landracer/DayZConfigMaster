// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/tools/iceaxe.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Iceaxe: Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionMineRock1H);
        AddAction(ActionDigWorms);
        AddAction(ActionSkinning);
        AddAction(ActionMineBush);
        AddAction(ActionMineTreeBark);
        AddAction(ActionUnrestrainTarget);
