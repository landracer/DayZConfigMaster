// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/diseases/commoncold.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CommonColdMdfr : ModifierBase
{
    const int AGENT_THRESHOLD_ACTIVATE      = 100;
    const int AGENT_THRESHOLD_DEACTIVATE    = 0;
    
    private const int SNEEZE_RND_DIVIDER_NORMAL_MIN     = 15;
    private const int SNEEZE_RND_DIVIDER_NORMAL_MAX     = 20;
    private const int SNEEZE_RND_DIVIDER_SUPPRESSED_MIN = 30;
    private const int SNEEZE_RND_DIVIDER_SUPPRESSED_MAX = 40;
    
    private const int TEMPORARY_RESISTANCE_TIME = 900;
    
    private ModifiersManager m_ModifiersManager;
 
    override void Init()
    {
        m_TrackActivatedTime    = false;
        m_AnalyticsStatsEnabled = true;
        m_ID                    = eModifiers.MDF_COMMON_COLD;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
