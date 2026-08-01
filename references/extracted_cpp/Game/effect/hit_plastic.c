// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/bulletimpactbase/hit_plastic.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hit_Plastic : EffBulletImpactBase
{
    void Hit_Plastic()
    {
        SetEnterParticle(ParticleList.IMPACT_PLASTIC_ENTER);
        SetRicochetParticle(ParticleList.IMPACT_PLASTIC_ENTER);
        SetExitParticle(ParticleList.IMPACT_PLASTIC_ENTER);
