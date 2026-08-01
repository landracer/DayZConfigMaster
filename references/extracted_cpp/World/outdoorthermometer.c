// Source: F:/Games/Dayz/scripts/4_world/entities/building/outdoorthermometer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class OutdoorThermometerManager
{
    private static float TIME_BETWEEN_UPDATE = 1;
    private static float LIMIT_FOR_UPDATE_ALL_THERMOMETERS = 100;
    private static ref array<OutdoorThermometer> m_Thermometers;
    private static int m_LastCheckIndex;
    private static float m_TimeToUpdate;
    //How many thermomethers will be set per update. 
    private static int m_NumUpdate = 1;
    
    static void Init()
    {
        m_Thermometers = new array<OutdoorThermometer>;
        m_LastCheckIndex = 0;
        m_TimeToUpdate = 0;
