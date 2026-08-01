// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/keybindings/keybindingsgroup.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class KeybindingsGroup extends ScriptedWidgetEventHandler
{
    protected Widget                                    m_Root;
    protected KeybindingsMenu                           m_Menu;
    
    protected ref map<int, ref KeybindingElement>       m_KeyWidgets;
    protected int                                       m_CurrentSettingKeyIndex = -1;
    protected int                                       m_CurrentSettingAlternateKeyIndex = -1;
    
    protected ref DropdownPrefab                        m_KBDropdown;
    
    void KeybindingsGroup( int index, Input input, Widget parent, KeybindingsMenu menu )
    {
        m_KeyWidgets    = new map<int, ref KeybindingElement>;
        m_Menu          = menu;
        
        string group_name;
        input.GetActionGroupName( index, group_name );
        
        m_Root  = g_Game.GetWorkspace().CreateWidgets( GetLayoutName(), parent );
        Widget subgroup = m_Root.FindAnyWidget( &quot;group_content&quot; );
        
//      for( int i = 0; i < 1; i++ )
//      {
            AddSubgroup( /*index, */subgroup, input );
//      }
        
        InitPresets( index, m_Root.FindAnyWidget( &quot;group_header&quot; ), input );
        
        subgroup.Update();
        
        m_Root.SetHandler( this );
