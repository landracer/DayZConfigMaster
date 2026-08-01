// ======================================================================
// File: fireextinguisher_8c_source.html
// Category: other
// ======================================================================

class FireExtinguisher: Inventory_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionClapBearTrapWithThisItem);
AddAction(ActionExtinguishFireplaceByExtinguisher);
}
};