// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/diseases/pneumonia.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PneumoniaMdfr : ModifierBase
{
    const int AGENT_THRESHOLD_ACTIVATE = 1150;
    const int AGENT_THRESHOLD_DEACTIVATE = 1100;
    
    const float STAMINA_DEPLETION_MULTIPLIER = 1.3;
    const float STAMINA_RECOVERY_MULTIPLIER = 0.5;
    
    const float HEALTH_LOSS_PNEUMONIA = 0.1;
    
    private const int TEMPORARY_RESISTANCE_TIME = 600;
    
    override void Init()
    {
        m_TrackActivatedTime    = false;
        m_AnalyticsStatsEnabled = true;
        m_ID                    = eModifiers.MDF_PNEUMONIA;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
