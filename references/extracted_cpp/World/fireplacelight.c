// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase/pointlightbase/fireplacelight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FireplaceLight extends PointLightBase
{
    static float m_FireplaceRadius = 25;
    static float m_FireplaceBrightness = 4.75;
    
    void FireplaceLight()
    {
        SetVisibleDuringDaylight(false);
        SetRadiusTo( m_FireplaceRadius );
        SetBrightnessTo(m_FireplaceBrightness);
        SetCastShadow(true);
        SetFadeOutTime(5);
        SetDiffuseColor(1.3, 0.35, 0.15);
        SetAmbientColor(1.3, 0.35, 0.15);
        SetFlareVisible(false);
        SetFlickerAmplitude(0.3);
        SetFlickerSpeed(0.9);
        SetExteriorMode();
        EnableHeatHaze(true);
        SetHeatHazeRadius(0.23);
        SetHeatHazePower(0.010);
