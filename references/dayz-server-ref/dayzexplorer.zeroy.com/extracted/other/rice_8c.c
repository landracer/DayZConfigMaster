// ======================================================================
// File: rice_8c_source.html
// Category: other
// ======================================================================

class Rice: Edible_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionForceFeed);
AddAction(ActionEatBig);
}
};