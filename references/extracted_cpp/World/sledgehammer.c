// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/sledgehammer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SledgeHammer extends Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionClapBearTrapWithThisItem);
        AddAction(ActionMineRock);
        AddAction(ActionDismantlePart);
        AddAction(ActionBuildPart);
