// ======================================================================
// File: wrench_8c_source.html
// Category: other
// ======================================================================

class Wrench: Inventory_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionLockAttachment);
AddAction(ActionMineRock1H);
}
};