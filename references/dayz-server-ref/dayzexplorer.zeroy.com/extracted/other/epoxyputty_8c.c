// ======================================================================
// File: epoxyputty_8c_source.html
// Category: other
// ======================================================================

class EpoxyPutty: Inventory_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionRepairCarPart); //Heals ONLY global health for now
AddAction(ActionRepairCarChassis);
}
};