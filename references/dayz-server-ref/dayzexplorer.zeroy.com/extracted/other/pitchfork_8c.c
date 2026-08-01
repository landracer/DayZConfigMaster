// ======================================================================
// File: pitchfork_8c_source.html
// Category: other
// ======================================================================

class Pitchfork extends ItemBase
{
void Pitchfork()
{
}
override void SetActions()
{
super.SetActions();
AddAction(ActionClapBearTrapWithThisItem);
}
}