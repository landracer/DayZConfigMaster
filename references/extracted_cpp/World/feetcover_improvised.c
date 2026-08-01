// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/feetcover_improvised.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FeetCover_Improvised extends Shoes_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
