// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase/pointlightbase/xmastreelight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class XmasTreeLight extends PointLightBase
{
    void XmasTreeLight()
    {
        SetVisibleDuringDaylight(false);
        SetRadiusTo( 12 );
        SetBrightnessTo( 0.50 );
        SetCastShadow(false);
        SetDiffuseColor(1.2, 1.0, 0.7);
        SetDancingShadowsMovementSpeed(0.005);
        SetDancingShadowsAmplitude(0.003);
