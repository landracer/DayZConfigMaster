// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/flies.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FliesMdfr: ModifierBase
{
    const float DISTANCE_SENSITIVITY_SQR = Math.SqrFloat(0.05/*actual distance in meters*/);
    const int TICK_FREQUENCY = 15;
    const int IDLE_COUNT_THRESHOLD = 40;
    vector m_PrevPosition;
    int m_IdleCount;
    
    
    override void Init()
    {
        m_TrackActivatedTime = false;
        m_ID                    = eModifiers.MDF_FLIES;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = TICK_FREQUENCY;
        DisableActivateCheck();
        DisableDeactivateCheck();
