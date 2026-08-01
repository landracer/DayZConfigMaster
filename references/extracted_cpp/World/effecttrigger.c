// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedentities/triggers/effecttrigger.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// Base class for &quot;Effect triggers&quot;
// Registers in TriggerEffectManager and handles parameter setting through cfgEffectArea.json file
class EffectTrigger : CylinderTrigger
{
    int                     m_AroundPartId; // The main particles spawned around player when in trigger
    int                     m_TinyPartId; // The smaller particles spawned around player when in trigger
    int                     m_PPERequester; // The Post Processing used when player is in trigger
    float                   m_DeltaTime;
    float                   m_TimeAccuStay;
    bool                    m_DealDamageFlag;
    TriggerEffectManager    m_Manager;
    EffectArea              m_EffectArea;
    int                     m_EffectsPriority;
    
    #ifdef DIAG_DEVELOPER
    Shape m_DbgShape;
    bool m_DebugShapeActive;
    #endif
    
    void EffectTrigger()
    {
        RegisterNetSyncVariableInt(&quot;m_AroundPartId&quot;);
        RegisterNetSyncVariableInt(&quot;m_TinyPartId&quot;);
        RegisterNetSyncVariableInt(&quot;m_PPERequester&quot;);
        RegisterNetSyncVariableInt(&quot;m_EffectsPriority&quot;);
        
        m_Manager = TriggerEffectManager.GetInstance();
        m_Manager.RegisterTriggerType( this );
