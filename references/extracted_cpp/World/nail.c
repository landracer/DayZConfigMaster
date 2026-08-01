// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/nail.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Nail extends ItemBase
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionAttachToConstruction);
