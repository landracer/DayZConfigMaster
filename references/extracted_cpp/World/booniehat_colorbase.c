// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/booniehat_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BoonieHat_ColorBase extends HeadGear_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
