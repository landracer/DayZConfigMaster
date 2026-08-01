// ======================================================================
// File: auriculariamushroom_8c_source.html
// Category: other
// ======================================================================

class AuriculariaMushroom : MushroomBase
{
override void SetActions()
{
super.SetActions();
AddAction(ActionCreateIndoorFireplace);
AddAction(ActionCreateIndoorOven);
}
}