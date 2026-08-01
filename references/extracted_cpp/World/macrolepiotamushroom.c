// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/edible_base/macrolepiotamushroom.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MacrolepiotaMushroom : MushroomBase
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionCreateIndoorFireplace);
        AddAction(ActionCreateIndoorOven);
