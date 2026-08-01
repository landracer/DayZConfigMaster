// Source: F:/Games/Dayz/scripts/4_world/classes/playerstats/playerstatrecord.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PlayerStatRecord
{
    float       m_Value;
    float       m_Time;
    string      m_System;
    
    void PlayerStatRecord(float value, float time, string system)
    {
        m_Value = value;
        m_Time = time;
        m_System = system;
