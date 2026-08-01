// Source: F:/Games/Dayz/scripts/5_mission/mission/missionbenchmark.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//  Struct for individual benchmark locations 
class BenchmarkLocation
{
    bool m_IsDummyTeleport;
    bool m_IsDummyWait;
    float m_CamSpeedMultiplier = 1;
    string m_Name;
    vector m_StartPos;
    vector m_StartLookAtPos;
    
    void BenchmarkLocation(string name)
    {
        m_Name = name;
