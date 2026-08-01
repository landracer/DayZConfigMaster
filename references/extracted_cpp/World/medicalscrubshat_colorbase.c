// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/medicalscrubshat_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MedicalScrubsHat_ColorBase extends HeadGear_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
