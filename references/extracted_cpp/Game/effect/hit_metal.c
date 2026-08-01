// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/bulletimpactbase/hit_metal.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hit_Metal : EffBulletImpactBase
{
    void Hit_Metal()
    {
        SetEnterParticle(ParticleList.IMPACT_METAL_ENTER);
        SetExitParticle(ParticleList.IMPACT_METAL_EXIT);
        SetRicochetParticle(ParticleList.IMPACT_METAL_RICOCHET);
