// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/tools/leathersewingkit.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class LeatherSewingKit: Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionRepairShelter);
