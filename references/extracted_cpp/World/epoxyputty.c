// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/tools/epoxyputty.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class EpoxyPutty: Inventory_Base 
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionRepairCarPart); //Heals ONLY global health for now
        AddAction(ActionRepairCarChassis);
