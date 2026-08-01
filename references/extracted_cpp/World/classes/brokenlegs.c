// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/conditions/brokenlegs.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BrokenLegsMdfr: ModifierBase
{
    private const   float   HEALTHY_LEG = 100; //Health at which legs are no longer considered broken
    private         int     currentState = -1; //Current broken leg state used upon reconnect
    private const   float   TIME_TO_UPDATE = 0.5;
    private         float   elapsedTime = TIME_TO_UPDATE + 1;
    
    #ifdef DEVELOPER
    ref static Timer timer;
    #endif
 
    override void Init()
    {
        m_TrackActivatedTime    = false;
        m_AnalyticsStatsEnabled = true;
        m_ID                    = eModifiers.MDF_BROKEN_LEGS;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = 0.5;
        m_ActivationType        = EActivationType.TRIGGER_EVENT_ON_ACTIVATION;
        m_IsPersistent          = true;
