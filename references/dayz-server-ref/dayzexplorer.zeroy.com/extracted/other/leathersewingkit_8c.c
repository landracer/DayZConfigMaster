// ======================================================================
// File: leathersewingkit_8c_source.html
// Category: other
// ======================================================================

class LeatherSewingKit: Inventory_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionRepairShelter);
}
};