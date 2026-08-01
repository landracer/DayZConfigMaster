// Source: F:/Games/Dayz/scripts/3_game/damagesystem.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TotalDamageResult: Managed
{
    proto native float GetDamage(string zoneName, string healthType);
    proto native float GetHighestDamage(string healthType);
};
 
//-----------------------------------------------------------------------------

enum DamageType
{
    CLOSE_COMBAT,   // 0
    FIRE_ARM,       // 1
    EXPLOSION,
    STUN,
    CUSTOM
