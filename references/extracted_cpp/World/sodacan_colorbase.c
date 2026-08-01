// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/drinks/sodacan_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SodaCan_ColorBase : Edible_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionForceDrink);
        AddAction(ActionDrinkCan);
