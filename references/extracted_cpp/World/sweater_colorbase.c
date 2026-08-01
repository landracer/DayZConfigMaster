// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/sweater_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Sweater_ColorBase extends Top_Base 
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
