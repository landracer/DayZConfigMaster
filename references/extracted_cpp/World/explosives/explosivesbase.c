// Source: F:/Games/Dayz/scripts/4_world/entities/explosivesbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ExplosiveLight : PointLightBase
{
    protected static float m_DefaultBrightness  = 10;
    protected static float m_DefaultRadius      = 30;
    
    void ExplosiveLight()
    {
        SetVisibleDuringDaylight(false);
        SetRadiusTo(m_DefaultRadius);
        SetBrightnessTo(m_DefaultBrightness);
        SetFlareVisible(false);
        SetAmbientColor(1.0, 1.0, 0.3);
        SetDiffuseColor(1.0, 1.0, 0.3);
        SetLifetime(0.15);
        SetDisableShadowsWithinRadius(-1);
