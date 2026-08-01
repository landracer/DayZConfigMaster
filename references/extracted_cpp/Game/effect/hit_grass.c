// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/bulletimpactbase/hit_grass.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hit_Grass : EffBulletImpactBase
{
    void Hit_Grass()
    {
        SetEnterParticle(ParticleList.IMPACT_GRASS_ENTER);
        SetExitParticle(ParticleList.IMPACT_GRASS_ENTER);
        SetRicochetParticle(ParticleList.IMPACT_GRASS_RICOCHET);
        
        m_AngledEnter = 0.6;
