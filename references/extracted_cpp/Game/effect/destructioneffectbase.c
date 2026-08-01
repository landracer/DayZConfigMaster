// Source: F:/Games/Dayz/scripts/3_game/effects/destructioneffects/destructioneffectbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DestructionEffectBase
{
    EntityAI        m_Entity;
    
    bool            m_EntityIsTakeable;
    
    ParticleSource  m_POneTime;
    ParticleSource  m_PPersistent;
    
    int             m_ParticleOneTime;
    int             m_ParticlePersistent;
    
    EffectSound     m_SOneTime;
    EffectSound     m_SPersistent;
    
    string          m_SoundSetOneTime;
    string          m_SoundSetPersistent;
 
    bool            m_KeepHealthOnReplace;
    string          m_ReplaceWithEntity;
    int             m_ReplaceDelay;
        
    bool            m_HasExplosionDamage;
    DamageType      m_DamageType;
    string          m_AmmoType;
    
    
    void ~DestructionEffectBase()
    {
        if (m_POneTime)
        {
            m_POneTime.Stop();
        }
        if (m_PPersistent)
        {
            m_PPersistent.Stop();
        }
        
        SEffectManager.DestroyEffect(m_SOneTime);
        SEffectManager.DestroyEffect(m_SPersistent);
