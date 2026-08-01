// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/serverbrowsermenu/serverbrowserfavoritestabconsolepages.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ServerBrowserFavoritesTabConsolePages extends ServerBrowserTabConsolePages
{
    protected bool m_IsFavoriteChanged;
 
    protected override void Construct(Widget parent, ServerBrowserMenuNew menu, TabType type)
    {
        super.Construct(parent, menu, type);
        
        // disabling filter section
        m_ResetFilters.Show(false);
        m_Root.FindAnyWidget(&quot;filters_content&quot;).Show(false);
        m_Root.FindAnyWidget(&quot;show_details_button&quot;).Show(false);
        m_Root.FindAnyWidget(&quot;spacer&quot;).Show(false);
        m_Root.FindAnyWidget(&quot;spacer1&quot;).Show(false);
        m_Root.FindAnyWidget(&quot;spacer5&quot;).Show(false);
        m_Root.FindAnyWidget(&quot;server_list_root_nav_wrapper&quot;).Show(false);
        m_RefreshList.Show(m_MouseKeyboardControlled);
        
        SwitchToDetails(false);
                    
        m_Menu.ShowYButton(false);
