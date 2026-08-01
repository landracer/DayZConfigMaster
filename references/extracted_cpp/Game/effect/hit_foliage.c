// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/bulletimpactbase/hit_foliage.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hit_Foliage : EffBulletImpactBase
{
    void Hit_Foliage()
    {
        SetEnterParticle(ParticleList.IMPACT_FOLIAGE_ENTER);
        SetExitParticle(ParticleList.IMPACT_FOLIAGE_EXIT);
        SetRicochetParticle(ParticleList.IMPACT_FOLIAGE_RICOCHET);
