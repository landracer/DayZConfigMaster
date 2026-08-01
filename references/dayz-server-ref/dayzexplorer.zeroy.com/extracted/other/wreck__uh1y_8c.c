// ======================================================================
// File: wreck__uh1y_8c_source.html
// Category: other
// ======================================================================

class Wreck_UH1Y extends CrashBase
{
void Wreck_UH1Y()
{
if ( !g_Game.IsDedicatedServer() )
{
m_ParticleEfx = ParticleManager.GetInstance().PlayOnObject(ParticleList.SMOKING_HELI_WRECK, this, Vector(-0.5, 0, -1.0));
}
}
}