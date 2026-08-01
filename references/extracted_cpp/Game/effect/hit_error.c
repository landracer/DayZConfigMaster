// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/bulletimpactbase/hit_error.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hit_ErrorNoMaterial : EffBulletImpactBase
{
    void Hit_ErrorNoMaterial()
    {
        #ifdef DEVELOPER
        SetSingleParticle(ParticleList.IMPACT_TEST_NO_MATERIAL_ERROR);
        #else
        SetSingleParticle(ParticleList.IMPACT_TEST);
        #endif
