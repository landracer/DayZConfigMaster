// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/bulletimpactbase/hit_gravel.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hit_Gravel : EffBulletImpactBase
{
    void Hit_Gravel()
    {
        SetEnterParticle(ParticleList.IMPACT_GRAVEL_ENTER);
        SetExitParticle(ParticleList.IMPACT_GRAVEL_EXIT);
        SetRicochetParticle(ParticleList.IMPACT_GRAVEL_RICOCHET);
