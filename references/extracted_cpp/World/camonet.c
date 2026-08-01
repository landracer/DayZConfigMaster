// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/camonet.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CamoNet extends ItemBase
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionAttachOnTentProxy); //tents
        AddAction(ActionAttachOnSelection); //basebuilding
        AddAction(ActionAttachToConstruction); //basebuilding 
