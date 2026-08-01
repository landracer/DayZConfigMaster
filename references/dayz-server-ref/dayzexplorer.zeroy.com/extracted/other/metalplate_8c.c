// ======================================================================
// File: metalplate_8c_source.html
// Category: other
// ======================================================================

class MetalPlate extends ItemBase
{
override void SetActions()
{
super.SetActions();
AddAction(ActionAttachToConstruction);
AddAction(ActionAttachOnSelection);
}
}