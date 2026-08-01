// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/hunger.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HungerMdfr: ModifierBase
{
    protected float m_EnergyDelta;
    protected float m_LastEnergyLevel;
    ref HumanMovementState      m_MovementState = new HumanMovementState();
        
    override void Init()
    {
        m_TrackActivatedTime = false;
        m_ID                    = eModifiers.MDF_HUNGER;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = 1;
        DisableDeactivateCheck();
