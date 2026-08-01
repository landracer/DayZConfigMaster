// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/woodenstick.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class WoodenStick : ItemBase
{
    override void SetActions()
    {
        super.SetActions();
 
        AddAction(ActionCreateIndoorFireplace);
        AddAction(ActionCreateIndoorOven);
        AddAction(ActionAttach);
        AddAction(ActionDetach);
        AddAction(ActionAttachToConstruction);
        AddAction(ActionCraftBolts);
