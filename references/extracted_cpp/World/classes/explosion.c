// Source: F:/Games/Dayz/scripts/4_world/classes/explosion.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// WIP
 
class Explosion
{
    void SpawnEffect( vector position, Effect eff, vector pos, vector ori)
    {
        SEffectManager.PlayInWorld(eff, pos);
