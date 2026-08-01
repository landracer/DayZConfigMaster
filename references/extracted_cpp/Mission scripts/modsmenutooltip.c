// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/modsmenu/modsmenutooltip.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ModsMenuTooltip extends ScriptedWidgetEventHandler
{
    protected Widget            m_Root;
    protected RichTextWidget    m_Text;
 
    void ModsMenuTooltip(Widget parent)
    {
        m_Root = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/new_ui/mods_menu/mods_tooltip.layout&quot;, parent);
        m_Text = RichTextWidget.Cast(m_Root.FindAnyWidget(&quot;TooltipText&quot;));
