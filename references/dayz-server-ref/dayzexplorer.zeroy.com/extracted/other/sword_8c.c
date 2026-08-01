// ======================================================================
// File: sword_8c_source.html
// Category: other
// ======================================================================

class Sword extends ToolBase
{
void Sword()
{
}
override bool IsMeleeFinisher()
{
return false;
}
override void SetActions()
{
super.SetActions();
AddAction(ActionUnrestrainTarget);
AddAction(ActionSkinning);
AddAction(ActionMineBush);
AddAction(ActionMineTreeBark);
}
}