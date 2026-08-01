// Source: F:/Games/Dayz/scripts/3_game/systems/temperatureaccess/temperaturedata.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TemperatureData
{
    ETemperatureAccessTypes m_AccessType;
    bool m_UseGlobalCooling;
    float m_Value;                  //target or increment, depends on context!
    float m_AdjustedTarget;         //actual target of the operation (can be adjusted via over-time interpolation, not necessarily the original target value!)
    float m_UpdateTimeInfo;         //if the temperature change was accumulated over some time, pass this info to temperature subsystems
    float m_UpdateTimeCoef;         //multiplies the time
    float m_HeatPermeabilityCoef;   //permeability multiplier (modifies change speed).
    
    float m_InterpolatedStepSize;   //only useful for interpolated temperature values
    float m_InterpolatedFraction;   //only useful for interpolated temperature values
    
    void TemperatureData(float val, ETemperatureAccessTypes accessType = ETemperatureAccessTypes.ACCESS_UNKNOWN, float time = -1, float timeCoef = 1, float heatPermCoef = 1)
    {
        m_Value = val;
        m_AdjustedTarget = val;
        m_AccessType = accessType;
        m_UpdateTimeInfo = time;
        m_UpdateTimeCoef = timeCoef;
        m_HeatPermeabilityCoef = heatPermCoef;
        
        Init();
