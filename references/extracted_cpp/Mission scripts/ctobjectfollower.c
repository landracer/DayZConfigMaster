// Source: F:/Games/Dayz/scripts/5_mission/gui/cameratools/ctobjectfollower.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CTObjectFollower extends ScriptedWidgetEventHandler
{
    protected Widget            m_FollowerRoot;
    protected Widget            m_FollowerButton;
    
    protected vector            m_Position;
    protected vector            m_Orientation;
    protected EntityAI          m_FollowedObject;
    
    protected float             m_MaxFade;
    protected float             m_MinFade;
    
    protected CameraToolsMenu   m_Menu;
    
    void ~CTObjectFollower()
    {
        DestroyFollowedObject();
        delete m_FollowerRoot;
