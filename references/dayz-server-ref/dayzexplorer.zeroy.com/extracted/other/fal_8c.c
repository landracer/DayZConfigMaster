// ======================================================================
// File: fal_8c_source.html
// Category: other
// ======================================================================

class FAL_Base : RifleBoltLock_Base
{
override RecoilBase SpawnRecoilObject()
{
return new FALRecoil(this);
}
//Debug menu Spawn Ground Special
override void OnDebugSpawn()
{
GameInventory inventory = GetInventory();
inventory.CreateInInventory( "Fal_OeBttstck" );
inventory.CreateInInventory( "M4_T3NRDSOptic" );
inventory.CreateInInventory( "Battery9V" );
SpawnAttachedMagazine("Mag_FAL_20Rnd");
}
};