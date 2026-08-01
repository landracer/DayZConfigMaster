// Source: F:/Games/Dayz/scripts/4_world/entities/building/specific/land_fuelstation_feed.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Land_FuelStation_Feed extends FuelStation
{
    override void EEKilled(Object killer)
    {
        super.EEKilled(killer);
 
        Explode(DamageType.EXPLOSION, &quot;LandFuelFeed_Ammo&quot;);
