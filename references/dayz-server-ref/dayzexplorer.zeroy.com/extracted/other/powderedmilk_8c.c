// ======================================================================
// File: powderedmilk_8c_source.html
// Category: other
// ======================================================================

class PowderedMilk: Edible_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionForceFeed);
AddAction(ActionEatBig);
}
};