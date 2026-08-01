// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/serverbrowsermenu/serverbrowsermenunew.c Source File
// Extracted from DayZ Code Explorer by Zeroy

const int MAX_FAVORITES = 25;
 
#ifdef PLATFORM_CONSOLE
const int SERVER_BROWSER_PAGE_SIZE = 22;
#else
const int SERVER_BROWSER_PAGE_SIZE = 5;
#endif
 
class ServerBrowserMenuNew extends UIScriptedMenu
{
    protected Widget                m_Play;
    protected TextWidget            m_PlayButtonLabel;
    protected TextWidget            m_OpenStoreButtonLabel;
    
    protected Widget                m_Back;
    protected Widget                m_CustomizeCharacter;
    protected TextWidget            m_PlayerName;
    
    protected TabberUI              m_Tabber;
    protected ref ServerBrowserTab  m_OfficialTab;
    protected ref ServerBrowserTab  m_CommunityTab;
    protected ref ServerBrowserTab  m_FavoritesTab;
    protected ref ServerBrowserTab  m_LANTab;
    
    protected TabType               m_IsRefreshing = TabType.NONE;
    protected ref TStringArray      m_Favorites;
    protected ServerBrowserEntry    m_SelectedServer;
    
    override Widget Init()
    {
        #ifdef PLATFORM_CONSOLE
        layoutRoot      = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/server_browser/xbox/server_browser.layout&quot;);
        m_FavoritesTab  = new ServerBrowserFavoritesTabConsolePages(layoutRoot.FindAnyWidget(&quot;Tab_0&quot;), this, TabType.FAVORITE);
        m_OfficialTab   = new ServerBrowserTabConsolePages(layoutRoot.FindAnyWidget(&quot;Tab_1&quot;), this, TabType.OFFICIAL);
        m_CommunityTab  = new ServerBrowserTabConsolePages(layoutRoot.FindAnyWidget(&quot;Tab_2&quot;), this, TabType.COMMUNITY);
        #else
        layoutRoot      = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/server_browser/pc/server_browser.layout&quot;);
        m_FavoritesTab  = new ServerBrowserFavoritesTabPc(layoutRoot.FindAnyWidget(&quot;Tab_0&quot;), this, TabType.FAVORITE);
        m_OfficialTab   = new ServerBrowserTabPc(layoutRoot.FindAnyWidget(&quot;Tab_1&quot;), this, TabType.OFFICIAL);
        m_CommunityTab  = new ServerBrowserTabPc(layoutRoot.FindAnyWidget(&quot;Tab_2&quot;), this, TabType.COMMUNITY);
        m_LANTab        = new ServerBrowserTabPc(layoutRoot.FindAnyWidget(&quot;Tab_3&quot;), this, TabType.LAN);
        #endif
        
        layoutRoot.FindAnyWidget(&quot;Tabber&quot;).GetScript(m_Tabber);
        
        m_Play                  = layoutRoot.FindAnyWidget(&quot;play&quot;);
        m_PlayButtonLabel       = TextWidget.Cast(m_Play.FindAnyWidget(&quot;play_label&quot;));
        m_OpenStoreButtonLabel  = TextWidget.Cast(m_Play.FindAnyWidget(&quot;open_store_label&quot;));
        m_Back                  = layoutRoot.FindAnyWidget(&quot;back_button&quot;);
        m_CustomizeCharacter    = layoutRoot.FindAnyWidget(&quot;customize_character&quot;);
        m_PlayerName            = TextWidget.Cast(layoutRoot.FindAnyWidget(&quot;character_name_text&quot;));
        m_Favorites             = new TStringArray;
        
        #ifndef PLATFORM_CONSOLE
        layoutRoot.FindAnyWidget(&quot;customize_character&quot;).Show(false);
        layoutRoot.FindAnyWidget(&quot;character&quot;).Show(false);
        #endif
        
        Refresh();
        
        string version;
        g_Game.GetVersion(version);
        
        #ifdef PLATFORM_CONSOLE
        version = &quot;#main_menu_version&quot; + &quot; &quot; + version + &quot; (&quot; + g_Game.GetDatabaseID() + &quot;)&quot;;
        if (g_Game.GetInput().IsEnabledMouseAndKeyboard())
        {
            layoutRoot.FindAnyWidget(&quot;toolbar_bg&quot;).Show(false);
        }
 
        RichTextWidget playPanelBack = RichTextWidget.Cast(layoutRoot.FindAnyWidget(&quot;BackIcon0&quot;));
        playPanelBack.SetText(InputUtils.GetRichtextButtonIconFromInputAction(&quot;UAUIBack&quot;, &quot;&quot;, EUAINPUT_DEVICE_CONTROLLER, InputUtils.ICON_SCALE_NORMAL));
 
        RichTextWidget playPanelPlay = RichTextWidget.Cast(layoutRoot.FindAnyWidget(&quot;PlayIcon0&quot;));
        playPanelPlay.SetText(InputUtils.GetRichtextButtonIconFromInputAction(&quot;UAUISelect&quot;, &quot;&quot;, EUAINPUT_DEVICE_CONTROLLER, InputUtils.ICON_SCALE_NORMAL));
        #else
        version = &quot;#main_menu_version&quot; + &quot; &quot; + version;
        #endif
        
        OnlineServices.m_ServersAsyncInvoker.Insert(OnLoadServersAsync);
        OnlineServices.m_ServerModLoadAsyncInvoker.Insert(OnLoadServerModsAsync);
        LoadFavoriteServers();
        
        m_Tabber.m_OnTabSwitch.Insert(OnTabSwitch);
        
        m_FavoritesTab.RefreshList();
 
        #ifdef PLATFORM_CONSOLE
        UpdateControlsElements();
        //Sort init
        TextWidget sort_text = TextWidget.Cast(layoutRoot.FindAnyWidget(&quot;SortText&quot;));
        sort_text.SetText(&quot;#str_serverbrowserroot_toolbar_bg_consoletoolbar_sort_sorttext0&quot;);
        #endif
        
        PPERequesterBank.GetRequester(PPERequester_ServerBrowserBlur).Start(new Param1<float>(0.5));
        
        g_Game.GetMission().GetOnInputPresetChanged().Insert(OnInputPresetChanged);
        g_Game.GetMission().GetOnInputDeviceChanged().Insert(OnInputDeviceChanged);
 
        #ifdef PLATFORM_WINDOWS
        g_Game.GetInput().EnableGamepad(false);
        #endif
        
        return layoutRoot;
