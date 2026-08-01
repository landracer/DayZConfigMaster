// Source: F:/Games/Dayz/scripts/1_core/proto/enstring.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class string
{
    static const string Empty;
            
    proto native int ToInt();
    
    proto native int HexToInt();
    
    proto native float ToFloat();
            
    proto vector ToVector();
        
    
    vector BeautifiedToVector()
    {
        string copy = value;
        copy.Replace(&quot;<&quot;, &quot;&quot;);
        copy.Replace(&quot;>&quot;, &quot;&quot;);
        copy.Replace(&quot;,&quot;, &quot; &quot;);
        return copy.ToVector();
