// ======================================================================
// File: anniversaryboxlight_8c_source.html
// Category: other
// ======================================================================

class AnniversaryBoxLight extends PointLightBase
{
void AnniversaryBoxLight()
{
SetVisibleDuringDaylight( true );
SetRadiusTo( 1 );
SetBrightnessTo( 3 );
SetCastShadow( false );
SetAmbientColor( 0.9, 0.85, 0.4 );
SetDiffuseColor( 0.9, 0.85, 0.4 );
}
}