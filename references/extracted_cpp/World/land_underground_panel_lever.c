// Source: F:/Games/Dayz/scripts/4_world/entities/building/underground/entrance/land_underground_panel_lever.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Land_Underground_Panel_Lever : Land_Underground_Panel
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionUseUndergroundLever);
