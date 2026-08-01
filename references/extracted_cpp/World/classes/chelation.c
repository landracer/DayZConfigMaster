// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/chelation.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ChelationMdfr : ModifierBase
{
    const int CHELATION_LIFETIME    = 300;
    const float CHELATION_STRENGTH  = 1;
    
    private float m_RegenTime;
    
    override void Init()
    {
        m_TrackActivatedTime    = true;
        m_AnalyticsStatsEnabled = true;
        m_IsPersistent          = true;
        m_ID                    = eModifiers.MDF_CHELATION;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = 3;
 
        m_RegenTime = CHELATION_LIFETIME;
        
        DisableActivateCheck();
