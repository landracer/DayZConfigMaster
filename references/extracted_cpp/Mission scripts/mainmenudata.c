// Source: F:/Games/Dayz/scripts/5_mission/gui/newui/mainmenu/mainmenudata.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MainMenuData
{
    protected static ref JsonDataNewsList m_NewsData;
    protected static ref array<ref ModInfo> m_AllMods;
    protected static ref map<string, ref ModInfo> m_AllDlcsMap;
    
    static JsonDataNewsList GetNewsData()
    {
        if (!m_NewsData)
            m_NewsData = new JsonDataNewsList();
        
        return m_NewsData;
