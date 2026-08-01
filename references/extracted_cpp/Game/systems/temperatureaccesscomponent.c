// Source: F:/Games/Dayz/scripts/3_game/systems/temperatureaccess/temperatureaccesscomponent.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TemperatureAccessComponent
{
    protected EntityAI m_Owner;
    protected TemperatureAccessTypeBase m_AccessDataCurrent;
    protected float m_LastAccessTime;
    
    void TemperatureAccessComponent(EntityAI owner)
    {
        m_Owner = owner;
