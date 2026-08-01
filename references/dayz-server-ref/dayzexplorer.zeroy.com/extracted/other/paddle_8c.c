// ======================================================================
// File: paddle_8c_source.html
// Category: other
// ======================================================================

class Paddle: Inventory_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionClapBearTrapWithThisItem);
}
};