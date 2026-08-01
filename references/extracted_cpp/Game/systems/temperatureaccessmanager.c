// Source: F:/Games/Dayz/scripts/3_game/systems/temperatureaccess/temperatureaccessmanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TemperatureAccessManager
{
    static ref map<ETemperatureAccessTypes,ref TemperatureAccessTypeBase> m_AccessTypesMap;
    
    void TemperatureAccessManager()
    {
        m_AccessTypesMap = new map<ETemperatureAccessTypes,ref TemperatureAccessTypeBase>;
        
        Init();
