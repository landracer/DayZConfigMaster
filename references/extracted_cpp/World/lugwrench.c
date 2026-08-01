// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/tools/lugwrench.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class LugWrench: Inventory_Base 
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionLockAttachment);
