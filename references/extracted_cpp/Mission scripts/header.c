// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/containeditems/headers/header.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Header: LayoutHolder
{
    protected int           m_DefaultColor;
    protected int           m_DefaultFontSize;
    
    protected EntityAI      m_Entity;
    
    protected Widget        m_CollapseButton;
    protected Widget        m_UpArrowButton;
    protected Widget        m_DownArrowButton;
    protected TextWidget    m_HeaderText;
    
    void Header( LayoutHolder parent, string function_name )
    {
        m_CollapseButton    = GetMainWidget().FindAnyWidget( &quot;collapse_button&quot; );
        m_UpArrowButton     = GetMainWidget().FindAnyWidget( &quot;opened&quot; );
        m_DownArrowButton   = GetMainWidget().FindAnyWidget( &quot;closed&quot; );
        
        #ifdef PLATFORM_CONSOLE
            m_CollapseButton.Show( false );
        #else
        WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( m_CollapseButton,  m_Parent, function_name );
        WidgetEventHandler.GetInstance().RegisterOnDropReceived( m_CollapseButton,  m_Parent, &quot;OnDropReceivedFromHeader&quot; );
        #endif
        WidgetEventHandler.GetInstance().RegisterOnDropReceived( GetMainWidget(),  m_Parent, &quot;OnDropReceivedFromHeader&quot; );
        WidgetEventHandler.GetInstance().RegisterOnDraggingOver( GetMainWidget(),  this, &quot;DraggingOverHeader&quot; );
        
        m_HeaderText        = TextWidget.Cast( GetMainWidget().FindAnyWidget( &quot;TextWidget0&quot; ) );
        
        m_DefaultColor      = GetRootWidget().GetColor();
