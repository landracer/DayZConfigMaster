// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/optics/binoculars.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Binoculars: ItemOptics
{
    override void SetActions()
    {
        super.SetActions();
        
        RemoveAction(ActionViewOptics);
        AddAction(ActionViewBinoculars);
