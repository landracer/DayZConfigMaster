// ======================================================================
// File: macrolepiotamushroom_8c_source.html
// Category: other
// ======================================================================

class MacrolepiotaMushroom : MushroomBase
{
override void SetActions()
{
super.SetActions();
AddAction(ActionCreateIndoorFireplace);
AddAction(ActionCreateIndoorOven);
}
}