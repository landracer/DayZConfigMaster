// ======================================================================
// File: rgd5grenade_8c_source.html
// Category: other
// ======================================================================

class RGD5Grenade extends Grenade_Base
{
void RGD5Grenade()
{
SetAmmoType("RGD5Grenade_Ammo");
SetFuseDelay(4);
SetParticleExplosion(ParticleList.RGD5);
}
void ~RGD5Grenade() {}
}