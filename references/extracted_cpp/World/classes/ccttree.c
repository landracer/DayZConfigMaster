// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/targetconditionscomponents/ccttree.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CCTTree : CCTBase
{
    protected float m_MaximalActionDistanceSq;
    
    void CCTTree ( float maximal_target_distance )
    {
        m_MaximalActionDistanceSq = maximal_target_distance * maximal_target_distance;
