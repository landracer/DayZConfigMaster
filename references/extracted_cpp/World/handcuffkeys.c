// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/tools/handcuffkeys.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HandcuffKeys: Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionUnrestrainTarget);
