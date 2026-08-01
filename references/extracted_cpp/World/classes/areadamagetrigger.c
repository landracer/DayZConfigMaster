// Source: F:/Games/Dayz/scripts/4_world/classes/areadamage/areadamagetrigger.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// DEPRECATED: Left for backwards compatibility with mods
// This file is NOT AN EXAMPLE of how to use triggers
// As overriding UpdateInsiders should ideally no longer be done
// Look at &quot;AreaDamageTriggerBase.c&quot; instead and use that as base
class AreaDamageTrigger extends AreaDamageTriggerBase
{
    protected ref AreaDamageBase    m_AreaDamageType;
    
    protected vector                m_ExtentMin;
    protected vector                m_ExtentMax;    
    protected float                 m_InnerDistance;
    
    protected int                   m_TriggerUpdateMs;
    
    void AreaDamageTrigger()
    {
        // Backwards compat, revert the flags back to what they used to be
        ClearEventMask(EntityEvent.ENTER | EntityEvent.LEAVE);
        SetEventMask(EntityEvent.TOUCH);
        
        m_AreaDamageType = null;
        
        m_ExtentMin = vector.Zero;
        m_ExtentMax = vector.Zero;  
        m_InnerDistance = 0;
        
        m_TriggerUpdateMs = 100;
