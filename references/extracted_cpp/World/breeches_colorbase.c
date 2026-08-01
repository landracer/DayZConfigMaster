// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/breeches_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Breeches_ColorBase extends Pants_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
