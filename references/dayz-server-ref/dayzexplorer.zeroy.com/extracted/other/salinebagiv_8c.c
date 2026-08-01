// ======================================================================
// File: salinebagiv_8c_source.html
// Category: other
// ======================================================================

class SalineBagIV: Inventory_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionGiveSalineTarget);
AddAction(ActionGiveSalineSelf);
}
};