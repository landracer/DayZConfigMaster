// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle/bulletimpactbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class EffBulletImpactBase : EffectParticle
{
    static const int    SURVIVOR_HEAD = 0; // Head component
    static const int    INFECTED_HEAD = 3; // Head component
    static float        DEFAULT_PROJECTILE_WEIGHT = 0.015;
    
    float               MIN_SCALING_PARAM = 0.1;
    
    Object              m_DirectHit;
    float               m_StoppingForce;
    float               m_Weight; // projectile weight in kg
    int                 m_ImpactType;
    int                 m_ComponentIndex;
    vector              m_Pos;
    vector              m_SurfNormal;
    vector              m_ExitPos;
    vector              m_InSpeed;
    vector              m_OutSpeed;
    string              m_AmmoType;
    
    static vector INVALID = &quot;0 0 0&quot;;
    
    // Particle Effects
    int m_ParticleEnter = -1;
    int m_ParticleExit = -1;
    int m_ParticleRicochet = -1;
    
    // Calculations
    float m_EnterSplashCoef = 0.003;
    float m_ExitSplashCoef = 0.002;
    float m_RicochetSplashCoef = 0.002;
    float m_EnterAngledSplashCoef = 0.01;
    float m_AngledEnter = 0.40;
    
    void EffBulletImpactBase()
    {
        
