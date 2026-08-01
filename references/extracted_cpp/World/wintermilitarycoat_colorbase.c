// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/wintermilitarycoat_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class WinterMilitaryCoat_ColorBase : Top_Base 
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
