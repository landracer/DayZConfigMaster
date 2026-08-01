// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuousemptymagazine.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousEmptyMagazine : CAContinuousBase
{
    protected float                 m_TargetUnits;
    protected float                 m_SpentQuantity;
    protected float                 m_Time;
    protected float                 m_TimeToEjectOneBullet;
    protected ref Param1<float> m_SpentUnits;
    
    void CAContinuousEmptyMagazine( float eject_time )
    {
        m_TimeToEjectOneBullet = eject_time;
        m_Time = 0;
