// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/tshirt_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TShirt_ColorBase extends Top_Base 
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
