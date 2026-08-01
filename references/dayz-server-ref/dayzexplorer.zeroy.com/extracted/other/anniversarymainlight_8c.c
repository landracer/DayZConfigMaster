// ======================================================================
// File: anniversarymainlight_8c_source.html
// Category: other
// ======================================================================

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
}
}