// ======================================================================
// File: cz527_8c_source.html
// Category: other
// ======================================================================

class CZ527_Base : BoltActionRifle_ExternalMagazine_Base
{
override RecoilBase SpawnRecoilObject()
{
return new CZ527Recoil(this);
}
//Debug menu Spawn Ground Special
override void OnDebugSpawn()
{
super.OnDebugSpawn();
GetInventory().CreateAttachment("HuntingOptic");
}
};