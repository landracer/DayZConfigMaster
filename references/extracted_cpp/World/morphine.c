// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/medical/morphine.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Morphine: Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionInjectMorphineTarget);
        AddAction(ActionInjectMorphineSelf);
