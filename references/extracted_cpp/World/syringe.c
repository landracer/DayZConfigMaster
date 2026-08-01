// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/bloodcontainerbase/syringe.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Syringe extends BloodContainerBase
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionCollectSampleTarget);
        AddAction(ActionCollectSampleSelf);
