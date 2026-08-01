// ======================================================================
// File: hammer_8c_source.html
// Category: other
// ======================================================================

class Hammer extends Inventory_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionRepairPart);
AddAction(ActionMineRock1H);
AddAction(ActionBuildPart);
}
}