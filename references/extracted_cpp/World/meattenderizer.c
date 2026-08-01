// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/tools/meattenderizer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MeatTenderizer: Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionBuildPart);
        AddAction(ActionRepairPart);
        AddAction(ActionMineRock1H);
 
 
