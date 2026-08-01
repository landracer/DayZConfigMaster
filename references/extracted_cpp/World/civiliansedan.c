// Source: F:/Games/Dayz/scripts/4_world/entities/vehicles/inheritedcars/civiliansedan.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CivilianSedan extends CarScript
{
    protected ref UniversalTemperatureSource m_UTSource;
    protected ref UniversalTemperatureSourceSettings m_UTSSettings;
    protected ref UniversalTemperatureSourceLambdaEngine m_UTSLEngine;
 
    void CivilianSedan()
    {
        //m_dmgContactCoef      = 0.065;
        
        m_EngineStartOK         = &quot;CivilianSedan_engine_start_SoundSet&quot;;
        m_EngineStartBattery    = &quot;CivilianSedan_engine_failed_start_battery_SoundSet&quot;;
        m_EngineStartPlug       = &quot;CivilianSedan_engine_failed_start_sparkplugs_SoundSet&quot;;
        m_EngineStartFuel       = &quot;CivilianSedan_engine_failed_start_fuel_SoundSet&quot;;
        m_EngineStop            = &quot;CivilianSedan_engine_stop_SoundSet&quot;;
        m_EngineStopFuel        = &quot;CivilianSedan_engine_stop_fuel_SoundSet&quot;;
 
        m_CarDoorOpenSound      = &quot;offroad_door_open_SoundSet&quot;;
        m_CarDoorCloseSound     = &quot;offroad_door_close_SoundSet&quot;;
        
        m_CarHornShortSoundName = &quot;CivilianSedan_Horn_Short_SoundSet&quot;;
        m_CarHornLongSoundName  = &quot;CivilianSedan_Horn_SoundSet&quot;;
        
        SetEnginePos(&quot;0 0.7 1.6&quot;);
