// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase/spotlightbase/universallightlight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UniversallightLight extends SpotLightBase
{
    private static float m_DefaultBrightness = 6.05;
    private static float m_DefaultRadius = 25;
    private static float m_DefaultAngle = 100;
    
    void UniversallightLight()
    {
        SetVisibleDuringDaylight( true );
        SetRadiusTo( m_DefaultRadius );
        SetSpotLightAngle( m_DefaultAngle );
        SetCastShadow( true );
        SetBrightnessTo( m_DefaultBrightness );
        SetFadeOutTime( 0.15 );
        SetAmbientColor( 0.95, 0.88, 0.8 );
        SetDiffuseColor( 0.95, 0.88, 0.8 );
        SetDisableShadowsWithinRadius( 1.2 );
