// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase/spotlightbase/flashlightlight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FlashlightLight extends SpotLightBase
{
    private static float m_DefaultBrightness = 4;
    private static float m_DefaultRadius = 25;
    
    void FlashlightLight()
    {
        SetVisibleDuringDaylight( true );
        SetRadiusTo( m_DefaultRadius );
        SetSpotLightAngle( 110 );
        SetCastShadow( true );
        EnableSpecular( true );
        SetBrightnessTo( m_DefaultBrightness );
        SetFadeOutTime( 0.15 );
        SetAmbientColor( 0.9, 0.85, 0.75 );
        SetDiffuseColor( 0.9, 0.85, 0.75 );
