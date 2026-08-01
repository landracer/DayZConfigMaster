// ======================================================================
// File: p1_8c_source.html
// Category: other
// ======================================================================

class P1_Base : Pistol_Base
{
override RecoilBase SpawnRecoilObject()
{
return new P1Recoil(this);
}
};
class P1 : P1_Base {};