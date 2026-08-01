// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/inherited/vicinitycontainer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class VicinityContainer: CollapsibleContainer
{
    ref VicinitySlotsContainer          m_VicinityIconsContainer;
    ref map<EntityAI, ref Container>    m_ShowedItems               = new map<EntityAI, ref Container>;
    ref map<int, ref Container>         m_ShowedItemsIDs            = new map<int, ref Container>;
    ref array<EntityAI>                 m_ShowedItemIcons           = new array<EntityAI>;
    ref map<CargoBase, ref Container>   m_ShowedCargos              = new map<CargoBase, ref Container>;
    protected bool                      m_IsProcessing = false; // Prevents refreshing every time a child is added while it is still processing
    
    #ifndef PLATFORM_CONSOLE
    protected ScrollWidget              m_CargoScrollWidget;
    #endif
        
    const float DISTANCE_TO_ENTITIES    = 1.0;
    const float DISTANCE_TO_THE_REST    = 0.5;
    
    const int ITEMS_PER_COLUMN_MAX = 8;
    const float VICINITY_CARGO_SCROLLER_HEIGHT_MIN = 0.77;
    const float VICINITY_CARGO_SCROLLER_HEIGHT_MID = 0.78;
    const float VICINITY_CARGO_SCROLLER_HEIGHT_MAX = 0.87;
    
    void VicinityContainer( LayoutHolder parent, int sort = -1 )
    {
        m_VicinityIconsContainer = new VicinitySlotsContainer( this );
        m_Body.Insert( m_VicinityIconsContainer );
        m_VicinityIconsContainer.GetRootWidget().SetColor(166 << 24 | 120 << 16 | 120 << 8 | 120);
                
        #ifndef PLATFORM_CONSOLE
        LeftArea leftArea = LeftArea.Cast(GetParent());
        if (leftArea)
        {
            m_CargoScrollWidget = leftArea.GetScrollWidget();
            if (m_CargoScrollWidget)
            {
                bool hasScrollerExactSize = m_CargoScrollWidget.GetFlags() & WidgetFlags.EXACTSIZE;
                if (!hasScrollerExactSize)
                {
                    m_CargoScrollWidget.SetFlags(WidgetFlags.IGNOREPOINTER);
                }
            }
            
            leftArea.GetSlotsHeader().AddChild(GetHeader().GetRootWidget());
            leftArea.GetSlotsArea().AddChild(m_VicinityIconsContainer.GetRootWidget());
        }
        #endif
        
        m_MainWidget = m_RootWidget.FindAnyWidget( &quot;body&quot; );
        WidgetEventHandler.GetInstance().RegisterOnChildAdd( m_MainWidget, this, &quot;OnChildAdd&quot; );
        WidgetEventHandler.GetInstance().RegisterOnChildRemove( m_MainWidget, this, &quot;OnChildRemove&quot; );
        
        RecomputeOpenedContainers();
        m_CollapsibleHeader.SetName(&quot;#container_vicinity&quot;);
        LoadDefaultState();
