// ======================================================================
// File: punchedcard_8c_source.html
// Category: other
// ======================================================================

class PunchedCard : Paper
{
override void SetActions()
{
super.SetActions();
AddAction(ActionUseUndergroundPanel);
}
void OnUse()
{
AddHealthLevel(1);
}
}