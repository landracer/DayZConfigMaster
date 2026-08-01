// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/bulletimpactbase/hit_foliage/hit_foliage_conifer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hit_Foliage_Conifer: Hit_Foliage
{
    void Hit_Foliage_Conifer()
    {
        SetEnterParticle(ParticleList.IMPACT_FOLIAGE_CONIFER_ENTER);
        SetExitParticle(ParticleList.IMPACT_FOLIAGE_CONIFER_EXIT);
        SetRicochetParticle(ParticleList.IMPACT_FOLIAGE_CONIFER_RICOCHET);
