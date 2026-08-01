// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/prisonuniformpants.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PrisonUniformPants extends Pants_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
