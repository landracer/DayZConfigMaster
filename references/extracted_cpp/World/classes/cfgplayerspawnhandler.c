// Source: F:/Games/Dayz/scripts/4_world/classes/playergearspawn/cfgplayerspawnhandler.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PlayerSpawnHandler
{
    private static bool m_Initialized;
    static ref PlayerSpawnJsonData m_Data = new PlayerSpawnJsonData();
    
    static bool LoadData()
    {
        array<string> spawnGearPresetFiles = CfgGameplayHandler.GetPlayerSpawnGearPresetFiles();
        if (!spawnGearPresetFiles || (spawnGearPresetFiles && spawnGearPresetFiles.Count() == 0))
            return false;
 
        m_Data.presets = {};
 
        foreach (string spawnPresetFile : spawnGearPresetFiles)
        {
            PlayerSpawnPreset preset;
            string path = &quot;$mission:&quot; + spawnPresetFile;
 
            string errorMessage;
            if (!JsonFileLoader<PlayerSpawnPreset>.LoadFile(path, preset, errorMessage))
            {
                ErrorEx(errorMessage);
                return false;
            }
            
            if (preset != null)
                m_Data.presets.Insert(preset);
        }
 
        m_Initialized = m_Data.presets.Count() > 0;
        
        return true;
