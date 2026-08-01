// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/inherited/containerwithcargo.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ContainerWithCargo extends ClosableContainer
{
    protected ref CargoContainer    m_CargoGrid;
    protected int m_CargoIndex = -1;
 
    void ContainerWithCargo( LayoutHolder parent, int sort = -1 )
    {
        m_LockCargo = false;
 
        m_CargoGrid = new CargoContainer( this );
        Insert( m_CargoGrid );
        
        m_CargoGrid.GetRootWidget().SetSort( 1 );
        
        WidgetEventHandler.GetInstance().RegisterOnDraggingOver( m_MainWidget,  this, &quot;DraggingOverGrid&quot; );
        RecomputeOpenedContainers();
