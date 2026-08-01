// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/targetconditionscomponents/cctsurface.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CCTSurface : CCTBase
{
    protected float m_MaximalActionDistanceSq;
    
    void CCTSurface ( float maximal_target_distance = UAMaxDistances.DEFAULT )
    {
        m_MaximalActionDistanceSq = maximal_target_distance * maximal_target_distance;
