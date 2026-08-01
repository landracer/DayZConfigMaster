// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/tankerhelmet.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TankerHelmet extends HeadGear_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
