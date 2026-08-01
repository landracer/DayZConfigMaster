// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedlightbase/pointlightbase/partylight.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PartyLight extends PointLightBase
{
    float m_Timer = 0;
    
    void PartyLight()
    {
        SetVisibleDuringDaylight(true);
        SetRadiusTo( 30 );
        SetBrightnessTo(1.0);
        SetCastShadow(true);
        //SetLifetime(600);
        SetFadeOutTime(1);
