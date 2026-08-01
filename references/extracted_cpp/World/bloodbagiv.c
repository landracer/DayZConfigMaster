// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/bloodcontainerbase/bloodbagiv.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BloodBagIV extends BloodContainerBase
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionGiveBloodTarget);
        AddAction(ActionGiveBloodSelf);
