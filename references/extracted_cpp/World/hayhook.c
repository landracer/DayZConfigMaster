// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/tools/hayhook.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HayHook: Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionUnrestrainTarget);
        AddAction(ActionSkinning);
