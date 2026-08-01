// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/tacticalshirt_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TacticalShirt_ColorBase extends Top_Base 
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
