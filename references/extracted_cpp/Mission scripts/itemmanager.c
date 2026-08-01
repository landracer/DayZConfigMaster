// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/itemmanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ItemManager
{
    private ref static ItemManager      m_Instance;
    protected bool                      m_IsDragging;
    protected EntityAI                  m_HoveredItem;
    protected bool                      m_SlotInfoShown;
    protected EntityAI                  m_DraggedItem;
    protected Icon                      m_DraggedIcon;
    protected ref Widget                m_TooltipWidget;
    protected ref Widget                m_TooltipSlotWidget;
    protected ref Widget                m_TooltipCategoryWidget;
    protected ItemPreviewWidget         m_ItemPreviewWidget;
    protected Widget                    m_RootWidget;
    protected ref map<string, bool>     m_DefautOpenStates;
    protected ref map<string, bool>     m_DefautHeaderOpenStates;
    protected int                       m_HandsDefaultOpenState;
    protected ref Timer                 m_ToolTipTimer;
    protected ref Timer                 m_TooltipSlotTimer;
 
    protected EntityAI                  m_SelectedItem;
    protected Container                 m_SelectedContainer;
    protected Widget                    m_SelectedWidget;
    protected SlotsIcon                 m_SelectedIcon;
    protected LayoutHolder              m_SelectedBaseIcon;
    
    protected HandsPreview              m_HandsPreview;
    
    protected bool                      m_ItemMicromanagmentMode;
    
    protected Widget                    m_LeftDropzone;
    protected Widget                    m_CenterDropzone;
    protected Widget                    m_RightDropzone;
    
    protected int                       m_TooltipPosX;
    protected int                       m_TooltipPosY;
    protected Widget                    m_TooltipSourceWidget; //stored here for tooltip position updates
    protected ScrollWidget              m_LeftSlotsScroller;
    
    #ifndef PLATFORM_CONSOLE
    protected const float TOOLTIP_DELAY = 0.25; // in seconds
    #else
    protected const float TOOLTIP_DELAY = 1.5; // in seconds
    #endif
    
    void ItemManager( Widget root )
    {
        m_Instance                  = this;
        m_RootWidget                = root;
        m_DefautOpenStates          = new map<string, bool>;
        m_DefautHeaderOpenStates    = new map<string, bool>;
        m_SlotInfoShown             = false;
        
        #ifdef PLATFORM_CONSOLE
            m_TooltipWidget         = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/inventory_new/day_z_inventory_new_tooltip_xbox.layout&quot;, root );
            m_TooltipSlotWidget     = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/inventory_new/day_z_inventory_new_tooltip_slot_xbox.layout&quot;, root );
        #else
            m_TooltipWidget         = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/inventory_new/day_z_inventory_new_tooltip.layout&quot;, root );
            m_TooltipSlotWidget     = g_Game.GetWorkspace().CreateWidgets(&quot;gui/layouts/inventory_new/day_z_inventory_new_tooltip_slot.layout&quot;, root );
        #endif
        m_TooltipWidget.Show( false );
        m_TooltipSlotWidget.Show( false );
