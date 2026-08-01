// ======================================================================
// File: syringe_8c_source.html
// Category: other
// ======================================================================

class Syringe extends BloodContainerBase
{
override void SetActions()
{
super.SetActions();
AddAction(ActionCollectSampleTarget);
AddAction(ActionCollectSampleSelf);
}
}