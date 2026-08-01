// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/tools/fireworkslauncher.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ExplosionLight : PointLightBase
{
    void ExplosionLight()
    {
        SetVisibleDuringDaylight(true);
        SetRadiusTo(60);
        SetBrightnessTo(0.05);
        SetFlareVisible(false);
        SetAmbientColor(1.0, 1.0, 1.0);
        SetDiffuseColor(1.0, 1.0, 1.0);
        SetLifetime(2.1);
        //SetDisableShadowsWithinRadius(-1);
        SetFadeOutTime(1);
        m_FadeInTime = 0.25;
        SetFlickerSpeed(7);
        //SetFlickerAmplitude(0.5);
        SetFlickerAmplitudeMax(3);
        SetFlickerAmplitudeMin(0);
        SetCastShadow( false );
