// ======================================================================
// File: sodacan__colorbase_8c_source.html
// Category: other
// ======================================================================

class SodaCan_ColorBase : Edible_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionForceDrink);
AddAction(ActionDrinkCan);
}
override int GetConsumptionPenaltyContext()
{
return EConsumptionPenaltyContext.NONE;
}
}