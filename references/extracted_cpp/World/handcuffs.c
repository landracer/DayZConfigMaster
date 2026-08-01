// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/tools/handcuffs.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Handcuffs: Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionRestrainTarget);
        AddAction(ActionRestrainSelf);
