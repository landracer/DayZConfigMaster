// ======================================================================
// File: tacticalbaconcan__opened_8c_source.html
// Category: other
// ======================================================================

class TacticalBaconCan_Opened: Edible_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionForceFeedCan);
AddAction(ActionEatCan);
}
}