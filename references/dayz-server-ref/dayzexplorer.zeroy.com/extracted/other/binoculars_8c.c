// ======================================================================
// File: binoculars_8c_source.html
// Category: other
// ======================================================================

class Binoculars: ItemOptics
{
override void SetActions()
{
super.SetActions();
RemoveAction(ActionViewOptics);
AddAction(ActionViewBinoculars);
}
};