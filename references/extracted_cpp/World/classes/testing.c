// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/testing.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TestingMdfr: ModifierBase
{
    float stuff;
    bool swch;
    override void Init()
    {
        m_TrackActivatedTime            = true;
        m_ID                    = eModifiers.MDF_TESTING;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
