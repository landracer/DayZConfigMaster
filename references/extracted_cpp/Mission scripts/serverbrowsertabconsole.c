// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/serverbrowsermenu/serverbrowsertabconsole.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ServerBrowserTabConsole extends ServerBrowserTab
{
    private bool m_IsFilterChanged;
    private bool m_IsFilterFocused;
    private bool m_IsDetailsFocused;
    
    private Widget m_WidgetNavFilters;
    private Widget m_WidgetNavServers;
    
    protected override void Construct( Widget parent, ServerBrowserMenuNew menu, TabType type )
    {
        m_Root = g_Game.GetWorkspace().CreateWidgets( &quot;gui/layouts/new_ui/server_browser/xbox/server_browser_tab.layout&quot;, parent );
        
        m_ServerListScroller    = ScrollWidget.Cast( m_Root.FindAnyWidget( &quot;server_list_scroller&quot; ) );
        m_ServerList            = SpacerBaseWidget.Cast( m_ServerListScroller.FindAnyWidget( &quot;server_list_content&quot; ) );
        m_ServerListScroller.VScrollToPos01( 0 );
        
        m_EntryWidgets          = new map<string, ref ServerBrowserEntry>;
        m_EntriesSorted         = new map<ESortType, ref array<ref GetServersResultRow>>;
        
        m_EntriesSorted[ESortType.HOST] = new array<ref GetServersResultRow>;
        m_EntriesSorted[ESortType.POPULATION] = new array<ref GetServersResultRow>;
        
        m_Menu                  = menu;
        m_TabType               = type;
        
        m_ApplyFilter           = m_Root.FindAnyWidget( &quot;apply_filter_button&quot; );
        m_RefreshList           = m_Root.FindAnyWidget( &quot;refresh_list_button&quot; );
        m_FiltersChanged        = m_Root.FindAnyWidget( &quot;unapplied_filters_notify&quot; );
        m_HostSort              = m_Root.FindAnyWidget( &quot;server_list_content_header_host&quot; );
        m_TimeSort              = m_Root.FindAnyWidget( &quot;server_list_content_header_time&quot; );
        m_PopulationSort        = m_Root.FindAnyWidget( &quot;server_list_content_header_population&quot; );
        m_SlotsSort             = m_Root.FindAnyWidget( &quot;server_list_content_header_slots&quot; );
        m_PingSort              = m_Root.FindAnyWidget( &quot;server_list_content_header_ping&quot; );
        m_MapSort               = m_Root.FindAnyWidget( &quot;server_list_content_header_map&quot; );
        m_LoadingText           = TextWidget.Cast( m_Root.FindAnyWidget( &quot;loading_servers_info&quot; ) );
        m_WidgetNavFilters      = m_Root.FindAnyWidget( &quot;filters_root_nav_wrapper&quot; );
        m_WidgetNavServers      = m_Root.FindAnyWidget( &quot;server_list_root_nav_wrapper&quot; );
        
        ShowHideConsoleWidgets();       
        
        m_Filters = new ServerBrowserFilterContainer(m_Root.FindAnyWidget(&quot;filters_content&quot;), this);
        
        SetSort(ESortType.HOST, ESortOrder.ASCENDING);
        
        m_Details = new ServerBrowserDetailsContainer(m_Root.FindAnyWidget(&quot;details_content&quot;), this);
        
        m_Root.SetHandler(this);
        
        SetFocusServers();
