// ======================================================================
// File: pluginpermanentcrosshair_8c_source.html
// Category: other
// ======================================================================

class PluginPermanentCrossHair extends PluginBase
{
private Hud m_Hud;
void SwitchPermanentCrossHair(bool state)
{
if (!m_Hud)
m_Hud = g_Game.GetMission().GetHud();
m_Hud.SetPermanentCrossHair(state);
}
}