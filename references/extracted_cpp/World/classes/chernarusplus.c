// Source: F:/Games/Dayz/scripts/4_world/classes/worlds/chernarusplus.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//#define WEATHER_DATA_LOGGING
class ChernarusPlusData : WorldData
{
    //-------test variables & methods ------
    #ifdef WEATHER_DATA_LOGGING
    int overcastChangeCount = 0;
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
    
    //All Chernarus firing coordinates 
    protected static const ref array<vector> CHERNARUS_ARTY_STRIKE_POS = 
    {
        &quot;-500.00 165.00 5231.69&quot;,
        &quot;-500.00 300.00 9934.41&quot;,
        &quot;10406.86 192.00 15860.00&quot;,
        &quot;4811.75 370.00 15860.00&quot;,
        &quot;-500.00 453.00 15860.00&quot;
