// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/bulletimpactbase/hit_meatbones.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Hit_MeatBones : EffBulletImpactBase
{
    float m_ScalingByDistance;
    
    void Hit_MeatBones()
    {
        SetEnterParticle(ParticleList.IMPACT_MEATBONES_ENTER);
        SetExitParticle(ParticleList.IMPACT_MEATBONES_EXIT);
        SetRicochetParticle(ParticleList.IMPACT_MEATBONES_RICOCHET);
        
        m_AngledEnter = 10;
        m_EnterSplashCoef = 0.002;
        m_ExitSplashCoef = 0.006;
        m_ScalingByDistance = 0.05;
        
        MIN_SCALING_PARAM = 0.2;
