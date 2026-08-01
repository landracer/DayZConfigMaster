// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/drysackbag_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DrysackBag_ColorBase: Backpack_Base
{
    
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionDeCraftDrysackBag);
