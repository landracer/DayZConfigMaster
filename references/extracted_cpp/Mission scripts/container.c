// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/containers/container.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Container extends LayoutHolder
{
    protected ref array<ref LayoutHolder>   m_Body;
    protected ref array<LayoutHolder>       m_OpenedContainers;
    protected int                           m_ActiveIndex = 0;
    protected bool                          m_LastIndex; //deprecated 
    protected bool                          m_Closed;
    protected Container                     m_FocusedContainer;
    protected float                         m_PrevAlpha;
    const int ITEMS_IN_ROW = 8;
    
    //protected int                         m_RowCount;
    protected int                           m_ColumnCount;
    
    protected int                           m_FocusedColumn = 0;
    protected bool                          m_ForcedHide;
    protected bool                          m_ForcedShow; //used to override displayability condition, but &#39;m_ForcedHide&#39; takes preference
    
    protected SlotsIcon                     m_SlotIcon;
    protected EntityAI                      m_Entity;
    
    const int SORT_ATTACHMENTS_OWN = 1; //direct attachments of the parent item
    const int SORT_CARGO_OWN = 2; //cargo of the parent item
    const int SORT_ATTACHMENTS_NEXT_OFFSET = 2;
    const int SORT_CARGO_NEXT_OFFSET = 3;
 
    void OnDropReceivedFromHeader( Widget w, int x, int y, Widget receiver );
    void DraggingOver( Widget w, int x, int y, Widget receiver );
    void DraggingOverHeader( Widget w, int x, int y, Widget receiver );
    void UpdateSpacer();
    Header GetHeader();
    void SetHeader(Header header);
    void CheckHeaderDragability();
    
    void Container( LayoutHolder parent )
    {
        m_Body = new array<ref LayoutHolder>;
        m_OpenedContainers = new array<LayoutHolder>;
        m_PrevAlpha = m_RootWidget.GetAlpha();
        m_SlotIcon = null;
        m_ForcedHide = false;
        m_ForcedShow = false;
        
        m_ActiveIndex = 0;
        m_IsActive = false;
