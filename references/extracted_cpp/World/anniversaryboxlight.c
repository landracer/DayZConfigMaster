// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase/pointlightbase/anniversaryboxlight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

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
