// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/metalplate.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MetalPlate extends ItemBase
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionAttachToConstruction);
        AddAction(ActionAttachOnSelection);
