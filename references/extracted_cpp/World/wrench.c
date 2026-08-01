// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/tools/wrench.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Wrench: Inventory_Base 
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionLockAttachment);
        AddAction(ActionMineRock1H);
