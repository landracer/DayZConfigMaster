// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/targetconditionscomponents/cctcursornoobject.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CCTCursorNoObject : CCTBase
{
    protected float m_MaximalActionDistanceSq;
    
    void CCTCursorNoObject ( float maximal_target_distance )
    {
        m_MaximalActionDistanceSq = maximal_target_distance * maximal_target_distance;
