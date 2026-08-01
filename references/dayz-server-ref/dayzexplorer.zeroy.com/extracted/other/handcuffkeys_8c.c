// ======================================================================
// File: handcuffkeys_8c_source.html
// Category: other
// ======================================================================

class HandcuffKeys: Inventory_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionUnrestrainTarget);
}
};