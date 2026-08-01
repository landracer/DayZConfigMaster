// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/diseases/influenza.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class InfluenzaMdfr : ModifierBase
{
    const int AGENT_THRESHOLD_ACTIVATE      = 600;
    const int AGENT_THRESHOLD_DEACTIVATE    = 400;
 
    private const int COUGH_RND_DIVIDER_NORMAL_MIN      = 5;
    private const int COUGH_RND_DIVIDER_NORMAL_MAX      = 20;
    private const int COUGH_RND_DIVIDER_SUPPRESSED_MIN  = 10;
    private const int COUGH_RND_DIVIDER_SUPPRESSED_MAX  = 40;
    
    private const int TEMPORARY_RESISTANCE_TIME = 300;
    
    private ModifiersManager m_ModifiersManager;
    
    override void Init()
    {
        m_TrackActivatedTime    = false;
        m_AnalyticsStatsEnabled = true;
        m_ID                    = eModifiers.MDF_INFLUENZA;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
