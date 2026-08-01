// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/facecover_improvised.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FaceCover_Improvised extends Mask_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
