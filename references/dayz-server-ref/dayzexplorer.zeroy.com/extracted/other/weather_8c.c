// ======================================================================
// File: weather_8c_source.html
// Category: other
// ======================================================================

//-----------------------------------------------------------------------------
enum EWeatherPhenomenon
{
OVERCAST,
FOG,
RAIN,
SNOWFALL,
WIND_DIRECTION,
WIND_MAGNITUDE,
VOLFOG_HEIGHT_DENSITY,
VOLFOG_DISTANCE_DENSITY,
VOLFOG_HEIGHT_BIAS
};
//-----------------------------------------------------------------------------
class WeatherPhenomenon
{
private void WeatherPhenomenon() {}
private void ~WeatherPhenomenon() {}
proto native EWeatherPhenomenon GetType();
proto native float GetActual();
proto native float GetForecast();
proto native void Set( float forecast, float time = 0, float minDuration = 0 );
proto native float GetNextChange();
proto native void SetNextChange( float time );
proto void GetLimits( out float fnMin, out float fnMax );
proto native void SetLimits( float fnMin, float fnMax );
proto void GetForecastChangeLimits( out float fcMin, out float fcMax );
proto native void SetForecastChangeLimits( float fcMin, float fcMax );
proto void GetForecastTimeLimits( out float ftMin, out float ftMax );
proto native void SetForecastTimeLimits( float ftMin, float ftMax );
bool OnBeforeChange( float change, float time )
{
// check if mission forces use of custom weather
Weather weather = g_Game.GetWeather();
if ( weather.GetMissionWeather() )
return false;
if (weather.GetWeatherUpdateFrozen())
return true;
// check for active worlddata with custom onbeforechange behaviour
Mission currentMission = g_Game.GetMission();
if ( currentMission )
{
WorldData worldData = currentMission.GetWorldData();
if ( worldData )
{
return worldData.WeatherOnBeforeChange( GetType(), GetActual(), change, time );
}
}
return false;
}
};
typedef WeatherPhenomenon Overcast;
typedef WeatherPhenomenon Fog;
typedef WeatherPhenomenon Rain;
typedef WeatherPhenomenon Snowfall;
typedef WeatherPhenomenon WindDirection;
typedef WeatherPhenomenon WindMagnitude;
//-----------------------------------------------------------------------------
class Weather
{
protected bool m_missionWeather;
protected bool m_UpdateFrozen;
private void Weather()
{
m_missionWeather = false;
}
private void ~Weather() {}
proto native float GetTime();
proto native Overcast GetOvercast();
proto native Fog GetFog();
proto native Rain GetRain();
proto native Snowfall GetSnowfall();
proto native WindDirection GetWindDirection();
proto native WindMagnitude GetWindMagnitude();
proto native void SetStorm( float density, float threshold, float timeOut );
proto native void SuppressLightningSimulation(bool state);
proto native vector GetWind();
proto native void SetWind( vector wind );
proto native float GetWindSpeed();
proto native void SetWindSpeed( float speed );
proto native float GetWindMaximumSpeed();
proto native void SetWindMaximumSpeed( float maxSpeed );
proto void GetWindFunctionParams( out float fnMin, out float fnMax, out float fnSpeed );
proto native void SetWindFunctionParams( float fnMin, float fnMax, float fnSpeed );
proto native void SetRainThresholds( float tMin, float tMax, float tTime );
proto native void SetSnowfallThresholds( float tMin, float tMax, float tTime );
proto native void SetSnowflakeScale(float scale);
proto native float GetSnowflakeScale();
static proto float WindDirectionToAngle( vector dir );
static proto vector AngleToWindDirection( float angle );
proto native bool IsDynVolFogEnabled();
proto native void SetDynVolFogDistanceDensity(float value, float time = 0);
proto native float GetDynVolFogDistanceDensity();
proto native void SetDynVolFogHeightDensity(float value, float time = 0);
proto native float GetDynVolFogHeightDensity();
proto native void SetDynVolFogHeightBias(float value, float time = 0);
proto native float GetDynVolFogHeightBias();
void MissionWeather( bool use )
{
m_missionWeather = use;
}
bool GetMissionWeather()
{
return m_missionWeather;
}
void SetWeatherUpdateFreeze(bool state)
{
m_UpdateFrozen = state;
}
bool GetWeatherUpdateFrozen()
{
return m_UpdateFrozen;
}
// Noise reduction due to environmental conditions, used for AI noise evaluation
float GetNoiseReductionByWeather()
{
float rainReduction = GetRain().GetActual() * GameConstants.RAIN_NOISE_REDUCTION_WEIGHT;
float snowfallReduction = GetSnowfall().GetActual() * GameConstants.SNOWFALL_NOISE_REDUCTION_WEIGHT;
if (rainReduction == 0 && snowfallReduction == 0)
return 1;
if (rainReduction > snowfallReduction) // combined phenomenons dont need to have multiplicative effects
return 1 - rainReduction;
else
return 1 - snowfallReduction;
}
};