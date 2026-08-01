// ======================================================================
// File: bloodbagiv_8c_source.html
// Category: other
// ======================================================================

class BloodBagIV extends BloodContainerBase
{
override void SetActions()
{
super.SetActions();
AddAction(ActionGiveBloodTarget);
AddAction(ActionGiveBloodSelf);
}
}