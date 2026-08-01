// Source: F:/Games/Dayz/scripts/3_game/systems/universaltemperaturesource/universaltemperaturesourcelambdafireplace.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UniversalTemperatureSourceLambdaFireplace : UniversalTemperatureSourceLambdaBase
{
    int m_FuelCount;
    int m_SmallFireplaceTemperatureMax;
    int m_NormalFireplaceTemperatureMax;
    float m_Temperature;
 
    void UniversalTemperatureSourceLambdaFireplace()
    {
        m_FuelCount                         = 0;
        m_SmallFireplaceTemperatureMax      = 0;
        m_NormalFireplaceTemperatureMax     = 0;
        m_Temperature                       = 0;
