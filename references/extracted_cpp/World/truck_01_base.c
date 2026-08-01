// Source: F:/Games/Dayz/scripts/4_world/entities/vehicles/inheritedcars/truck_01_base.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Truck_01_Base extends CarScript
{
    protected ref UniversalTemperatureSource m_UTSource;
    protected ref UniversalTemperatureSourceSettings m_UTSSettings;
    protected ref UniversalTemperatureSourceLambdaEngine m_UTSLEngine;
 
    void Truck_01_Base()
    {
        //m_dmgContactCoef      = 0.018;
        m_enginePtcPos          = &quot;0 1.346 2.205&quot;;
        
        m_EngineStartOK         = &quot;Truck_01_engine_start_SoundSet&quot;;
        m_EngineStartBattery    = &quot;Truck_01_engine_failed_start_battery_SoundSet&quot;;
        m_EngineStartPlug       = &quot;Truck_01_engine_failed_start_sparkplugs_SoundSet&quot;;
        m_EngineStartFuel       = &quot;Truck_01_engine_failed_start_fuel_SoundSet&quot;;
        m_EngineStop            = &quot;Truck_01_engine_stop_SoundSet&quot;;
        m_EngineStopFuel        = &quot;Truck_01_engine_stop_fuel_SoundSet&quot;;
 
        m_CarDoorOpenSound      = &quot;Truck_01_door_open_SoundSet&quot;;
        m_CarDoorCloseSound     = &quot;Truck_01_door_close_SoundSet&quot;;
        
        m_CarHornShortSoundName = &quot;Truck_01_Horn_Short_SoundSet&quot;;
        m_CarHornLongSoundName  = &quot;Truck_01_Horn_SoundSet&quot;;
        
        SetEnginePos(&quot;0 1.4 2.25&quot;);
