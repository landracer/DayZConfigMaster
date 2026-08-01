// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/morozkohat.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MorozkoHat extends HeadGear_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
