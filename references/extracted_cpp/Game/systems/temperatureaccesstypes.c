// Source: F:/Games/Dayz/scripts/3_game/systems/temperatureaccess/temperatureaccesstypes.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TemperatureAccessTypeBase
{
    protected ETemperatureAccessTypes m_ID = ETemperatureAccessTypes.ACCESS_UNKNOWN;
    protected int m_Priority;
    
    void TemperatureAccessTypeBase()
    {
        Init();
