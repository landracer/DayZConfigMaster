// ======================================================================
// File: firewood_8c_source.html
// Category: other
// ======================================================================

class Firewood extends ItemBase
{
override void SetActions()
{
super.SetActions();
AddAction(ActionClapBearTrapWithThisItem);
AddAction(ActionCreateIndoorFireplace);
AddAction(ActionCreateIndoorOven);
AddAction(ActionAttach);
AddAction(ActionDetach);
}
}