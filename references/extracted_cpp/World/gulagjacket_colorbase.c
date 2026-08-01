// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/gulagjacket_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class GulagJacket_ColorBase : Top_Base 
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
