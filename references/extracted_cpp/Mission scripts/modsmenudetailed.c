// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/modsmenu/modsmenudetailed.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ModsMenuDetailed extends ScriptedWidgetEventHandler
{
    protected Widget                                            m_Root;
    protected Widget                                            m_Content;
    protected Widget                                            m_CloseButton;
    protected ScrollWidget                                      m_Scroll;
    protected ref map<ref ModInfo, ref ModsMenuDetailedEntry>   m_Data;
    
    protected ModInfo                                           m_Highlighted;
    
    //protected MainMenu                                            m_Menu;
    protected UIScriptedMenu                                            m_Menu;
    protected ModsMenuTooltip                                   m_Tooltip;
    protected ref Timer                                         m_TooltipTimer;
    protected ModInfo                                           m_TooltipMod;
 
    void ModsMenuDetailed(array<ref ModInfo> data, Widget parent, ModsMenuTooltip tooltip, UIScriptedMenu menu_parent)
    {
        m_Root = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/mods_menu/mods_menu_detailed.layout&quot;, parent);
        m_Content = m_Root.FindAnyWidget(&quot;ModsDetailedContent&quot;);
        m_Scroll = ScrollWidget.Cast(m_Root.FindAnyWidget(&quot;ModsDetailedScroller&quot;));
        m_CloseButton = m_Root.FindAnyWidget(&quot;ModsDetailedHeaderButton&quot;);
        
        m_Menu = menu_parent;
        m_Data = new map<ref ModInfo, ref ModsMenuDetailedEntry>;
        m_Tooltip = tooltip;
        
        m_Root.SetHandler(this);
 
        LoadEntries(data);
