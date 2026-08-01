// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase/spotlightbase/headtorchlight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HeadtorchLight extends SpotLightBase
{
    private static float m_DefaultBrightness = 3;
    private static float m_DefaultRadius = 20;
    
    void HeadtorchLight()
    {
        SetVisibleDuringDaylight( true );
        SetRadiusTo( m_DefaultRadius );
        SetSpotLightAngle( 95 );
        SetCastShadow( true );
        FadeIn( 0.06 );
        SetBrightnessTo( m_DefaultBrightness );
        SetAmbientColor( 0.92, 0.85, 0.58 );
        SetDiffuseColor( 0.92, 0.85, 0.58 );
        SetFadeOutTime( 0.1 );
        //SetDisableShadowsWithinRadius(0.25); // Idea for optimization: Uncomment this to disable shadows from Headtorch while it&#39;s on player&#39;s head during 1P view.
