// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/hammer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hammer extends Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionRepairPart);
        AddAction(ActionMineRock1H);
        AddAction(ActionBuildPart);
