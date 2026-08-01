// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/food/powderedmilk.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PowderedMilk: Edible_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionForceFeed);
        AddAction(ActionEatBig);
