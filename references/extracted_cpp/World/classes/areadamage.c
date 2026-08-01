// Source: F:/Games/Dayz/scripts/4_world/classes/areadamage/areadamage.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// DEPRECATED: Backwards compatibility class to prevent existing mods breaking
// I wish I could rename this to AreaDamageTimer, but can&#39;t, because of backwards compatibility with mods
class AreaDamageBase : AreaDamageManager
{
    protected AreaDamageBase            m_AreaDamage;
    
    protected float                     m_PlayerDamage;
    protected float                     m_OthersDamage;
    
    protected string                    m_AmmoName;
    protected int                       m_DamageType;
    
    protected float                     m_LoopInterval;
    protected float                     m_DeferDuration;
    
    protected ref array<string>         m_HitZones;
    protected ref array<string>         m_RaycastSources;
    protected ref array<typename>       m_DamageableTypes
    
    protected ref Timer                 m_LoopTimer;
    protected ref Timer                 m_DeferTimer;
    
    void AreaDamageBase(EntityAI parent)
    {
        m_AreaDamage        = this;
        
        m_PlayerDamage      = 0.0;
        m_OthersDamage      = 0.0;
        
        m_AmmoName          = &quot;MeleeDamage&quot;;
        m_DamageType        = DamageType.CUSTOM;
        
        m_LoopInterval      = 1.0;
        m_DeferDuration     = 1.0;
        
        m_HitZones          = new array<string>;
        m_RaycastSources    = new array<string>;
        m_DamageableTypes   = new array<typename>;
        m_DamageableTypes.Insert(DayZPlayer);
        
        m_LoopTimer         = new Timer(CALL_CATEGORY_SYSTEM);
        m_DeferTimer        = new Timer(CALL_CATEGORY_SYSTEM);
        
        m_TriggerBaseClass = &quot;AreaDamageTrigger&quot;;
