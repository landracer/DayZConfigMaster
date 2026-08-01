// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/bulletimpactbase/hit_snow.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hit_Snow : EffBulletImpactBase
{
    void Hit_Snow()
    {
        SetEnterParticle(ParticleList.IMPACT_SNOW_ENTER);
        SetExitParticle(ParticleList.IMPACT_SNOW_EXIT);
        SetRicochetParticle(ParticleList.IMPACT_SNOW_RICOCHET);
