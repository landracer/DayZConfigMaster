// Source: F:/Games/Dayz/scripts/4_world/entities/building/wrecks/wreck_uh1y.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Wreck_UH1Y extends CrashBase
{
    void Wreck_UH1Y()
    {
        if ( !g_Game.IsDedicatedServer() )
        {
            m_ParticleEfx = ParticleManager.GetInstance().PlayOnObject(ParticleList.SMOKING_HELI_WRECK, this, Vector(-0.5, 0, -1.0));
        }
