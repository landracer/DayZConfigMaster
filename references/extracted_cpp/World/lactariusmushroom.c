// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/edible_base/lactariusmushroom.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class LactariusMushroom : MushroomBase
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionCreateIndoorFireplace);
        AddAction(ActionCreateIndoorOven);
