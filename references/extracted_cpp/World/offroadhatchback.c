// Source: F:/Games/Dayz/scripts/4_world/entities/vehicles/inheritedcars/offroadhatchback.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class OffroadHatchback extends CarScript
{
    protected ref UniversalTemperatureSource m_UTSource;
    protected ref UniversalTemperatureSourceSettings m_UTSSettings;
    protected ref UniversalTemperatureSourceLambdaEngine m_UTSLEngine;
 
    void OffroadHatchback()
    {
        //m_dmgContactCoef      = 0.075;
    
        m_EngineStartOK         = &quot;offroad_engine_start_SoundSet&quot;;
        m_EngineStartBattery    = &quot;offroad_engine_failed_start_battery_SoundSet&quot;;
        m_EngineStartPlug       = &quot;offroad_engine_failed_start_sparkplugs_SoundSet&quot;;
        m_EngineStartFuel       = &quot;offroad_engine_failed_start_fuel_SoundSet&quot;;
        m_EngineStop            = &quot;offroad_engine_stop_SoundSet&quot;;
        m_EngineStopFuel        = &quot;offroad_engine_stop_fuel_SoundSet&quot;;
        
        m_CarDoorOpenSound      = &quot;offroad_door_open_SoundSet&quot;;
        m_CarDoorCloseSound     = &quot;offroad_door_close_SoundSet&quot;;
        m_CarSeatShiftInSound   = &quot;Offroad_SeatShiftIn_SoundSet&quot;;
        m_CarSeatShiftOutSound  = &quot;Offroad_SeatShiftOut_SoundSet&quot;;
        
        m_CarHornShortSoundName = &quot;Offroad_Horn_Short_SoundSet&quot;;
        m_CarHornLongSoundName  = &quot;Offroad_Horn_SoundSet&quot;;
        
        SetEnginePos(&quot;0 0.7 1.2&quot;);
