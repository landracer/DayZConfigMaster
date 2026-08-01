// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/bloodcontainerbase/bloodbagempty.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BloodBagEmpty extends BloodContainerBase
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionCollectBloodTarget);
        AddAction(ActionCollectBloodSelf);
