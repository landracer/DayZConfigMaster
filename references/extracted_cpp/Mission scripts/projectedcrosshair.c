// Source: F:/Games/Dayz/scripts/5_mission/gui/projectedcrosshair.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ProjectedCrosshair extends ScriptedWidgetEventHandler
{
    protected Widget        m_Root;
    protected vector        m_Position;
    protected bool          m_Visible;
    protected bool          m_Debug;
    
    protected PlayerBase    m_Player;
    protected Weapon_Base   m_Weapon;       
 
    void ProjectedCrosshair()
    {
        m_Player = NULL;
        m_Weapon = NULL;
        m_Visible = false;
        m_Debug = false;
 
        g_Game.GetUpdateQueue(CALL_CATEGORY_GUI).Insert(this.Update);
