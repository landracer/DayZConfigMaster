// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/headcover_improvised.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HeadCover_Improvised extends HeadGear_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
