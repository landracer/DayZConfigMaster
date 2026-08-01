// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/containeditems/cargocontainerrow.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CargoContainerRow: LayoutHolder
{
    protected int               m_NumberRow;
    protected int               m_NumberColumnFocused;
    protected int               m_Width;
    protected int               m_MaxColumns;
    
    protected EntityAI          m_Entity;   
    
    protected CargoContainer    m_ParentContainer;
    protected ref SizeToChild   m_Resizer1;
    protected ref SizeToParent  m_Resizer2;
    
    void CargoContainerRow( LayoutHolder parent )
    {
        m_ParentContainer = CargoContainer.Cast( parent );
        #ifdef PLATFORM_CONSOLE
        m_MaxColumns = 5;
        #else
        m_MaxColumns = 10;
        #endif
        
        for ( int i = 0; i < m_MaxColumns; i++ )
        {
            Widget iconWidget = GetMainWidget().FindAnyWidget( &quot;Icon&quot; + i );
            
            iconWidget.SetUserID(i);
            
            WidgetEventHandler.GetInstance().RegisterOnDropReceived( iconWidget,  this, &quot;DropReceived&quot; );
            WidgetEventHandler.GetInstance().RegisterOnDraggingOver( iconWidget,  this, &quot;DraggingOver&quot; );
        }
        
        m_RootWidget.GetScript( m_Resizer1 );
        m_RootWidget.FindAnyWidget( &quot;SpacerTop&quot; ).GetScript( m_Resizer2 );
