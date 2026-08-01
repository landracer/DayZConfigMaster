// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/santashat.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SantasHat extends HeadGear_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
