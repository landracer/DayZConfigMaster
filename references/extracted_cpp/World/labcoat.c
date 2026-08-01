// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/labcoat.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class LabCoat extends Top_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
