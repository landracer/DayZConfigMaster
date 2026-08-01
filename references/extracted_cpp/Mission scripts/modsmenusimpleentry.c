// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/modsmenu/modsmenusimpleentry.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ModsMenuSimpleEntry extends ScriptedWidgetEventHandler
{
    protected ButtonWidget      m_ModButton;
    protected ImageWidget       m_Icon;
    protected Widget            m_Hover;
 
    protected bool              m_HasLogoOver;
    protected ModInfo           m_Data;
    protected ModsMenuSimple    m_ParentMenu;
 
    void ModsMenuSimpleEntry(ModInfo data, int index, Widget parent, ModsMenuSimple parent_menu)
    {
        m_ModButton     = ButtonWidget.Cast(g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/mods_menu/mods_menu_simple_entry.layout&quot;, parent));
        m_Icon          = ImageWidget.Cast(m_ModButton.FindAnyWidget(&quot;Icon&quot;));
        m_Hover         = m_ModButton.FindAnyWidget(&quot;Overlay&quot;);
        m_Data          = data;
        m_ParentMenu    = parent_menu;
        
        m_ModButton.SetSort(index);
        m_ModButton.SetHandler(this);
        
        LoadData();
