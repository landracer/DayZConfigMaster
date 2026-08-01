// Source: F:/Games/Dayz/scripts/4_world/classes/contaminatedarea/contaminatedarea_local.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ContaminatedArea_Local : ContaminatedArea_DynamicBase
{
    const float TICK_RATE   = 1;
    ref Timer   m_Timer1    = new Timer;
    float       m_Lifetime  = 360;
    
    void ContaminatedArea_Local()
    {
        m_EffectsPriority = -10;
