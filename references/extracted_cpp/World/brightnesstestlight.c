// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase/pointlightbase/brightnesstestlight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// Test case for light sources not working during daytime as expected
 
class BrightnessTestLight extends PointLightBase
{
    float m_Timer = 0;
    
    void BrightnessTestLight()
    {
        SetVisibleDuringDaylight(true);
        SetRadiusTo( 30 );
        SetCastShadow(true);
        SetDiffuseColor(1, 1, 1);
        SetAmbientColor(1, 1, 1);
 
        SetBrightnessTo(10);
