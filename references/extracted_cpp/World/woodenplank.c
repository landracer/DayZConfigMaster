// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/woodenplank.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class WoodenPlank extends ItemBase
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionClapBearTrapWithThisItem);
        AddAction(ActionAttachToConstruction);
        AddAction(ActionRepairCarChassis);
        AddAction(ActionAttachOnSelection);
