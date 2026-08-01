// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/bulletimpactbase/hit_sand.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hit_Sand : EffBulletImpactBase
{
    void Hit_Sand()
    {
        SetEnterParticle(ParticleList.IMPACT_SAND_ENTER);
        SetRicochetParticle(ParticleList.IMPACT_SAND_ENTER);
        SetExitParticle(ParticleList.IMPACT_SAND_ENTER);
