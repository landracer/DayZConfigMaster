// ======================================================================
// File: boxcerealcrunchin_8c_source.html
// Category: other
// ======================================================================

class BoxCerealCrunchin extends Edible_Base
{
void BoxCerealCrunchin()
{
}
override void SetActions()
{
super.SetActions();
AddAction(ActionForceFeed);
AddAction(ActionEatCereal);
}
}