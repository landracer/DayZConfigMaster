// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/containeditems/icon.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Icon: LayoutHolder
{
    protected int                   m_SizeX;
    protected int                   m_SizeY;
    protected int                   m_PosX;
    protected int                   m_PosY;
    
    protected EntityAI              m_Lock;
    protected bool                  m_IsWeapon          = false;
    protected bool                  m_IsMagazine        = false;
    protected bool                  m_HasTemperature    = false;
    protected bool                  m_HasQuantity       = false;
    protected float                 m_CurrQuantity      = -1;
    
    protected EntityAI              m_Obj;
    protected ItemBase              m_Item;
    protected bool                  m_HandsIcon;
    protected int                   m_CargoPos;
    protected bool                  m_IsDragged;
    protected bool                  m_PreviousFlipOrientation;
    
    const int NUMBER_OF_TIMERS = 2;
    
    protected ItemPreviewWidget     m_ItemPreview;
    
    protected Widget                m_ColorWidget;
    protected Widget                m_SelectedPanel;
    protected Widget                m_MicromanagedPanel;
    protected Widget                m_CursorWidget;
    
    protected Widget                m_QuantityPanel;
    protected TextWidget            m_QuantityItem;
    protected ProgressBarWidget     m_QuantityProgress;
    protected Widget                m_QuantityStack;
    
    protected Widget                m_ItemSizePanel;
    protected TextWidget            m_ItemSizeWidget;
    
    protected ref array<ImageWidget>    m_AmmoIcons;
    protected ImageWidget               m_AmmoTypeIcon;
 
    void Icon( LayoutHolder parent, bool hands_icon = false )
    {
        m_HandsIcon = hands_icon;
        ItemManager.GetInstance().SetSelectedItemEx(null, null, null);
        
        m_ItemPreview       = ItemPreviewWidget.Cast( GetMainWidget().FindAnyWidget( &quot;Render&quot; ));
        
        m_ColorWidget       = GetMainWidget().FindAnyWidget( &quot;Color&quot; );
        m_SelectedPanel     = GetMainWidget().FindAnyWidget( &quot;Selected&quot; );
        m_MicromanagedPanel = GetMainWidget().FindAnyWidget( &quot;Micromanaged&quot; );
        m_CursorWidget      = GetMainWidget().FindAnyWidget( &quot;Cursor&quot; );
        
        m_QuantityPanel     = GetMainWidget().FindAnyWidget( &quot;QuantityPanel&quot; );
        m_QuantityItem      = TextWidget.Cast( GetMainWidget().FindAnyWidget( &quot;Quantity&quot; ) );
        m_QuantityProgress  = ProgressBarWidget.Cast( GetMainWidget().FindAnyWidget( &quot;QuantityBar&quot; ) );
        m_QuantityStack     = GetMainWidget().FindAnyWidget( &quot;QuantityStackPanel&quot; );
        
        m_ItemSizePanel     = GetMainWidget().FindAnyWidget( &quot;ItemSizePanel&quot; );
        m_ItemSizeWidget    = TextWidget.Cast( GetMainWidget().FindAnyWidget( &quot;ItemSize&quot; ) );
        
        m_AmmoTypeIcon      = ImageWidget.Cast( GetMainWidget().FindAnyWidget( &quot;AmmoTypeIcon&quot; ) );
        SetActive( false );
