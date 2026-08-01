// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/edible_base/agaricusmushroom.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AgaricusMushroom : MushroomBase
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionCreateIndoorFireplace);
        AddAction(ActionCreateIndoorOven);
