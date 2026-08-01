// Source: F:/Games/Dayz/scripts/5_mission/gui/objectfollower.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ObjectFollower extends ScriptedWidgetEventHandler
{
    reference int                       m_PivotYOffset;
 
    protected Widget                    m_Root;
    protected Object                    m_TargetObject;
    protected bool                      m_Visible;
 
    void ObjectFollower()
    {
        m_Visible = false;
        //g_Game.GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
