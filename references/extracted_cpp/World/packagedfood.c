// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/food/packagedfood.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Honey: Edible_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionForceFeedSmall);
        AddAction(ActionEatSmall);
