// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/food/tacticalbaconcan_opened.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TacticalBaconCan_Opened: Edible_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionForceFeedCan);
        AddAction(ActionEatCan);
