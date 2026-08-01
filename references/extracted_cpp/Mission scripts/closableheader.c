// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/containeditems/headers/closableheader.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ClosableHeader: Header
{
    protected float         m_SquareSize;
    protected int           m_DefaultSort;
    
    protected Widget        m_MovePanel;
    protected Widget        m_PanelWidget;
    protected Widget        m_DragRender;
    protected ButtonWidget  m_MoveUp;
    protected ButtonWidget  m_MoveDown;
    
    protected bool          m_IsInLocalEquipment;
    
    void ClosableHeader( LayoutHolder parent, string function_name )
    {
        m_MovePanel = GetMainWidget().FindAnyWidget( &quot;MovePanel&quot; );
        m_PanelWidget   = GetMainWidget().FindAnyWidget( &quot;PanelWidget&quot; );
        m_DragRender    = GetMainWidget().FindAnyWidget( &quot;Drag_Render&quot; );
        m_MoveUp    = ButtonWidget.Cast( GetMainWidget().FindAnyWidget( &quot;MoveUp&quot; ) );
        m_MoveDown  = ButtonWidget.Cast( GetMainWidget().FindAnyWidget( &quot;MoveDown&quot; ) );
        
        WidgetEventHandler.GetInstance().RegisterOnClick( m_MoveUp,  m_Parent, &quot;MoveContainerUp&quot; );
        WidgetEventHandler.GetInstance().RegisterOnClick( m_MoveDown,  m_Parent, &quot;MoveContainerDown&quot; );
        WidgetEventHandler.GetInstance().RegisterOnMouseLeave( m_MoveUp,  this, &quot;MouseLeave&quot; );
        WidgetEventHandler.GetInstance().RegisterOnMouseLeave( m_MoveDown,  this, &quot;MouseLeave&quot; );
        
        WidgetEventHandler.GetInstance().RegisterOnDrag( GetMainWidget() ,  this, &quot;OnDragHeader&quot; );
        WidgetEventHandler.GetInstance().RegisterOnDrop( GetMainWidget() ,  this, &quot;OnDropHeader&quot; );
        
        WidgetEventHandler.GetInstance().RegisterOnMouseEnter( GetMainWidget(),  this, &quot;MouseEnter&quot; );
        WidgetEventHandler.GetInstance().RegisterOnMouseLeave( GetMainWidget(),  this, &quot;MouseLeave&quot; );
        
        float temp;
        GetMainWidget().GetScreenSize( temp, m_SquareSize );
        
        m_DefaultColor          = m_PanelWidget.GetColor();
        m_DefaultFontSize       = 20;
        
        if( GetParent() && GetParent().GetParent() && GetParent().GetParent().GetParent() )
            m_IsInLocalEquipment    = GetParent().GetParent().GetParent().IsInherited( RightArea );
