// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/handscover_improvised.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HandsCover_Improvised extends Gloves_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
