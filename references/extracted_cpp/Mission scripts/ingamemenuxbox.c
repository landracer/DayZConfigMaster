// Source: F:/Games/Dayz/scripts/5_mission/gui/ingamemenuxbox.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class InGameMenuXbox extends UIScriptedMenu
{
    // Widgets texts id
    protected  string                       m_MuteButtonTextID;
    protected  string                       m_UnmuteButtonTextID;
    protected  string                       m_BackButtonTextID;
    protected  string                       m_SelectButtonTextID;
    protected  string                       m_OpenGameCardButtonTextID;
    protected  string                       m_CurrentMuteButtonText;
    
    protected bool                          m_SelectAvailable;
    protected bool                          m_MuteAvailable;
    protected bool                          m_BackAvailable;
    protected bool                          m_GamercardAvailable;
    
    protected bool                          m_PlayerAlive;
    
    private float                           m_ExitButtonUpdateTimerSum;
    private bool                            m_ExitOnCooldown;
    
    protected ref PlayerListScriptedWidget  m_ServerInfoPanel;
    
    protected Widget                        m_OnlineMenu;
    
    protected Widget                        m_IngameMenuPanel;
    protected Widget                        m_DialogPanel;
    protected ButtonWidget                  m_ContinueButton;
    protected ButtonWidget                  m_ExitButton;
    protected ButtonWidget                  m_RestartDeadButton;
    protected ButtonWidget                  m_RestartButton;
    protected ButtonWidget                  m_OptionsButton;
    protected ButtonWidget                  m_InviteButton;
    protected ButtonWidget                  m_ControlsButton;
    protected ButtonWidget                  m_OnlineButton;
    protected ButtonWidget                  m_TutorialsButton;  
    protected TextWidget                    m_Version;
    protected TextWidget                    m_ExitButtonLabel;
    protected TextWidget                    m_ExitButtonTime;
 
    protected ButtonWidget                  m_ShowFeedback;
    protected ImageWidget                   m_FeedbackQRCode;
    protected ImageWidget                   m_FeedbackPlatformIcon;
    protected ButtonWidget                  m_FeedbackClose;
    protected RichTextWidget                m_FeedbackCloseLabel;
    
    const int                               BUTTON_XBOX_CONTROLS = 201;
    
    void InGameMenuXbox()
    {
        g_Game.GetMission().GetOnInputPresetChanged().Insert(OnInputPresetChanged);
        g_Game.GetMission().GetOnInputDeviceChanged().Insert(OnInputDeviceChanged);
