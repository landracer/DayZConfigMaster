// ======================================================================
// File: pipewrench_8c_source.html
// Category: other
// ======================================================================

class PipeWrench extends Inventory_Base
{
override void SetActions()
{
super.SetActions();
//AddAction(ActionDismantlePart);
AddAction(ActionMineRock);
}
}