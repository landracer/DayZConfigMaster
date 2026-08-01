// Source: F:/Games/Dayz/scripts/5_mission/mission/missiongameplay.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MissionGameplay extends MissionBase
{
    int                             m_LifeState;
    bool                            m_Initialized;
    
    protected UIManager             m_UIManager;
    
    Widget                          m_HudRootWidget;
    ImageWidget                     m_MicrophoneIcon;
    
    ref InventoryMenu               m_InventoryMenu;
    ref Chat                        m_Chat;
    ref ActionMenu                  m_ActionMenu;
    ref IngameHud                   m_Hud;
    ref HudDebug                    m_HudDebug;
    ref LogoutMenu                  m_Logout;
    ref DebugMonitor                m_DebugMonitor;
    ref Watermark                   m_Watermark;
    
    protected ref ScriptInvoker         m_OnConnectivityChanged;
    
    protected ref GameplayEffectWidgets     m_EffectWidgets;
    
    ref Timer                       m_ChatChannelHideTimer;
    ref WidgetFadeTimer             m_ChatChannelFadeTimer;
    ref WidgetFadeTimer             m_MicFadeTimer;
    
    Widget                          m_ChatChannelArea;
    TextWidget                      m_ChatChannelText;
    NoteMenu                        m_Note;
    
    protected ref Timer             m_ToggleHudTimer;
    protected const int             HOLD_LIMIT_TIME = 300; //ms
    protected int                   m_ActionDownTime;
    protected int                   m_ActionUpTime;
    protected bool                  m_InitOnce;
    protected bool                  m_ControlDisabled; //DEPRECATED; disabled mode stored below
    protected int                   m_ControlDisabledMode;
    protected ref array<string>     m_ActiveInputExcludeGroups; //exclude groups defined in &#39;specific.xml&#39; file
    protected ref array<int>        m_ActiveInputRestrictions; //additional scripted restrictions
    protected bool                  m_ProcessInputExcludes;
    protected bool                  m_QuickbarHold;
    protected bool                  m_PlayerRespawning;
    protected int                   m_RespawnModeClient; //for client-side usage
    protected bool                  m_PauseQueued;
    
    // von control info
    protected bool                  m_VoNActive;
    protected Widget                m_VoiceLevels;
    protected ref map<int,ImageWidget> m_VoiceLevelsWidgets;
    protected ref map<int,ref WidgetFadeTimer> m_VoiceLevelTimers;
    
    protected bool                  m_InputBufferFull;
    UIScriptedMenu                  m_ConnectionMenu;
    
    private float                   m_ExitButtonDisabledRemainingTime;
    
    void MissionGameplay()
    {
        DestroyAllMenus();
        m_Initialized               = false;
        m_EffectWidgets             = new GameplayEffectWidgets;
        m_HudRootWidget             = null;
        m_Chat                      = new Chat;
        m_ActionMenu                = new ActionMenu;
        m_LifeState                 = -1;
        m_Hud                       = new IngameHud;
        m_VoNActive                 = false;
        m_PauseQueued               = false;
        m_ChatChannelFadeTimer      = new WidgetFadeTimer;
        m_MicFadeTimer              = new WidgetFadeTimer;
        m_ChatChannelHideTimer      = new Timer(CALL_CATEGORY_GUI);
        m_ToggleHudTimer            = new Timer(CALL_CATEGORY_GUI);
        
        m_ActiveRefresherLocations  = new array<vector>;
        SyncEvents.RegisterEvents();
