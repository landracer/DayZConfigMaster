// Source: F:/Games/Dayz/scripts/4_world/entities/dayzplayerimplementheading.c Source File
// Extracted from DayZ Code Explorer by Zeroy

/* 
DayZPlayerImplement
 
this file is implemenation of dayzPlayer in script 
- logic of movement 
- camera switching logic
 
*/
 
class DayZPlayerImplementHeading
{
 
    //-------------------------------------------------------------

    static bool ClampHeading (float pDt, SDayZPlayerHeadingModel pModel, out float pLastHeadingDiff)
    {
        float       aDiff = pModel.m_fHeadingAngle - pModel.m_fOrientationAngle;
        if (aDiff < -Math.PI)
        {
            aDiff += Math.PI2;
        }
        else if (aDiff > Math.PI)
        {
            aDiff -= Math.PI2;
        }
 
        // Print(&quot;Heading model: or: &quot; + pModel.m_fOrientationAngle.ToString() + &quot; head:&quot; + pModel.m_fHeadingAngle.ToString() + &quot; dif:&quot; + aDiff.ToString());
 
        if (pLastHeadingDiff < -Math.PI_HALF && aDiff > 0)
        {
            aDiff                   = -Math.PI + 0.01;
            pLastHeadingDiff        = aDiff;
            pModel.m_fHeadingAngle  = pModel.m_fOrientationAngle + aDiff;
        
            // Print(&quot;-APA- : or: &quot; + pModel.m_fOrientationAngle.ToString() + &quot; head:&quot; + pModel.m_fHeadingAngle.ToString() + &quot; dif:&quot; + aDiff.ToString());
 
            return true;        // modify heading
        }
        else if (pLastHeadingDiff > Math.PI_HALF && aDiff < 0)
        {
            aDiff                   = Math.PI - 0.01;
            pLastHeadingDiff        = aDiff;
            pModel.m_fHeadingAngle  = pModel.m_fOrientationAngle + aDiff;
 
            // Print(&quot;-APA- : or: &quot; + pModel.m_fOrientationAngle.ToString() + &quot; head:&quot; + pModel.m_fHeadingAngle.ToString() + &quot; dif:&quot; + aDiff.ToString());
 
            return true;        // modify heading
        }
 
        pLastHeadingDiff    = aDiff;
        // Print(&quot;Heading model diff &quot; + aDiff.ToString());
        return false;
