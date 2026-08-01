// Source: F:/Games/Dayz/scripts/4_world/entities/building/underground/entrance/land_underground_entrance.c Source File
// Extracted from DayZ Code Explorer by Zeroy

enum EUndegroundEntranceState
{
    UNINITIALIZED,
    CLOSED,//fully closed
    //opening
    OPENING_A,
    OPENING_B,
    OPENING_C,
    OPENING_D,
    OPENING_E,//fully open
    OPENING_F,
    OPENING_G,
    //closing
    CLOSING_A,
    CLOSING_B,
    CLOSING_C,
    CLOSING_D,
    CLOSING_E,
    CLOSING_F,
    CLOSING_G
}
 
enum EUndegroundDoorType
{
    MAIN,
    SMALL,
}
 
class AlarmLight : SpotlightLight
{
    void AlarmLight()
    {
        SetVisibleDuringDaylight(true);
        SetRadiusTo(15);
        SetBrightnessTo(10);
        SetFlareVisible(false);
        SetAmbientColor(1.0, 0.0, 0.0);
        SetDiffuseColor(1.0, 0.0, 0.0);
        SetLifetime(1000);
        SetDisableShadowsWithinRadius(-1);
        SetFadeOutTime(1);
        SetCastShadow(false);
        m_FadeInTime = 0.25;
