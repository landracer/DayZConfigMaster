// Source: F:/Games/Dayz/scripts/4_world/entities/grenade_base/m67grenade.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class M67Grenade extends Grenade_Base
{
    void M67Grenade()
    {
        SetAmmoType(&quot;M67Grenade_Ammo&quot;);
        SetFuseDelay(4);
        SetParticleExplosion(ParticleList.M67);
