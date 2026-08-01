// Source: F:/Games/Dayz/scripts/4_world/classes/recoilbase/recoils/defaultrecoil.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DefaultRecoil: RecoilBase
{
    override void Init()
    {
        m_HandsCurvePoints.Insert(&quot;0 0 0&quot;);//forms a 2 dimensional spline(z is ignored)
        m_HandsCurvePoints.Insert(&quot;0 1 0&quot;);
        m_HandsCurvePoints.Insert(&quot;0 0 0&quot;);
        m_HandsOffsetRelativeTime = 0.4;
        
        m_MouseOffsetRangeMin = 60;//in degrees min
        m_MouseOffsetRangeMax = 120;//in degrees max
        m_MouseOffsetDistance = 1;//how far should the mouse travel
        m_MouseOffsetRelativeTime = 0;//[0..1] a time it takes to move the mouse the required distance relative to the reload time of the weapon(firing mode)
