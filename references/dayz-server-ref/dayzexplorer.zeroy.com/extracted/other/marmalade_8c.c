// ======================================================================
// File: marmalade_8c_source.html
// Category: other
// ======================================================================

class Marmalade: Edible_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionForceFeed);
AddAction(ActionEatBig);
}
};