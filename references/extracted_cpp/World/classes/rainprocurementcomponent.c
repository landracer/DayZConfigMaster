// Source: F:/Games/Dayz/scripts/4_world/classes/rainprocurementcomponent.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class RainProcurementComponentBase : Managed
{
    protected ItemBase                  m_ProcuringItem;    
    protected RainProcurementHandler    m_Handler;
    protected bool                      m_IsUnderRoof;
    protected bool                      m_IsActive;
    protected int                       m_UpdateCounter;
    protected const int                 UPDATE_ROOFCHECK_COUNT = 3; //do roofcheck every n updates
    
    void RainProcurementComponentBase(ItemBase procuringItem)
    {
        m_ProcuringItem = procuringItem;
        Reset();
        m_Handler = MissionBaseWorld.Cast(g_Game.GetMission()).GetRainProcurementHandler();
