// Source: F:/Games/Dayz/scripts/3_game/gui/dlcs/dlcdataloader.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DlcDataLoader
{
    protected static const string JSON_FILE_PATH = &quot;scripts/data/dlcinfo.json&quot;;
    
    static JsonDataDLCList GetData()
    {
        JsonDataDLCList data;
        
        string errorMessage;
        if (!JsonFileLoader<JsonDataDLCList>.LoadFile(JSON_FILE_PATH, data, errorMessage))
            ErrorEx(errorMessage);
        
        return data;
