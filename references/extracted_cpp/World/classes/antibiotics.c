// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/antibiotics.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AntibioticsMdfr : ModifierBase
{
    const int ANTIBIOTICS_LIFETIME      = 300;
    const float ANTIBIOTICS_STRENGTH    = 1;
 
    float m_RegenTime;
    
    override void Init()
    {
        m_TrackActivatedTime    = true;
        m_AnalyticsStatsEnabled = true;
        m_IsPersistent          = true;
        m_ID                    = eModifiers.MDF_ANTIBIOTICS;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = 3;
 
        m_RegenTime = ANTIBIOTICS_LIFETIME;
        
        DisableActivateCheck();
