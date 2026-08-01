// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase/pointlightbase/anniversarymainlight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AnniversaryMainLight : PointLightBase
{
    void AnniversaryMainLight()
    {
        SetVisibleDuringDaylight(false);
        SetRadiusTo(50);
        SetBrightnessTo(8);
        SetCastShadow(false);
        SetAmbientColor(1, 0.87, 0.6);
        SetDiffuseColor(1, 0.87, 0.6);
