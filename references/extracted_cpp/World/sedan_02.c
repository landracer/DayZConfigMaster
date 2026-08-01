// Source: F:/Games/Dayz/scripts/4_world/entities/vehicles/inheritedcars/sedan_02.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Sedan_02 extends CarScript
{
    protected ref UniversalTemperatureSource m_UTSource;
    protected ref UniversalTemperatureSourceSettings m_UTSSettings;
    protected ref UniversalTemperatureSourceLambdaEngine m_UTSLEngine;
 
    void Sedan_02()
    {
        m_dmgContactCoef        = 0.0790;
 
        m_EngineStartOK         = &quot;Sedan_02_engine_start_SoundSet&quot;;
        m_EngineStartBattery    = &quot;Sedan_02_engine_failed_start_battery_SoundSet&quot;;
        m_EngineStartPlug       = &quot;Sedan_02_engine_failed_start_sparkplugs_SoundSet&quot;;
        m_EngineStartFuel       = &quot;Sedan_02_engine_failed_start_fuel_SoundSet&quot;;
        m_EngineStop            = &quot;Sedan_02_engine_stop_SoundSet&quot;;
        m_EngineStopFuel        = &quot;Sedan_02_engine_stop_fuel_SoundSet&quot;;
 
        m_CarDoorOpenSound      = &quot;offroad_door_open_SoundSet&quot;;
        m_CarDoorCloseSound     = &quot;offroad_door_close_SoundSet&quot;;
        
        m_CarHornShortSoundName = &quot;Sedan_02_Horn_Short_SoundSet&quot;;
        m_CarHornLongSoundName  = &quot;Sedan_02_Horn_SoundSet&quot;;
        
        SetEnginePos(&quot;0 0.7 -1.7&quot;);
