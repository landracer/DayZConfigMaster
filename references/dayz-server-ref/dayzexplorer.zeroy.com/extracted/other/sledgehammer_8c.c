// ======================================================================
// File: sledgehammer_8c_source.html
// Category: other
// ======================================================================

class SledgeHammer extends Inventory_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionClapBearTrapWithThisItem);
AddAction(ActionMineRock);
AddAction(ActionDismantlePart);
AddAction(ActionBuildPart);
}
}