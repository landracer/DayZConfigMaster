// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/dropdownprefab/dropdownprefab.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DropdownPrefab extends ScriptedWidgetEventHandler
{
    protected Widget                m_Root;
    protected Widget                m_Parent;
    protected ScrollWidget          m_Scroller;
    protected Widget                m_ContentContainer;
    protected ref array<Widget>     m_Content = new array<Widget>;
    
    protected Widget                m_Button;
    protected Widget                m_Holder;
    protected TextWidget            m_Text;
    protected ImageWidget           m_ImageExpand;
    protected ImageWidget           m_ImageCollapse;
    
    protected bool                  m_IsExpanded;
    ref ScriptInvoker               m_OnSelectItem  = new ScriptInvoker();
 
    void DropdownPrefab( Widget root, string text = &quot;&quot; )
    {
        m_Parent = root;
        m_Root              = g_Game.GetWorkspace().CreateWidgets( &quot;gui/layouts/new_ui/dropdown_prefab/dropdown_prefab.layout&quot;, root );
        
        m_Scroller          = ScrollWidget.Cast( m_Root.FindAnyWidget( &quot;dropdown_container&quot; ) );
        m_ContentContainer  = m_Root.FindAnyWidget( &quot;dropdown_content&quot; );
        m_Text              = TextWidget.Cast( m_Root.FindAnyWidget( &quot;dropdown_text&quot; ) );
        m_Holder            = m_Root.FindAnyWidget( &quot;holder&quot; );
        SetText( text );
        
        m_Button            = m_Root.FindAnyWidget( &quot;dropdown_selector_button&quot; );
        m_ImageExpand       = ImageWidget.Cast( m_Root.FindAnyWidget( &quot;expand_image&quot; ) );
        m_ImageCollapse     = ImageWidget.Cast( m_Root.FindAnyWidget( &quot;collapse_image&quot; ) );
        
        m_Root.SetHandler( this );
        
        RefreshContent();
