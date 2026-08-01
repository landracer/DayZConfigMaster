// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/torsocover_improvised.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TorsoCover_Improvised extends Top_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
