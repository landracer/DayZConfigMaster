// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase/spotlightbase/pistollightlight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PistollightLight extends SpotLightBase
{
    private static float m_DefaultBrightness = 5.25;
    private static float m_DefaultRadius = 18;
    private static float m_DefaultAngle = 100;
    
    
    void PistollightLight()
    {
        SetVisibleDuringDaylight( true );
        SetRadiusTo( m_DefaultRadius );
        SetSpotLightAngle( m_DefaultAngle );
        SetCastShadow( true );
        SetBrightnessTo( m_DefaultBrightness );
        SetFadeOutTime( 0.05 );
        SetAmbientColor( 0.92, 0.88, 0.82 );
        SetDiffuseColor( 0.92, 0.88, 0.82 );
        SetDisableShadowsWithinRadius( 1.2 );
