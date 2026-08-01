// ======================================================================
// File: woodenplank_8c_source.html
// Category: other
// ======================================================================

class WoodenPlank extends ItemBase
{
override void SetActions()
{
super.SetActions();
AddAction(ActionClapBearTrapWithThisItem);
AddAction(ActionAttachToConstruction);
AddAction(ActionRepairCarChassis);
AddAction(ActionAttachOnSelection);
}
}