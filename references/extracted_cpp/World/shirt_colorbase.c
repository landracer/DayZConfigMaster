// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/shirt_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Shirt_ColorBase extends Top_Base 
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
