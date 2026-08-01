// Source: F:/Games/Dayz/scripts/5_mission/mission/missionmainmenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MissionMainMenu extends MissionBase
{
    private UIScriptedMenu m_mainmenu;
    private CreditsMenu m_CreditsMenu;
    private ref DayZIntroScenePC m_IntroScenePC;
    private ref DayZIntroSceneXbox m_IntroSceneXbox;
 
    bool m_NoCutscene;
 
    override void OnInit()
    {
        if (!m_NoCutscene)
        {
            CreateIntroScene();
        }
        
        if (!m_mainmenu)
        {
            #ifdef PLATFORM_CONSOLE
            if ( g_Game.GetGameState() != DayZGameState.PARTY )
            {
                m_mainmenu = UIScriptedMenu.Cast( g_Game.GetUIManager().EnterScriptedMenu( MENU_TITLE_SCREEN, null ) );
            }
            #else
                m_mainmenu = UIScriptedMenu.Cast( g_Game.GetUIManager().EnterScriptedMenu( MENU_MAIN, null ) );
            #endif
        }
        
        GetOnInputDeviceChanged().Insert(OnInputDeviceChanged);
