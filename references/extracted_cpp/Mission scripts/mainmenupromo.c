// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/mainmenu/mainmenupromo.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class MainMenuDlcHandlerBase extends ScriptedWidgetEventHandler
{
    protected const string  TEXT_OWNED = &quot;#layout_dlc_owned&quot;;
    protected const string  TEXT_UNOWNED = &quot;#layout_dlc_unowned&quot;;
    
    protected int           m_ColorBackgroundOriginal;
    
    protected Widget        m_Root;
    protected Widget        m_BannerFrame;
    protected Widget        m_Background;
    protected Widget        m_StoreButton;
    protected Widget        m_GamepadStoreImage;
    protected ImageWidget   m_DlcPromotionImage;
    protected TextWidget    m_TitleTextDlc;
    protected MultilineTextWidget   m_DescriptionTextDlc;
    protected VideoWidget   m_VideoWidget;
    protected ref ModInfo   m_ThisModInfo;
    protected ref JsonDataDLCInfo   m_DlcInfo;
    
    protected ref BannerHandlerBase m_BannerHandler;
    
    void MainMenuDlcHandlerBase(ModInfo info, Widget parent, JsonDataDLCInfo DlcInfo)
    {
        CreateRootWidget(parent);
        m_Root.SetHandler(this);
        m_DlcInfo = DlcInfo;
        m_ThisModInfo = info;
        Init();
        
        #ifdef PLATFORM_CONSOLE
        g_Game.GetContentDLCService().m_OnChange.Insert(OnDLCChange);
        if (g_Game.GetMission())
        {
            g_Game.GetMission().GetOnInputDeviceChanged().Insert(OnInputDeviceChanged);
        }
        #endif
