// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/serverbrowsermenu/serverbrowserfiltercontainer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ServerBrowserFilterContainer extends ScriptedWidgetEventHandler
{
    ref map<string, string>     m_Options = new map<string, string>;
    
    protected EditBoxWidget m_SearchByName;
    protected EditBoxWidget m_SearchByIP;
    protected ButtonWidget m_RefreshList;
 
    ref OptionSelectorMultistate m_CharacterAliveFilter;
    ref OptionSelectorMultistate m_SortingFilter;
    ref OptionSelectorMultistate m_RegionFilter;
    ref OptionSelectorMultistate m_PingFilter;
    ref OptionSelectorMultistate m_MapFilter;
    ref OptionSelector m_FavoritedFilter;
    ref OptionSelector m_FriendsPlayingFilter;
    ref OptionSelector m_PasswordFilter;
    ref OptionSelector m_WhitelistFilter;
    ref OptionSelector m_KeyboardFilter;
    ref OptionSelector m_PreviouslyPlayedFilter;
    ref OptionSelector m_VersionMatchFilter;
    ref OptionSelector m_FullServerFilter;
    ref OptionSelector m_ThirdPersonFilter;
    protected ServerBrowserTab m_Tab;
    
    void ServerBrowserFilterContainer(Widget root, ServerBrowserTab parent)
    {
        string player_name;
        g_Game.GetPlayerName(player_name);
        m_Tab = parent;
        
        array<string> character_name_options ={&quot;#server_browser_disabled&quot;, player_name};
        array<string> region_options = {&quot;#server_browser_all&quot;, &quot;#server_browser_americas&quot;, &quot;#server_browser_europe&quot;, &quot;#server_browser_asia&quot;};
        array<string> sort_options = {&quot;#server_browser_column_host A-Z&quot;, &quot;#server_browser_column_host Z-A&quot;, &quot;#players_title >&quot;, &quot;#players_title <&quot;};
        array<string> ping_options = {&quot;#server_browser_disabled&quot;, &quot;<30&quot;, &quot;<50&quot;, &quot;<100&quot;, &quot;<200&quot;, &quot;<300&quot;, &quot;<500&quot;};
        array<string> two_options = {&quot;#server_browser_disabled&quot;, &quot;#server_browser_show&quot;};
        array<string> map_options = LoadMapFilterOptions();
 
        m_SearchByName = EditBoxWidget.Cast(root.FindAnyWidget(&quot;search_name_setting_option&quot;));
        m_RefreshList = ButtonWidget.Cast(root.GetParent().FindAnyWidget(&quot;refresh_list_button&quot;));
        
        m_RegionFilter = new OptionSelectorMultistate(root.FindAnyWidget(&quot;region_setting_option&quot;), 0, this, false, region_options);
        m_PingFilter = new OptionSelectorMultistate(root.FindAnyWidget(&quot;ping_setting_option&quot;), 0, this, false, ping_options);
        #ifdef PLATFORM_CONSOLE
        m_FavoritedFilter = new OptionSelectorMultistate(root.FindAnyWidget(&quot;favorites_setting_option&quot;), 0, this, false, two_options);
        #else
        m_FavoritedFilter = new OptionSelector(root.FindAnyWidget(&quot;favorites_setting_option&quot;), 0, this, false);
        #endif
 
        m_MapFilter = new OptionSelectorMultistate(root.FindAnyWidget(&quot;map_setting_option&quot;), 0, this, false, map_options);
        
        m_FriendsPlayingFilter = new OptionSelector(root.FindAnyWidget(&quot;friends_setting_option&quot;), 0, this, false);
        m_PreviouslyPlayedFilter = new OptionSelector(root.FindAnyWidget(&quot;prev_played_setting_option&quot;), 0, this, false);
        m_FullServerFilter = new OptionSelector(root.FindAnyWidget(&quot;full_server_setting_option&quot;), 0, this, false);
        m_ThirdPersonFilter = new OptionSelector(root.FindAnyWidget(&quot;tps_setting_option&quot;), 0, this, false);
        m_PasswordFilter = new OptionSelector(root.FindAnyWidget(&quot;password_setting_option&quot;), 0, this, false);
        m_WhitelistFilter = new OptionSelector(root.FindAnyWidget(&quot;whitelist_setting_option&quot;), 0, this, false);
 
        m_RegionFilter.m_OptionChanged.Insert(OnFilterChanged);
        m_PingFilter.m_OptionChanged.Insert(OnFilterChanged);
        m_FavoritedFilter.m_OptionChanged.Insert(OnFilterChanged);
        m_MapFilter.m_OptionChanged.Insert(OnMapFilterChanged);
        m_FriendsPlayingFilter.m_OptionChanged.Insert(OnFilterChanged);
        m_PreviouslyPlayedFilter.m_OptionChanged.Insert(OnFilterChanged);
        m_FullServerFilter.m_OptionChanged.Insert(OnFilterChanged);
        m_ThirdPersonFilter.m_OptionChanged.Insert(OnFilterChanged);
        m_PasswordFilter.m_OptionChanged.Insert(OnFilterChanged);
        m_WhitelistFilter.m_OptionChanged.Insert(OnFilterChanged);
        
        #ifdef PLATFORM_CONSOLE
            m_SortingFilter = new OptionSelectorMultistate(root.FindAnyWidget(&quot;sort_setting_option&quot;), 0, this, false, sort_options);
            m_KeyboardFilter = new OptionSelector(root.FindAnyWidget(&quot;keyboard_setting_option&quot;), 0, this, false);
        
            m_SortingFilter.m_OptionChanged.Insert(OnSortChanged);
            m_SortingFilter.m_OptionChanged.Insert(OnFilterChanged);
            m_KeyboardFilter.m_OptionChanged.Insert(OnFilterChanged);
        
            m_PingFilter.Disable();
            m_FriendsPlayingFilter.Disable();
        #endif
        
        #ifdef PLATFORM_WINDOWS
            #ifndef PLATFORM_CONSOLE    
                m_SearchByIP = EditBoxWidget.Cast(root.FindAnyWidget(&quot;search_ip_setting_option&quot;));
        
                m_SearchByName.SetHandler(this);
                m_SearchByIP.SetHandler(this);
        
                m_CharacterAliveFilter = new OptionSelectorMultistate(root.FindAnyWidget(&quot;restrict_char_setting_option&quot;), 0, this, false, character_name_options);
                m_VersionMatchFilter = new OptionSelector(root.FindAnyWidget(&quot;ver_match_setting_option&quot;), 0, this, false);
        
                m_CharacterAliveFilter.m_OptionChanged.Insert(OnFilterChanged);
                m_VersionMatchFilter.m_OptionChanged.Insert(OnFilterChanged);
            #endif
        #endif
        LoadFilters();
