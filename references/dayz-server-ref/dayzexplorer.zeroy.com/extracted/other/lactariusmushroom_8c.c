// ======================================================================
// File: lactariusmushroom_8c_source.html
// Category: other
// ======================================================================

class LactariusMushroom : MushroomBase
{
override void SetActions()
{
super.SetActions();
AddAction(ActionCreateIndoorFireplace);
AddAction(ActionCreateIndoorOven);
}
}