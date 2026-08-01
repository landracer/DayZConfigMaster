// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/bulletimpactbase/hit_ice.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hit_Ice : EffBulletImpactBase
{
    void Hit_Ice()
    {
        SetEnterParticle(ParticleList.IMPACT_ICE_ENTER);
        SetExitParticle(ParticleList.IMPACT_ICE_EXIT);
        SetRicochetParticle(ParticleList.IMPACT_ICE_RICOCHET);
