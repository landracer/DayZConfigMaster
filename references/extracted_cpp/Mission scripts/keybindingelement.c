// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/keybindings/keybindingelement.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class KeybindingElement extends ScriptedWidgetEventHandler
{
    protected KeybindingsGroup              m_Group;
    
    protected Widget                        m_Root;
    protected TextWidget                    m_ElementName;
    protected TextWidget                    m_ElementModifier;
    protected ButtonWidget                  m_PrimaryBindButton;
    protected ButtonWidget                  m_AlternativeBindButton;
    protected Widget                        m_PrimaryClear;
    protected Widget                        m_AlternativeClear;
    
    protected int                           m_ElementIndex;
    protected bool                          m_IsEdited;
    protected bool                          m_IsAlternateEdited;
    
    protected ref array<int>                m_CustomBind;
    protected ref array<int>                m_CustomAlternateBind;
    
    protected ref Timer                     m_EntryTimer = new Timer( CALL_CATEGORY_GUI );
    
    void KeybindingElement( int key_index, Widget parent, KeybindingsGroup group )
    {
        m_Root                  = g_Game.GetWorkspace().CreateWidgets( GetLayoutName(), parent );
        m_ElementName           = TextWidget.Cast( m_Root.FindAnyWidget( &quot;setting_label&quot; ) );
        m_ElementModifier       = TextWidget.Cast( m_Root.FindAnyWidget( &quot;modifier_label&quot; ) );
        m_PrimaryBindButton     = ButtonWidget.Cast( m_Root.FindAnyWidget( &quot;primary_bind&quot; ) );
        m_AlternativeBindButton = ButtonWidget.Cast( m_Root.FindAnyWidget( &quot;alternative_bind&quot; ) );
        m_PrimaryClear          = m_Root.FindAnyWidget( &quot;primary_clear&quot; );
        m_AlternativeClear      = m_Root.FindAnyWidget( &quot;alternative_clear&quot; );
        
        m_Group                 = group;
        m_ElementIndex          = key_index;
        
        Reload();
        m_Root.SetHandler( this );
