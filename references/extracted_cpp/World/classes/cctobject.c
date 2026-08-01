// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/targetconditionscomponents/cctobject.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CCTObject : CCTBase
{
    protected float m_MaximalActionDistanceSq;
    
    void CCTObject ( float maximal_target_distance = UAMaxDistances.DEFAULT )
    {       
        m_MaximalActionDistanceSq = maximal_target_distance * maximal_target_distance;
