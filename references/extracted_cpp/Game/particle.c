// Source: F:/Games/Dayz/scripts/3_game/particles/particle.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class Particle : ParticleBase
{

    protected   int     m_ParticleID;
    protected   float   m_Lifetime;
    protected   bool    m_IsRepeat;
    private     bool    m_MarkedForDeletion;
    

    bool                m_WiggleProcessing;
    bool                m_ForceOrientationRelativeToWorld;
    vector              m_DefaultOri;
    vector              m_DefaultPos;
    vector              m_DefaultWorldOri;
    vector              m_DefaultWorldPos;
    
    float               m_MaxOriWiggle;
    float               m_MaxOriInterval;
    ref Timer           m_RandomizeOri;
    
    protected Object    m_ParentObject;
    protected Object    m_ParticleEffect;
    
    protected int       m_PreviousFrame;
    private vector      m_GlobalPosPreviousFrame;
    static private const int MAX_EMITORS = 30;
    
    
    void Particle()
    {
        ParticleInit();
