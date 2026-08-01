// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/legcover_improvised.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class LegsCover_Improvised extends Pants_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
