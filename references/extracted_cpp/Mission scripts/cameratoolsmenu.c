// Source: F:/Games/Dayz/scripts/5_mission/gui/cameratools/cameratoolsmenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CameraToolsMenu extends UIScriptedMenu
{
    protected static CameraToolsMenu                                            m_Instance;
    vector                                                                      m_FreeflightCamTransform[4];
    protected vector                                                            m_NextCamPosition;
    
    protected float                                                             m_Time;
    
    protected Camera                                                            m_Camera1;
    protected Camera                                                            m_Camera2;
    
    protected ref array<ref Param6<vector, vector, float, float, float, int>>   m_CameraLines;
    protected bool                                                              m_IsPlaying;
    protected int                                                               m_NextCameraIndex = 1;
    
    protected float                                                             m_FollowingTimeFinished;
    protected ref Param2<int, int>                                              m_FollowingBone;
    
    protected CTKeyframe                                                        m_SelectedKeyframe;
    protected CTEvent                                                           m_SelectedEvent;
    protected CTActor                                                           m_SelectedActor;
    
    protected ref array<ref CTActor>                                            m_Actors;
    protected ref array<ref CTKeyframe>                                         m_Cameras;
    protected ref array<ref CTEvent>                                            m_Events;
    
    protected Widget                                                            m_KeyframeContent;
    protected Widget                                                            m_EventContent;
    
    protected XComboBoxWidget                                                   m_InterpTypeSpeedCombo;
    protected XComboBoxWidget                                                   m_InterpTypeCombo;
    protected EditBoxWidget                                                     m_BlurEditWidget;
    protected EditBoxWidget                                                     m_ActorTypeWidget;
    
    protected EditBoxWidget                                                     m_ActorIdWidget;
    protected EditBoxWidget                                                     m_ActorItemTypeWidget;
    
    protected ButtonWidget                                                      m_AddItem;
    protected ButtonWidget                                                      m_AddHandsItem;
    protected ButtonWidget                                                      m_AddActor;
    protected ButtonWidget                                                      m_Play;
    protected ButtonWidget                                                      m_Stop;
    protected ButtonWidget                                                      m_Save;
    protected ButtonWidget                                                      m_Load;
    protected ButtonWidget                                                      m_Reset;
    
    protected ButtonWidget                                                      m_AddKeyframe;
    protected ButtonWidget                                                      m_SaveKeyframe;
    protected ButtonWidget                                                      m_DeleteKeyframe;
    protected ButtonWidget                                                      m_ViewKeyframe;
    
    protected ButtonWidget                                                      m_AddEvent;
    protected ButtonWidget                                                      m_SaveEvent;
    protected ButtonWidget                                                      m_DeleteEvent;
    protected ButtonWidget                                                      m_ViewEvent;    
    
    protected ButtonWidget                                                      m_CopyButton;
    protected ButtonWidget                                                      m_ApplyButton;
    protected EditBoxWidget                                                     m_CameraEditbox;
    protected TextWidget                                                        m_CameraValues;
    
    protected string m_CameraToolsDataPath;
    
    void CameraToolsMenu()
    {
        m_CameraToolsDataPath = &quot;$profile:/CameraTools&quot;;
        m_Cameras   = new array<ref CTKeyframe>();
        m_Events    = new array<ref CTEvent>();
        m_Actors    = new array<ref CTActor>();
        
        m_Camera1   = Camera.Cast(g_Game.CreateObject(&quot;staticcamera&quot;, g_Game.GetPlayer().GetPosition(), true));
        m_Camera2   = Camera.Cast(g_Game.CreateObject(&quot;staticcamera&quot;, g_Game.GetPlayer().GetPosition(), true));
        
        m_Instance  = this;
