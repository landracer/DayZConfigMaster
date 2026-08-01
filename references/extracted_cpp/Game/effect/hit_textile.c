// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/bulletimpactbase/hit_textile.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hit_Textile : EffBulletImpactBase
{
    void Hit_Textile()
    {
        SetEnterParticle(ParticleList.IMPACT_TEXTILE_ENTER);
        SetRicochetParticle(ParticleList.IMPACT_TEXTILE_ENTER);
        SetExitParticle(ParticleList.IMPACT_TEXTILE_EXIT);
