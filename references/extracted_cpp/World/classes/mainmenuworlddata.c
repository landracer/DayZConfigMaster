// Source: F:/Games/Dayz/scripts/4_world/classes/worlds/mainmenuworlddata.c Source File
// Extracted from DayZ Code Explorer by Zeroy

#ifdef PLATFORM_CONSOLE
class MainMenuWorldData : WorldData
{
    override bool WeatherOnBeforeChange(EWeatherPhenomenon type, float actual, float change, float time)
    {
        return false;
    }
}
#endif
