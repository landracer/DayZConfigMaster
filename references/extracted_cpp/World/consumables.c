// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/consumables/consumables.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Bandana_ColorBase: Clothing
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionBandageTarget);
        AddAction(ActionBandageSelf);
        AddAction(ActionWringClothes);
