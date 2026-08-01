// ======================================================================
// File: hayhook_8c_source.html
// Category: other
// ======================================================================

class HayHook: Inventory_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionUnrestrainTarget);
AddAction(ActionSkinning);
}
};