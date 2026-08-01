// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/mainmenu/mainmenuvideo.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MainMenuVideo extends UIScriptedMenu
{
    protected string                m_BackButtonTextID;
    
    protected VideoWidget           m_Video;
    
    override Widget Init()
    {
        layoutRoot              = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/xbox/video_menu.layout&quot;);
        m_Video                 = VideoWidget.Cast(layoutRoot.FindAnyWidget(&quot;video&quot;));
        
        g_Game.GetMission().GetOnInputDeviceChanged().Insert(OnInputDeviceChanged);
        
        m_Video.Load(&quot;video\\DayZ_onboarding_MASTER.mp4&quot;);
            
        m_Video.Play();
        
        m_Video.SetCallback(VideoCallback.ON_END, StopVideo);
        
        return layoutRoot;
