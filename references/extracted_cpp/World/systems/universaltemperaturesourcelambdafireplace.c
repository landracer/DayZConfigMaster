// Source: F:/Games/Dayz/scripts/4_world/systems/universaltemperaturesource/universaltemperaturesourcelambdafireplace.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UniversalTemperatureSourceLambdaFireplace : UniversalTemperatureSourceLambdaBaseImpl
{
    int m_NormalFireplaceTemperatureMax;
    
    void UniversalTemperatureSourceLambdaFireplace()
    {
        m_FuelCount                         = 0;
        m_SmallFireplaceTemperatureMax      = 0;
        m_NormalFireplaceTemperatureMax     = 0;
        m_Temperature                       = 0;
