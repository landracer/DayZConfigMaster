// ======================================================================
// File: woodenstick_8c_source.html
// Category: other
// ======================================================================

class WoodenStick : ItemBase
{
override void SetActions()
{
super.SetActions();
AddAction(ActionCreateIndoorFireplace);
AddAction(ActionCreateIndoorOven);
AddAction(ActionAttach);
AddAction(ActionDetach);
AddAction(ActionAttachToConstruction);
AddAction(ActionCraftBolts);
}
}