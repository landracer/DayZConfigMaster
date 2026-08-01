// Source: F:/Games/Dayz/scripts/5_mission/gui/cameratools/ctactor.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CTActor extends CTObjectFollower
{
    protected int               m_Index;
 
    protected Widget            m_Root;
    protected TextWidget        m_IndexWidget;
    
    protected string            m_HandsItem;
    protected ref array<string> m_Items;
    
    protected EntityAI          m_HandsItemObj;
    
    void CTActor( int index, vector pos, vector orient, string type, array<string> items, string hands_item, CameraToolsMenu parent )
    {
        m_FollowerRoot      = g_Game.GetWorkspace().CreateWidgets( &quot;gui/layouts/camera_tools/event_tracker.layout&quot;, null );
        m_FollowerButton    = m_FollowerRoot.FindAnyWidget( &quot;IconPanel&quot; );
        m_IndexWidget       = TextWidget.Cast( m_FollowerRoot.FindAnyWidget( &quot;Text&quot; ) );
        m_Index             = index;
        m_Position          = pos;
        m_Orientation       = orient;
        m_Menu              = parent;
        
        m_IndexWidget.SetText( m_Index.ToString() );
        m_FollowerRoot.SetHandler( this );
        CreateFollowedObject( type );
        SetHandsItem( hands_item );
        SetItems( items );
