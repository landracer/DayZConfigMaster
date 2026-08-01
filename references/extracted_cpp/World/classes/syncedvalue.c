// Source: F:/Games/Dayz/scripts/4_world/classes/modules/syncedvalue.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SyncedValue
{
    string m_Name;
    float m_Value;
    float m_ValueNorm;
    bool m_State;
    
    void SyncedValue( string name, float value, bool state, float val_norm )
    {
        m_Name = name;
        m_Value = value;
        m_State = state;
        m_ValueNorm = val_norm;
