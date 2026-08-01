// ======================================================================
// File: meattenderizer_8c_source.html
// Category: other
// ======================================================================

class MeatTenderizer: Inventory_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionBuildPart);
AddAction(ActionRepairPart);
AddAction(ActionMineRock1H);
}
};