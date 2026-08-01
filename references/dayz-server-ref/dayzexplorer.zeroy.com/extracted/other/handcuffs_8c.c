// ======================================================================
// File: handcuffs_8c_source.html
// Category: other
// ======================================================================

class Handcuffs: Inventory_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionRestrainTarget);
AddAction(ActionRestrainSelf);
}
};