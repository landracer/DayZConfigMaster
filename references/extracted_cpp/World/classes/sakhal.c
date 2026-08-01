// Source: F:/Games/Dayz/scripts/4_world/classes/worlds/sakhal.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//#define WEATHER_DATA_LOGGING
class SakhalData : WorldData
{
    const float SPAWN_CHANCE_CHOLERA = 20;
    const float COLD_AREA_TOOL_DMG_MODIF = 1.2;
    
    //-------test variables & methods ------
    #ifdef WEATHER_DATA_LOGGING
    int overcastChangeCount = 0;
    int directSuddenChangeCount = 0;
    int badWeatherSuddenChangeCount = 0;
    int thundersnowCount = 0;
    int suddenThundersnowCount = 0;
    int badWeatherCount = 0;
    int cloudyWeatherCount = 0;
    int clearWeatherCount = 0;
    int startYear = 0;
    int startMonth = 0;
    int startDay = 0;
    int startHour = 0;
    int startMinute = 0;
    int currentDay = 0;
    int daysToRun = 10;
    bool dayInit = false;
    #endif
    //------------------------  
    
    override void Init()
    {       
        super.Init();
        
        // new temperature curve settings
        m_Sunrise_Jan = 8.41;
        m_Sunset_Jan = 15.58;
        m_Sunrise_Jul = 3.75;
        m_Sunset_Jul = 20.25;
 
        int tempIdx;
        m_MinTemps = {-6.5, -9.5, -6.5, -9.5, 2, 6, 9, 10, 6, 1, -5, -10}; //{-13, -11, -7, -3, 2, 6, 9, 10, 6, 1, -5, -10} original values
        if (CfgGameplayHandler.GetEnvironmentMinTemps() && CfgGameplayHandler.GetEnvironmentMinTemps().Count() == 12)
        {
            for (tempIdx = 0; tempIdx < CfgGameplayHandler.GetEnvironmentMinTemps().Count(); tempIdx++)
            {
                m_MinTemps[tempIdx] = CfgGameplayHandler.GetEnvironmentMinTemps().Get(tempIdx);
            }
        }
 
        m_MaxTemps = {-3, -5, -3, -5, 9, 14, 16, 17, 14, 8, 1, -3}; //{-6, -4, -1, 3, 9, 14, 16, 17, 14, 8, 1, -3} original values
        if (CfgGameplayHandler.GetEnvironmentMaxTemps() && CfgGameplayHandler.GetEnvironmentMaxTemps().Count() == 12)
        {
            for (tempIdx = 0; tempIdx < CfgGameplayHandler.GetEnvironmentMaxTemps().Count(); tempIdx++)
            {
                m_MaxTemps[tempIdx] = CfgGameplayHandler.GetEnvironmentMaxTemps().Get(tempIdx);
            }
        }
        m_Pollution = m_Pollution | EPollution.HEAVYMETAL;
        
        m_TemperatureInsideBuildingsModifier    = -1.0;
        
        m_UniversalTemperatureSourceCapModifier = -3.0;
        
        m_ClearWeatherChance    = m_WeatherDefaultSettings.m_ClearWeatherChance;
        m_BadWeatherChance      = m_WeatherDefaultSettings.m_BadWeatherChance;
        
        if (g_Game.IsServer() || !g_Game.IsMultiplayer()) 
        {
            m_Weather.SetDynVolFogHeightDensity( 1, 0 );
            m_Weather.SetDynVolFogHeightBias( 0, 0 );
            m_Weather.GetFog().Set(0,0,1000);
            m_Weather.GetFog().SetLimits(0,0);
            m_Weather.GetOvercast().SetLimits(0.07, 1);
            
            if (g_Game.IsMultiplayer()) 
            {
                m_Weather.GetOvercast().Set(Math.RandomFloat(0,0.75),0,5); //forcing a random weather at a clean server start and an instant change for overcast
                CalculateVolFog(m_Weather.GetSnowfall().GetActual(),m_Weather.GetWindSpeed(),0);
            }
            
            m_DefaultPlayerRestrictedAreas = {&quot;pra/warheadstorage.json&quot;};
        }
        
        
