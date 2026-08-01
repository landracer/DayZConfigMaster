// ======================================================================
// File: agaricusmushroom_8c_source.html
// Category: other
// ======================================================================

class AgaricusMushroom : MushroomBase
{
override void SetActions()
{
super.SetActions();
AddAction(ActionCreateIndoorFireplace);
AddAction(ActionCreateIndoorOven);
}
}