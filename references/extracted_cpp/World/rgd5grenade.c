// Source: F:/Games/Dayz/scripts/4_world/entities/grenade_base/rgd5grenade.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class RGD5Grenade extends Grenade_Base
{
    void RGD5Grenade()
    {
        SetAmmoType(&quot;RGD5Grenade_Ammo&quot;);
        SetFuseDelay(4);
        SetParticleExplosion(ParticleList.RGD5);
