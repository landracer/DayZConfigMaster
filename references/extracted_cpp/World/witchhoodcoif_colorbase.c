// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/witchhoodcoif_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class WitchHoodCoif_ColorBase: HeadGear_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
        AddAction(ActionDeCraftWitchHoodCoif);
