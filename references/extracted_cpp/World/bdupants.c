// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/bdupants.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BDUPants extends Pants_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
