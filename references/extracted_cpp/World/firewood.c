// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/firewood.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Firewood extends ItemBase
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionClapBearTrapWithThisItem);
 
        AddAction(ActionCreateIndoorFireplace);
        AddAction(ActionCreateIndoorOven);
        AddAction(ActionAttach);
        AddAction(ActionDetach);
