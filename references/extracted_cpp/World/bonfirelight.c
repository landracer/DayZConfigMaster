// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase/pointlightbase/bonfirelight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BonfireLight extends PointLightBase
{
    static float m_FireRadius = 40;
    static float m_FireBrightness = 15.0;
    static string m_MemoryPoint = &quot;bonfire_fire&quot;;
    
    void BonfireLight()
    {
        SetVisibleDuringDaylight(true);
        SetRadiusTo(m_FireRadius);
        SetBrightnessTo(m_FireBrightness);
        SetCastShadow(false);
        SetFadeOutTime(1);
        SetDiffuseColor(1.0, 0.45, 0.25);
        SetAmbientColor(1.0, 0.45, 0.25);
        SetFlareVisible(false);
        SetFlickerAmplitude(0.55);
        SetFlickerSpeed(0.75);
        SetDancingShadowsMovementSpeed(0.1);
        SetDancingShadowsAmplitude(0.03);
