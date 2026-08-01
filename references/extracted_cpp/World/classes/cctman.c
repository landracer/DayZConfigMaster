// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/targetconditionscomponents/cctman.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CCTMan : CCTBase
{
    protected float m_MaximalActionDistanceSq;
    protected bool m_MustBeAlive;
    
    void CCTMan ( float maximal_target_distance = UAMaxDistances.DEFAULT, bool must_be_alive = true )
    {
        m_MaximalActionDistanceSq = maximal_target_distance * maximal_target_distance;  
        m_MustBeAlive = must_be_alive;
