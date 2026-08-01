// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/woolglovesfingerless_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class WoolGlovesFingerless_ColorBase extends Gloves_Base 
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
