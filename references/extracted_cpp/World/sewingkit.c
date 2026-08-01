// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/tools/sewingkit.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SewingKit: Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionRepairTent);
        AddAction(ActionRepairShelter);
        AddAction(ActionSewTarget);
        AddAction(ActionSewSelf);
