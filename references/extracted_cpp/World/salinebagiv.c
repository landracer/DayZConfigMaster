// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/medical/salinebagiv.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SalineBagIV: Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionGiveSalineTarget);
        AddAction(ActionGiveSalineSelf);
