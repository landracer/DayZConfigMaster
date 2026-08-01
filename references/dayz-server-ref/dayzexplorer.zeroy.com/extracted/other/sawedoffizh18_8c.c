// ======================================================================
// File: sawedoffizh18_8c_source.html
// Category: other
// ======================================================================

class SawedoffIzh18 extends Izh18_Base
{
void SawedoffIzh18 ()
{
}
override RecoilBase SpawnRecoilObject()
{
return new Izh18SawedOffRecoil(this);
}
};