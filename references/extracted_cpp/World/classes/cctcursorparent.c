// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/targetconditionscomponents/cctcursorparent.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CCTCursorParent : CCTBase
{
    protected float m_MaximalActionDistanceSq;
    
    //approximate head heights
    const float HEIGHT_ERECT = 1.6;
    const float HEIGHT_CROUCH = 1.05;
    const float HEIGHT_PRONE = 0.66;
    
    void CCTCursorParent( float maximal_target_distance = UAMaxDistances.DEFAULT )
    {
        m_MaximalActionDistanceSq = maximal_target_distance * maximal_target_distance;
