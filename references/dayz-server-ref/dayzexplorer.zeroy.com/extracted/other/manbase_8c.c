// ======================================================================
// File: manbase_8c_source.html
// Category: other
// ======================================================================

class ManBase extends DayZPlayerImplement // PSOVIS originaly extends Man
{
void ManBase()
{
//Print("PSOVIS: this is ManBase");
}
// -------------------------------------------------------------------------
// ! On Client, &#39;true&#39; if this instance of a character is controlled by the player(ie. not a remote player)
bool IsControlledPlayer()
{
return( g_Game.GetPlayer() == this );
}
// -------------------------------------------------------------------------
override void OnVariablesSynchronized()
{
super.OnVariablesSynchronized();
}
}