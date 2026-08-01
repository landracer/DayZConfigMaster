// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase/pointlightbase/personallight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PersonalLight extends PointLightBase
{   
    void PersonalLight()
    {
        SetVisibleDuringDaylight(false);
        SetRadiusTo( 3 );
        SetBrightnessTo(0.38);
        SetDiffuseColor(0.2, 0.23, 0.25);
        SetCastShadow(false);
        EnableSpecular(false);
        EnableLinear(true);
        SetFlareVisible(false);
