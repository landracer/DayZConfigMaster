// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/santasbeard.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SantasBeard extends Mask_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
