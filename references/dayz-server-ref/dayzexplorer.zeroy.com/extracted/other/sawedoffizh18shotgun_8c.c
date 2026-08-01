// ======================================================================
// File: sawedoffizh18shotgun_8c_source.html
// Category: other
// ======================================================================

class SawedoffIzh18Shotgun extends Izh18Shotgun_Base
{
void SawedoffIzh18Shotgun ()
{
}
override RecoilBase SpawnRecoilObject()
{
return new Izh18ShotgunRecoil(this);
}
};