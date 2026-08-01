// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/navyuniformpants.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class NavyUniformPants : Pants_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
