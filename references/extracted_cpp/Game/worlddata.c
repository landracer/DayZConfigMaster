// Source: F:/Games/Dayz/scripts/3_game/worlddata.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class WorldData
{
    const float SPAWN_CHANCE_CHOLERA_DEF = 50;
    const float COLD_AREA_TOOL_DMG_MODIF_DEF = 1;
    
    float m_TemperaturePerHeightReductionModifier;  
    float m_CloudsTemperatureEffectModifier;        
    float m_TemperatureInsideBuildingsModifier;
    float m_WaterContactTemperatureModifier;
 
    protected float SUDDENCHANGE_TIME_MULTIPLIER    = 0.2;
    protected float SUDDENCHANGE_LENGTH_MULTIPLIER  = 0.4;
    protected float WIND_MAGNITUDE_TIME_MULTIPLIER  = 0.1;
    protected float WIND_DIRECTION_TIME_MULTIPLIER  = 0.05;
    
    protected Weather m_Weather;
    protected float m_EnvironmentTemperature;
    protected bool m_EnTempUpdated;
    protected float m_Timer;
    protected float m_MaxTemps[12];
    protected float m_MinTemps[12];
    protected float m_Sunrise_Jan;
    protected float m_Sunset_Jan;
    protected float m_Sunrise_Jul;
    protected float m_Sunset_Jul;
    protected ref array<vector> m_FiringPos; // Where we should fire from. On Init set the relevant data
    protected bool m_Pollution;
    protected ref CatchYieldBank m_YieldBank;
    protected ref WorldDataWeatherSettings m_WeatherDefaultSettings;
    protected ref WorldDataLiquidSettings m_LiquidSettings;
    protected ref TStringArray m_DefaultPlayerRestrictedAreas;

    protected int m_BadWeatherChance;
    protected int m_ClearWeatherChance;
    protected bool m_IsSuddenChange;
    protected float m_WorldWindCoef;
 
    protected float m_UniversalTemperatureSourceCapModifier;    
    
    //used at next weather calculation
    protected int m_SameWeatherCnt = 0;
    protected int m_StepValue = 5;
    protected int m_Chance = 50;
    protected int m_ChoosenWeather = 1;
    protected int m_LastWeather = 0;
 
    void WorldData()
    {
        Init();
        CreateYieldBank();
        InitYieldBank();
        UpdateBaseEnvTemperature(0);
