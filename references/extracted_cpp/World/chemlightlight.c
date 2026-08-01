// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase/pointlightbase/chemlightlight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ChemlightLight extends PointLightBase
{
    private static float m_DefaultBrightness = 1.5;
    private static float m_DefaultRadius = 7.5;
    
    void ChemlightLight()
    {
        SetVisibleDuringDaylight(false);
        SetRadiusTo(m_DefaultRadius);
        SetBrightnessTo(m_DefaultBrightness);
        FadeIn(1);
        SetFadeOutTime(2);
        SetFlareVisible(false);
