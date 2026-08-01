// Source: F:/Games/Dayz/scripts/5_mission/gui/cameratools/ctevent.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CTEvent extends ScriptedWidgetEventHandler
{
    protected int               m_Index;
    protected int               m_ActorIndex;
    protected int               m_EventType;
    protected float             m_EventTime;
    
    protected CameraToolsMenu   m_Menu;
    
    protected Widget            m_Root;
    protected TextWidget        m_IndexWidget;
    protected EditBoxWidget     m_EventActorWidget;
    protected EditBoxWidget     m_EventTimeWidget;
    protected EditBoxWidget     m_EventTypeWidget;
    protected CheckBoxWidget    m_EventWalkWidget;
    
    protected HumanCommandActionCallback    m_Callback;
    
    void CTEvent( int index, int actor, Widget root, bool walk, CameraToolsMenu parent )
    {
        m_Menu              = parent;
        m_Index             = index;
        m_ActorIndex        = actor;
        
        m_Root              = g_Game.GetWorkspace().CreateWidgets( &quot;gui/layouts/camera_tools/event_entry.layout&quot;, root );
        m_IndexWidget       = TextWidget.Cast( m_Root.FindAnyWidget( &quot;event_id&quot; ) );
        m_EventActorWidget  = EditBoxWidget.Cast( m_Root.FindAnyWidget( &quot;event_actor_edit&quot; ) );
        m_EventTimeWidget   = EditBoxWidget.Cast( m_Root.FindAnyWidget( &quot;event_time_edit&quot; ) );
        m_EventTypeWidget   = EditBoxWidget.Cast( m_Root.FindAnyWidget( &quot;event_type_edit&quot; ) );
        m_EventWalkWidget   = CheckBoxWidget.Cast( m_Root.FindAnyWidget( &quot;auto_walk&quot; ) );
        
        m_EventWalkWidget.SetChecked( walk );
        m_IndexWidget.SetText( m_Index.ToString() );
        m_EventActorWidget.SetText( m_ActorIndex.ToString() );
        m_Root.SetHandler( this );
