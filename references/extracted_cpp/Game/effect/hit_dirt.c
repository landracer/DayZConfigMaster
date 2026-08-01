// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/bulletimpactbase/hit_dirt.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hit_Dirt : EffBulletImpactBase
{
    void Hit_Dirt()
    {
        SetEnterParticle(ParticleList.IMPACT_DIRT_ENTER);
        SetExitParticle(ParticleList.IMPACT_DIRT_EXIT);
        SetRicochetParticle(ParticleList.IMPACT_DIRT_RICOCHET);
