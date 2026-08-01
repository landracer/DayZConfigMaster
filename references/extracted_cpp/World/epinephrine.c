// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/medical/epinephrine.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Epinephrine: Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionInjectEpinephrineTarget);
        AddAction(ActionInjectEpinephrineSelf);
