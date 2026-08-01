// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/bulletimpactbase/hit_water.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hit_Water : EffBulletImpactBase
{
    void Hit_Water()
    {
        SetSingleParticle(ParticleList.IMPACT_WATER_SMALL_ENTER);
        
        m_AngledEnter = 10;
