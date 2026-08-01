// ======================================================================
// File: bloodbagempty_8c_source.html
// Category: other
// ======================================================================

class BloodBagEmpty extends BloodContainerBase
{
override void SetActions()
{
super.SetActions();
AddAction(ActionCollectBloodTarget);
AddAction(ActionCollectBloodSelf);
}
}