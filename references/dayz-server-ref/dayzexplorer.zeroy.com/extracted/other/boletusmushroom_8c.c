// ======================================================================
// File: boletusmushroom_8c_source.html
// Category: other
// ======================================================================

class BoletusMushroom : MushroomBase
{
override void SetActions()
{
super.SetActions();
AddAction(ActionCreateIndoorFireplace);
AddAction(ActionCreateIndoorOven);
}
}