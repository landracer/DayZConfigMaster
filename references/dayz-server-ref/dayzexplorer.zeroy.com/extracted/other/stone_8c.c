// ======================================================================
// File: stone_8c_source.html
// Category: other
// ======================================================================

class Stone extends ItemBase
{
override void SetActions()
{
super.SetActions();
AddAction(ActionAttach);
AddAction(ActionDetach);
AddAction(ActionAttachToConstruction);
}
}