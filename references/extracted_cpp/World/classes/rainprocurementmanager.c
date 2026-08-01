// Source: F:/Games/Dayz/scripts/4_world/classes/rainprocurementmanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class RainProcurementManager
{
    protected           ItemBase    m_ProcuringItem;    
    protected           int         m_IsUnderRoof;
    protected   ref Timer       m_UpdateTimer;
    protected   const   int         RAIN_COEFFICIENT = 10;
 
    void RainProcurementManager( ItemBase procuring_item )
    {
        m_ProcuringItem = procuring_item;
