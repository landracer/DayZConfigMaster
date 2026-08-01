// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/bulletimpactbase/hit_glass.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hit_Glass : EffBulletImpactBase
{
    void Hit_Glass()
    {
        SetEnterParticle(ParticleList.IMPACT_GLASS_ENTER);
        SetExitParticle(ParticleList.IMPACT_GLASS_EXIT);
        SetRicochetParticle(ParticleList.IMPACT_GLASS_RICOCHET);
