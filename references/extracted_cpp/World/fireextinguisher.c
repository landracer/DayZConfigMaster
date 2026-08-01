// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/tools/fireextinguisher.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FireExtinguisher: Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionClapBearTrapWithThisItem);
        AddAction(ActionExtinguishFireplaceByExtinguisher);
