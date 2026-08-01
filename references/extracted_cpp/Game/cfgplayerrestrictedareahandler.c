// Source: F:/Games/Dayz/scripts/3_game/cfgplayerrestrictedareahandler.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CfgPlayerRestrictedAreaHandler
{
    private static bool m_Initialized;
    
    static ref TStringArray m_PRAFiles = new TStringArray();
    static ref CfgPlayerRestrictedAreaJsonData m_Data = new CfgPlayerRestrictedAreaJsonData();
    
    //----------------------------------------------------------
    //load, inits, validation
    
    static bool LoadData()
    {
        if (m_Initialized)
            return true;
        
        m_Initialized = false;
        
        if (g_Game.ServerConfigGetInt( &quot;enableCfgGameplayFile&quot; )) //only reach into the CfgGameplayHandler if properly loaded!
            m_PRAFiles = CfgGameplayHandler.GetPlayerRestrictedAreaFiles();
        else
            m_PRAFiles = g_Game.GetMission().GetWorldData().GetDefaultPRAPaths();
        
        if (!m_PRAFiles)
            return true;
 
        string worldName;
        g_Game.GetWorldName(worldName);
    
        foreach (string filename : m_PRAFiles)
        {
            PlayerRestrictedAreaInstance area = new PlayerRestrictedAreaInstance();
            
            string errorMessage, path;
 
            path = string.Format(&quot;$mission:%1&quot;, filename);
            if (!JsonFileLoader<PlayerRestrictedAreaInstance>.LoadFile(path, area, errorMessage))
            {
                if (!FileExist(filename))
                    path = string.Format(&quot;dz/worlds/%1/ce/%2&quot;, worldName, filename);
                
                if (!JsonFileLoader<PlayerRestrictedAreaInstance>.LoadFile(path, area, errorMessage))
                {
                    ErrorEx(errorMessage);
                    continue;
                }
            }
            
            if (area != null)
                m_Data.m_Areas.Insert(area);
        }
        
        m_Initialized = m_Data.InitializeAreas();
        
        return true;
