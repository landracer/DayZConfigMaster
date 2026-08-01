// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/conditions/bleeding.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//checks for critical blood level and kills the character if bellow
class BleedingCheckMdfr: ModifierBase
{
    protected const float       BLOOD_DECREMENT_PER_SEC = -10;
    PluginAdminLog              m_AdminLog;
        
    override void Init()
    {
        m_TrackActivatedTime    = false;
        m_AnalyticsStatsEnabled = true;
        m_ID                    = eModifiers.MDF_BLEEDING;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
        
        if( g_Game.IsServer() )
        {
            m_AdminLog              = PluginAdminLog.Cast( GetPlugin(PluginAdminLog) );
        }
        
        DisableDeactivateCheck();
