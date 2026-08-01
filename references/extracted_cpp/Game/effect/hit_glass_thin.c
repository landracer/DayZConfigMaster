// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/bulletimpactbase/hit_glass_thin.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hit_Glass_Thin : EffBulletImpactBase
{
    void Hit_Glass_Thin()
    {
        SetEnterParticle(ParticleList.IMPACT_GLASS_ENTER);
        SetExitParticle(ParticleList.IMPACT_GLASS_EXIT);
        SetRicochetParticle(ParticleList.IMPACT_GLASS_RICOCHET);
