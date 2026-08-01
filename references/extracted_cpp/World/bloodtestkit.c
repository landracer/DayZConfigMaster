// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/medical/bloodtestkit.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BloodTestKit: Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionTestBloodTarget);
        AddAction(ActionTestBloodSelf);
