// Source: F:/Games/Dayz/scripts/5_mission/gui/ingamemenu_xbox/playerlistscriptedwidget.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PlayerListScriptedWidget extends ScriptedWidgetEventHandler
{
    protected Widget                                                m_Root;
    protected ScrollWidget                                          m_ScrollContainer;
    protected Widget                                                m_Content;
    protected ref map<string, ref PlayerListEntryScriptedWidget>    m_Entries;
    
    protected int                                                   m_TotalEntries;
    protected PlayerListEntryScriptedWidget                         m_SelectedEntry;
    
    void PlayerListScriptedWidget( Widget parent, string header_text = &quot;&quot; )
    {
        m_Root              = g_Game.GetWorkspace().CreateWidgets( &quot;gui/layouts/xbox/ingamemenu_xbox/players_info_panel.layout&quot;, parent );
        m_ScrollContainer   = ScrollWidget.Cast( m_Root.FindAnyWidget( &quot;ScrollFrame&quot; ) );
        m_Content           = m_Root.FindAnyWidget( &quot;Content&quot; );
        
        m_Entries           = new map<string, ref PlayerListEntryScriptedWidget>;
 
        m_ScrollContainer.VScrollToPos01( 0 );
