// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/bulletimpactbase/hit_plaster.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hit_Plaster : EffBulletImpactBase
{
    void Hit_Plaster()
    {
        SetEnterParticle(ParticleList.IMPACT_PLASTER_ENTER);
        SetExitParticle(ParticleList.IMPACT_PLASTER_EXIT);
        SetRicochetParticle(ParticleList.IMPACT_PLASTER_RICOCHET);
