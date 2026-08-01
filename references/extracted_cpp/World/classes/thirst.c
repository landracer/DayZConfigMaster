// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/thirst.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ThirstMdfr: ModifierBase
{
    float           m_LastWaterLevel;
    ref HumanMovementState      m_MovementState = new HumanMovementState();
    
    override void Init()
    {
        m_TrackActivatedTime = false;
        m_ID                    = eModifiers.MDF_THIRST;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = 1;
        DisableDeactivateCheck();
