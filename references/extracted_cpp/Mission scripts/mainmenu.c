// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/mainmenu/mainmenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MainMenu extends UIScriptedMenu
{
    protected ref MainMenuStats     m_Stats;
    protected ref MainMenuVideo     m_Video;
    
    protected MissionMainMenu       m_Mission;
    protected DayZIntroScenePC      m_ScenePC;
    
    protected TextWidget            m_PlayerName;
    protected TextWidget            m_Version;
    
    protected Widget                m_CharacterRotationFrame;
    
    protected Widget                m_Play;
    protected Widget                m_ChooseServer;
    protected Widget                m_CustomizeCharacter;
    protected Widget                m_PlayVideo;
    protected Widget                m_Feedback;
    protected Widget                m_Tutorials;
    protected Widget                m_TutorialButton;
    protected Widget                m_MessageButton;
    protected Widget                m_SettingsButton;
    protected Widget                m_Exit;
    protected Widget                m_NewsMain;
    protected Widget                m_NewsSec1;
    protected Widget                m_NewsSec2;
    protected Widget                m_PrevCharacter;
    protected Widget                m_NextCharacter;
    
    protected Widget                m_LastPlayedTooltip;
    protected Widget                m_LastPlayedTooltipLabel;
    protected TextWidget            m_LastPlayedTooltipName;
    protected TextWidget            m_LastPlayedTooltipIP;
    protected TextWidget            m_LastPlayedTooltipPort;
    
    protected ref WidgetFadeTimer   m_LastPlayedTooltipTimer;
    protected ref Widget            m_LastFocusedButton;
    
    protected ref TextWidget        m_ModdedWarning;
    protected ref ModsMenuSimple    m_ModsSimple;
    protected ref ModsMenuDetailed  m_ModsDetailed;
    protected ref ModsMenuTooltip   m_ModsTooltip;
    
    protected Widget                m_DlcFrame;
 
    protected Widget                m_NewsCarouselFrame;
    protected ref NewsCarousel      m_NewsCarousel;
    protected bool                  m_DisplayCarousel;
    
    protected int                   m_PrevWidth, m_PrevHeight;
    protected int                   m_Width, m_Height;
    
    override Widget Init()
    {
        layoutRoot = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/main_menu.layout&quot;);
        
        m_Play                      = layoutRoot.FindAnyWidget(&quot;play&quot;);
        m_ChooseServer              = layoutRoot.FindAnyWidget(&quot;choose_server&quot;);
        m_CustomizeCharacter        = layoutRoot.FindAnyWidget(&quot;customize_character&quot;);
        m_PlayVideo                 = layoutRoot.FindAnyWidget(&quot;play_video&quot;);
        m_Feedback                  = layoutRoot.FindAnyWidget(&quot;feedback_button&quot;);
        m_Tutorials                 = layoutRoot.FindAnyWidget(&quot;tutorials&quot;);
        m_TutorialButton            = layoutRoot.FindAnyWidget(&quot;tutorial_button&quot;);
        m_MessageButton             = layoutRoot.FindAnyWidget(&quot;message_button&quot;);
        m_SettingsButton            = layoutRoot.FindAnyWidget(&quot;settings_button&quot;);
        m_Exit                      = layoutRoot.FindAnyWidget(&quot;exit_button&quot;);
        m_PrevCharacter             = layoutRoot.FindAnyWidget(&quot;prev_character&quot;);
        m_NextCharacter             = layoutRoot.FindAnyWidget(&quot;next_character&quot;);
 
        m_DlcFrame                  = layoutRoot.FindAnyWidget(&quot;dlc_Frame&quot;);
        m_NewsCarouselFrame         = layoutRoot.FindAnyWidget(&quot;carousel_Frame&quot;);
        m_Version                   = TextWidget.Cast(layoutRoot.FindAnyWidget(&quot;version&quot;));
        m_ModdedWarning             = TextWidget.Cast(layoutRoot.FindAnyWidget(&quot;ModdedWarning&quot;));
        m_CharacterRotationFrame    = layoutRoot.FindAnyWidget(&quot;character_rotation_frame&quot;);
        
        m_LastPlayedTooltip         = layoutRoot.FindAnyWidget(&quot;last_server_info&quot;);
        m_LastPlayedTooltip.Show(false);
        m_LastPlayedTooltipLabel    = m_LastPlayedTooltip.FindAnyWidget(&quot;last_server_info_label&quot;);
        m_LastPlayedTooltipName     = TextWidget.Cast(m_LastPlayedTooltip.FindAnyWidget(&quot;last_server_info_name&quot;));
        m_LastPlayedTooltipIP       = TextWidget.Cast(m_LastPlayedTooltip.FindAnyWidget(&quot;last_server_info_ip&quot;));
        m_LastPlayedTooltipPort     = TextWidget.Cast(m_LastPlayedTooltip.FindAnyWidget(&quot;last_server_info_port&quot;));
        
        m_LastPlayedTooltipTimer    = new WidgetFadeTimer();
        
        m_Stats                     = new MainMenuStats(layoutRoot.FindAnyWidget(&quot;character_stats_root&quot;));
        
        m_Mission                   = MissionMainMenu.Cast(g_Game.GetMission());
        
        m_LastFocusedButton         = m_Play;
 
        m_ScenePC                   = m_Mission.GetIntroScenePC();
        
        if (m_ScenePC)
        {
            m_ScenePC.ResetIntroCamera();
        }
        
        m_PlayVideo.Show(false);
        
        m_PlayerName                = TextWidget.Cast(layoutRoot.FindAnyWidget(&quot;character_name_text&quot;));
        
        // Set Version
        string version;
        g_Game.GetVersion(version);
        m_Version.SetText(&quot;#main_menu_version&quot; + &quot; &quot; + version);
        
        g_Game.GetUIManager().ScreenFadeOut(0);
 
        SetFocus(null);
        
        Refresh();
        
        LoadMods();
        
        CheckWidth();
        
        #ifndef BUILD_EXPERIMENTAL
        m_NewsCarousel = new NewsCarousel(m_NewsCarouselFrame, this);
        #else
        m_NewsCarousel = null;
        #endif
 
        ShowNewsCarousel(true);
        
        g_Game.GetMission().GetOnModMenuVisibilityChanged().Insert(ShowNewsCarousel);
        GetDayZGame().GetBacklit().MainMenu_OnShow();
    
        g_Game.SetLoadState(DayZLoadState.MAIN_MENU_CONTROLLER_SELECT);
                
        return layoutRoot;
