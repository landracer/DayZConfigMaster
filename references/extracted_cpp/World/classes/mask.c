// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/mask.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MaskMdfr: ModifierBase
{
    const float IN_AREA_CONSUME_FILTER_QUANTITY_PER_SEC = 0.3;
    const float OUT_AREA_CONSUME_FILTER_QUANTITY_PER_SEC = 0.03;
    
    const float STAMINA_RECOVERY_MODIFIER = 0.5;
    const float STAMINA_DEPLETION_MODIFIER = 1.25;
    
    const float LOW_FILTER_SOUND_EVENT_MIN = 3;
    const float LOW_FILTER_SOUND_EVENT_MAX = 9;
    
    const float LOW_FILTER_SOUND_THRESHOLD = 0.2;//[0..1] , what&#39;s the filter quantity between 0 and 1 at which low filter sounds begin playing ?
    
    float m_SoundTimeAccu1;
    float m_NextSoundEventTime;
    
    override void Init()
    {
        m_TrackActivatedTime    = false;
        m_AnalyticsStatsEnabled = true;
        m_ID                    = eModifiers.MDF_MASK;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE_SHORT;
