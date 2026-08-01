// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/inherited/vicinityslotscontainer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class VicinitySlotsContainer: Container
{
    protected ref AttachmentsGroupContainer     m_Container;
    protected int                               m_ItemsCount;
    protected int                               m_SlotsCount;
    protected ref array<EntityAI>               m_ShowedItems;
    
    void VicinitySlotsContainer( LayoutHolder parent )
    {
        m_Container = new AttachmentsGroupContainer(this);
        ref SlotsContainer con = new SlotsContainer( m_Container, null );
        m_Container.Insert( con );
        m_Body.Insert( m_Container );
        for( int j = 0; j < ITEMS_IN_ROW; j++ )
        {
            SlotsIcon icon = con.GetSlotIcon( j );
            WidgetEventHandler.GetInstance().RegisterOnDropReceived( icon.GetPanelWidget(),  m_Parent, &quot;OnDropReceivedFromIcon&quot; );
            WidgetEventHandler.GetInstance().RegisterOnDropReceived( icon.GetGhostSlot(),  m_Parent, &quot;OnDropReceivedFromHeader&quot; );
            WidgetEventHandler.GetInstance().RegisterOnDropReceived( icon.GetMainWidget(),  m_Parent, &quot;OnDropReceivedFromHeader&quot; );
            
            WidgetEventHandler.GetInstance().RegisterOnDraggingOver( icon.GetPanelWidget(),  m_Parent, &quot;DraggingOverIcon&quot; );
            WidgetEventHandler.GetInstance().RegisterOnDraggingOver( icon.GetGhostSlot(),  m_Parent, &quot;DraggingOverHeader&quot; );
            WidgetEventHandler.GetInstance().RegisterOnDraggingOver( icon.GetMainWidget(),  m_Parent, &quot;DraggingOverHeader&quot; );
            
            WidgetEventHandler.GetInstance().RegisterOnDoubleClick( icon.GetPanelWidget(),  this, &quot;DoubleClick&quot; );
            WidgetEventHandler.GetInstance().RegisterOnMouseButtonUp( icon.GetPanelWidget(),  this, &quot;MouseClick&quot; );
            WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( icon.GetPanelWidget(),  this, &quot;MouseButtonDown&quot; );
        }
        
        con.SetColumnCount(0);
        con.SetForceShow(true);
        
        WidgetEventHandler.GetInstance().RegisterOnDropReceived( m_Container.GetMainWidget(),  m_Parent, &quot;OnDropReceivedFromHeader&quot; );
        WidgetEventHandler.GetInstance().RegisterOnDraggingOver( m_Container.GetMainWidget(),  m_Parent, &quot;DraggingOverHeader&quot; );
        
        m_ShowedItems = new array<EntityAI>;
