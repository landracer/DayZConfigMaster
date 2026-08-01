// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/tools/crowbar.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Crowbar: Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionDismantlePart);
        AddAction(ActionSkinning);
        AddAction(ActionMineRock1H);
