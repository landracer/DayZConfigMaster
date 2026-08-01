// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/medical/anticheminjector.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AntiChemInjector: Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionInjectTarget);
        AddAction(ActionInjectSelf);
