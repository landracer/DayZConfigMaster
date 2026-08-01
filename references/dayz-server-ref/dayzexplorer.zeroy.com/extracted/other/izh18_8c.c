// ======================================================================
// File: izh18_8c_source.html
// Category: other
// ======================================================================

class Izh18_Base extends RifleSingleShot_Base
{
void Izh18_Base ()
{
}
override RecoilBase SpawnRecoilObject()
{
return new Izh18Recoil(this);
}
//Debug menu Spawn Ground Special
override void OnDebugSpawn()
{
SpawnAmmo("Ammo_762x39",SAMF_DEFAULT);
EntityAI entity;
if ( Class.CastTo(entity, this) )
{
entity.SpawnEntityOnGroundPos("Ammo_762x39", entity.GetPosition());
}
}
};