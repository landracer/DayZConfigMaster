// Source: F:/Games/Dayz/scripts/5_mission/gui/continuousactionprogress.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ContinuousActionProgress extends ScriptedWidgetEventHandler
{
    reference string                RadialBarWidgetName;
 
    protected PlayerBase            m_Player;
    protected ActionBase            m_Action;
    protected int                   m_ActionState;
    protected ActionManagerBase     m_AM;
    
    protected ref WidgetFadeTimer   m_FadeTimer;
    protected bool                  m_Faded;
    
    protected float                 m_InitProgress;
    protected float                 m_Speed;
 
    protected Widget                m_Root;
    protected Widget                m_RadialWidget;
    protected ImageWidget           m_LoaderImage;
    ref RadialProgressBar           m_Radial;
 
    void ContinuousActionProgress()
    {
        m_Action = null;
        m_ActionState = -1;
        m_AM = null;
        m_RadialWidget = null;
        m_LoaderImage = null;
        m_Radial = null;
        m_Speed = 0;
        m_InitProgress = 100;
        
        m_FadeTimer = new WidgetFadeTimer;
        m_Faded = true;
 
        g_Game.GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
