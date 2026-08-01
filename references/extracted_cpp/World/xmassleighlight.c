// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase/pointlightbase/xmassleighlight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class XmasSleighLight extends PointLightBase
{
    void XmasSleighLight()
    {
        SetVisibleDuringDaylight(false);
        SetRadiusTo(24);
        SetBrightnessTo(3);
        SetCastShadow(false);
        SetDiffuseColor(0.7, 0.7, 1.0);
        SetFlareVisible(false);
        SetFlickerAmplitude(0.5);
        SetFlickerSpeed(1);
