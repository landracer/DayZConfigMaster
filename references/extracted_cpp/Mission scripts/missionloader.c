// Source: F:/Games/Dayz/scripts/5_mission/gui/scriptedwindows/missionloader.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class JsonMissionLoaderData
{
    ref TStringArray MissionPaths;
 
    static JsonMissionLoaderData GetData()
    {
        JsonMissionLoaderData data;
        
        string path;
        string errorMessage;
 
        if (GetCLIParam(&quot;missionLoaderPath&quot;, path) == false)
        {
            path = CFG_FILE_MISSION_LIST;
        }
        
        if (!FileExist(path))
        {
            DayZGame dzg = GetDayZGame();
            
            data = new JsonMissionLoaderData();
            data.MissionPaths = {dzg.GetMissionFolderPath()};
            if (!JsonFileLoader<JsonMissionLoaderData>.SaveFile(path, data, errorMessage))
                ErrorEx(errorMessage);
        }
        else
        {
            if (!JsonFileLoader<JsonMissionLoaderData>.LoadFile(path, data, errorMessage))
                ErrorEx(errorMessage);
        }
 
        return data;
