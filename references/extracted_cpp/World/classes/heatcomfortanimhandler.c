// Source: F:/Games/Dayz/scripts/4_world/classes/heatcomfortanimhandler.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HeatComfortAnimHandler
{
    const float TICK_INTERVAL  = 2;
    
    float m_TimeSinceLastTick;
    float m_ProcessTimeAccuFreeze;
    float m_ProcessTimeAccuFreezeRattle;
    float m_ProcessTimeAccuHot;
    
    PlayerBase m_Player;
    
    float m_EventTimeFreezeRattle = -1;         // -1 = symptom effect needs to be triggered for first time
    float m_EventTimeFreeze = -1;           
    float m_EventTimeHot = -1;  
    
    protected ref HumanMovementState m_MovementState = new HumanMovementState();
    
    const float TIME_INTERVAL_HC_MINUS_LOW_MIN  = 12;   // Freezing and rattle symptoms interval minimum, lerped according to heat confort
    const float TIME_INTERVAL_HC_MINUS_HIGH_MIN = 25; 
    
    const float TIME_INTERVAL_HC_MINUS_LOW_MAX  = 20;   // Interval maximum, lerped according to heat confort
    const float TIME_INTERVAL_HC_MINUS_HIGH_MAX = 40;
    
    const float TIME_INTERVAL_HC_PLUS_LOW_MIN   = 12;   // Ditto for heat symptoms
    const float TIME_INTERVAL_HC_PLUS_HIGH_MIN  = 25;
    
    const float TIME_INTERVAL_HC_PLUS_LOW_MAX   = 20;
    const float TIME_INTERVAL_HC_PLUS_HIGH_MAX  = 40;
    
    void HeatComfortAnimHandler(PlayerBase player)
    {
        m_Player = player;
