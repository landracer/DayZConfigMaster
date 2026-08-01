// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/mainmenu/mainmenuconsoles.c Source File
// Extracted from DayZ Code Explorer by Zeroy

#ifndef BUILD_EXPERIMENTAL
    #define ENABLE_CAROUSEL
#endif
 
class MainMenuConsole extends UIScriptedMenu
{
    protected ref MainMenuVideo     m_Video;
    
    protected MissionMainMenu       m_Mission;
    protected DayZIntroScenePC      m_ScenePC;
    
    protected TextWidget            m_PlayerName;
    protected TextWidget            m_Version;
    
    protected Widget                m_MainMenuPanel;
    protected Widget                m_DialogPanel;
    protected Widget                m_ChangeAccount;
    protected Widget                m_CustomizeCharacter;
    protected Widget                m_PlayVideo;
    protected Widget                m_Tutorials;
    protected Widget                m_Options;
    protected Widget                m_Exit;
    protected Widget                m_Controls;
    protected Widget                m_Play;
    protected Widget                m_MessageButton;
    protected Widget                m_ShowFeedback;
    protected ImageWidget           m_FeedbackQRCode;
    protected ImageWidget           m_FeedbackPlatformIcon;
    protected ButtonWidget          m_FeedbackClose;
    protected RichTextWidget        m_FeedbackCloseLabel;
    
    protected ref Widget            m_LastFocusedButton;
    
    protected Widget                m_DlcFrame;
        
    protected ref NewsCarousel      m_NewsCarousel;
 
    protected Widget                m_NewsCarouselFrame;
    protected Widget                m_DisplayCarousel;
    
    protected ScreenWidthType       m_WidthType;
    protected int                   m_Width, m_Height;
 
    override Widget Init()
    {
        #ifdef PLATFORM_MSSTORE
        layoutRoot = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/main_menu_msstore.layout&quot;);
        #else
        layoutRoot = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/main_menu_console.layout&quot;);
        #endif
 
        m_MainMenuPanel = layoutRoot.FindAnyWidget(&quot;main_menu_panel&quot;);
        m_PlayerName = TextWidget.Cast(layoutRoot.FindAnyWidget(&quot;character_name_xbox&quot;));
        m_ChangeAccount = layoutRoot.FindAnyWidget(&quot;choose_account&quot;);
        m_CustomizeCharacter = layoutRoot.FindAnyWidget(&quot;customize_character&quot;);
        m_PlayVideo = layoutRoot.FindAnyWidget(&quot;play_video&quot;);
        m_Tutorials = layoutRoot.FindAnyWidget(&quot;tutorials&quot;);
        m_Options = layoutRoot.FindAnyWidget(&quot;options&quot;);
        m_Exit = layoutRoot.FindAnyWidget(&quot;exit&quot;);
        m_Controls = layoutRoot.FindAnyWidget(&quot;controls&quot;);
        m_Play = layoutRoot.FindAnyWidget(&quot;play&quot;);
        m_MessageButton = layoutRoot.FindAnyWidget(&quot;message_button&quot;);
        
        m_DlcFrame = layoutRoot.FindAnyWidget(&quot;dlc_Frame&quot;);
        m_Version = TextWidget.Cast(layoutRoot.FindAnyWidget(&quot;version&quot;));
        m_Mission = MissionMainMenu.Cast(g_Game.GetMission());
        m_ShowFeedback = layoutRoot.FindAnyWidget(&quot;feedback&quot;);
        m_FeedbackQRCode = ImageWidget.Cast(layoutRoot.FindAnyWidget(&quot;qr_image&quot;));
        m_FeedbackClose = ButtonWidget.Cast(layoutRoot.FindAnyWidget(&quot;close_button&quot;));
        m_FeedbackCloseLabel = RichTextWidget.Cast(layoutRoot.FindAnyWidget(&quot;close_button_label&quot;));
        m_DialogPanel = layoutRoot.FindAnyWidget(&quot;main_menu_dialog&quot;);
        m_NewsCarouselFrame = layoutRoot.FindAnyWidget(&quot;carousel_Frame&quot;);
        
        m_LastFocusedButton = m_Play;
                
        g_Game.GetUIManager().ScreenFadeOut(1);
 
        string launch_done;
        if (!g_Game.GetProfileString(&quot;FirstLaunchDone&quot;, launch_done) || launch_done != &quot;true&quot;)
        {
            g_Game.SetProfileString(&quot;FirstLaunchDone&quot;, &quot;true&quot;);
            g_Game.GetUIManager().ShowDialog(&quot;#main_menu_tutorial&quot;, &quot;#main_menu_tutorial_desc&quot;, 555, DBT_YESNO, DBB_YES, DMT_QUESTION, this);
            g_Game.SaveProfile();
        }
        
        UpdateControlsElementVisibility();
        LoadMods();
        Refresh();
        
        CheckWidth();
 
        #ifdef ENABLE_CAROUSEL
        m_NewsCarousel = new NewsCarousel(m_NewsCarouselFrame, this);
        #else
        m_NewsCarousel = null;
        #endif
        
        if (g_Game.GetMission())
        {
            g_Game.GetMission().GetOnInputPresetChanged().Insert(OnInputPresetChanged);
            g_Game.GetMission().GetOnInputDeviceChanged().Insert(OnInputDeviceChanged);
        }
        
        OnInputDeviceChanged(g_Game.GetInput().GetCurrentInputDevice());
        
        g_Game.GetContentDLCService().m_OnChange.Insert(OnDLCChange);
 
        #ifdef PLATFORM_MSSTORE
        // Enable exit button
        m_Exit.Show(true);
        #endif
        
        #ifdef PLATFORM_CONSOLE
        #ifndef PLATFORM_PS4
        #ifdef PLATFORM_MSSTORE
        m_ChangeAccount.Show(false);
        #else
        m_ChangeAccount.Show(GetGame().GetInput().IsEnabledMouseAndKeyboard());
        #endif
        m_FeedbackQRCode.LoadImageFile(0, &quot;gui/textures/feedback_qr_xbox.edds&quot;);
        #else
        m_FeedbackQRCode.LoadImageFile(0, &quot;gui/textures/feedback_qr_ps.edds&quot;);
        #endif
        #endif
        
        #ifdef PLATFORM_CONSOLE
        #ifdef PLATFORM_XBOX
        m_ChangeAccount.Show(false);
        #endif
        #endif
        m_DlcFrame.Show(false);
 
        #ifdef PLATFORM_MSSTORE
        // Disable controls button on MS Store by default   
        m_Controls.Show(false);
        #endif
 
        return layoutRoot;
