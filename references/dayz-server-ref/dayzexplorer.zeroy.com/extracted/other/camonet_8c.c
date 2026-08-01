// ======================================================================
// File: camonet_8c_source.html
// Category: other
// ======================================================================

class CamoNet extends ItemBase
{
override void SetActions()
{
super.SetActions();
AddAction(ActionAttachOnTentProxy); //tents
AddAction(ActionAttachOnSelection); //basebuilding
AddAction(ActionAttachToConstruction); //basebuilding
}
}