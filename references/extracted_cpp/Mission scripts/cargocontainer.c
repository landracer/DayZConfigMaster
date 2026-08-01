// Source: F:/Games/Dayz/scripts/5_mission/gui/inventorynew/containeditems/cargocontainer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//cargo grid wrapper
class CargoContainer extends Container
{
    protected const int ROWS_NUMBER_XBOX = 5;
    
    protected CargoBase                                             m_Cargo;
    protected int                                                   m_CargoIndex = -1;
    
    protected int                                                   m_FocusedItemPosition = -1;
    protected ref array<ref CargoContainerRow>                      m_Rows;
    protected ref array<ref Icon>                                   m_Icons;
    protected ref map<EntityAI, ref Param3<ref Icon, int, int>>     m_ShowedItemPositions;
    protected ref map<EntityAI, ref Param3<ref Icon, int, int>>     m_ShowedLockPositions;
    
    protected float                                                 m_IconSize;
    protected float                                                 m_SpaceSize;
    
    protected bool                                                  m_IsAttachment;
    protected TextWidget                                            m_FalseHeaderTextWidget;
    protected TextWidget                                            m_AlternateFalseHeaderTextWidget; //to be set and updated along with the main one
    protected Widget                                                m_CargoHeader;
    protected Widget                                                m_CargoContainer;
    protected Widget                                                m_ItemsContainer;
#ifndef PLATFORM_CONSOLE
    protected ref SizeToChild                                       m_Resizer1;
#endif
    protected ref SizeToChild                                       m_Resizer2;
    protected ref Timer                                             m_ResizeTimer;
    
    protected int                                                   m_AttachmentSlotID = -1;
    
    void CargoContainer( LayoutHolder parent, bool is_attachment = false )
    {
        m_IsAttachment          = is_attachment;
        m_Rows                  = new array<ref CargoContainerRow>;
        m_Icons                 = new array<ref Icon>;
        m_ShowedItemPositions   = new map<EntityAI, ref Param3<ref Icon, int, int>>;
        m_ShowedLockPositions   = new map<EntityAI, ref Param3<ref Icon, int, int>>;
        
        m_CargoContainer        = m_RootWidget.FindAnyWidget( &quot;grid_background&quot; );
        m_ItemsContainer        = m_RootWidget.FindAnyWidget( &quot;grid_overlay&quot; );
        m_CargoHeader           = m_RootWidget.FindAnyWidget( &quot;grid_container_header_spacer&quot; );
#ifndef PLATFORM_CONSOLE
        m_RootWidget.GetScript( m_Resizer1 );
#endif      
        m_RootWidget.FindAnyWidget( &quot;grid_container&quot; ).GetScript( m_Resizer2 );
        m_CargoHeader.Show( is_attachment );
        m_FalseHeaderTextWidget = TextWidget.Cast(m_CargoHeader.FindAnyWidget( &quot;TextWidget0&quot; ));
        
        m_MainWidget            = m_CargoContainer;
        m_FocusedItemPosition   = -1;
