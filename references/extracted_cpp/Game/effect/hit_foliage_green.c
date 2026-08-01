// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/bulletimpactbase/hit_foliage/hit_foliage_green.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hit_Foliage_Green : Hit_Foliage
{
    void Hit_Foliage_Green()
    {
        SetEnterParticle(ParticleList.IMPACT_FOLIAGE_GREEN_ENTER);
        SetExitParticle(ParticleList.IMPACT_FOLIAGE_GREEN_EXIT);
        SetRicochetParticle(ParticleList.IMPACT_FOLIAGE_GREEN_RICOCHET);
