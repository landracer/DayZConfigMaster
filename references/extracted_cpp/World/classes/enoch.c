// Source: F:/Games/Dayz/scripts/4_world/classes/worlds/enoch.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//#define WEATHER_DATA_LOGGING
class EnochData : WorldData
{
    //-------test variables & methods ------
    #ifdef WEATHER_DATA_LOGGING
    int overcastChangeCount = 0;
    int suddenChangeCount = 0;
    int stormCount = 0;
    int suddenStormCount = 0;
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
    
    protected static const ref array<vector> LIVONIA_ARTY_STRIKE_POS = 
    { 
        &quot;7440.00 417.00 -500.00&quot;,
        &quot;-500.00 276.00 5473.00&quot;,
        &quot;-500.00 265.00 9852.00&quot;,
        &quot;4953.00 240.00 13300.00&quot;,
        &quot;9620.00 188.00 13300.00&quot;,
        &quot;13300.00 204.00 10322.00&quot;,
        &quot;13300.00 288.00 6204.00&quot;,
        &quot;13300.00 296.00 -500.00&quot;
