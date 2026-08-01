// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/containers/handscontainer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HandsContainer: Container
{
    protected bool                                          m_Hidden;
    protected ref HandsHeader                               m_CollapsibleHeader;
    protected ref HandsPreview                              m_HandsPreview;
    
    protected ref Attachments                               m_Atts;
    protected ref CargoContainer                            m_CargoGrid;
    
    protected ref map<EntityAI, ref CargoContainer>         m_AttachmentCargos;
    protected ref map<EntityAI, AttachmentsWrapper>         m_AttachmentAttachmentsContainers;
    protected ref map<EntityAI, ref Attachments>            m_AttachmentAttachments;
    protected ref array<int>                                m_AttachmentSlotsSorted;
    
    protected int                                           m_StaticAttCount = 0;
    protected int                                           m_StaticCargoCount = 0;
    
    protected ScrollWidget                                  m_ScrollWidget;
 
    void HandsContainer( LayoutHolder parent )
    {
        m_AttachmentCargos                  = new map<EntityAI, ref CargoContainer>;
        m_AttachmentAttachmentsContainers   = new map<EntityAI, AttachmentsWrapper>;
        m_AttachmentAttachments             = new map<EntityAI, ref Attachments>;
        
        m_CollapsibleHeader = new HandsHeader( this, &quot;CollapseButtonOnMouseButtonDown&quot; );
        GetMainWidget().SetFlags( WidgetFlags.IGNOREPOINTER );
        m_MainWidget = m_MainWidget.FindWidget( &quot;body&quot; );
        GetMainWidget().SetFlags( WidgetFlags.IGNOREPOINTER );
        m_Body = new array<ref LayoutHolder>;
        m_HandsPreview = new HandsPreview( this );
        ItemManager.GetInstance().SetHandsPreview( m_HandsPreview );
        m_Body.Insert( m_HandsPreview );
        
        ItemBase hands_item = ItemBase.Cast( g_Game.GetPlayer().GetEntityInHands() );
        if ( hands_item )
            m_HandsPreview.CreateNewIcon( hands_item );
        
        m_ScrollWidget                      = ScrollWidget.Cast( m_RootWidget.GetParent().GetParent() );
        RecomputeOpenedContainers();
