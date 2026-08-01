// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/painkillersmdfr.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PainKillersMdfr : ModifierBase
{
    const int LIFETIME = 240;
    const int ACTIVATION_DELAY = 15;
 
    override void Init()
    {
        m_TrackActivatedTime    = true;
        m_AnalyticsStatsEnabled = true;
        m_IsPersistent          = true;
        m_ID                    = eModifiers.MDF_PAINKILLERS;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = 1;
 
        DisableActivateCheck();
