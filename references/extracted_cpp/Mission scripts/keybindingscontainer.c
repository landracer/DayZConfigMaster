// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/keybindings/keybindingscontainer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//container for various subgroups and their elements
class KeybindingsContainer extends ScriptedWidgetEventHandler
{
    protected const int                                 NO_SORTING = -1;
    protected Widget                                    m_Root;
    protected KeybindingsMenu                           m_Menu;
    protected ScrollWidget                              m_Scroll;
    
    protected ref map<int, ref ElementArray>            m_KeyWidgetElements; //<input_action_id,<KeybindingElementNew>>
    protected int                                       m_CurrentSettingKeyIndex = -1;
    protected int                                       m_CurrentSettingAlternateKeyIndex = -1;
    
    protected ref array<Widget>                         m_Subgroups;
    
    void KeybindingsContainer( int index, Input input, Widget parent, KeybindingsMenu menu )
    {
        m_KeyWidgetElements = new map<int, ref ElementArray>;
        m_Menu          = menu;
        
        m_Root  = g_Game.GetWorkspace().CreateWidgets( GetLayoutName(), parent );
        m_Scroll = ScrollWidget.Cast(m_Root.FindAnyWidget(&quot;group_scroll&quot;));
        Widget container    = m_Root.FindAnyWidget( &quot;group_scroll&quot; );
        
        CreateSubgroups(container,input);
        
        container.Update();
        
        m_Root.SetHandler( this );
