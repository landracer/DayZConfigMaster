// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/navigation/gpsreceiver.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class GPSReceiver : ItemGPS
{
 
    protected const string DISPLAY_TEXTURE_PATH_FORMAT  = &quot;dz\\gear\\navigation\\data\\GPS_%1_ca.paa&quot;;
    protected const string ANIM_PHASE_DISPLAY_HIDE      = &quot;DisplayState&quot;;
    
    protected const int PHASE_OFF = 1;
    protected const int PHASE_ON = 0;
 
    protected ref set<string>       m_DisplayGridSelections;
    protected ref set<string>       m_DisplayAltSelections;
    protected ref map<int, string>  m_DisplayNumericSignTextureMap;
    
    protected ref array<int>        m_OrderedPositionNumbersLast;
    protected ref array<int>        m_AltitudeNumbersLast;
    
    protected bool                  m_InitGPS;
    
    void GPSReceiver()
    {
        m_OrderedPositionNumbersLast    = new array<int>;
        m_AltitudeNumbersLast           = new array<int>;
        
        int i;
        for (i = 0; i < MapNavigationBehaviour.DISPLAY_GRID_POS_MAX_CHARS_COUNT * 2; ++i)
        {
            m_OrderedPositionNumbersLast.Insert(0);
        }
        
        for (i = 0; i < MapNavigationBehaviour.DISPLAY_ALT_MAX_CHARS_COUNT; ++i)
        {
            m_AltitudeNumbersLast.Insert(0);
        }   
        
        m_DisplayGridSelections = new set<string>();
        m_DisplayGridSelections.Insert(&quot;grid_1_0&quot;);
        m_DisplayGridSelections.Insert(&quot;grid_1_1&quot;);
        m_DisplayGridSelections.Insert(&quot;grid_1_2&quot;);
        m_DisplayGridSelections.Insert(&quot;grid_2_0&quot;);
        m_DisplayGridSelections.Insert(&quot;grid_2_1&quot;);
        m_DisplayGridSelections.Insert(&quot;grid_2_2&quot;);
        
        m_DisplayAltSelections = new set<string>();
        m_DisplayAltSelections.Insert(&quot;alt_0&quot;);
        m_DisplayAltSelections.Insert(&quot;alt_1&quot;);
        m_DisplayAltSelections.Insert(&quot;alt_2&quot;);
        m_DisplayAltSelections.Insert(&quot;alt_3&quot;);
        
        m_DisplayNumericSignTextureMap = new map<int, string>();
        for (i = -1; i < 11; i++)
        {
            
            string texturePath = string.Format(DISPLAY_TEXTURE_PATH_FORMAT, i);
            
            if (i == -1)
            {
                texturePath = string.Format(DISPLAY_TEXTURE_PATH_FORMAT, &quot;dash&quot;);
            }
 
            m_DisplayNumericSignTextureMap.Insert(i, texturePath);
        }
