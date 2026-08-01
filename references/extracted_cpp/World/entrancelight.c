// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase/pointlightbase/entrancelight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class EntranceLight extends PointLightBase
{
    protected float m_DefaultBrightness = 0.5;
    protected float m_DefaultRadius = 5.5;
    
    void EntranceLight()
    {
        SetVisibleDuringDaylight(true);
        SetRadiusTo(m_DefaultRadius);
        SetBrightnessTo(m_DefaultBrightness);
        FadeIn(1);
        SetFadeOutTime(2);
        SetFlareVisible(false);
        SetCastShadow(false);
        SetAmbientColor(0.9, 0.9, 0.7);
        SetDiffuseColor(0.9, 0.9, 0.7);
