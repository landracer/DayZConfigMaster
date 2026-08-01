// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/witchhood.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class WitchHood extends Clothing //old name or typo? Leaving it for purposes of backwards compatibility
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
