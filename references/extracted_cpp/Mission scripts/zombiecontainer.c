// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/inherited/zombiecontainer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ZombieContainer: CollapsibleContainer
{
    protected ref AttachmentsGroupContainer     m_Container;
    protected ref ContainerWithCargo            m_CargoGrid;
    protected ref map<int, SlotsIcon>           m_InventorySlots;
    protected ref map<EntityAI, ref Container>  m_ShowedItems = new map<EntityAI, ref Container>;
    protected EntityAI                          m_ZombieEntity;
    
    void ZombieContainer( LayoutHolder parent, int sort = -1 )
    {
        m_InventorySlots = new map<int, SlotsIcon>;
        m_Container = new AttachmentsGroupContainer(this);
        
        m_Container.SetHeader(GetHeader());
        SetHeaderName();
        SetHeader(null);
        m_Body.Insert( m_Container );
        
        m_MainWidget = m_RootWidget.FindAnyWidget( &quot;body&quot; );
        
        WidgetEventHandler.GetInstance().RegisterOnChildAdd( m_MainWidget, this, &quot;OnChildAdd&quot; );
        WidgetEventHandler.GetInstance().RegisterOnChildRemove( m_MainWidget, this, &quot;OnChildRemove&quot; );
        
        
        RecomputeOpenedContainers();
