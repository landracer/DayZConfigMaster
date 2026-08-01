// ======================================================================
// File: nail_8c_source.html
// Category: other
// ======================================================================

class Nail extends ItemBase
{
override void SetActions()
{
super.SetActions();
AddAction(ActionAttachToConstruction);
}
}