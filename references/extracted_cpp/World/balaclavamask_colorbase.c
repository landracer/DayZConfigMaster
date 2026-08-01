// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/balaclavamask_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BalaclavaMask_ColorBase extends Mask_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
