// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/diseases/braindisease.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BrainDiseaseMdfr : ModifierBase
{
    static const int AGENT_THRESHOLD_ACTIVATE = 2000;
    static const int AGENT_THRESHOLD_DEACTIVATE = 0;
    const int SHAKE_INTERVAL_MIN = 1;
    const int SHAKE_INTERVAL_MAX = 4;
    
    float m_Time;
    float m_ShakeTime;
    
    override void Init()
    {
        m_TrackActivatedTime    = false;
        m_AnalyticsStatsEnabled = true;
        m_ID                    = eModifiers.MDF_BRAIN;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
