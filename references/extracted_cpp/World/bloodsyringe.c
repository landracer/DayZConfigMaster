// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/bloodcontainerbase/bloodsyringe.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BloodSyringe extends BloodContainerBase
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionGiveBloodTarget);
        AddAction(ActionGiveBloodSelf);
