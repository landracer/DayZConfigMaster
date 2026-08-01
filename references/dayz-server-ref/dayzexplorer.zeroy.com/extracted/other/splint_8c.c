// ======================================================================
// File: splint_8c_source.html
// Category: other
// ======================================================================

class Splint: Inventory_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionSplintTarget);
AddAction(ActionSplintSelf);
}
};
class Splint_Applied: Clothing
{
void ~Splint_Applied()
{
}
};