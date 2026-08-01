// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/diseases/testdisease.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TestDiseaseMdfr: ModifierBase
{
    float m_Interval;
    ref Param1<float> m_SomeValue;
 
    override void Init()
    {
        m_TrackActivatedTime            = false;
        m_ID                            = 9999;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = 5;
        m_SomeValue     = new Param1<float>(0);
        MakeParamObjectPersistent(m_SomeValue);
        
        DisableDeactivateCheck();
        DisableActivateCheck();
