// ======================================================================
// File: bloodsyringe_8c_source.html
// Category: other
// ======================================================================

class BloodSyringe extends BloodContainerBase
{
override void SetActions()
{
super.SetActions();
AddAction(ActionGiveBloodTarget);
AddAction(ActionGiveBloodSelf);
}
}