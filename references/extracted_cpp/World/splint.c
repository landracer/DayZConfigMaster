// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/medical/splint.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Splint: Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionSplintTarget);
        AddAction(ActionSplintSelf);
