// Source: F:/Games/Dayz/scripts/4_world/classes/playernotifiers/notifierbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class NotifierBase
{
    float               m_DeltaT; // time in seconds since the last tick
    ref Timer           m_Timer1; // timer which can be used for whatever
    PlayerBase          m_Player; //the player this Notifier belongs to
    int                 m_Type;
    NotifiersManager    m_Manager;
    int                 m_TendencyBufferSize = 3;//for best results, this should be somewhat aligned with modifier frequency
    const int           TENDENCY_BUFFER_SIZE = 30;//this needs to be bigger or same size as buffer size of any invidual buffer size
    bool                m_ShowTendency;
    bool                m_Active;
    int                 m_TickInterval;
    int                 m_TickIntervalLastTick;
    float               m_TendencyBuffer[TENDENCY_BUFFER_SIZE];
    int                 m_TendencyBufferWriteIterator;
    float               m_LastTendency;
    float               m_LastMA;
    bool                m_FirstPass = true;
    
    PluginPlayerStatus  m_ModulePlayerStatus;
        
    void NotifierBase(NotifiersManager manager)
    {
        m_ModulePlayerStatus = PluginPlayerStatus.Cast(GetPlugin(PluginPlayerStatus));
        m_Active = true; 
        m_Manager = manager;
        m_Player = manager.GetPlayer();
        m_TickInterval = 1000;
        manager.RegisterItself(GetNotifierType(), this);
