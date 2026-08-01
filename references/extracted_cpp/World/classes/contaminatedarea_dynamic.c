// Source: F:/Games/Dayz/scripts/4_world/classes/contaminatedarea/contaminatedarea_dynamic.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// The parameters for the explosion light when creating dynamic area
class ShellLight : PointLightBase
{
    protected float m_DefaultBrightness = 10;
    protected float m_DefaultRadius = 100;
    
    void ShellLight()
    {
        SetVisibleDuringDaylight(false);
        SetRadiusTo(m_DefaultRadius);
        SetBrightnessTo(m_DefaultBrightness);
        SetFlareVisible(false);
        SetAmbientColor(1.0, 1.0, 0.3);
        SetDiffuseColor(1.0, 1.0, 0.3);
        SetLifetime(0.15);
        SetDisableShadowsWithinRadius(-1);
        SetCastShadow( false );
