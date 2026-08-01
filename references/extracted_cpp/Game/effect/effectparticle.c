// Source: F:/Games/Dayz/scripts/3_game/effects/effectparticle.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class EffectParticle : Effect
{
    protected Particle          m_ParticleObj;
    

    protected int               m_ParticleID;
    protected vector            m_Orientation;
    protected bool              m_ForceRotationRelativeToWorld;
        
    protected vector            m_ParticleOrientation;
    protected Object            m_Object;
    
    
    
    void EffectParticle()
    {
        
