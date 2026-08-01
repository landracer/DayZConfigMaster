// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/modsmenu/modsmenusimple.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ModsMenuSimple extends ScriptedWidgetEventHandler
{
    protected const int                                     MOD_DISPLAY_COUNT_MAX = 3;
    
    protected Widget                                        m_Root;
    protected Widget                                        m_MoreButton;
    protected Widget                                        m_MoreHighlight;
    protected ref map<ModInfo, ref ModsMenuSimpleEntry>     m_Data;
    protected ModsMenuDetailed                              m_DetailMenu;
    
    void ModsMenuSimple(array<ref ModInfo> data, Widget parent, ModsMenuDetailed detail_menu)
    {
        m_Root = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/mods_menu/mods_menu_simple.layout&quot;, parent);
        m_MoreButton = m_Root.FindAnyWidget(&quot;ModMore&quot;);
        m_MoreHighlight = m_Root.FindAnyWidget(&quot;ModMoreOverlay&quot;);
        m_Data = new map<ModInfo, ref ModsMenuSimpleEntry>;
        m_DetailMenu = detail_menu;
        
        m_Root.SetHandler(this);
        LoadEntries(data);
