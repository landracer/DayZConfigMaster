// ======================================================================
// File: lugwrench_8c_source.html
// Category: other
// ======================================================================

class LugWrench: Inventory_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionLockAttachment);
}
};