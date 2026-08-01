// ======================================================================
// File: woodenlog_8c_source.html
// Category: other
// ======================================================================

class WoodenLog extends ItemBase
{
override void SetActions()
{
super.SetActions();
AddAction(ActionAttachToConstruction);
AddAction(ActionAttachOnSelection);
}
}