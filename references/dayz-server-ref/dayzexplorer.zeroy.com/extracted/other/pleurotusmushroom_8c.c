// ======================================================================
// File: pleurotusmushroom_8c_source.html
// Category: other
// ======================================================================

class PleurotusMushroom : MushroomBase
{
override void SetActions()
{
super.SetActions();
AddAction(ActionCreateIndoorFireplace);
AddAction(ActionCreateIndoorOven);
}
}