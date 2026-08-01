// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/petushokhat_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PetushokHat_ColorBase extends HeadGear_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
