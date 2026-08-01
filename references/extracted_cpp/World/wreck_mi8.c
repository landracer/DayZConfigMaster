// Source: F:/Games/Dayz/scripts/4_world/entities/building/wrecks/wreck_mi8.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//New russian helicopter crash site
class Wreck_Mi8_Crashed extends CrashBase
{
    void Wreck_Mi8_Crashed()
    {
        if ( !g_Game.IsDedicatedServer() )
        {
            m_ParticleEfx = ParticleManager.GetInstance().PlayOnObject(ParticleList.SMOKING_HELI_WRECK, this, Vector(2, 0, -5));
        }
