// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/targetconditionscomponents/cctwatersurface.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CCTWaterSurface : CCTBase
{
    protected const int HEIGHT_DIFF_LIMIT_METERS    = 1.0;
 
    protected float m_MaximalActionDistanceSq;
    protected string m_SurfaceType; 
    protected ref array<string> m_AllowedSurfaceList;
    
    void CCTWaterSurface(float maximal_target_distance = UAMaxDistances.DEFAULT, string surfaceType = &quot;&quot;)
    {
        m_MaximalActionDistanceSq   = maximal_target_distance * maximal_target_distance;
        m_SurfaceType               = surfaceType;
        
        m_AllowedSurfaceList        = new array<string>();
        surfaceType.Split(&quot;|&quot;, m_AllowedSurfaceList);
