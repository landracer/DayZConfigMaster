// ======================================================================
// File: handsaw_8c_source.html
// Category: other
// ======================================================================

class HandSaw: Inventory_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionUnrestrainTarget);
AddAction(ActionMineBush);
AddAction(ActionSawPlanks);
AddAction(ActionDismantlePart);
AddAction(ActionSkinning);
AddAction(ActionMineTreeBark);
}
};