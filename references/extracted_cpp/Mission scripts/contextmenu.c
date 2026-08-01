// Source: F:/Games/Dayz/scripts/5_mission/gui/contextmenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//--------------------------------------------------------------------------
class ContextMenu extends ScriptedWidgetEventHandler
{
    private Widget m_context_menu_root_widget;
    private Widget m_context_menu_panel_widget;
    private ref array<ref CallQueueContext> m_commands;
    private int m_max_item_width;
    private int m_count;
    const int ITEMS_COUNT = 27;
    
    //--------------------------------------------------------------------------
    void ContextMenu()
    {
        m_commands = new array<ref CallQueueContext>;
        m_count = 0;
