// Source: F:/Games/Dayz/scripts/3_game/tools/timeconversions.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class FullTimeData
{
    int m_Days
    int m_Hours
    int m_Minutes
    int m_Seconds
    
    string FormatedAsTimestamp()
    {
        m_Hours += m_Days * 24;
        
        string h;
        string m;
        string s;
        
        if (m_Hours < 10)
            h = &quot;0&quot;;
        
        if (m_Minutes < 10)
            m = &quot;0&quot;;
        
        if (m_Seconds < 10)
            s = &quot;0&quot;;
        
        return string.Format(&quot;%1%2:%3%4:%5%6&quot;, h, m_Hours, m, m_Minutes, s, m_Seconds);
