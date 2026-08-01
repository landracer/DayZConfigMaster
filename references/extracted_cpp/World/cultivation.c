// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/cultivation/cultivation.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AntiPestsSpray: Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionDisinfectPlantBit);
        AddAction(ActionDisinfectPlant);
        AddAction(ActionForceDrink);
        AddAction(ActionDrink);
