// ======================================================================
// File: hacksaw_8c_source.html
// Category: other
// ======================================================================

class Hacksaw extends ToolBase
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
}