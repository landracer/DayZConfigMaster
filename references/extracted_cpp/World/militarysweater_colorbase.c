// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/militarysweater_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MilitarySweater_ColorBase : Top_Base 
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
