// Source: F:/Games/Dayz/scripts/3_game/cfgplayerrestrictedareajsondata.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CfgPlayerRestrictedAreaJsonData
{
    ref array<ref PlayerRestrictedAreaInstance> m_ValidatedAreas = new array<ref PlayerRestrictedAreaInstance>();
    ref array<ref PlayerRestrictedAreaInstance> m_Areas = new array<ref PlayerRestrictedAreaInstance>();
    
    bool InitializeAreas()
    {
        m_ValidatedAreas.Clear();
        
        foreach (PlayerRestrictedAreaInstance instance : m_Areas)
        {
            instance.Initialize();
            if (instance.IsValid())
                m_ValidatedAreas.Insert(instance);
        }
        
        m_Areas.Clear();
        
        return m_ValidatedAreas.Count() > 0;
