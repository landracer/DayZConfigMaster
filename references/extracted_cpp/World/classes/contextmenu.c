// Source: F:/Games/Dayz/scripts/4_world/classes/contextmenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//--------------------------------------------------------------------------
class ContextMenu extends ScriptedWidgetEventHandler
{
    protected static ref ContextMenu m_ContextMenuInstance;
    
    Widget m_context_menu_root_widget;
    private Widget m_context_menu_panel_widget;
    private ref array<ref CallQueueContext> m_commands;
    private int m_max_item_width;
    private int m_count;
    private bool m_builtIn = false;
    const int ITEMS_COUNT = 27;
    
    //--------------------------------------------------------------------------
    void ContextMenu()
    {
        m_commands = new array<ref CallQueueContext>;
        m_count = 0;
