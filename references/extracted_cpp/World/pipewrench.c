// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/pipewrench.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PipeWrench extends Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        //AddAction(ActionDismantlePart);
        AddAction(ActionMineRock);
