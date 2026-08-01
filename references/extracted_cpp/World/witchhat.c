// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/witchhat.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class WitchHat extends HeadGear_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
