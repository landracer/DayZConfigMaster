// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/policepantsorel.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PolicePantsOrel extends Pants_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
