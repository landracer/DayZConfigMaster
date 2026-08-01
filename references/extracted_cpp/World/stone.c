// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/stone.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Stone extends ItemBase
{
    override void SetActions()
    {
        super.SetActions();
 
        AddAction(ActionAttach);
        AddAction(ActionDetach);
        AddAction(ActionAttachToConstruction);
