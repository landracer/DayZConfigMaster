// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/conditions/heatbuffer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HeatBufferMdfr : ModifierBase
{
    const int NUMBER_OF_STAGES = 4;
    const float STAGE_THRESHOLDS[NUMBER_OF_STAGES] = {0.0, 0.60, 0.85, 1.0};
 
    protected int m_Stage;
    protected int m_StageLast;
 
    protected float m_Value;
    
    override void Init()
    {
        m_TrackActivatedTime    = false;
        m_AnalyticsStatsEnabled = true;
        m_ID                    = eModifiers.MDF_HEATBUFFER;
        m_TickIntervalInactive  = 1;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
