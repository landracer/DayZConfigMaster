// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/bulletimpactbase/hit_rubber.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hit_Rubber : EffBulletImpactBase
{
    void Hit_Rubber()
    {
        SetEnterParticle(ParticleList.IMPACT_RUBBER_ENTER);
        SetExitParticle(ParticleList.IMPACT_RUBBER_EXIT);
        SetRicochetParticle(ParticleList.IMPACT_RUBBER_RICOCHET);
