// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/bulletimpactbase/hit_wood.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hit_Wood : EffBulletImpactBase
{
    void Hit_Wood()
    {
        SetEnterParticle(ParticleList.IMPACT_WOOD_ENTER);
        SetExitParticle(ParticleList.IMPACT_WOOD_EXIT);
        SetRicochetParticle(ParticleList.IMPACT_WOOD_RICOCHET);
