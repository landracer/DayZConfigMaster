// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/inherited/attachmentcategoriesslotscontainer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AttachmentCategoriesSlotsContainer: Container
{
    ref SlotsContainer m_ItemsCont;
    
    void AttachmentCategoriesSlotsContainer( LayoutHolder parent, int index )
    {
        m_ItemsCont = new SlotsContainer(this, null);
        m_Body.Insert( m_ItemsCont );
        m_ItemsCont.GetMainWidget().SetUserID( index );
        
        for ( int k = 0; k < ITEMS_IN_ROW; k++ )
        {
            //WidgetEventHandler.GetInstance().RegisterOnDropReceived( m_ItemsCont.GetSlotIcon( k ).GetMainWidget(),  m_Parent, &quot;OnDropReceivedFromHeader&quot; );
            //WidgetEventHandler.GetInstance().RegisterOnDropReceived(  m_ItemsCont.GetSlotIcon( k ).GetPanelWidget(),  m_Parent, &quot;OnDropReceivedFromHeader&quot; );
                  
            WidgetEventHandler.GetInstance().RegisterOnDraggingOver( m_ItemsCont.GetSlotIcon( k ).GetMainWidget(),  m_Parent, &quot;DraggingOverHeader&quot; );
            WidgetEventHandler.GetInstance().RegisterOnDraggingOver( m_ItemsCont.GetSlotIcon( k ).GetPanelWidget(),  m_Parent, &quot;DraggingOverHeader&quot; );
            
            WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( m_ItemsCont.GetSlotIcon( k ).GetMainWidget(),  m_Parent, &quot;MouseClick&quot; );
            WidgetEventHandler.GetInstance().RegisterOnMouseButtonDown( m_ItemsCont.GetSlotIcon( k ).GetPanelWidget(),  m_Parent, &quot;MouseClick&quot; );
            
            WidgetEventHandler.GetInstance().RegisterOnMouseEnter( m_ItemsCont.GetSlotIcon( k ).GetMainWidget(), m_ItemsCont.GetSlotIcon( k ), &quot;MouseEnterGhostSlot&quot; );
            WidgetEventHandler.GetInstance().RegisterOnMouseLeave( m_ItemsCont.GetSlotIcon( k ).GetMainWidget(), m_ItemsCont.GetSlotIcon( k ), &quot;MouseLeaveGhostSlot&quot; );
            
            m_ItemsCont.GetSlotIcon( k ).GetMainWidget().SetUserID( k );
            m_ItemsCont.GetSlotIcon( k ).GetPanelWidget().SetUserID( k );
        }
