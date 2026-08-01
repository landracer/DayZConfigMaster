// Source: F:/Games/Dayz/scripts/5_mission/gui/cameratools/ctkeyframe.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CTKeyframe extends ScriptedWidgetEventHandler
{
    protected int               m_Index;
    protected float             m_InterpTime;
    protected float             m_TotalTimeBefore;
    
    protected vector            m_Position;
    protected vector            m_Orientation;
    protected CameraToolsMenu   m_Menu;
    
    protected Widget            m_Root;
    protected TextWidget        m_IndexWidget;
    protected EditBoxWidget     m_InterpTimeWidget;
    protected EditBoxWidget     m_FOVWidget;
    protected EditBoxWidget     m_DOFWidget;
    protected EditBoxWidget     m_PinWidget;
    protected TextWidget        m_TotalTimeWidget;
    
    void CTKeyframe( int index, vector pos, vector orient, float int_value, float fov, float dof, int pin, float time_before, Widget root, CameraToolsMenu parent )
    {
        m_Menu              = parent;
        
        m_Root              = g_Game.GetWorkspace().CreateWidgets( &quot;gui/layouts/camera_tools/keyframe_entry.layout&quot;, root );
        
        m_IndexWidget       = TextWidget.Cast( m_Root.FindAnyWidget( &quot;keyframe_id&quot; ) );
        m_InterpTimeWidget  = EditBoxWidget.Cast( m_Root.FindAnyWidget( &quot;keyframe_time_edit&quot; ) );
        m_FOVWidget         = EditBoxWidget.Cast( m_Root.FindAnyWidget( &quot;keyframe_fov_edit&quot; ) );
        m_DOFWidget         = EditBoxWidget.Cast( m_Root.FindAnyWidget( &quot;keyframe_dof_edit&quot; ) );
        m_PinWidget         = EditBoxWidget.Cast( m_Root.FindAnyWidget( &quot;keyframe_pin_edit&quot; ) );
        m_TotalTimeWidget   = TextWidget.Cast( m_Root.FindAnyWidget( &quot;keyframe_time&quot; ) );
        
        m_Index             = index;
        m_TotalTimeBefore   = time_before;
        m_Position          = pos;
        m_Orientation       = orient;
        
        SetInterpTime( int_value );
        SetFOV( fov );
        SetDOF( dof );
        SetPin( pin );
        m_IndexWidget.SetText( m_Index.ToString() );
        m_Root.SetHandler( this );
