// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/containeditems/slotsicon.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SlotsIcon: LayoutHolder
{
    protected static int            m_NormalWidth;
    protected static int            m_NormalHeight;
    
    protected bool                  m_IsWeapon          = false;
    protected bool                  m_IsMagazine        = false;
    protected bool                  m_HasTemperature    = false;
    protected bool                  m_HasQuantity       = false;
    protected bool                  m_HasItemSize       = false;
    protected float                 m_CurrQuantity      = -1;
    
    protected EntityAI              m_Obj;
    protected ItemBase              m_Item;
    protected EntityAI              m_SlotParent;
    protected Container             m_Container;
    protected int                   m_SlotID;
    protected bool                  m_IsDragged         = false;
    
    protected Widget                m_PanelWidget;
    
    protected Widget                m_CursorWidget;
    protected Widget                m_ColWidget;
    protected Widget                m_MountedWidget;
    protected Widget                m_OutOfReachWidget;
    protected Widget                m_ReservedWidget;
    
    protected ItemPreviewWidget     m_ItemPreview;
    protected ImageWidget           m_GhostSlot;
    
    protected Widget                m_ColorWidget;
    protected Widget                m_SelectedPanel;
    protected Widget                m_EmptySelectedPanel;
    protected Widget                m_MicromanagedPanel;
    
    protected Widget                m_QuantityPanel;
    protected TextWidget            m_QuantityItem;
    protected ProgressBarWidget     m_QuantityProgress;
    protected Widget                m_QuantityStack;
    
    protected string                m_SlotDisplayName;
    protected string                m_SlotDesc;
    
    protected Widget                m_ItemSizePanel;
    protected TextWidget            m_ItemSizeWidget;
    
    protected Widget                m_RadialIconPanel;
    protected Widget                m_RadialIconClosed;
    protected Widget                m_RadialIcon;
 
    protected bool                  m_Reserved;
 
    protected ImageWidget               m_AmmoIcon;
    protected ref array<ImageWidget>    m_AmmoIcons;
 
    void SlotsIcon( LayoutHolder parent, Widget root, int index, EntityAI slot_parent )
    {
        m_MainWidget            = root;
        
        m_PanelWidget           = m_MainWidget.FindAnyWidget( &quot;PanelWidget&quot; + index );
        
        m_CursorWidget          = m_MainWidget.FindAnyWidget( &quot;Cursor&quot; + index );
        m_ColWidget             = m_MainWidget.FindAnyWidget( &quot;Col&quot; + index );
        m_MountedWidget         = m_MainWidget.FindAnyWidget( &quot;Mounted&quot; + index );
        m_OutOfReachWidget      = m_MainWidget.FindAnyWidget( &quot;OutOfReach&quot; + index );
 
        m_ItemPreview           = ItemPreviewWidget.Cast( m_MainWidget.FindAnyWidget( &quot;Render&quot; + index ) );
        m_ItemPreview.SetForceFlipEnable(true);
        m_ItemPreview.SetForceFlip(false);
        
        m_GhostSlot             = ImageWidget.Cast( m_MainWidget.FindAnyWidget( &quot;GhostSlot&quot; + index ) );
        
        m_ColorWidget           = m_MainWidget.FindAnyWidget( &quot;Color&quot; + index );
        m_SelectedPanel         = m_MainWidget.FindAnyWidget( &quot;Selected&quot; + index );
        m_EmptySelectedPanel    = m_MainWidget.FindAnyWidget( &quot;EmptySelected&quot; + index );
        m_MicromanagedPanel    = m_MainWidget.FindAnyWidget( &quot;Micromanaged&quot; + index );
        
        m_QuantityPanel         = m_MainWidget.FindAnyWidget( &quot;QuantityPanel&quot; + index );
        m_QuantityItem          = TextWidget.Cast( m_MainWidget.FindAnyWidget( &quot;Quantity&quot; + index ) );
        m_QuantityProgress      = ProgressBarWidget.Cast( m_MainWidget.FindAnyWidget( &quot;QuantityBar&quot; + index ) );
        m_QuantityStack         = m_MainWidget.FindAnyWidget( &quot;QuantityStackPanel&quot; + index );
        
        m_ItemSizePanel         = m_MainWidget.FindAnyWidget( &quot;ItemSizePanel&quot; + index );
        m_ItemSizeWidget        = TextWidget.Cast( m_MainWidget.FindAnyWidget( &quot;ItemSize&quot; + index ) );
        
        m_AmmoIcon              = ImageWidget.Cast(m_MainWidget.FindAnyWidget( &quot;AmmoIcon&quot; + index ));
        
        m_RadialIconPanel       = m_MainWidget.FindAnyWidget( &quot;RadialIconPanel&quot; + index );
        m_RadialIconClosed      = m_MainWidget.FindAnyWidget( &quot;RadialIconClosed&quot; + index );
        m_RadialIcon            = m_MainWidget.FindAnyWidget( &quot;RadialIcon&quot; + index );
        
        m_ReservedWidget = Widget.Cast( g_Game.GetWorkspace().CreateWidgets( &quot;gui/layouts/inventory_new/reserved_icon.layout&quot;, m_MainWidget ) );
        m_ReservedWidget.Show(false);
        
        WidgetEventHandler.GetInstance().RegisterOnMouseEnter( m_PanelWidget,  this, &quot;MouseEnter&quot; );
        WidgetEventHandler.GetInstance().RegisterOnMouseLeave( m_PanelWidget,  this, &quot;MouseLeave&quot; );
        
        WidgetEventHandler.GetInstance().RegisterOnMouseEnter( m_GhostSlot,  this, &quot;MouseEnterGhostSlot&quot; );
        WidgetEventHandler.GetInstance().RegisterOnMouseLeave( m_GhostSlot,  this, &quot;MouseLeaveGhostSlot&quot; );
        
        WidgetEventHandler.GetInstance().RegisterOnMouseEnter( m_RadialIconPanel,  this, &quot;MouseEnterCategory&quot; );
        WidgetEventHandler.GetInstance().RegisterOnMouseLeave( m_RadialIconPanel,  this, &quot;MouseLeaveCategory&quot; );
        
        WidgetEventHandler.GetInstance().RegisterOnDrag( m_PanelWidget, this, &quot;OnIconDrag&quot; );
        WidgetEventHandler.GetInstance().RegisterOnDrop( m_PanelWidget, this, &quot;OnIconDrop&quot; );
        
        m_Reserved              = false;
        m_SlotID                = -1;
        m_Item                  = null;
        m_Obj                   = null;
        m_Container             = null;
        
        m_SlotParent            = slot_parent;
        
        m_PanelWidget.SetUserData(this);
        m_ItemPreview.SetUserData(this);
        m_GhostSlot.SetUserData(this);
        m_MainWidget.SetUserData(this);
        
        float w,h;
        root.GetSize(w,h);
        
        m_NormalWidth = w;
        m_NormalHeight = h;
        
        SetActive( false );
