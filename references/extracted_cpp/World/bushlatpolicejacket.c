// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/bushlatpolicejacket.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BushlatPoliceJacket_ColorBase : Top_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
