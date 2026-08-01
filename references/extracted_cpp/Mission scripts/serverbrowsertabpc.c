// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/serverbrowsermenu/serverbrowsertabpc.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ServerBrowserTabPc extends ServerBrowserTab
{
    protected const int SERVERS_VISIBLE_COUNT = 24;
    protected const int PAGES_BUTTONS_COUNT = 10;
    
    protected int m_TotalServersCount; //UNUSED
    protected int m_PageIndex;
    protected int m_PageStartNum;
    protected int m_PageEndNum;
    protected int m_PagesCount;
    protected int m_ServersEstimateCount;
        
    protected Widget m_PnlPagesPanel;
    protected TextWidget m_PnlPagesLoadingText;
    protected ButtonWidget m_BtnPagesFirst;
    protected ButtonWidget m_BtnPagesLast;
 
    protected Widget m_FilterSearchIP;
    protected Widget m_FilterSearchIPBox;
    protected Widget m_FilterPanelPing;
    protected Widget m_FilterPanelAccTime;
    
    protected ref array<ButtonWidget> m_BtnPages;
    protected ref array<ref ServerBrowserEntry> m_ServerListEntries;
    
    protected ref TStringArray m_TempTime = new TStringArray();
    
    protected override void Construct(Widget parent, ServerBrowserMenuNew menu, TabType type)
    {
    #ifdef PLATFORM_CONSOLE
        m_Root = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/server_browser/xbox/server_browser_tab.layout&quot;, parent);
    #else
    #ifdef PLATFORM_WINDOWS
        m_Root = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/server_browser/pc/server_browser_tab_pages.layout&quot;, parent);
    #endif
    #endif
 
        m_ServerListScroller    = ScrollWidget.Cast(m_Root.FindAnyWidget(&quot;server_list_scroller&quot;));
        m_ServerList            = SpacerBaseWidget.Cast(m_ServerListScroller.FindAnyWidget(&quot;server_list_content&quot;));
        m_ServerListScroller.VScrollToPos01(0);
                
        m_ServerListEntries = new array<ref ServerBrowserEntry>();      
        m_EntryWidgets = new map<string, ref ServerBrowserEntry>();
        m_SortInverted = new map<ESortType, ESortOrder>();
        m_EntriesSorted = new map<ESortType, ref array<ref GetServersResultRow>>();
        m_EntryMods = new map<string, ref array<string>>();
        
        m_EntriesSorted[ESortType.HOST] = new array<ref GetServersResultRow>();
        m_EntriesSorted[ESortType.TIME] = new array<ref GetServersResultRow>();
        m_EntriesSorted[ESortType.POPULATION] = new array<ref GetServersResultRow>();
        m_EntriesSorted[ESortType.SLOTS] = new array<ref GetServersResultRow>();
        m_EntriesSorted[ESortType.PING] = new array<ref GetServersResultRow>();
        m_EntriesSorted[ESortType.FAVORITE] = new array<ref GetServersResultRow>();
        m_EntriesSorted[ESortType.PASSWORDED] = new array<ref GetServersResultRow>();
        m_EntriesSorted[ESortType.MAP] = new array<ref GetServersResultRow>();
        
        m_Menu = menu;
        m_TabType = type;
        
        m_RightAreaHeaderText = TextWidget.Cast(m_Root.FindAnyWidget(&quot;right_area_header_text&quot;));
        m_BtnShowFilters = ButtonWidget.Cast(m_Root.FindAnyWidget(&quot;show_filters_button&quot;));
        
        m_DetailsRoot = m_Root.FindAnyWidget(&quot;details_root&quot;);
        m_BtnShowDetails = ButtonWidget.Cast(m_Root.FindAnyWidget(&quot;show_details_button&quot;));
        
        m_FilterRoot = m_Root.FindAnyWidget(&quot;filters_root&quot;);
        m_FilterContent = m_Root.FindAnyWidget(&quot;filters_content&quot;);
        m_ApplyFilter = m_Root.FindAnyWidget(&quot;apply_filter_button&quot;);
        m_RefreshList = m_Root.FindAnyWidget(&quot;refresh_list_button&quot;);
        m_FiltersChanged = m_Root.FindAnyWidget(&quot;unapplied_filters_notify&quot;);
        m_HostSort = m_Root.FindAnyWidget(&quot;server_list_content_header_host&quot;);
        m_TimeSort = m_Root.FindAnyWidget(&quot;server_list_content_header_time&quot;);
        m_PopulationSort = m_Root.FindAnyWidget(&quot;server_list_content_header_population&quot;);
        m_SlotsSort = m_Root.FindAnyWidget(&quot;server_list_content_header_slots&quot;);
        m_PingSort = m_Root.FindAnyWidget(&quot;server_list_content_header_ping&quot;);
        m_MapSort = m_Root.FindAnyWidget(&quot;server_list_content_header_map&quot;);
        
        m_FilterSearchText = m_Root.FindAnyWidget(&quot;search_name_setting&quot;);
        m_FilterSearchTextBox = m_Root.FindAnyWidget(&quot;search_name_setting_option&quot;);
        m_FilterSearchIP = m_Root.FindAnyWidget(&quot;search_ip_setting&quot;);
        m_FilterSearchIPBox = m_Root.FindAnyWidget(&quot;search_ip_setting_option&quot;);
        
        m_FilterPanelAccTime = m_Root.FindAnyWidget(&quot;accelerated_time_setting&quot;);
        m_FilterPanelPing = m_Root.FindAnyWidget(&quot;ping_setting&quot;);
        //m_FilterPanelPing.SetHandler(this);
        
        m_LoadingText = TextWidget.Cast(m_Root.FindAnyWidget(&quot;loading_servers_info&quot;));
        
        ButtonDisable(m_ApplyFilter);
        
        m_Filters = new ServerBrowserFilterContainer(m_Root.FindAnyWidget(&quot;filters_content&quot;), this);
                
        m_Root.SetHandler(this);        
        
        SetSort(ESortType.HOST, ESortOrder.ASCENDING);
        
        m_BtnPages = new array<ButtonWidget>();
        
        int page_button_num = 1;
        ButtonWidget page_button = ButtonWidget.Cast(m_Root.FindAnyWidget(&quot;servers_navigation_page&quot; + page_button_num));
        
        if (PAGES_BUTTONS_COUNT < 3 || PAGES_BUTTONS_COUNT > 10)
        {
            Error(&quot;ServerBrowserTabPage->PAGES_BUTTONS_COUNT must be in range 3 - 10!&quot;);
            return;
        }
        
        while (page_button)
        {
            page_button.Show(false);
            
            if (page_button_num <= PAGES_BUTTONS_COUNT)
            {
                m_BtnPages.Insert(page_button);
            }
            
            page_button_num++;
            page_button = ButtonWidget.Cast(m_Root.FindAnyWidget(&quot;servers_navigation_page&quot;+ page_button_num));
        }       
        
        m_ResetFilters = ButtonWidget.Cast(m_Root.FindAnyWidget( &quot;reset_filter_button&quot;));
        
        m_PnlPagesPanel = m_Root.FindAnyWidget(&quot;servers_navigation_spacer&quot;);
        m_BtnPagePrev = ButtonWidget.Cast(m_Root.FindAnyWidget(&quot;servers_navigation_prev&quot;));
        m_BtnPageNext = ButtonWidget.Cast(m_Root.FindAnyWidget(&quot;servers_navigation_next&quot;));
        m_BtnPagesFirst = ButtonWidget.Cast(m_Root.FindAnyWidget(&quot;servers_navigation_page_first&quot;));
        m_BtnPagesLast = ButtonWidget.Cast(m_Root.FindAnyWidget(&quot;servers_navigation_page_last&quot;));
        m_PageIndex = 0;
        m_PnlPagesPanel.Show(true);
        
        UpdatePageButtons();
        
        m_Details = new ServerBrowserDetailsContainer(m_Root.FindAnyWidget(&quot;details_content&quot;), this);
            
        if (m_TabType == TabType.FAVORITE || m_TabType == TabType.LAN)
        {
            m_Root.FindAnyWidget(&quot;filters_content&quot;).Show(false);
            m_Root.FindAnyWidget(&quot;reset_filter_button&quot;).Show(false);
            m_ApplyFilter.Show(false);
            m_Root.FindAnyWidget(&quot;spacer&quot;).Show(false);
            m_Root.FindAnyWidget(&quot;spacer1&quot;).Show(false);
            m_Root.FindAnyWidget(&quot;spacer5&quot;).Show(false);
            m_BtnShowFilters.Show(false);
            m_FilterRoot.Show(true);
        }
        
        if (m_TabType != TabType.FAVORITE && m_TabType != TabType.LAN)
        {
            SwitchToFilters();
        }
        else
        {
            SwitchToDetails();
        }
        
    #ifdef DIAG_DEVELOPER
        InitDummyServers();
    #endif
