// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/mainmenu/tutorialsmenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TutorialsMenu extends UIScriptedMenu
{
    protected const string PATH_MOUSEKEY        = &quot;scripts/data/pagedatatutorials.json&quot;;
    protected const string PATH_MOUSEKEY_720P   = &quot;scripts/data/pagedatatutorials_720p.json&quot;;
    protected const string PATH_X1_OLD          = &quot;xbox/pagedatatutorials.json&quot;;
    protected const string PATH_X1_OLD_720P     = &quot;xbox/pagedatatutorials_720p.json&quot;;
    protected const string PATH_X1_NEW          = &quot;xbox/pagedatatutorialsalternate.json&quot;;
    protected const string PATH_X1_NEW_720p     = &quot;xbox/pagedatatutorialsalternate_720p.json&quot;;
    protected const string PATH_PS_OLD          = &quot;ps4/pagedatatutorials.json&quot;;
    protected const string PATH_PS_NEW          = &quot;ps4/pagedatatutorialsalternate.json&quot;;
    
    protected const float MIN_LINE_SCALE        = 1.0;
    protected const float MAX_LINE_SCALE        = 1.5;
    protected const float WITDH_SCALE_RES       = 1920.0;
    protected const float HEIGHT_SCALE_RES      = 1080.0;
    protected const float LINE_THICKNESS_BASE   = 2.0;
    protected const float BRANCH_OFFSET_BASE    = 50.0;
    
    protected const int EXACT_TEXT_SIZE_LOW_RES = 32;
 
    protected string                    m_BackButtonTextID;
    
    protected Widget                    m_InfoTextLeft;
    protected Widget                    m_InfoTextRight;
    protected ButtonWidget              m_Back;
    
    protected ImageWidget               m_ControlsLayoutImage;
    protected const int                 TABS_COUNT = 4;
    protected ImageWidget               m_tab_images[TABS_COUNT];
    protected TabberUI                  m_TabScript;
    //protected ref TutorialKeybinds        m_KeybindsTab;
    protected int m_CurrentTabIndex = 0;
    protected Input m_Input;
    
    protected bool m_LowResTutorialMode;
    
    void TutorialsMenu()
    {
        m_Input = g_Game.GetInput();
