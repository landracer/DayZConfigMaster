// Source: F:/Games/Dayz/scripts/4_world/classes/playerstats/statdebugobject.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class StatDebugObject
{
    string m_Name;
    float m_Value;
    eRemoteDebugType m_Type;
    
    void StatDebugObject(string name, float value, eRemoteDebugType type)
    {
        m_Name = name;
        m_Value = value;
        m_Type = type;
